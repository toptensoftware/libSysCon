const fs = require('fs');
const path = require('path');

let bufs = [];
let bank = 64;

for (let i=3; i<process.argv.length; i++)
{
    let binary = fs.readFileSync(process.argv[i]);
    let size = parseInt(binary.length / 1024);
    if (binary.length % 1024)
        size++;

    // Create header
    let header = Buffer.allocUnsafe(2);
    header.writeUInt8(bank, 0);
    header.writeUInt8(size, 1);
    bufs.push(header);

    // Write data
    bufs.push(binary);

    // Pad to 1k boundary
    let paddingSize = 0;
    if (size * 1024 > binary.length)
    {
        paddingSize = size * 1024 - binary.length;
        padding = Buffer.allocUnsafe(paddingSize);
        padding.fill(0xFF);
        bufs.push(padding);
    }

    console.log(`Bank 0x${bank.toString(16).padStart(2, '0')} size 0x${size.toString(16).padStart(2, '0')} ${path.basename(process.argv[i])}`);

    bank += 32;
}

fs.writeFileSync(process.argv[2], Buffer.concat(bufs), 'binary');
