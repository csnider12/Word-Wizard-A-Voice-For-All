/**
 * Python script to read NFC card and push text data to Piper TTS
 * rpi_read_ndef_text.c
 *
 * Reads NDEF Text records from NFC cards (NTAG2xx / Mifare Ultralight)
 * using the pn532-lib on Raspberry Pi.
 *
 * Based on: https://github.com/soonuse/pn532-lib/blob/master/examples/raspberrypi/rpi_dump_ntag2.c
 *
 * Build:
 *   gcc -o rpi_read_ndef_text rpi_read_ndef_text.c -lpn532 -lpn532_rpi
 *
 * Wiring (I2C, default):
 *   PN532 SDA -> RPi GPIO 2 (Pin 3)
 *   PN532 SCL -> RPi GPIO 3 (Pin 5)
 *   PN532 VCC -> 3.3V or 5V
 *   PN532 GND -> GND
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "pn532.h"
#include "pn532_rpi.h"

/* ── NTAG2xx layout ──────────────────────────────────────────────────────────
 * Block 0-3   : UID / lock bytes / capability container
 * Block 4+    : user data (TLV-encoded NDEF messages)
 * Last blocks : dynamic lock / CFG (tag-size dependent)
 *
 * Each block is 4 bytes; total user blocks vary by tag variant:
 *   NTAG213 -> blocks 4-39   (144 bytes)
 *   NTAG215 -> blocks 4-129  (496 bytes)
 *   NTAG216 -> blocks 4-225  (888 bytes)
 * We read up to block 135 (same ceiling as the dump example).
 * ─────────────────────────────────────────────────────────────────────────── */

#define MAX_BLOCKS       135
#define BLOCK_SIZE       4
#define USER_START_BLOCK 4      /* first data block */

/* TLV tags */
#define TLV_NULL         0x00
#define TLV_NDEF         0x03
#define TLV_TERMINATOR   0xFE

/* NDEF TNF (Type Name Format) mask */
#define NDEF_TNF_MASK        0x07
#define NDEF_TNF_WELL_KNOWN  0x01

/* NDEF flags */
#define NDEF_FLAG_MB  0x80  /* Message Begin */
#define NDEF_FLAG_ME  0x40  /* Message End */
#define NDEF_FLAG_SR  0x10  /* Short Record */

/* NDEF Well-Known Type: "T" = Text */
#define NDEF_TYPE_TEXT 'T'

/* ── Helper: print raw hex bytes ─────────────────────────────────────────── */
static void print_hex(const uint8_t *data, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

/* ── Parse and print one NDEF Text record payload ────────────────────────── */
/*
 * Text record payload format (RFC 5646):
 *   Byte 0       : status byte - bit7=UTF16, bits5-0=lang length
 *   Bytes 1..L   : language code (e.g. "en")
 *   Bytes L+1..N : text (UTF-8 or UTF-16)
 */
static void parse_ndef_text(const uint8_t *payload, uint32_t payload_len)
{
    if (payload_len < 1) {
        printf("[!] Payload too short\n");
        return;
    }

    uint8_t status   = payload[0];
    uint8_t lang_len = status & 0x3F;      /* lower 6 bits */
    int is_utf16     = (status & 0x80) != 0;

    if ((uint32_t)(1 + lang_len) >= payload_len) {
        printf("[!] Malformed Text record\n");
        return;
    }

    /* Language code */
    char lang[64] = {0};
    memcpy(lang, payload + 1, lang_len);

    /* Text content */
    const uint8_t *text = payload + 1 + lang_len;
    uint32_t text_len   = payload_len - 1 - lang_len;

    printf("Encoding: %s\n", is_utf16 ? "UTF-16" : "UTF-8");
    printf("Language: %s\n", lang);
    printf("Text: ");

    if (is_utf16) {
        /* Naive UTF-16LE print - strips high byte */
        for (uint32_t i = 0; i + 1 < text_len; i += 2) {
            putchar(text[i]);
        }
    } else {
        /* UTF-8: just print as-is (terminal must support UTF-8) */
        for (uint32_t i = 0; i < text_len; i++) {
            putchar(text[i]);
        }
    }
    putchar('\n');
}

/* ── Walk the TLV stream in the user-data buffer ────────────────────────── */
static void parse_ndef_tlv(const uint8_t *data, size_t data_len)
{
    size_t pos = 0;
    int found_any = 0;

    while (pos < data_len) {
        uint8_t tag = data[pos++];

        if (tag == TLV_NULL) { continue; }
        if (tag == TLV_TERMINATOR) { break; }

        if (pos >= data_len) break;

        /* TLV length - extended (3-byte) form if 0xFF */
        uint32_t length;
        if (data[pos] == 0xFF) {
            if (pos + 2 >= data_len) break;
            length = ((uint32_t)data[pos + 1] << 8) | data[pos + 2];
            pos += 3;
        } else {
            length = data[pos++];
        }

        if (pos + length > data_len) {
            printf("[!] TLV value exceeds buffer - tag 0x%02X, len %u\n",
                   tag, length);
            break;
        }

        if (tag != TLV_NDEF) {
            pos += length; /* skip unknown TLV */
            continue;
        }

        /* ── Found an NDEF message TLV ─────────────────────────── */
        const uint8_t *ndef_msg = data + pos;
        size_t ndef_remain = length;
        pos += length;

        /* Walk individual NDEF records inside the message */
        size_t rpos = 0;
        while (rpos < ndef_remain) {
            if (rpos + 3 > ndef_remain) break;

            uint8_t flags    = ndef_msg[rpos++];
            uint8_t type_len = ndef_msg[rpos++];

            /* Payload length: SR flag -> 1 byte, else 4 bytes */
            uint32_t payload_len;
            if (flags & NDEF_FLAG_SR) {
                if (rpos >= ndef_remain) break;
                payload_len = ndef_msg[rpos++];
            } else {
                if (rpos + 4 > ndef_remain) break;
                payload_len = ((uint32_t)ndef_msg[rpos    ] << 24) |
                              ((uint32_t)ndef_msg[rpos + 1] << 16) |
                              ((uint32_t)ndef_msg[rpos + 2] << 8)  |
                              (uint32_t)ndef_msg[rpos + 3];
                rpos += 4;
            }

            /* ID length (if IL flag set) */
            uint8_t id_len = 0;
            if (flags & 0x08) { /* IL flag */
                if (rpos >= ndef_remain) break;
                id_len = ndef_msg[rpos++];
            }

            if (rpos + type_len + id_len + payload_len > ndef_remain) break;

            const uint8_t *type = ndef_msg + rpos; rpos += type_len;
            /* skip ID */ rpos += id_len;
            const uint8_t *payload = ndef_msg + rpos; rpos += payload_len;

            uint8_t tnf = flags & NDEF_TNF_MASK;

            printf("\n[NDEF Record]\n");
            printf("  TNF  : 0x%02X (%s)\n", tnf,
                   tnf == NDEF_TNF_WELL_KNOWN ? "Well-Known" : "other");

            if (type_len > 0) {
                printf("  Type : %c\n", type[0]);
            }

            if (tnf == NDEF_TNF_WELL_KNOWN &&
                type_len == 1 && type[0] == NDEF_TYPE_TEXT) {
                parse_ndef_text(payload, payload_len);
                found_any = 1;
            } else {
                printf("  Payload : ");
                print_hex(payload, payload_len);
            }

            if (flags & NDEF_FLAG_ME) break; /* last record in message */
        }
    }

    if (!found_any) {
        printf("[i] No NDEF Text records found on this card.\n");
    }
}

/* ── main ────────────────────────────────────────────────────────────────── */
int main(void)
{
    uint8_t block_buf[BLOCK_SIZE];
    uint8_t uid[MIFARE_UID_MAX_LENGTH];
    uint8_t user_data[MAX_BLOCKS * BLOCK_SIZE];
    uint32_t user_data_len = 0;

    printf("=== PN532 NDEF Text Reader ===\n\n");

    PN532 pn532;
    /* Choose ONE init method to match your hardware wiring: */
    PN532_I2C_Init(&pn532);
    /* PN532_SPI_Init(&pn532); */
    /* PN532_UART_Init(&pn532); */

    uint8_t fw[4];
    if (PN532_GetFirmwareVersion(&pn532, fw) != PN532_STATUS_OK) {
        fprintf(stderr, "[!] PN532 not found. Check wiring.\n");
        return -1;
    }
    printf("[+] PN532 detected - firmware v%d.%d\n\n", fw[1], fw[2]);

    PN532_SamConfiguration(&pn532);

    printf("Waiting for NFC card...\n");

    /* ── Poll until a card is presented ──────────────────────────────── */
    int32_t uid_len;
    while (1) {
        uid_len = PN532_ReadPassiveTarget(&pn532, uid,
                                           PN532_MIFARE_ISO14443A, 1000);
        if (uid_len != PN532_STATUS_ERROR) break;
        printf(".");
        fflush(stdout);
    }

    printf("\n[+] Card detected!\n");
    printf("    UID: ");
    for (int32_t i = 0; i < uid_len; i++) {
        printf("%02X ", uid[i]);
    }
    printf("(%d bytes)\n\n", uid_len);

    /* ── Read user data blocks ───────────────────────────────────────── */
    printf("Reading data blocks...\n");
    for (uint8_t blk = USER_START_BLOCK; blk < MAX_BLOCKS; blk++) {
        uint32_t err = PN532_Ntag2xxReadBlock(&pn532, block_buf, blk);
        if (err != PN532_ERROR_NONE) {
            /* End of readable area (e.g. hit config pages) */
            break;
        }
        memcpy(user_data + user_data_len, block_buf, BLOCK_SIZE);
        user_data_len += BLOCK_SIZE;
    }

    printf("[+] Read %u bytes of user data.\n\n", user_data_len);

    /* ── Parse TLV / NDEF ────────────────────────────────────────────── */
    printf("Parsing NDEF records...\n");
    parse_ndef_tlv(user_data, user_data_len);

    printf("\nDone.\n");
    return 0;
}
