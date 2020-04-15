    .module exports

    .globl _disk_read
    .globl _disk_write
    .globl _disk_status
    .globl _disk_initialize
    .globl _disk_ioctl
    .globl _ff_req_grant
    .globl _ff_rel_grant
    .globl _ff_cre_syncobj
    .globl _ff_del_syncobj
    .globl _f_init
    .globl _f_mount
    .globl _f_open
    .globl _f_close
    .globl _f_read
    .globl _f_read_no_marshal
    .globl _f_write
    .globl _f_lseek
    .globl _f_current_sector
    .globl _f_create_sector
    .globl _f_unlink
    .globl _f_rename
    .globl _f_opendir
    .globl _f_closedir
    .globl _f_readdir
    .globl _f_mkdir
    .globl _f_truncate
    .globl _pattern_matching
    .globl _create_fiber
    .globl _get_current_fiber
    .globl _tolower
    .globl _stricmp
    .globl _stralloc
    .globl _ll_push
    .globl _ll_shift
    .globl _ll_unshift
    .globl _ll_remove
    .globl _ll_contains
    .globl _init_signal
    .globl _set_signal
    .globl _wait_signal
    .globl _init_mutex
    .globl _enter_mutex
    .globl _leave_mutex
    .globl _uart_write
    .globl _uart_write_sz
    .globl _uart_read
    .globl _uart_read_wait
    .globl _video_clear
    .globl _video_write
    .globl _video_write_sz
    .globl _video_set_color
    .globl _video_draw_box_indirect
    .globl _video_draw_box
    .globl _video_text_out
    .globl _video_draw_text
    .globl _video_measure_multiline_text
    .globl _video_draw_multiline_text
    .globl _video_scroll
    .globl _video_begin_offscreen
    .globl _video_end_offscreen
    .globl _video_save
    .globl _video_restore
    .globl _key_to_char
    .globl _msg_copy
    .globl _msg_enqueue
    .globl _msg_post
    .globl _msg_peek
    .globl _msg_get
    .globl _window_send_message
    .globl _window_get_client_rect
    .globl _window_def_proc
    .globl _window_get_active
    .globl _window_create
    .globl _window_destroy
    .globl _window_run_modeless
    .globl _window_update_modeless
    .globl _window_end_modeless
    .globl _window_run_modal
    .globl _window_end_modal
    .globl _window_get_attr_normal
    .globl _window_get_attr_selected
    .globl _window_invalidate
    .globl _window_set_hook
    .globl _message_box_modeless
    .globl _message_box
    .globl _prompt_input
    .globl _listbox_wndproc
    .globl _listbox_draw
    .globl _listbox_drawitem
    .globl _listbox_update
    .globl _listbox_ensure_visible
    .globl _listbox_select
    .globl _listbox_set_data
    .globl _choose_file
    .globl _malloc
    .globl _free
    .globl _realloc
    .globl _sprintf
    .globl _strlen
    .globl _strcpy

    .area	_EXPORTS (ABS)
    .org 0x0100

    jp _disk_read	; 0x0100
    jp _disk_write	; 0x0103
    jp _disk_status	; 0x0106
    jp _disk_initialize	; 0x0109
    jp _disk_ioctl	; 0x010c
    jp _ff_req_grant	; 0x010f
    jp _ff_rel_grant	; 0x0112
    jp _ff_cre_syncobj	; 0x0115
    jp _ff_del_syncobj	; 0x0118
    jp _f_init	; 0x011b
    jp _f_mount	; 0x011e
    jp _f_open	; 0x0121
    jp _f_close	; 0x0124
    jp _f_read	; 0x0127
    jp _f_read_no_marshal	; 0x012a
    jp _f_write	; 0x012d
    jp _f_lseek	; 0x0130
    jp _f_current_sector	; 0x0133
    jp _f_create_sector	; 0x0136
    jp _f_unlink	; 0x0139
    jp _f_rename	; 0x013c
    jp _f_opendir	; 0x013f
    jp _f_closedir	; 0x0142
    jp _f_readdir	; 0x0145
    jp _f_mkdir	; 0x0148
    jp _f_truncate	; 0x014b
    jp _pattern_matching	; 0x014e
    jp _create_fiber	; 0x0151
    jp _get_current_fiber	; 0x0154
    jp _tolower	; 0x0157
    jp _stricmp	; 0x015a
    jp _stralloc	; 0x015d
    jp _ll_push	; 0x0160
    jp _ll_shift	; 0x0163
    jp _ll_unshift	; 0x0166
    jp _ll_remove	; 0x0169
    jp _ll_contains	; 0x016c
    jp _init_signal	; 0x016f
    jp _set_signal	; 0x0172
    jp _wait_signal	; 0x0175
    jp _init_mutex	; 0x0178
    jp _enter_mutex	; 0x017b
    jp _leave_mutex	; 0x017e
    jp _uart_write	; 0x0181
    jp _uart_write_sz	; 0x0184
    jp _uart_read	; 0x0187
    jp _uart_read_wait	; 0x018a
    jp _video_clear	; 0x018d
    jp _video_write	; 0x0190
    jp _video_write_sz	; 0x0193
    jp _video_set_color	; 0x0196
    jp _video_draw_box_indirect	; 0x0199
    jp _video_draw_box	; 0x019c
    jp _video_text_out	; 0x019f
    jp _video_draw_text	; 0x01a2
    jp _video_measure_multiline_text	; 0x01a5
    jp _video_draw_multiline_text	; 0x01a8
    jp _video_scroll	; 0x01ab
    jp _video_begin_offscreen	; 0x01ae
    jp _video_end_offscreen	; 0x01b1
    jp _video_save	; 0x01b4
    jp _video_restore	; 0x01b7
    jp _key_to_char	; 0x01ba
    jp _msg_copy	; 0x01bd
    jp _msg_enqueue	; 0x01c0
    jp _msg_post	; 0x01c3
    jp _msg_peek	; 0x01c6
    jp _msg_get	; 0x01c9
    jp _window_send_message	; 0x01cc
    jp _window_get_client_rect	; 0x01cf
    jp _window_def_proc	; 0x01d2
    jp _window_get_active	; 0x01d5
    jp _window_create	; 0x01d8
    jp _window_destroy	; 0x01db
    jp _window_run_modeless	; 0x01de
    jp _window_update_modeless	; 0x01e1
    jp _window_end_modeless	; 0x01e4
    jp _window_run_modal	; 0x01e7
    jp _window_end_modal	; 0x01ea
    jp _window_get_attr_normal	; 0x01ed
    jp _window_get_attr_selected	; 0x01f0
    jp _window_invalidate	; 0x01f3
    jp _window_set_hook	; 0x01f6
    jp _message_box_modeless	; 0x01f9
    jp _message_box	; 0x01fc
    jp _prompt_input	; 0x01ff
    jp _listbox_wndproc	; 0x0202
    jp _listbox_draw	; 0x0205
    jp _listbox_drawitem	; 0x0208
    jp _listbox_update	; 0x020b
    jp _listbox_ensure_visible	; 0x020e
    jp _listbox_select	; 0x0211
    jp _listbox_set_data	; 0x0214
    jp _choose_file	; 0x0217
    jp _malloc	; 0x021a
    jp _free	; 0x021d
    jp _realloc	; 0x0220
    jp _sprintf	; 0x0223
    jp _strlen	; 0x0226
    jp _strcpy	; 0x0229
