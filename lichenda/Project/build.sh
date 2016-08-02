gcc -g\
   -D_DEBUG_VERSION \
   -D_STRING_EX_COMPILE \
   -IPUBLIC/include/comware \
   INFO/src/info_main.c \
   INFO/src/info_dbg.c \
   INFO/src/info_parse.c \
   INFO/src/info_data.c \
   INFO/src/info_proc.c \
    -o m_info #名字从info改为m_info，因为在windows下忽略大小写会和目录名冲突
