# DIY 4x4 RP2040 Vial Macropad

This repository directory defines a 16-key orthogonal keypad for a Raspberry Pi Pico (RP2040). It uses a diode-less 4-row x 4-column switch matrix, VIA/Vial dynamic keymaps, four firmware layers, mouse/media keys, and host OS detection.

The project name is `diy_macropad`, the USB VID/PID is `0xFEED` / `0x4416`, and the USB device version is `1.0.0`.

## 1. Project Architecture and Source of Truth

The following table identifies the file that owns each project setting. Vial GUI layouts and firmware keymaps are separate: `vial.json` describes the 16 physical positions to Vial, while `keymap.c` supplies the default firmware key assignments.

| Project area | Source file | Exact configured values |
|---|---|---|
| MCU and bootloader | `info.json` | `RP2040`, bootloader `rp2040` |
| Matrix direction and pinout | `info.json` | `COL2ROW`; rows `GP2`, `GP3`, `GP4`, `GP5`; columns `GP6`, `GP7`, `GP8`, `GP9` |
| USB IDs and layout | `info.json` | VID `0xFEED`, PID `0x4416`, device version `1.0.0`, 4x4 `LAYOUT_ortho_4x4` |
| Debouncing | `config.h` | `DEBOUNCE 5` |
| Dynamic capacity | `config.h` | 4 dynamic keymap layers, 16 dynamic macros |
| Vial identity and unlock | `keymaps/vial/config.h` | UID `{0xD4, 0x19, 0xA6, 0x37, 0x5E, 0xC2, 0x8B, 0xF1}`; unlock rows `{0, 0}`; unlock columns `{0, 3}` |
| Vial feature capacity | `keymaps/vial/config.h` | 16 tap dances, 16 combos, 8 key overrides |
| Enabled QMK/Vial features | `info.json` | VIA, Vial, extra keys, mouse keys, tap dance, combos, key overrides, OS detection, NKRO |
| Build feature files | `rules.mk` and `keymaps/vial/rules.mk` | Both files contain only an explanatory comment. This project uses `info.json` as the feature source of truth for this build |
| Default 4-layer keymap | `keymaps/vial/keymap.c` | `_BASE`, `_MEDIA`, `_DEV`, and `_NUMPAD` |
| OS detection hook | `keymaps/vial/keymap.c` | `process_detected_host_os_kb()` receives Linux, Windows, macOS, iOS, or unsure status |
| Vial GUI physical layout | `keymaps/vial/vial.json` | 4 rows x 4 columns, positions `(0,0)` through `(3,3)` |

The data-driven `info.json` enables:

- `vial`
- `via`
- `extrakey`
- `mousekey`
- `tap_dance`
- `combo`
- `key_override`
- `os_detection`
- `nkro`

Bootmagic is disabled in `info.json`.

## 2. Hardware Wiring and ASCII Pinout Diagram

The keypad uses no diodes. Wires or traces connect every switch from a row to a column. A matrix position is active when the firmware drives or scans the matching row/column pair.

### Exact row mapping

| Keypad row | RP2040 GPIO |
|---|---|
| R0 | GP2 |
| R1 | GP3 |
| R2 | GP4 |
| R3 | GP5 |

### Exact column mapping

| Keypad column | RP2040 GPIO |
|---|---|
| C0 | GP6 |
| C0 key labels | Matrix column 0 |
| C1 | GP7 |
| C2 | GP8 |
| C3 | GP9 |

The C0 entry in the table is split only to clarify that this keypad column maps to matrix column 0.

### 4x4 matrix positions

```text
Vial / firmware matrix                  Keypad-to-Pico wiring

          Col 0   Col 1   Col 2   Col 3
          GP6     GP7     GP8     GP9
          +       +       +       +
          |       |       |       |
Row 0 GP2 +-------+-------+-------+-------+
          | 0,0   | 0,1   | 0,2   | 0,3   |
          +-------+-------+-------+-------+
Row 1 GP3 | 1,0   | 1,1   | 1,2   | 1,3   |
          +-------+-------+-------+-------+
Row 2 GP4 | 2,0   | 2,1   | 2,2   | 2,3   |
          +-------+-------+-------+-------+
Row 3 GP5 | 3,0   | 3,1   | 3,2   | 3,3   |
          +-------+-------+-------+-------+

Keypad pin R0 -> GP2
Keypad pin R1 -> GP3
Keypad pin R2 -> GP4
Keypad pin R3 -> GP5

Keypad pin C0 -> GP6
Keypad pin C1 -> GP7
Keypad pin C2 -> GP8
Keypad pin C3 -> GP9
```

Wire each row and column pin directly to the corresponding Pico GPIO. Do not insert diodes. Verify that every keypad row and column is unique and that the physical keypad is wired as shown above before installing it permanently.

## 3. Compiling and Flashing

Run the compile from the repository root:

```bash
qmk compile -kb diy_macropad -km vial
```

The build creates:

```text
.build/diy_macropad_vial.uf2
```

A successful build is a UF2 image for the RP2040 in that location.

### Flash through BOOTSEL

1. Disconnect the Pico or macropad USB cable.
2. Press and hold the Pico **BOOTSEL** button.
3. Connect USB while continuing to hold BOOTSEL.
4. Release BOOTSEL after the Pico mounts.
5. The host shows an **RPI-RP2** removable drive.
6. Copy `diy_macropad_vial.uf2` from `.build/` to the `RPI-RP2` drive.

On Linux, for example:

```bash
cp .build/diy_macropad_vial.uf2 /media/$USER/RPI-RP2/
```

The exact mount point can differ on Linux. Check `/media/$USER/`, `/run/media/$USER/`, or use your desktop file manager. The Pico reboots automatically after it accepts the UF2 file.

## 4. How to Use and Remap Keys

### Linux setup

Install the Vial udev rule with:

```bash
sudo tee /etc/udev/rules.d/99-vial.rules >/dev/null <<'EOF'
KERNEL=="hidraw*", SUBSYSTEM=="hidraw", ATTRS{serial}=="*", TAG+="uaccess", TAG+="udev-acl"
EOF
sudo udevadm control --reload-rules
sudo udevadm trigger
```

Afterward, disconnect and reconnect the macropad. Log out and back in if your desktop session needs to refresh `uaccess` permissions.

Open either of these:

- Vial Desktop on Linux
- `https://vial.rocks` in a Chromium-family browser that supports WebHID

Select the detected `diy_macropad` or `vial` device. WebHID can ask for device permission on first connection.

### Windows setup

Open Vial Desktop or `https://vial.rocks` in a supported Chromium-family browser. The firmware uses standard HID interfaces and does not require a custom INF or vendor driver. Grant the WebHID device permission in the browser if prompted.

### Daily remapping

1. Connect the macropad.
2. Open Vial Desktop or `vial.rocks`.
3. Confirm that the 4x4 layout is recognized.
4. Select a physical key in the GUI.
5. Choose a keycode, layer action, macro, tap dance, combo, or key override.
6. Apply the change. Vial writes dynamic settings into the device's EEPROM without compiling new firmware.

The GUI layout matrix is:

```text
0,0  0,1  0,2  0,3
1,0  1,1  1,2  1,3
2,0  2,1  2,2  2,3
3,0  3,1  3,2  3,3
```

### Layer 0: Base/Productivity

| Matrix position | Default assignment |
|---|---|
| `(0,0)` | Copy: Ctrl+C |
| `(0,1)` | Paste: Ctrl+V |
| `(0,2)` | Cut: Ctrl+X |
| `(0,3)` | Undo: Ctrl+Z |
| `(1,0)` | Redo: Ctrl+Y |
| `(1,1)` | Select All: Ctrl+A |
| `(1,2)` | Save: Ctrl+S |
| `(1,3)` | Find: Ctrl+F |
| `(2,0)` | Close tab/window shortcut: Ctrl+W |
| `(2,1)` | Reopen closed tab: Ctrl+Shift+T |
| `(2,2)` | Print Screen |
| `(2,3)` | Application/Menu key |
| `(3,0)` | Momentary Media layer |
| `(3,1)` | Toggle Media layer |
| `(3,2)` | Jump to Developer layer |
| `(3,3)` | Jump to Numpad/Mouse layer |

`MO(_MEDIA)` activates Layer 1 while held. `TG(_MEDIA)` toggles its state. `TO(_DEV)` switches directly to Layer 2. `TO(_NUMPAD)` switches directly to Layer 3.

### Layer 1: Media and System

| Matrix position | Default assignment |
|---|---|
| `(0,0)` | Volume Up |
| `(0,1)` | Volume Down |
| `(0,2)` | Mute |
| `(0,3)` | Play/Pause |
| `(1,0)` | Next Track |
| `(1,1)` | Previous Track |
| `(1,2)` | Brightness Up |
| `(1,3)` | Brightness Down |
| `(2,0)` | Calculator |
| `(2,1)` | GUI/Windows/Super + L |
| `(2,2)` | Application/Menu key |
| `(2,3)` | My Computer |
| `(3,0)` | Jump to Base |
| `(3,1)` | Jump to Developer |
| `(3,2)` | Jump to Numpad/Mouse |
| `(3,3)` | Reboot into bootloader |

The lock action is `LGUI(KC_L)`, so its behavior follows the host's implementation of that key combination.

### Layer 2: Developer / Extended Functions

| Matrix positions | Default assignments |
|---|---|
| `(0,0)`–`(0,3)` | F13, F14, F15, F16 |
| `(1,0)`–`(1,3)` | F17, F18, F19, F20 |
| `(2,0)`–`(2,3)` | F21, F22, F23, F24 |
| `(3,0)` | Ctrl+Backtick |
| `(3,1)` | Ctrl+B |
| `(3,2)` | Ctrl+P |
| `(3,3)` | Jump to Base |

### Layer 3: Numpad and Mouse

| Matrix position | Default assignment |
|---|---|
| `(0,0)` | Keypad 7 |
| `(0,1)` | Keypad 8 |
| `(0,2)` | Keypad 9 |
| `(0,3)` | Keypad Enter |
| `(1,0)` | Keypad 4 |
| `(1,1)` | Keypad 5 |
| `(1,2)` | Keypad 6 |
| `(1,3)` | Backspace |
| `(2,0)` | Keypad 1 |
| `(2,1)` | Keypad 2 |
| `(2,2)` | Keypad 3 |
| `(2,3)` | Keypad 0 |
| `(3,0)` | Mouse Up |
| `(3,1)` | Mouse Down |
| `(3,2)` | Left Mouse Button |
| `(3,3)` | Right Mouse Button |

This layer contains no dedicated mouse left/right movement or scroll keys. It does provide mouse up/down and the two primary buttons.

### OS Detection

The firmware includes `process_detected_host_os_kb()` and imports `os_detection.h`. QMK's OS detection can report Linux, Windows, macOS, iOS, or unsure status.

In this exact project, the callback stores no separate OS state or remap: it receives the detected OS and then returns normally. Therefore, keys remain freely remappable in Vial and do not change behavior automatically based on the detected host. Add custom `process_record_*` logic and rebuild if you need OS-specific keycode behavior.

### Tap Dance

The Vial configuration reserves 16 tap-dance entries. Open the Vial Tap Dance editor to assign a key's:

- single-tap action
- double-tap action
- hold action
- and other supported tap-dance timing behavior

The capacity is available to Vial, but no custom tap-dance function or preconfigured tap-dance key is present in `keymap.c`.

### Combos

The Vial configuration reserves 16 combo entries. In Vial, select two or more configured positions and assign the key action emitted when the chord is pressed.

The fixed physical unlock combination is separate from normal combos. It is:

```text
Top-left key  (0,0)
Top-right key (0,3)
```

Both keys correspond to the row/column arrays:

```c
VIAL_UNLOCK_COMBO_ROWS { 0, 0 }
VIAL_UNLOCK_COMBO_COLS { 0, 3 }
```

Hold both unlock keys together according to Vial's unlock behavior. The unlock combo does not remap or alter the firmware layers itself.

### Key Overrides

The firmware reserves 8 Vial key override entries. Vial can use them to replace or modify key behavior based on active modifiers. This project does not define any specific override in `keymap.c`.

### Dynamic Macros

`DYNAMIC_KEYMAP_MACRO_COUNT` is 16, so Vial exposes 16 macro slots. Create multi-key sequences or text snippets in Vial and assign a macro key to a dynamic keymap position. Save the layout to persist and transfer your configuration.

## 5. Back Up and Restore Layouts

In Vial:

1. Open the layout/keymap backup tools.
2. Export the layout to a `.vil` file.
3. Store the `.vil` file outside the keyboard or repository so it is not lost when firmware is erased or reflashed.

To restore:

1. Connect the macropad.
2. Open Vial Desktop or `vial.rocks`.
3. Load the saved `.vil` file using Vial's layout restore/import operation.
4. Apply it to the macropad.

A `.vil` file is the layout backup for Vial's dynamic configuration. Firmware flashing replaces the flashed firmware image and may affect persisted dynamic keymap data if NVRAM or EEPROM is cleared or changed. Keep a separate `.vil` copy of the layout before major firmware changes.
