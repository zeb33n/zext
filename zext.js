
'use strict';

let app = document.getElementById("zext");
let ctx = app.getContext("2d");
let w = null;
let text = "";

function color_hex(colour) {
    const r = ((colour>>(0*8))&0xFF).toString(16).padStart(2, '0');
    const g = ((colour>>(1*8))&0xFF).toString(16).padStart(2, '0');
    const b = ((colour>>(2*8))&0xFF).toString(16).padStart(2, '0');
    const a = ((colour>>(3*8))&0xFF).toString(16).padStart(2, '0');
    return "#"+r+g+b+a;
}

function js_fill_rect(x, y, w, h, colour) {
    ctx.fillStyle = color_hex(colour); 
    ctx.fillRect(x, y, w, h);
}

function js_write_char(x, y, c, colour, size) {
    const character = String.fromCharCode(c);    
    ctx.fillStyle = color_hex(colour);
    ctx.font = size + "px monospace";
    ctx.fillText(character, x, y); 
}

function js_unwrite_char(x, y, colour, size) {
    ctx.fillStyle = color_hex(colour);
    ctx.font = size + "px monospace";
    // █ character
    ctx.fillText(String.fromCharCode(9608), x, y);
}

function js_dump_line(msg) {
  text += get_text(msg) + "\n";
}

function js_log_str(msg) {
    console.log(get_text(msg));
}

function js_log(msg) {
    console.log(msg);
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


function get_text(msg) {
    const buffer = w.instance.exports.memory.buffer;
    const message = cstr_by_ptr(buffer, msg);
    return message;
}

const wasm_path = new URL('editor.wasm', import.meta.url);

WebAssembly.instantiateStreaming(fetch(wasm_path), {
    env: {
        js_fill_rect,
        js_write_char,
        js_log,
        js_log_str,
        js_unwrite_char,
        js_dump_line,
    }
}).then((wasm) => {
    w = wasm;
    w.instance.exports.editor_init(app.width, app.height, 90);
    app.addEventListener("keydown", (c) =>  {
      if (c.key.length > 1) {
        w.instance.exports.editor_special_keypress(c.keyCode);
      } else {
        w.instance.exports.editor_keypress(c.key.charCodeAt());
      }
    });
   }
);

export function zext_dump_text() {
  text = ""
  w.instance.exports.editor_dump_text();
  const text_copy = (' ' + text).slice(1);
  return text_copy;
}
