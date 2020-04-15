let fs = require('fs');

function make_exports(options)
{
    let baseAddr = options.baseAddr === undefined ? 0x180 : options.baseAddr;

    let s = "";

    if (options.exportsFile)
    {
        // Generate exports .s file
        s += "    .module exports\n\n";
        for (let i=0; i<options.functions.length; i++)
        {
            s += "    .globl " + options.functions[i] + "\n";
        }

        s += "\n";
        s += "    .area	_EXPORTS (ABS)\n";
        s += "    .org 0x" + baseAddr.toString(16).padStart(4, '0') +"\n";
        s += "\n";

        for (let i=0; i<options.functions.length; i++)
        {
            s += "    jp " + options.functions[i] + "\t; 0x" + (baseAddr + i * 3).toString(16).padStart(4, '0') + "\n";
        }

        fs.writeFileSync(options.exportsFile, s, "UTF8");
    }

    if (options.importsFile)
    {
        let prefix = options.importsPrefix || "";


        // Generate the imports .s file
        s = "";
        s += "    .module " + options.moduleName + "\n\n";

        if (options.constants)
        {
            for (let i=0; i<options.constants.length; i++)
            {
                s += "    .globl " + options.constants[i].name + "\n";
            }
        }

        for (let i=0; i<options.functions.length; i++)
        {
            s += "    .globl " + prefix + options.functions[i] + "\n";
        }

        s += "\n";

        if (options.constants)
        {
            for (let i=0; i<options.constants.length; i++)
            {
                s += "    " + options.constants[i].name  + " = 0x" + options.constants[i].value.toString(16).padStart(4, '0') + "\n";
            }
        }

        for (let i=0; i<options.functions.length; i++)
        {
            s += "    " + prefix + options.functions[i] + " = 0x" + (baseAddr + i * 3).toString(16).padStart(4, '0') + "\n";
        }


        fs.writeFileSync(options.importsFile, s, "UTF8");
    }
}

module.exports = make_exports;