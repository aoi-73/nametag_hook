```bash
g++ -shared -o nametag.dll dllmain.cpp minhook/src/buffer.c minhook/src/hook.c minhook/src/trampoline.c minhook/src/hde/hde64.c -I./minhook/include -I./minhook/src -std=c++17 -luser32 -Wall
```
