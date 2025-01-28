
all: 
    clang -Os -fno-builtin -Wall -Wextra -Wswitch-enum --target=wasm32 -Wl,--no-entry -Wl,--allow-undefined --no-standard-libraries -o editor.wasm editor.c


  
