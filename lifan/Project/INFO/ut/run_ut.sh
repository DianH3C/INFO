
UT_PATH=$(cd $(dirname $0) && pwd)
HEADER_PATH=$UT_PATH/../../PUBLIC/include/comware
SRC_PATH=$UT_PATH/../../INFO/src

function msg_info
{
    echo "############################ $* ############################"
}

function gtest_isNotInstall
{
    cd $UT_PATH
    if [ -z "$(ls lib/libgtest*.a)" ]
    then
        return 0
    fi
    
    return 1
}

function gtest_install
{
    msg_info "Compile GTest"
    cd $UT_PATH/gtest
    tar xzf gtest-1.7.0.tar
    cd gtest-1.7.0 
    ./configure || return 1
    make || return 1
    cp lib/.libs/*.a $UT_PATH/lib
    cp -rf include/gtest $UT_PATH/include
}

function gtest_remove
{
    cd $UT_PATH
    rm -rf include/gtest
    rm -rf lib/libgtest*.a
    rm -rf $UT_PATH/gtest/gtest-1.7.0
}

function ut_compile
{
    msg_info "Compile UT"
    cd $UT_PATH
    rm -rf objs/*.o

    gcc -D_UT_ -D_STRING_EX_COMPILE -I $HEADER_PATH -g -c $SRC_PATH/info_data.c  -o objs/info_data.o
    gcc -D_UT_ -D_STRING_EX_COMPILE -I $HEADER_PATH -g -c $SRC_PATH/info_parse.c -o objs/info_parse.o
    gcc -D_UT_ -D_STRING_EX_COMPILE -I $HEADER_PATH -g -c $SRC_PATH/info_proc.c  -o objs/info_proc.o
    gcc -D_UT_ -D_STRING_EX_COMPILE -I $HEADER_PATH -g -c $SRC_PATH/info_dbg.c   -o objs/info_dbg.o
    gcc -D_UT_ -D_STRING_EX_COMPILE -I $HEADER_PATH -g -c stub/stub.c         -o objs/stub.o 2> /dev/null || return
    g++ -D_UT_ -D_STRING_EX_COMPILE -g -c -I $HEADER_PATH -I include/ test/courseware_test.c -o objs/courseware_test.o 

    g++ -D_UT_ -lpthread \
               objs/info_data.o \
               objs/info_dbg.o \
               objs/info_parse.o \
               objs/info_proc.o \
               objs/stub.o \
               objs/courseware_test.o \
               lib/libgtest.a \
               lib/libgtest_main.a \
        -o ut || return
}

function ut_run
{
    msg_info "Run UT"
    echo -e "\r\n\r\n\r\n\r\n\r\n\r\n"
    cd $UT_PATH
    ./ut
}

if [ "$#" -ne 0 ]
then
    if [ "$1" == "clean" ]
    then
        msg_info "Remove Gtest"
        gtest_remove
        rm -rf objs/*.o
        rm -f ut
        exit
    else
        echo "Input arguments error."
        exit 1
    fi    
fi

if gtest_isNotInstall
then
    if gtest_install
    then
        msg_info "Gtest Compile Success"
    else
        msg_info "Gtest Compile Failed"
    fi
fi

if ut_compile
then
    msg_info "Compile UT Success"
    ut_run
else
    msg_info "Compile UT Failed"
fi

