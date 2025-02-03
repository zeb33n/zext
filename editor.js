
'use strict';

let app = document.getElementById("app");
let ctx = app.getContext("2d");
let w = null;

function color_hex(colour) {
    const r = ((colour>>(0*8))&0xFF).toString(16).padStart(2, '0');
    const g = ((colour>>(1*8))&0xFF).toString(16).padStart(2, '0');
    const b = ((colour>>(2*8))&0xFF).toString(16).padStart(2, '0');
    const a = ((colour>>(3*8))&0xFF).toString(16).padStart(2, '0');
    return "#"+r+g+b+a;
}

function fill_rect(x, y, w, h, colour) {
    ctx.fillStyle = color_hex(colour); 
    ctx.fillRect(x, y, w, h);
}

function write_char(x, y, c, colour, size) {
    const character = String.fromCharCode(c);    
    ctx.fillStyle = color_hex(colour);
    ctx.font = size + "px monospace";
    ctx.fillText(character, x, y);
}

function unwrite_char(x, y, colour, size) {
    ctx.fillStyle = color_hex(colour);
    ctx.font = size + "px monospace";
    // █ character
    ctx.fillText(String.fromCharCode(9608), x, y);
}

function cstrlen(mem, ptr) {
    let len = 0;
    while (mem[ptr] != 0) {
        len++;
        ptr++;
    }
    return len;
}

function cstr_by_ptr(mem_buffer, ptr) {
    const mem = new Uint8Array(mem_buffer);
    const len = cstrlen(mem, ptr);
    const bytes = new Uint8Array(mem_buffer, ptr, len);
    return new TextDecoder().decode(bytes);
}

function log_str(msg) {
    const buffer = w.instance.exports.memory.buffer;
    const message = cstr_by_ptr(buffer, msg);
    console.log(message);
}

function log(msg) {
    console.log(msg);
}

WebAssembly.instantiateStreaming(fetch('editor.wasm'), {
    env: {
        fill_rect,
        write_char,
        log,
        log_str,
        unwrite_char,
    }
}).then((wasm) => {
    w = wasm;
    w.instance.exports.editor_init(app.width, app.height, 90);
    console.log(w.instance.exports);

    document.addEventListener("keydown", (c) =>  {
      if (c.key.length > 1) {
        w.instance.exports.editor_special_keypress(c.keyCode);
      } else {
        w.instance.exports.editor_keypress(c.key.charCodeAt());
      }
    });
   }
);

