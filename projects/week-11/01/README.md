# Week 11 - ZVM using Blobify as memory manager

The VM stores its program memory in one blob and splits it into segments:



## Compilation & Run

```bash
 gcc -g ./src/blobify/blobify.c \
       ./src/zvm/*.c \
       ./src/main.c \
       -I./include \
       -o zvm
./zvm
```

