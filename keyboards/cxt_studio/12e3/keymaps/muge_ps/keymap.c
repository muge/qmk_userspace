// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#define TAP_TIME_DEF 175

enum my_layers {
    _BASE,
    _PS,
    _RGBL,
};

enum custom_keycodes {
    TH_DX = SAFE_RANGE,
    CCUNDO, CCREDO,
    TH_BS, TH_JE, TH_VW, TH_GSP, TH_ML, TH_F1D, TH_05, TH_1T, TH_CX, TH_PST, TH_SAV,
    PS_ZI, PS_ZO,
};

const uint16_t PROGMEM pslayeron_combo[] = {KC_ESC, C(KC_X), C(KC_C), COMBO_END};
const uint16_t PROGMEM pslayeroff_combo[] = {KC_C, KC_D, COMBO_END};
combo_t key_combos[] = {
    COMBO(pslayeron_combo, TG(1)),
    COMBO(pslayeroff_combo, TG(1)), // keycodes with modifiers are possible too!
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_BASE] = LAYOUT(
        KC_ESC, C(KC_X), C(KC_C), C(KC_V),  MS_BTN3, RM_TOGG,
        KC_PSCR,C(KC_Z), KC_CALC, KC_MNXT,     KC_MPLY,
        MO(2), KC_LGUI, KC_DEL, KC_APP
    ),

    [_PS] = LAYOUT(
        KC_ESC, C(KC_X), C(KC_C), C(KC_V),  KC_F5, C(KC_1),
        C(KC_Z),C(KC_D), C(KC_J), C(KC_T),     KC_MPLY,
        MO(2),  KC_DEL,  KC_APP,  KC_SPC
    ),

    [_RGBL] = LAYOUT(
        RM_NEXT, RM_SATU, KC_INS,  KC_DEL,   _______, _______,
        RM_PREV, RM_SATD, KC_PGUP, KC_HOME,     KC_MUTE,
        _______, QK_BOOT, KC_PGDN, KC_END
    ),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    // Encoders: Left, Right, Big
    [_BASE] = {
        ENCODER_CCW_CW(MS_WHLD, MS_WHLU),
        ENCODER_CCW_CW(KC_PGDN, KC_PGUP),
        ENCODER_CCW_CW(KC_VOLD, KC_VOLU)
    },
    [_PS] = {
        ENCODER_CCW_CW(KC_LBRC, KC_RBRC),
        ENCODER_CCW_CW(A(MS_WHLD), A(MS_WHLU)),
        ENCODER_CCW_CW(KC_VOLD, KC_VOLU)
    },
    [_RGBL] = {
        ENCODER_CCW_CW(RM_HUED, RM_HUEU),
        ENCODER_CCW_CW(RM_SPDD, RM_SPDU),
        ENCODER_CCW_CW(RM_VALD, RM_VALU)
    },
};
#endif


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case PS_ZI:
            register_mods(MOD_BIT(KC_LALT));
            tap_code16(MS_WHLU);
            unregister_mods(MOD_BIT(KC_LALT));
            return true;
        case PS_ZO:
            register_mods(MOD_BIT(KC_LALT));
            tap_code16(MS_WHLD);
            unregister_mods(MOD_BIT(KC_LALT));
            return true;
        case CCUNDO:
            register_mods(MOD_BIT(KC_LCTL));
            tap_code16(KC_Z);
            unregister_mods(MOD_BIT(KC_LCTL));
            return true;
        case CCREDO:
            register_mods(MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT));
            tap_code16(KC_Z);
            unregister_mods(MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT));
            return true;
        case TH_DX:
            static uint16_t cdx_timer;
            if (record->event.pressed) {
                cdx_timer = timer_read();
            } else {
                if (timer_elapsed(cdx_timer) < TAP_TIME_DEF) {
                    tap_code16(KC_D);
                } else {
                    tap_code16(KC_X);
                }
            }
            return false;
        case TH_ML:
            static uint16_t cml_timer;
            if (record->event.pressed) {
                cml_timer = timer_read();
            } else {
                if (timer_elapsed(cml_timer) < TAP_TIME_DEF) {
                    tap_code16(KC_M);
                } else {
                    tap_code16(KC_L);
                }
            }
            return false;

        case TH_BS:
            static uint16_t cbs_timer;
            if (record->event.pressed) {
                cbs_timer = timer_read();
            } else {
                if (timer_elapsed(cbs_timer) < TAP_TIME_DEF) {
                    tap_code16(KC_B);
                } else {
                    tap_code16(KC_S);
                }
            }
            return false;
        case TH_GSP:
            static uint16_t cgsp_timer;
            if (record->event.pressed) {
                cgsp_timer = timer_read();
                if (timer_elapsed(cgsp_timer) > TAP_TIME_DEF) {
                    tap_code16(KC_SPC);
                }
            } else {
                if (timer_elapsed(cgsp_timer) < TAP_TIME_DEF) {
                    tap_code16(KC_G);
                } else {
                    //tap_code16(KC_SPC);
                }
            }
            return false;
        case TH_VW:
            static uint16_t cvw_timer;
            if (record->event.pressed) {
                cvw_timer = timer_read();
            } else {
                if (timer_elapsed(cvw_timer) < TAP_TIME_DEF) {
                    tap_code16(KC_V);
                } else {
                    tap_code16(KC_W);
                }
            }
            return false;
        case TH_JE:
            static uint16_t cje_timer;
            if (record->event.pressed) {
                cje_timer = timer_read();
            } else {
                if (timer_elapsed(cje_timer) < TAP_TIME_DEF) {
                    register_mods(MOD_BIT(KC_LCTL));
                    tap_code16(KC_J);
                    unregister_mods(MOD_BIT(KC_LCTL));
                } else {
                    tap_code16(KC_E);
                }
            }
            return false;
        case TH_F1D:
            static uint16_t cfd_timer;
            if (record->event.pressed) {
                cfd_timer = timer_read();
            } else {
                if (timer_elapsed(cfd_timer) < TAP_TIME_DEF) {
                    tap_code16(KC_F);
                } else {
                    register_mods(MOD_BIT(KC_LCTL));
                    tap_code16(KC_D);
                    unregister_mods(MOD_BIT(KC_LCTL));
                }
            }
            return false;
        case TH_05:
            static uint16_t c05_timer;
            if (record->event.pressed) {
                c05_timer = timer_read();
            } else {
                if (timer_elapsed(c05_timer) < TAP_TIME_DEF) {
                    tap_code16(KC_0);
                } else {
                    tap_code16(KC_5);
                }
            }
            return false;
        case TH_1T:
            static uint16_t c1t_timer;
            if (record->event.pressed) {
                c1t_timer = timer_read();
            } else {
                if (timer_elapsed(c1t_timer) < TAP_TIME_DEF) {
                    register_mods(MOD_BIT(KC_LCTL));
                    tap_code16(KC_1);
                    unregister_mods(MOD_BIT(KC_LCTL));
                } else {
                    register_mods(MOD_BIT(KC_LCTL));
                    tap_code16(KC_T);
                    unregister_mods(MOD_BIT(KC_LCTL));
                }
            }
            return false;
        case TH_CX:
            static uint16_t ccx_timer;
            if (record->event.pressed) {
                ccx_timer = timer_read();
            } else {
                if (timer_elapsed(ccx_timer) < TAP_TIME_DEF) {
                    register_mods(MOD_BIT(KC_LCTL));
                    tap_code16(KC_C);
                    unregister_mods(MOD_BIT(KC_LCTL));
                } else {
                    register_mods(MOD_BIT(KC_LCTL));
                    tap_code16(KC_X);
                    unregister_mods(MOD_BIT(KC_LCTL));
                }
            }
            return false;
        case TH_PST:
            static uint16_t cpst_timer;
            if (record->event.pressed) {
                cpst_timer = timer_read();
            } else {
                if (timer_elapsed(cpst_timer) < TAP_TIME_DEF) {
                    register_mods(MOD_BIT(KC_LCTL));
                    tap_code16(KC_V);
                    unregister_mods(MOD_BIT(KC_LCTL));
                } else {
                    register_mods(MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT));
                    tap_code16(KC_V);
                    unregister_mods(MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT));
                }
            }
            return false;
        case TH_SAV:
            static uint16_t csav_timer;
            if (record->event.pressed) {
                csav_timer = timer_read();
            } else {
                if (timer_elapsed(csav_timer) < TAP_TIME_DEF) {
                    register_mods(MOD_BIT(KC_LCTL));
                    tap_code16(KC_S);
                    unregister_mods(MOD_BIT(KC_LCTL));
                } else {
                    register_mods(MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT));
                    tap_code16(KC_S);
                    unregister_mods(MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT));
                }
            }
            return false;

    }
    return true;
};
