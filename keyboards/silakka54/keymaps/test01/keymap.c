// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum layers {
    _BASE,
    _LAY1,
    _LAY2,
};

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Send two single taps
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

//Tapdance Enum
enum {
    TD_ESF1,
    TD_F1LS,
    TD_BSFT,
    TD_NSFT,
    TD_APF2,
    TD_QUF1,
    TD_BSDE,
    TD_SPF2,
};

td_state_t cur_dance(tap_dance_state_t *state);

// tapdance codes
void esf1_finished(tap_dance_state_t *state, void *user_data);
void esf1_reset(tap_dance_state_t *state, void *user_data);
void f1ls_finished(tap_dance_state_t *state, void *user_data);
void f1ls_reset(tap_dance_state_t *state, void *user_data);
void quf1_finished(tap_dance_state_t *state, void *user_data);
void quf1_reset(tap_dance_state_t *state, void *user_data);
void nsft_finished(tap_dance_state_t *state, void *user_data);
void nsft_reset(tap_dance_state_t *state, void *user_data);
void apf2_finished(tap_dance_state_t *state, void *user_data);
void apf2_reset(tap_dance_state_t *state, void *user_data);


// keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_BASE] = LAYOUT(
        TD(TD_ESF1), KC_1, KC_2,    KC_3,   KC_4,    KC_5,                                 KC_6,    KC_7,   KC_8,    KC_9,    KC_0,    KC_MINS,
        KC_TAB,      KC_Q, KC_W,    KC_E,   KC_R,    KC_T,                                 KC_Y,    KC_U,   KC_I,    KC_O,    KC_P,    KC_APP,
        MO(1),       KC_A, KC_S,    KC_D,   KC_F,    KC_G,                                 KC_H,    KC_J,   KC_K,    KC_L,    KC_SCLN, TD(TD_QUF1),
        KC_LSFT,  KC_Z,    KC_X,    KC_C,   KC_V,   KC_B,                                 KC_N,    KC_M,   KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                                           KC_LALT, KC_LCTL, KC_SPC,             TD(TD_SPF2),  KC_ENTER,  KC_BSPC
    ),
    [_LAY1] = LAYOUT(
        KC_GRV,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                              KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_EQL,
        KC_LCTL,  KC_LALT, KC_TRNS, KC_PGUP, KC_TRNS, KC_TRNS,                            KC_F11,  KC_F12,  KC_UP,   KC_TRNS, KC_TRNS, KC_BSLS,
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_PGDN, KC_HOME, KC_END,                             KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_TRNS, KC_TRNS,
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                            KC_TRNS, KC_TRNS, KC_LBRC, KC_RBRC, KC_TRNS, KC_TRNS,
                                            KC_TRNS, KC_TRNS, KC_TRNS,           KC_TRNS,  KC_TRNS,  KC_DEL
    ),
    [_LAY2] = LAYOUT(
        KC_GRV,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                              KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_PSCR,
        KC_LSFT,  KC_LALT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                            KC_F11,  KC_F12,  KC_UP,   KC_END,  KC_DEL,  KC_RGUI,
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_HOME, KC_END,                             KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_TRNS, KC_BSLS,
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                            KC_COMM, KC_DOT,  KC_LBRC, KC_RBRC, KC_DOWN, KC_RGHT,
                                             KC_TRNS, KC_LGUI, KC_TRNS,           KC_TRNS,  KC_TRNS,  KC_DEL
    ),
};

/* Return an integer that corresponds to what kind of tap dance should be executed.
 *
 * How to figure out tap dance state: interrupted and pressed.
 *
 * Interrupted: If the state of a dance is "interrupted", that means that another key has been hit
 *  under the tapping term. This is typically indicative that you are trying to "tap" the key.
 *
 * Pressed: Whether or not the key is still being pressed. If this value is true, that means the tapping term
 *  has ended, but the key is still being pressed down. This generally means the key is being "held".
 *
 * One thing that is currently not possible with qmk software in regards to tap dance is to mimic the "permissive hold"
 *  feature.
 * For the third point, there does exist the 'TD_DOUBLE_SINGLE_TAP', however this is not fully tested
 *
 */
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    }

    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TD_TRIPLE_SINGLE_TAP', and define that enum just like 'TD_DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted || !state->pressed) return TD_TRIPLE_TAP;
        else return TD_TRIPLE_HOLD;
    } else return TD_UNKNOWN;
}

/* -----------------------
 *  Tapdances start here!
 * -----------------------
 */

// ESF1
static td_tap_t esf1_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void esf1_finished(tap_dance_state_t *state, void *user_data) {
    esf1_tap_state.state = cur_dance(state);
    switch (esf1_tap_state.state) {
        case TD_SINGLE_TAP: register_code(KC_ESC); break;
        case TD_SINGLE_HOLD: layer_on(_LAY1); register_code(KC_LCTL); break;
        //case TD_DOUBLE_TAP: register_code(KC_ESC); break;
        case TD_DOUBLE_HOLD: layer_on(_LAY1); register_code(KC_LCTL); register_code(KC_LSFT); break;
        // Last case is for fast typing. Assuming your key is `f`:
        // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
        // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
        //case TD_DOUBLE_SINGLE_TAP: tap_code(KC_X); register_code(KC_X); break;
        default: break;
    }
}

void esf1_reset(tap_dance_state_t *state, void *user_data) {
    switch (esf1_tap_state.state) {
        case TD_SINGLE_TAP: unregister_code(KC_ESC); break;
        case TD_SINGLE_HOLD: layer_off(_LAY2); unregister_code(KC_LCTL); break;
        //case TD_DOUBLE_TAP: unregister_code(KC_ESC); break;
        case TD_DOUBLE_HOLD: layer_off(_LAY2); unregister_code(KC_LCTL); unregister_code(KC_LSFT); break;
        //case TD_DOUBLE_SINGLE_TAP: unregister_code(KC_X); break;
        default: break;
    }
    esf1_tap_state.state = TD_NONE;
}

// F1LS
static td_tap_t f1ls_tap_state = { .is_press_action = true, .state = TD_NONE };

void f1ls_finished(tap_dance_state_t *state, void *user_data) {
    f1ls_tap_state.state = cur_dance(state);
    switch (f1ls_tap_state.state) {
        //case TD_SINGLE_TAP: register_code(KC_ESC); break;
        case TD_SINGLE_HOLD: layer_on(_LAY1); break;
        //case TD_DOUBLE_TAP: register_code(KC_ESC); break;
        case TD_DOUBLE_HOLD: layer_on(_LAY1); register_code(KC_LSFT); break;
        // Last case is for fast typing. Assuming your key is `f`:
        // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
        // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
        //case TD_DOUBLE_SINGLE_TAP: tap_code(KC_X); register_code(KC_X); break;
        default: break;
    }
}

void f1ls_reset(tap_dance_state_t *state, void *user_data) {
    switch (f1ls_tap_state.state) {
        //case TD_SINGLE_TAP: unregister_code(KC_ESC); break;
        case TD_SINGLE_HOLD: layer_off(_LAY1); break;
        //case TD_DOUBLE_TAP: unregister_code(KC_ESC); break;
        case TD_DOUBLE_HOLD: layer_off(_LAY1); unregister_code(KC_LSFT); break;
        //case TD_DOUBLE_SINGLE_TAP: unregister_code(KC_X); break;
        default: break;
    }
    f1ls_tap_state.state = TD_NONE;
}

// BSFT
static td_tap_t bsft_tap_state = { .is_press_action = true, .state = TD_NONE };

void bsft_finished(tap_dance_state_t *state, void *user_data) {
    bsft_tap_state.state = cur_dance(state);
    switch (bsft_tap_state.state) {
        case TD_SINGLE_TAP: register_code(KC_B); break;
        case TD_SINGLE_HOLD: register_code(KC_LSFT); break;
        //case TD_DOUBLE_TAP: register_code(KC_ESC); break;
        //case TD_DOUBLE_HOLD: layer_on(_LAY2); register_code(KC_LCTL); break;
        // Last case is for fast typing. Assuming your key is `f`:
        // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
        // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
        //case TD_DOUBLE_SINGLE_TAP: tap_code(KC_X); register_code(KC_X); break;
        default: break;
    }
}

void bsft_reset(tap_dance_state_t *state, void *user_data) {
    switch (bsft_tap_state.state) {
        case TD_SINGLE_TAP: unregister_code(KC_B); break;
        case TD_SINGLE_HOLD: unregister_code(KC_LSFT); break;
        //case TD_DOUBLE_TAP: unregister_code(KC_ESC); break;
        //case TD_DOUBLE_HOLD: layer_off(_LAY2); unregister_code(KC_LCTL); break;
        //case TD_DOUBLE_SINGLE_TAP: unregister_code(KC_X); break;
        default: break;
    }
    bsft_tap_state.state = TD_NONE;
}

// NSFT
static td_tap_t nsft_tap_state = { .is_press_action = true, .state = TD_NONE };

void nsft_finished(tap_dance_state_t *state, void *user_data) {
    nsft_tap_state.state = cur_dance(state);
    switch (nsft_tap_state.state) {
        case TD_SINGLE_TAP: register_code(KC_N); break;
        case TD_SINGLE_HOLD: register_code(KC_RSFT); break;
        //case TD_DOUBLE_TAP: register_code(KC_LALT); break;
        //case TD_DOUBLE_HOLD: layer_on(_LAY2); register_code(KC_LALT); break;
        //case TD_DOUBLE_SINGLE_TAP: tap_code(KC_X); register_code(KC_X); break;
        default: break;
    }
}

void nsft_reset(tap_dance_state_t *state, void *user_data) {
    switch (nsft_tap_state.state) {
        case TD_SINGLE_TAP: unregister_code(KC_N); break;
        case TD_SINGLE_HOLD: unregister_code(KC_RSFT); break;
        //case TD_DOUBLE_TAP: unregister_code(KC_LALT); break;
        //case TD_DOUBLE_HOLD: layer_off(_LAY2); unregister_code(KC_LALT); break;
        //case TD_DOUBLE_SINGLE_TAP: unregister_code(KC_X); break;
        default: break;
    }
    nsft_tap_state.state = TD_NONE;
}

// APF2
static td_tap_t apf2_tap_state = { .is_press_action = true, .state = TD_NONE };

void apf2_finished(tap_dance_state_t *state, void *user_data) {
    apf2_tap_state.state = cur_dance(state);
    switch (apf2_tap_state.state) {
        case TD_SINGLE_TAP: register_code(KC_APP); break;
        case TD_SINGLE_HOLD: layer_on(_LAY2); break;
        case TD_DOUBLE_TAP: register_code(KC_RGUI); break;
        //case TD_DOUBLE_HOLD: layer_on(_LAY2); register_code(KC_RSFT); break;
        //case TD_DOUBLE_SINGLE_TAP: tap_code(KC_X); register_code(KC_X); break;
        default: break;
    }
}

void apf2_reset(tap_dance_state_t *state, void *user_data) {
    switch (apf2_tap_state.state) {
        case TD_SINGLE_TAP: unregister_code(KC_APP); break;
        case TD_SINGLE_HOLD: layer_off(_LAY2); break;
        case TD_DOUBLE_TAP: unregister_code(KC_RGUI); break;
        case TD_DOUBLE_HOLD: layer_off(_LAY2); unregister_code(KC_RSFT); break;
        //case TD_DOUBLE_SINGLE_TAP: unregister_code(KC_X); break;
        default: break;
    }
    apf2_tap_state.state = TD_NONE;
}

// QUF1
static td_tap_t quf1_tap_state = { .is_press_action = true, .state = TD_NONE };

void quf1_finished(tap_dance_state_t *state, void *user_data) {
    quf1_tap_state.state = cur_dance(state);
    switch (quf1_tap_state.state) {
        case TD_SINGLE_TAP: register_code(KC_QUOTE); break;
        case TD_SINGLE_HOLD: layer_on(_LAY1); break;
        //case TD_DOUBLE_TAP: register_code(KC_RGUI); break;
        //case TD_DOUBLE_HOLD: layer_on(_LAY2); register_code(KC_RSFT); break;
        //case TD_DOUBLE_SINGLE_TAP: tap_code(KC_X); register_code(KC_X); break;
        default: break;
    }
}

void quf1_reset(tap_dance_state_t *state, void *user_data) {
    switch (quf1_tap_state.state) {
        case TD_SINGLE_TAP: unregister_code(KC_QUOTE); break;
        case TD_SINGLE_HOLD: layer_off(_LAY1); break;
        //case TD_DOUBLE_TAP: unregister_code(KC_RGUI); break;
        //case TD_DOUBLE_HOLD: layer_off(_LAY2); unregister_code(KC_RSFT); break;
        //case TD_DOUBLE_SINGLE_TAP: unregister_code(KC_X); break;
        default: break;
    }
    quf1_tap_state.state = TD_NONE;
}

// BSDE
static td_tap_t bsde_tap_state = { .is_press_action = true, .state = TD_NONE };

void bsde_finished(tap_dance_state_t *state, void *user_data) {
    bsde_tap_state.state = cur_dance(state);
    switch (bsde_tap_state.state) {
        case TD_SINGLE_TAP: register_code(KC_BSPC); break;
        case TD_SINGLE_HOLD: register_code(KC_BSPC); break;
        case TD_DOUBLE_TAP: register_code(KC_DEL); break;
        case TD_DOUBLE_HOLD: register_code(KC_DEL); break;
        default: break;
    }
}

void bsde_reset(tap_dance_state_t *state, void *user_data) {
    switch (bsde_tap_state.state) {
        case TD_SINGLE_TAP: unregister_code(KC_BSPC); break;
        case TD_SINGLE_HOLD: unregister_code(KC_BSPC); break;
        case TD_DOUBLE_TAP: unregister_code(KC_DEL); break;
        case TD_DOUBLE_HOLD: unregister_code(KC_DEL); break;
        default: break;
    }
    bsde_tap_state.state = TD_NONE;
}

// SPF2
static td_tap_t spf2_tap_state = { .is_press_action = true, .state = TD_NONE };

void spf2_finished(tap_dance_state_t *state, void *user_data) {
    spf2_tap_state.state = cur_dance(state);
    switch (spf2_tap_state.state) {
        case TD_SINGLE_TAP: register_code(KC_SPC); break;
        case TD_SINGLE_HOLD: layer_on(_LAY2); break;
        //case TD_DOUBLE_TAP: register_code(KC_DEL); break;
        //case TD_DOUBLE_HOLD: register_code(KC_DEL); break;
        default: break;
    }
}

void spf2_reset(tap_dance_state_t *state, void *user_data) {
    switch (spf2_tap_state.state) {
        case TD_SINGLE_TAP: unregister_code(KC_SPC); break;
        case TD_SINGLE_HOLD: layer_off(_LAY2); break;
        //case TD_DOUBLE_TAP: unregister_code(KC_DEL); break;
        //case TD_DOUBLE_HOLD: unregister_code(KC_DEL); break;
        default: break;
    }
    spf2_tap_state.state = TD_NONE;
}


//Tapdance Defines
tap_dance_action_t tap_dance_actions[] = {
    [TD_ESF1] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, esf1_finished, esf1_reset),
    [TD_F1LS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, f1ls_finished, f1ls_reset),
    [TD_BSFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, bsft_finished, bsft_reset),
    [TD_NSFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, nsft_finished, nsft_reset),
    [TD_APF2] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, apf2_finished, apf2_reset),
    [TD_QUF1] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, quf1_finished, quf1_reset),
    [TD_BSDE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, bsde_finished, bsde_reset),
    [TD_SPF2] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, spf2_finished, spf2_reset),
};

// COMBOS
const uint16_t PROGMEM alt_bspc_combo[] = {KC_LALT, KC_BSPC, COMBO_END};
//const uint16_t PROGMEM test_combo2[] = {KC_C, KC_D, COMBO_END};
combo_t key_combos[] = {
    COMBO(alt_bspc_combo, LALT(KC_LEFT)),
    //COMBO(test_combo2, LCTL(KC_Z)), // keycodes with modifiers are possible too!
};
