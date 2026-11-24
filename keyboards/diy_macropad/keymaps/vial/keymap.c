/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H
#include "os_detection.h"

enum macropad_layers {
    _BASE,
    _MEDIA,
    _DEV,
    _NUMPAD
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_ortho_4x4(
        C(KC_C), C(KC_V), C(KC_X), C(KC_Z),
        C(KC_Y), C(KC_A), C(KC_S), C(KC_F),
        C(KC_W), C(S(KC_T)), KC_PSCR, KC_APP,
        MO(_MEDIA), TG(_MEDIA), TO(_DEV), TO(_NUMPAD)
    ),

    [_MEDIA] = LAYOUT_ortho_4x4(
        KC_VOLU, KC_VOLD, KC_MUTE, KC_MPLY,
        KC_MNXT, KC_MPRV, KC_BRIU, KC_BRID,
        KC_CALC, LGUI(KC_L), KC_APP, KC_MYCM,
        TO(_BASE), TO(_DEV), TO(_NUMPAD), QK_BOOT
    ),

    [_DEV] = LAYOUT_ortho_4x4(
        KC_F13, KC_F14, KC_F15, KC_F16,
        KC_F17, KC_F18, KC_F19, KC_F20,
        KC_F21, KC_F22, KC_F23, KC_F24,
        C(KC_GRV), C(KC_B), C(KC_P), TO(_BASE)
    ),

    [_NUMPAD] = LAYOUT_ortho_4x4(
        KC_P7,   KC_P8,   KC_P9,   KC_PENT,
        KC_P4,   KC_P5,   KC_P6,   KC_BSPC,
        KC_P1,   KC_P2,   KC_P3,   KC_P0,
        KC_MS_U, KC_MS_D, KC_BTN1, KC_BTN2
    )
};

bool process_detected_host_os_kb(os_variant_t detected_os) {
    switch (detected_os) {
        case OS_LINUX:
        case OS_WINDOWS:
        case OS_MACOS:
        case OS_IOS:
        case OS_UNSURE:
        default:
            break;
    }

    return true;
}
