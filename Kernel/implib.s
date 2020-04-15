    .module kernel

    .globl _g_szTemp
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

    _g_szTemp = 0x0300
    _disk_read = 0x0100
    _disk_write = 0x0103
    _disk_status = 0x0106
    _disk_initialize = 0x0109
    _disk_ioctl = 0x010c
    _ff_req_grant = 0x010f
    _ff_rel_grant = 0x0112
    _ff_cre_syncobj = 0x0115
    _ff_del_syncobj = 0x0118
    _f_init = 0x011b
    _f_mount = 0x011e
    _f_open = 0x0121
    _f_close = 0x0124
    _f_read = 0x0127
    _f_read_no_marshal = 0x012a
    _f_write = 0x012d
    _f_lseek = 0x0130
    _f_current_sector = 0x0133
    _f_create_sector = 0x0136
    _f_unlink = 0x0139
    _f_rename = 0x013c
    _f_opendir = 0x013f
    _f_closedir = 0x0142
    _f_readdir = 0x0145
    _f_mkdir = 0x0148
    _f_truncate = 0x014b
    _pattern_matching = 0x014e
    _create_fiber = 0x0151
    _get_current_fiber = 0x0154
    _tolower = 0x0157
    _stricmp = 0x015a
    _stralloc = 0x015d
    _ll_push = 0x0160
    _ll_shift = 0x0163
    _ll_unshift = 0x0166
    _ll_remove = 0x0169
    _ll_contains = 0x016c
    _init_signal = 0x016f
    _set_signal = 0x0172
    _wait_signal = 0x0175
    _init_mutex = 0x0178
    _enter_mutex = 0x017b
    _leave_mutex = 0x017e
    _uart_write = 0x0181
    _uart_write_sz = 0x0184
    _uart_read = 0x0187
    _uart_read_wait = 0x018a
    _video_clear = 0x018d
    _video_write = 0x0190
    _video_write_sz = 0x0193
    _video_set_color = 0x0196
    _video_draw_box_indirect = 0x0199
    _video_draw_box = 0x019c
    _video_text_out = 0x019f
    _video_draw_text = 0x01a2
    _video_measure_multiline_text = 0x01a5
    _video_draw_multiline_text = 0x01a8
    _video_scroll = 0x01ab
    _video_begin_offscreen = 0x01ae
    _video_end_offscreen = 0x01b1
    _video_save = 0x01b4
    _video_restore = 0x01b7
    _key_to_char = 0x01ba
    _msg_copy = 0x01bd
    _msg_enqueue = 0x01c0
    _msg_post = 0x01c3
    _msg_peek = 0x01c6
    _msg_get = 0x01c9
    _window_send_message = 0x01cc
    _window_get_client_rect = 0x01cf
    _window_def_proc = 0x01d2
    _window_get_active = 0x01d5
    _window_create = 0x01d8
    _window_destroy = 0x01db
    _window_run_modeless = 0x01de
    _window_update_modeless = 0x01e1
    _window_end_modeless = 0x01e4
    _window_run_modal = 0x01e7
    _window_end_modal = 0x01ea
    _window_get_attr_normal = 0x01ed
    _window_get_attr_selected = 0x01f0
    _window_invalidate = 0x01f3
    _window_set_hook = 0x01f6
    _message_box_modeless = 0x01f9
    _message_box = 0x01fc
    _prompt_input = 0x01ff
    _listbox_wndproc = 0x0202
    _listbox_draw = 0x0205
    _listbox_drawitem = 0x0208
    _listbox_update = 0x020b
    _listbox_ensure_visible = 0x020e
    _listbox_select = 0x0211
    _listbox_set_data = 0x0214
    _choose_file = 0x0217
    _malloc = 0x021a
    _free = 0x021d
    _realloc = 0x0220
    _sprintf = 0x0223
    _strlen = 0x0226
    _strcpy = 0x0229
