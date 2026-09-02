# Design — CAD & Mechanical

Above

## Assembly Overview

**Overall dimensions (L×W×H):** 216.00 × 125.40 × 162.18 mm
**Total weight:** ~951 g

| # | Part | Material | Weight (g) | Manufacturing |
|---|---|---|---|---|
| 1 | Drawer Box | PLA | 237 | FDM 3D printed |
| 2 | Drawer | PLA | 302 | FDM 3D printed, ~7h 52m print time |
| 3 | Slider | PLA | 3.55 | FDM 3D printed, ~11 min print time |
| 4 | Slider Guide | PLA | 9.58 | FDM 3D printed, ~23 min print time |
| 5 | Case | PLA | 170 | FDM 3D printed, ~3.75 hr print time (BambuLab A1) |
| 6 | Lid | Cast Acrylic | 118 | — |
| 7 | Speaker Gasket | TPU | 0.47 | FDM 3D printed |
| 8 | Bumper Bottom | TPU | 52.4 | FDM 3D printed, ~2.8 hr print time |
| 9 | Bumper Top | TPU | 58.2 | FDM 3D printed, ~2.9 hr print time |

**Misc. assembly hardware:** 4x M3×5mm screws + inserts, 4x M4×15mm screws + inserts, cyanoacrylate glue.

## Assembly Notes

- Insert the slider first, and lock it in place with the slider guide.
- Secure the speaker gasket between the speaker and casing before screwing the assembly together.
- Screw the acrylic lid to the case.
- Glue bumpers to the edges of the case (×2).
- The case includes 9x standoffs for the PCB, plus cutouts for the speaker grille, switch port, charging port, and NFC reader slot.
- All fillets on TPU bumper parts are 3mm.
- The drawer/slider system uses a gravity latch so the drawer can be lifted without requiring much dexterity — a deliberate accessibility choice for the primary stakeholder (the student/user).

See the [Future Work section of the main README](../README.md#future-work) for planned mechanical revisions (top-loading card access, injection-molded silicone bumper, single-piece device/storage design).
