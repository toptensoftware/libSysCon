let make_exports = require('../make_exports');

make_exports({
    baseAddr: 0x8000,
    moduleName: "fatfs",
    exportsFile: "exports.s",
    importsFile: "implib.s",
    importsPrefix: "_imp",
    functions: [
        "_f_init",
        "_f_mount",
        "_f_open",
        "_f_close",
        "_f_read",
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
        "_malloc",
        "_free",
        "_realloc",
    ],
});

