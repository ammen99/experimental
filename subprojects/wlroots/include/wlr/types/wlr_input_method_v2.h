/*
 * This an unstable interface of wlroots. No guarantees are made regarding the
 * future consistency of this API.
 */
#ifndef WLR_USE_UNSTABLE
#error "Add -DWLR_USE_UNSTABLE to enable unstable wlroots features"
#endif

#ifndef WLR_TYPES_WLR_INPUT_METHOD_V2_H
#define WLR_TYPES_WLR_INPUT_METHOD_V2_H
#include <stdint.h>
#include <stdlib.h>
#include <wayland-server-core.h>
#include <wlr/types/wlr_seat.h>

struct wlr_input_method_v2_preedit_string {
	char *text;
	int32_t cursor_begin;
	int32_t cursor_end;
};

struct wlr_input_method_v2_delete_surrounding_text {
	uint32_t before_length;
	uint32_t after_length;
};

struct wlr_input_method_v2_state {
	struct wlr_input_method_v2_preedit_string preedit;
	char *commit_text;
	struct wlr_input_method_v2_delete_surrounding_text delete;
};

struct wlr_input_method_v2 {
	struct wl_resource *resource;

	struct wlr_seat *seat;

	struct wlr_input_method_v2_state pending;
	struct wlr_input_method_v2_state current;
	bool active; // pending compositor-side state
	bool client_active; // state known to the client
	uint32_t current_serial; // received in last commit call

	struct wl_list link;

	struct wl_listener seat_destroy;

	struct {
		struct wl_signal commit; // (struct wlr_input_method_v2*)
		struct wl_signal destroy; // (struct wlr_input_method_v2*)
	} events;
};

struct wlr_input_method_manager_v2 {
	struct wl_global *global;
	struct wl_list input_methods; // struct wlr_input_method_v2*::link

	struct wl_listener display_destroy;

	struct {
		struct wl_signal input_method; // (struct wlr_input_method_v2*)
		struct wl_signal destroy; // (struct wlr_input_method_manager_v2*)
	} events;
};

struct wlr_input_method_manager_v2 *wlr_input_method_manager_v2_create(
	struct wl_display *display);

void wlr_input_method_v2_send_activate(
	struct wlr_input_method_v2 *input_method);
void wlr_input_method_v2_send_deactivate(
	struct wlr_input_method_v2 *input_method);
void wlr_input_method_v2_send_surrounding_text(
	struct wlr_input_method_v2 *input_method, const char *text,
	uint32_t cursor, uint32_t anchor);
void wlr_input_method_v2_send_content_type(
	struct wlr_input_method_v2 *input_method, uint32_t hint,
	uint32_t purpose);
void wlr_input_method_v2_send_text_change_cause(
	struct wlr_input_method_v2 *input_method, uint32_t cause);
void wlr_input_method_v2_send_done(struct wlr_input_method_v2 *input_method);
void wlr_input_method_v2_send_unavailable(
	struct wlr_input_method_v2 *input_method);

#endif
