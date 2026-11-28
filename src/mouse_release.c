/*
 * Resend the mouse button state after the output target changes.
 *
 * ZMK v0.3 endpoints.c zmk_endpoints_clear_current() clears the mouse HID state
 * on an endpoint switch (USB <-> BLE, BLE profile change) but only sends the
 * keyboard and consumer reports, not the mouse report. A button held at that
 * moment stays "down" on the host it was sent to. macOS then treats every pointer
 * movement as a drag (no hover), and new presses of that button do nothing, for
 * every pointing device, until the keyboard sends a release.
 *
 * The same happens when the BLE link drops while a button is held: the host
 * never got the release, and ZMK sends no mouse report on reconnect.
 *
 * This listens for endpoint changes and for the active BLE profile (re)connecting
 * and then sends the current button state with zero motion to USB and to BLE.
 * When nothing is held that is a harmless all-released report.
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zmk/event_manager.h>
#include <zmk/events/endpoint_changed.h>
#include <zmk/hid.h>

#if IS_ENABLED(CONFIG_ZMK_BLE)
#include <zmk/ble.h>
#include <zmk/hog.h>
#include <zmk/events/ble_active_profile_changed.h>
#endif

#if IS_ENABLED(CONFIG_ZMK_USB)
#include <zmk/usb.h>
#include <zmk/usb_hid.h>
#endif

LOG_MODULE_REGISTER(charybdis_mouse_release, CONFIG_ZMK_LOG_LEVEL);

// Give a fresh connection time to finish security and subscribe to HID reports,
// then repeat once in case the first report went out too early.
#define FIRST_DELAY K_MSEC(500)
#define SECOND_DELAY K_MSEC(2000)

static void resend(void) {
    // Motion is set fresh for every input event, so zeroing it here only keeps
    // this report from moving the cursor. Buttons keep their real state.
    zmk_hid_mouse_movement_set(0, 0);
    zmk_hid_mouse_scroll_set(0, 0);

    struct zmk_hid_mouse_report *report = zmk_hid_get_mouse_report();
    LOG_DBG("Resending mouse report, buttons 0x%02x", report->body.buttons);

#if IS_ENABLED(CONFIG_ZMK_USB)
    if (zmk_usb_is_hid_ready()) {
        zmk_usb_hid_send_mouse_report();
    }
#endif

#if IS_ENABLED(CONFIG_ZMK_BLE)
    if (zmk_ble_active_profile_is_connected()) {
        zmk_hog_send_mouse_report(&report->body);
    }
#endif
}

static void first_cb(struct k_work *work);
static void second_cb(struct k_work *work);
static K_WORK_DELAYABLE_DEFINE(first_work, first_cb);
static K_WORK_DELAYABLE_DEFINE(second_work, second_cb);

static void first_cb(struct k_work *work) {
    resend();
    k_work_reschedule(&second_work, SECOND_DELAY);
}

static void second_cb(struct k_work *work) { resend(); }

static int mouse_release_listener(const zmk_event_t *eh) {
    k_work_reschedule(&first_work, FIRST_DELAY);
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(charybdis_mouse_release, mouse_release_listener);
ZMK_SUBSCRIPTION(charybdis_mouse_release, zmk_endpoint_changed);
#if IS_ENABLED(CONFIG_ZMK_BLE)
ZMK_SUBSCRIPTION(charybdis_mouse_release, zmk_ble_active_profile_changed);
#endif
