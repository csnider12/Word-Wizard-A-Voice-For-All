# Appendix 1: Problem Framing

**Overview:** Problem Framing Documentation for FoD2 Semester Project "Word Wizard"

## Broader Impacts (Summary)

1. **People** — Inclusion and accessibility, reducing stigma around disability.
2. **Planet** — A long-lasting, purpose-built solution allows for less material waste and power consumption.
3. **Profit** — Lower cost and a scalable solution compared to existing assistive technologies.

## Stakeholders

| Stakeholder | Impact | Design Decision Considerations |
|---|---|---|
| Student | Usability, comfort, reliability | Safer casing, minimal simplistic inputs |
| Teachers | Integration and manageability | Small, rechargeable, minimal design, clear colors for locatability |
| Administrators | Budget and policy compliance | Low-tech design, cost less than common alternatives (tablet or repurposed smart device) |
| Parents | Independence and safety | Soft outer casing, simplistic usability for a wide age range |

## Preliminary Research

### Design Round 1 (DR1)

| # | Prompting Question | Source | Info Gathered | Use of Info |
|---|---|---|---|---|
| 1 | How tall are classroom desks and tables? | Dimensions.com | 24"–30" | Informs durability/drop benchmarks |
| 2 | How can we smooth 3D prints? | [Wevolver, BASF] | ABS can be smoothed with acetone, TPU with DMSO | Consider solvent smoothing for surface finish |
| 3 | What is the most common consumer charging cable? | GP Batteries | USB-C is the modern industry standard | Use a USB-C port for power input |
| 4 | What 3D printing filament is most durable? | Ultimaker | ABS has higher impact resistance than PLA/PETG but worse printability | ABS preferred for strength; print carefully |
| 5 | Why use NFC tags instead of alternatives for voice recording? | The Blind Life (YouTube) | NFC offers easy, touch-based playback/recording/updating of audio | Use NFC tags for easy, low-complexity use and updating |
| 6 | What are the issues with paper communication cards? | Christina Teevan, Ashland Special Needs Ministry | Paper cards are not durable, difficult to organize, and require concentration from both parties | Combine physical cards with a text-to-speech device |

### Design Round 2 (DR2)

| # | Prompting Question | Source | Info Gathered | Use of Info |
|---|---|---|---|---|
| 1 | What's an effective dB level for communication, and how does enclosure design affect it? | ASHA Classroom Acoustics | Target output 60–65 dB at listener; upper range 65–70 dB in noisy classrooms | Aim for 60–65 dB output range |
| 2 | How do educators currently store/organize communication cards? | The Autism Helper | Bins and binder pockets are common storage methods | Blend both systems into an organized drawer |

### Design Round 3 (DR3)

| # | Prompting Question | Source | Info Gathered | Use of Info |
|---|---|---|---|---|
| 1 | How can we reduce speaker rattle in the enclosure? | Suzuki & Tanaka, J. Technol. Soc. Sci. | Sealing between speaker and surface dictates sound pressure level | Add a gasket to seal the speaker |
| 2 | How can we further make cards easier to organize? | Fitzgerald (1926); Wu et al., Front. Psychol. 2021 | AAC systems use the Fitzgerald Key to color-code grammatical categories; color coding reduces cognitive load | Color-code the cards for organization and tracking |

## Assumptions

1. Classroom desks/tables are typically 24"–30" tall, so accidental drops of ~3 ft onto hard flooring are likely — the enclosure is designed to withstand at least a 3 ft drop without functional failure.
2. Some non-verbal individuals may also have visual processing challenges, so high-contrast visual cues (colors, iconography) improve usability.
3. In classrooms with many simultaneous interactable objects, cards may be frequently misplaced — a dedicated card storage/tethering solution was integrated.
4. Hygiene practices in schools/therapy settings require frequent surface cleaning — enclosure materials (ABS/TPU/PLA) were selected to tolerate repeated wiping.
5. USB-C cables are commonly included with consumer electronics and are easily purchasable, so a USB-C port was used for power input.

## Design Specifications

| # | Evaluation Metric | Qty. to Measure | Units | Objective | Approach |
|---|---|---|---|---|---|
| 1 | Battery Life | Both | Watt-hour (Wh) | ≥ 30 Wh | Rechargeable battery with safety margin |
| 2 | Boot Time | EM | Seconds (s) | Minimize | Optimize hardware using a compute module |
| 3 | Response Time | EM | Seconds (s) | Minimize | Optimize hardware using a compute module |
| 4 | Sound Level | EM | Decibels (dB) | ≥ 60 dB | Functionally test different speaker grilles |
| 5 | Portability | EM | Grams (g) | Minimize | Minimize unnecessary cables/hardware |
| 6 | Variability of Word List | C | # of cards | ≥ 30 | Diversified vocabulary for versatility |
| 7 | Customizability of Word List | EM | # of ways to change each card | Maximize | Cards changeable on demand via smartphone |
