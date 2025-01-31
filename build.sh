
    clang -Os -fno-builtin -Wall -Wextra -Wswitch-enum --target=wasm32 -Wl,--export=editor_dump_text -Wl,--export=editor_special_keypress -Wl,--export=editor_keypress -Wl,--export=editor_init -Wl,--no-entry -Wl,--allow-undefined --no-standard-libraries -o editor.wasm editor.c
