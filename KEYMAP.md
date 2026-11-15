# Charybdis 4x6: vendor-style QWERTY for ZMK

Based on [BastardKB's official vendor layout](https://github.com/Bastardkb/qmk_userspace/blob/main/keyboards/bastardkb/charybdis/4x6/keymaps/vendor/keymap.c), adapted for this wireless nice!nano / PMW3610 build. The QWERTY letters and 1-0 row are plain keys. No letter hold-taps, gaming toggles, B+N combo, RGB commands or external-power toggle.

## Thumb keys (top view, outer-to-inner on the left)

| Position | Base action |
|---|---|
| 48, left upper outer | GUI (Command / Windows) |
| 49, left upper middle | Space |
| 50, left upper inner | Hold LOWER |
| 51, right upper inner | Hold RAISE |
| 52, right upper outer | Enter |
| 53, left lower outer | Hold SETUP |
| 54, left lower inner | Backspace |
| 55, right lower | Delete |

Shift / Ctrl remain on the left edge; Alt is at the right end of the bottom letter row. Unlike the previous layout, the top-right key is minus, and the key below it is backslash. Backspace is on the left thumb.

## Layers

| Index | Name | Access / purpose |
|---|---|---|
| 0 | BASE | QWERTY and numbers |
| 1 | LOWER | Hold 50: shifted symbols and right-hand keypad |
| 2 | RAISE | Hold 51: F-keys, left-hand arrows and media |
| 3 | POINTER | Automatic mouse layer on ball motion |
| 4 | MOUSE_HOLD | Hold LOWER + Tab: same mouse buttons, no automatic timeout |
| 5 | SCROLL | On POINTER, hold X: vertical scrolling |
| 6 | SNIPING | On POINTER, hold C: cursor at one-third normal speed |
| 7 | SETUP | Hold 53: Bluetooth and output selection |

All layer access is momentary. LOWER, RAISE and SETUP remain on the same physical keys on every layer; SETUP is above the automatic pointer layer. No combo timing is required.

LOWER keypad: U/I/O = 7/8/9, J/K/L = 4/5/6, M/comma/dot = 1/2/3, right lower thumb = 0. LOWER + T toggles Num Lock (needed by some operating systems). LOWER + left upper outer thumb = keypad decimal.

RAISE: number keys 1-0 give F1-F10, minus gives F11, Esc gives F12. A/S/D/F = Left/Up/Down/Right, Z/X/C/V = Home/Page Up/Page Down/End. Q = next track, left Shift = play/pause, left Ctrl = previous track, backslash = volume up, P = volume down, apostrophe = mute.

## Bluetooth and USB

Hold SETUP (53), then:

- 1-5: select profiles 1-5 (ZMK indexes 0-4).
- Backspace (54): clear **only the selected** profile's pairing.
- U: force USB output.
- B: force Bluetooth output.
- Esc: return to BASE (recovery; release all held keys afterward).

Profile selection does not force Bluetooth output. If USB is attached, use SETUP+B when you want Bluetooth. To pair again, also forget the keyboard on the computer.

## Trackball

The existing CPI, axis orientation and normal cursor scale are preserved. Motion enables POINTER for 700 ms, with a 250 ms prior-typing guard. Ordinary typing exits the automatic mouse layer; mouse actions are exempt. Manual mouse access uses a separate layer, so the automatic timer cannot switch off the manually held layer.

On POINTER: left thumb Space (49) = left click; GUI (48) / Backspace (54) = right click; right lower thumb Delete (55) = middle click. Hold LOWER + Tab for a deterministic manual pointer layer, including during drag operations. Keep both held while using the mouse buttons. To scroll hold X on POINTER; C enables precision motion. Scrolling retains the previous vertical direction and 1/14 scale; precision uses 3/10 versus the normal 9/10 scale.

Intentional deviations from the official QMK map: plain Z instead of Z/POINTER hold-tap; SETUP replaces redundant left-thumb Alt; manual POINTER replaces unused/RGB Tab positions on LOWER and the next-track position on RAISE; next-track moves to RAISE+Q. Layer access remains available on POINTER, so its middle click moves to the right lower thumb. QMK-specific gestures and runtime DPI controls are not emulated. No automatic precision slowdown is applied just because POINTER activates.

## Installing this change

Use `charybdis_4x6_LEFT.uf2` and `charybdis_4x6_RIGHT.uf2` from the same successful BT build. Do not use dongle/wired artifacts for the customer's keyboard.

**After flashing, connect the right half to ZMK Studio over USB and run Restore Stock Settings, then save if prompted.** Stored Studio remaps can otherwise hide the new keymap. Back up the previous layout first. A full `settings_reset` is not required and would erase pairings.

The customer PDF must be version 3.0 or later for this layout; version 2.0 describes the previous FN/GAME layout.
