#!/bin/bash
gcc -g\
   -D_DEBUG_VERSION \
   -D_STRING_EX_COMPILE \
   -IPUBLIC/include/comware \
   INFO/src/info_main.c \
   INFO/src/info_dbg.c \
   INFO/src/info_parse.c \
   INFO/src/info_data.c \
   INFO/src/info_proc.c \
    -o info
