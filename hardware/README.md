# Hardware — Word Wizard PCB

Custom carrier/IO board for the Raspberry Pi Compute Module 4, designed in **KiCad 9.0.4**.

| Sheet | Contents |
|---|---|
| `CM4 IO Board` (top) | CM4 module footprint, mounting holes, sheet references |
| `CM4_GPIO` | 5x GPIO breakout, I2C (NFC reader), I2S audio, boot jumpers |
| `CM4_HighSpeed` | Micro-USB (CM4 programming) with ESD protection, USB OTG |
| `Power Management` | BQ25886 Li-Ion charger, USB-C power-only input, 5V/5A LDO, status LEDs |
| `DAC` (under CM4_GPIO) | PCM5122 I2S DAC → MAX98357A Class-D audio amplifier |

## Key design notes

- **Power path:** USB-C (power only) → BQ25886 charge management → 2S Li-Ion pack → LM1084-5.0 LDO for a clean 5V rail to electronics.
  - R2 (8kΩ) sets ~2.2A charge current (~0.4C for the battery)
  - R3 (150kΩ) sets 8.4V charging voltage
  - R4 (383Ω) sets ~2.9A max input current
  - R7/R9 (5.1kΩ on CC pins) configure 5V/3A charge negotiation over USB-C
- **NFC reader:** PN532, I2C interface, connected via the GPIO breakout sheet.
- **Audio:** PCM5122 I2S DAC feeding a MAX98357A mono Class-D amplifier (gain set by R1: 0Ω = left channel / 9dB gain).
- **Programming:** Micro-USB OTG port for flashing the CM4's eMMC, protected by a TPD4EUSB30 ESD array.

## ⚠️ Known erratum — GPIO_VREF must be tied, not left floating

On the first board revision, **GPIO_VREF (CM4 pin 78)** was left floating, which averaged to ~2.3V and caused the CM4 to enter a thermal fail-safe shutdown roughly 20–30 seconds after boot (see the [PCB fault deep-dive in the main README](../README.md#engineering-deep-dive-the-pcb-fault)). If you're laying out your own revision of this board, **hard-tie GPIO_VREF directly to 3.3V** in the schematic and layout — don't rely on external pull-ups alone.
