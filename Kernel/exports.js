let make_exports = require('../make_exports');

make_exports({
    baseAddr: 0x100,
    moduleName: "kernel",
    exportsFile: "exports.s",
    importsFile: "implib.s",
    constants: [
        { name: "_g_szTemp", value: 0x300 },
    ],
    functions: [

        // Callbacks from fatfs.sys
        "_disk_read",
        "_disk_write",
        "_disk_status",
        "_disk_initialize",
        "_disk_ioctl",
        "_ff_req_grant",
        "_ff_rel_grant",
        "_ff_cre_syncobj",
        "_ff_del_syncobj",

        // Exported functions
        "_f_init",
        "_f_mount",
        "_f_open",
        "_f_close",
        "_f_read",
        "_f_read_no_marshal",
        "_f_write",
        "_f_lseek",
        "_f_current_sector",
        "_f_create_sector",
        "_f_unlink",
        "_f_rename",
        "_f_opendir",
        "_f_closedir",
        "_f_readdir",
        "_f_mkdir",
        "_f_truncate",
        "_pattern_matching",
        "_create_fiber",
        "_get_current_fiber",
        "_tolower",
        "_stricmp",
        "_stralloc",
        "_ll_push",
        "_ll_shift",
        "_ll_unshift",
        "_ll_remove",
        "_ll_contains",
        "_init_signal",
        "_set_signal",
        "_wait_signal",
        "_init_mutex",
        "_enter_mutex",
        "_leave_mutex",
        "_uart_write",
        "_uart_write_sz",
        "_uart_read",
        "_uart_read_wait",
        "_video_clear",
        "_video_write",
        "_video_write_sz",
        "_video_set_color",
        "_video_draw_box_indirect",
        "_video_draw_box",
        "_video_text_out",
        "_video_draw_text",
        "_video_measure_multiline_text",
        "_video_draw_multiline_text",
        "_video_scroll",
        "_video_begin_offscreen",
        "_video_end_offscreen",
        "_video_save",
        "_video_restore",
        "_key_to_char",
        "_msg_copy",
        "_msg_enqueue",
        "_msg_post",
        "_msg_peek",
        "_msg_get",
        "_window_send_message",
        "_window_get_client_rect",
        "_window_def_proc",
        "_window_get_active",
        "_window_create",
        "_window_destroy",
        "_window_run_modeless",
        "_window_update_modeless",
        "_window_end_modeless",
        "_window_run_modal",
        "_window_end_modal",
        "_window_get_attr_normal",
        "_window_get_attr_selected",
        "_window_invalidate",
        "_window_set_hook",
        "_message_box_modeless",
        "_message_box",
        "_prompt_input",
        "_listbox_wndproc",
        "_listbox_draw",
        "_listbox_drawitem",
        "_listbox_update",
        "_listbox_ensure_visible",
        "_listbox_select",
        "_listbox_set_data",
        "_choose_file",
        "_malloc",
        "_free",
        "_realloc",
        "_sprintf",
        "_strlen",
        "_strcpy",
    ],
});

