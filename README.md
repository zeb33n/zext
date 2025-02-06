# zext 
A very lightweight, customisable text editor designed for performance whilst embedded on the web. Built using clang and wasm. 
## Run the Example.
simply clone the project and run a the repo as a static site. 
```
git clone https://github.com/zeb33n/zext.git
cd zext && python -m http.server 6969
```
Then using your browser navigate to localhost:6969 and enjoy!
## configuration (Make it Zexy!)
All colours are configurable (currently this is only possible at compile time by editing the define macros in the header file). 
![image](https://github.com/user-attachments/assets/4cac784c-50db-472a-995c-69b7600c88bc)
![image](https://github.com/user-attachments/assets/718e4c93-aae7-4220-96b0-cd910f7d8d3e)


The size of the text relative to the window is also customisable using the font size arg to the `editor_init` function.

## TODO for v1.0
- javascript/runtime config
- text linting interface
- clipboard support
- Easy embedding interface
