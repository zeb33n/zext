
'use strict';

let app = document.getElementById("app");
let ctx = app.getContext("2d");
let w = null;

function color_hex(color) {
    const r = ((color>>(0*8))&0xFF).toString(16).padStart(2, '0');
    const g = ((color>>(1*8))&0xFF).toString(16).padStart(2, '0');
    const b = ((color>>(2*8))&0xFF).toString(16).padStart(2, '0');
    const a = ((color>>(3*8))&0xFF).toString(16).padStart(2, '0');
    return "#"+r+g+b+a;
}

function fill_rect(x, y, w, h, color) {
    ctx.fillStyle = color_hex(color); 
    ctx.fillRect(x, y, w, h);
}

function write_char(x, y, c, color, size) {
    const character = String.fromCharCode(c);    
    ctx.fillStyle = color_hex(color);
    ctx.font = size + "px AnekLatin";
    ctx.fillText(character, x, y);
}

function log(msg) {
    console.log(msg);
}

WebAssembly.instantiateStreaming(fetch('editor.wasm'), {
    env: {
        fill_rect,
        write_char,
        log,
    }
}).then((w) => {
    w.instance.exports.init(app.width, app.height, 20);
    console.log(w.instance.exports);
    w.instance.exports.foo();

    document.addEventListener("keydown", (c) =>  {
      console.log(c);
      w.instance.exports.editor_keypress(c.key.charCodeAt());
    });
   }
);

