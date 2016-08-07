macro AutoExpand()
{
    // get window, sel, and buffer handles
    hwnd = GetCurrentWnd()
    if (hwnd == 0)
        stop

    sel = GetWndSel(hwnd)
    if (sel.ichFirst == 0)
    {
        searchForward()
        return
    }

    hbuf = GetWndBuf(hwnd)

    // get line the selection (insertion point) is on
    szLine = GetBufLine(hbuf, sel.lnFirst);
    // parse word just to the left of the insertion point
    wordinfo = getWordLeftOfIch(sel.ichFirst, szLine)
    ln = sel.lnFirst;
    chTab = CharFromAscii(9)

    // prepare a new indented blank line to be inserted.
    // keep white space on left and add a tab to indent.
    // this preserves the indentation level.
    chSpace = CharFromAscii(32);
    ich = 0
    while (szLine[ich] == chSpace || szLine[ich] == chTab)
    {
        ich = ich + 1
    }
    szLine1 = StrMid(szLine, 0, ich)
    szLine  = StrMid(szLine, 0, ich) # "    "

    wordinfo.szWord = expandShortCommand(hbuf,wordinfo.szWord)
    szCmd = wordinfo.szWord
    sel = GetWndSel(hwnd)

    if (false)  msg ""
    else if (szCmd == "#if")     expandIfdef(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "#ifdef")  expandIfdef(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "#ifndef") expandIfdef(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "*/")      expandCommentBlock(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "/*")      expandComment(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "ab")      expandBegin(hbuf,wordinfo,szLine,szLine1,ln,sel,"Added")
    else if (szCmd == "ae")      expandEnd(hbuf,wordinfo,szLine,szLine1,ln,sel,"Added")
    else if (szCmd == "case")    expandCase(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "cfile")   expandCfile(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "config")  doConfig(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "cpp")     expandCpp(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "debug")   expandDebug(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "decl")    expandDeclare(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "do")      expandDo(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "ef")      expandElseIf(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "else")    expandElse(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "enum")    expandEnum(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "for")     expandFor(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "func")    expandFunc(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "head")    expandHead(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "hfile")   expandHfile(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "history") expandHistory(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "if")      expandWhile(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "ife")     expandIfAndElse(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "ifs")     expandIfAndElseIf(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "main")    expandMain(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "mb")      expandBegin(hbuf,wordinfo,szLine,szLine1,ln,sel,"Modified")
    else if (szCmd == "me")      expandEnd(hbuf,wordinfo,szLine,szLine1,ln,sel,"Modified")
    else if (szCmd == "pn")      doSetPN(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "struct")  expandStruct(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "switch")  expandSwitch(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "temp")    expandTemp(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "ut")      expandUt(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "while")   expandWhile(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (szCmd == "{")       expandBrace(hbuf,wordinfo,szLine,szLine1,ln,sel)
    else if (isInComment(hbuf, ln)) expandCommentNewline(hbuf, ln, sel)
    else searchForward()
}

macro expandShortCommand(hbuf,szCmd)
{
    if(szCmd == "ca")
    {
        SetBufSelText(hbuf, "se")
        szCmd = "case"
    }
    else if(szCmd == "sw")
    {
        SetBufSelText(hbuf, "itch")
        szCmd = "switch"
    }
    else if(szCmd == "el")
    {
        SetBufSelText(hbuf, "se")
        szCmd = "else"
    }
    else if(szCmd == "wh")
    {
        SetBufSelText(hbuf, "ile")
        szCmd = "while"
    }
    else if(szCmd == "#ifd")
    {
        SetBufSelText(hbuf, "ef")
        szCmd = "#ifdef"
    }
    else if(szCmd == "#ifn")
    {
        SetBufSelText(hbuf, "def")
        szCmd = "#ifndef"
    }
    else if (szCmd == "st")
    {
        SetBufSelText(hbuf, "ruct")
        szCmd = "struct"
    }
    else if (szCmd == "en")
    {
        SetBufSelText(hbuf, "um")
        szCmd = "enum"
    }
    else if (szCmd == "fi")
    {
        SetBufSelText(hbuf, "le")
        szCmd = "file"
    }
    else if (szCmd == "fu")
    {
        SetBufSelText(hbuf, "nc")
        szCmd = "func"
    }
    else if (szCmd == "be")
    {
        SetBufSelText(hbuf, "gin")
        szCmd = "begin"
    }
    else if (szCmd == "co")
    {
        SetBufSelText(hbuf, "nfig")
        szCmd = "config"
    }
    else if (szCmd == "his")
    {
        SetBufSelText(hbuf, "tory")
        szCmd = "history"
    }
    else if (szCmd == "ed")
    {
        SetBufSelText(hbuf, "it")
        szCmd = "edit"
    }
    else if (szCmd == "de")
    {
        SetBufSelText(hbuf, "bug")
        szCmd = "debug"
    }
    else if (szCmd == "deb")
    {
        SetBufSelText(hbuf, "ug")
        szCmd = "debug"
    }

    return szCmd
}

macro expandHistory(hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    DelBufLine(hbuf, ln)

    szDate = getDateStr()
    szMyName = getMyName()
    if( StrLen(szMyName) < 0 )
    {
       szMyName = "#"
    }
    szContent = Ask("Please input modification")
    szHead = "   @szDate@"
    iLen = StrLen(szHead)
    while (iLen <= 14)
    {
        szHead = cat(szHead, " ")
        iLen = iLen + 1
    }
    szHead = cat(szHead, szMyName)
    iLen = StrLen(szHead)
    while (iLen <= 31)
    {
        szHead = cat(szHead, " ")
        iLen = iLen + 1
    }

    InsBufLine(hbuf, ln, "")
    commentContent(hbuf,ln, szHead, szContent, 0)
}

macro doConfig(hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    DelBufLine(hbuf, ln)

    szName = ASK("Please input your name");
    if(szName == "#")
    {
       szName = ""
    }
   SetReg ("MYNAME", szName)
}

macro doSetPN(hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    DelBufLine(hbuf, ln)
    szQuestion = ASK("Please input trace number:");
    if(szQuestion == "#")
    {
       szQuestion = ""
    }
    SetReg ("PNO", szQuestion)
}

macro insertComment(hbuf, szLine1, where, action, ln)
{
    szDate = getDateStr()
    szMyName = getMyName()
    szQuestion = GetReg ("PNO")

    if (StrLen(szQuestion) > 0)
        szQuestion = cat("for ", szQuestion);

    InsBufLine(hbuf, ln, "@szLine1@/* @where@: @action@ by @szMyName@, @szDate@ @szQuestion@ */");
}

macro expandEnd(hbuf,wordinfo,szLine,szLine1,ln,sel,title)
{
    DelBufLine(hbuf, ln)
    insertComment(hbuf, szLine1, " End ", title, ln)
}

macro expandBegin(hbuf,wordinfo,szLine,szLine1,ln,sel,title)
{
    DelBufLine(hbuf, ln)
    insertComment(hbuf, szLine1, "Begin", title, ln)
}

macro expandHfile(hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    DelBufLine(hbuf, ln)
    createHeadFile(hbuf)
}

macro expandCfile(hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    DelBufLine(hbuf, ln)
    createCFile(hbuf)
}

macro expandFunc(hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    var szFuncName

    szMyName = getMyName()
    isNew = 1

    DelBufLine(hbuf,ln)
    lnMax = GetBufLineCount(hbuf)
    if(ln != lnMax)
    {
        szNextLine = GetBufLine(hbuf,ln)
        if(strstr(szNextLine,"(") != -1)
        {
            szFuncName = GetCurSymbol()
            if(StrLen(szFuncName) != 0)
            {
                isNew = 0
            }
        }
    }
    if (szFuncName == nil)
    {
        szFuncName = Ask("Please input function name:")
    }
    insertFuncHeader(hbuf, ln, szFuncName, szMyName, isNew)
}

macro expandUt(hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    var szFuncName;
    var szCaseNum;

    isNew = 1

    DelBufLine(hbuf,ln);
    lnMax = GetBufLineCount(hbuf);
    if(ln != lnMax)
    {
        symbol = GetSymbolLocationFromLn(hbuf, ln)
        if(StrLen(symbol) > 0)
        {
            szNextLine = GetBufLine(hbuf, symbol.lnFirst);
            t1 = strtoken(szNextLine, "C Token", 0);
            t2 = strtoken(szNextLine, "C Token", t1.iEnd);
            t3 = strtoken(szNextLine, "C Token", t2.iEnd);

            if (t1.szToken == "TEST")
            {
                isNew = 0;
                szFuncName = strmid(t2.szToken, 3, strlen(t2.szToken));
                szCaseNum  = t3.szToken;
            }
        }
    }

    if (isNew)
    {
        szFuncName = Ask("Please input test function name:");
        iNameLen = strlen(szFuncName);
        i = 0;
        while (i < iNameLen)
        {
            if (szFuncName[i] == ".")
                szFuncName[i] = "_";
            i = i + 1;
        }
        szCaseNum  = Ask("Please input test case number:");
    }

    insertUtHeader(hbuf, ln, szFuncName, szCaseNum);
    if (isNew)
    {
        insertUtBody(hbuf, ln + 16, szFuncName, szCaseNum);
    }
}

macro insertFileHead(hbuf)
{
    ln = 0
    SetBufIns (hbuf, 0, 0)

    SysTime  = GetSysTime(1)
    szYear   = SysTime.Year
    szMyName = getMyName()
    szFile   = getFileName(GetBufName (hbuf), 1)
    szDate = getDateStr()
    szModule = Ask("Please input moudle name:")
    szContent= Ask("Please input file description:")

    InsBufLine(hbuf, ln + 0,  "/*******************************************************************************")
    InsBufLine(hbuf, ln + 1,  "  Copyright (c) @szYear@, Hangzhou H3C Technologies Co., Ltd. All rights reserved.")
    InsBufLine(hbuf, ln + 2,  "--------------------------------------------------------------------------------")
    InsBufLine(hbuf, ln + 3,  "                              @szFile@")
    InsBufLine(hbuf, ln + 4,  "  Project Code: Comware V7")
    InsBufLine(hbuf, ln + 5,  "   Module Name: @szModule@")
    InsBufLine(hbuf, ln + 6,  "  Date Created: @szDate@")
    InsBufLine(hbuf, ln + 7,  "        Author: @szMyName@")
    InsBufLine(hbuf, ln + 8,  "   Description: ")
    InsBufLine(hbuf, ln + 9,  "")
    InsBufLine(hbuf, ln + 10, "--------------------------------------------------------------------------------")
    InsBufLine(hbuf, ln + 11, "  Modification History")
    InsBufLine(hbuf, ln + 12, "  DATE        NAME             DESCRIPTION")
    InsBufLine(hbuf, ln + 13, "  ----------------------------------------------------------------------------  ")
    InsBufLine(hbuf, ln + 14, "")
    InsBufLine(hbuf, ln + 15, "*******************************************************************************/")

    //如果没有功能描述内容则提示输入
    //注释输出处理,自动换行
    SetBufIns(hbuf, ln + 8 ,0)
    DelBufLine(hbuf,ln + 8)
    commentContent(hbuf, ln + 8, "   Description: ", szContent, 0)
}

macro expandHead(hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    DelBufLine(hbuf, ln)
    insertFileHead(hbuf)
}

macro expandEnum(hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    DelBufLine(hbuf, ln)
    szStructName = Ask("Please input enum name")
    InsBufLine(hbuf, ln, "@szLine1@typedef enum tag@szStructName@");
    InsBufLine(hbuf, ln + 1, "@szLine1@{");
    InsBufLine(hbuf, ln + 2, "@szLine@");
    InsBufLine(hbuf, ln + 3, "@szLine1@} @szStructName@;");
    SetBufIns (hbuf, ln + 2, StrLen(szLine))
}

macro expandStruct(hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    DelBufLine(hbuf, ln)
    szStructName = Ask("Please input struct name")
    InsBufLine(hbuf, ln, "@szLine1@typedef struct tag@szStructName@");
    InsBufLine(hbuf, ln + 1, "@szLine1@{");
    InsBufLine(hbuf, ln + 2, "@szLine@");
    InsBufLine(hbuf, ln + 3, "@szLine1@} @szStructName@;");
    SetBufIns (hbuf, ln + 2, StrLen(szLine))
}

macro expandCase (hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    nSwitch = ask("Please input the number of case")

    DelBufLine(hbuf, ln)
    ln = ln - 1;
    if(nSwitch == 0)
    {
        hNewBuf = newbuf("clip")
        if(hNewBuf == hNil)
            return
        SetCurrentBuf(hNewBuf)
        PasteBufLine (hNewBuf, 0)
        nLeftMax = 0
        lnMax = GetBufLineCount(hNewBuf )
        i = 0
        fIsEnd = 1
        while ( i < lnMax)
        {
            szCase = GetBufLine(hNewBuf , i)
            RetVal = skipCommentFromString(szCase,fIsEnd)
            szCase = RetVal.szContent
            fIsEnd = RetVal.fIsEnd
            szCase = getSwitchVar(szCase)
            if(StrLen(szCase) != 0 )
            {
                ln = insertCase(hbuf, ln, szLine, szLine1, szCase)
            }
            i = i + 1
        }
        closebuf(hNewBuf)
    }
    else
    {
        nIdx = 0
        while(nIdx < nSwitch)
        {
            ln = insertCase(hbuf, ln, szLine, szLine1, "###")
            nIdx = nIdx + 1
        }
    }

    hwnd = GetCurrentWnd()
    SetWndSel(hwnd, sel)
    searchForward()
}

macro expandSwitch (hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    nSwitch = ask("Please input the number of case")
    SetBufSelText(hbuf, " (###)")
    InsBufLine(hbuf, ln + 1, "@szLine1@" # "{")
    InsBufLine(hbuf, ln + 2, "@szLine1@    " # "default:")
    InsBufLine(hbuf, ln + 3, "@szLine1@    " # "{")
    InsBufLine(hbuf, ln + 4, "@szLine1@    " # "    " # "###")
    InsBufLine(hbuf, ln + 5, "@szLine1@    " # "    " # "break;")
    InsBufLine(hbuf, ln + 6, "@szLine1@    " # "}")
    InsBufLine(hbuf, ln + 7, "@szLine1@" # "}")

    ln = ln + 1;
    szLine = cat(szLine,  "    ");
    szLine1= cat(szLine1, "    ");

    hwnd = GetCurrentWnd()
    nIdx = 0
    if(nSwitch == 0)
    {
        hNewBuf = newbuf("clip")
        if(hNewBuf == hNil)
            return
        SetCurrentBuf(hNewBuf)
        PasteBufLine (hNewBuf, 0)
        nLeftMax = 0
        lnMax = GetBufLineCount(hNewBuf )
        i = 0
        fIsEnd = 1
        while ( i < lnMax)
        {
            szCase = GetBufLine(hNewBuf , i)
            RetVal = skipCommentFromString(szCase,fIsEnd)
            szCase = RetVal.szContent
            fIsEnd = RetVal.fIsEnd
            szCase = getSwitchVar(szCase)
            if(StrLen(szCase) != 0 )
            {
                ln = insertCase(hbuf, ln, szLine, szLine1, szCase)
            }
            i = i + 1
        }
        closebuf(hNewBuf)
    }
    else
    {
        while(nIdx < nSwitch)
        {
            ln = insertCase(hbuf, ln, szLine, szLine1, "###")
            nIdx = nIdx + 1
        }
    }

    SetWndSel(hwnd, sel)
    searchForward()
}


macro insertCpp(hbuf,ln)
{
    InsBufLine(hbuf, ln, "")
    InsBufLine(hbuf, ln, "#endif")
    InsBufLine(hbuf, ln, "extern \"C\"{")
    InsBufLine(hbuf, ln, "#ifdef __cplusplus")
    InsBufLine(hbuf, ln, "")

    iTotalLn = GetBufLineCount (hbuf)
    InsBufLine(hbuf, iTotalLn, "")
    InsBufLine(hbuf, iTotalLn, "#endif")
    InsBufLine(hbuf, iTotalLn, "} /* extern \"C\" */")
    InsBufLine(hbuf, iTotalLn, "#ifdef __cplusplus")
    InsBufLine(hbuf, iTotalLn, "")
}

macro expandCpp(hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    DelBufLine(hbuf, ln)
    insertCpp(hbuf, ln)
}

macro expandIfdef(hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    SetBufSelText(hbuf, " (###)")
    InsBufLine(hbuf, ln+1, "@szLine1@###")
    InsBufLine(hbuf, ln+2, "@szLine1@#endif")
    searchForward()

    return
}

macro expandElse(hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    insertBrace(hbuf, ln, szLine, szLine1)
    searchForward()
    return
}

macro expandElseIf(hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    PutBufLine(hbuf, ln, szLine1 # "else if (###)")
    insertBrace(hbuf, ln, szLine, szLine1)
    searchForward()
    return
}

macro expandIfAndElse(hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    PutBufLine(hbuf, ln, szLine1 # "if (###)")
    ln = insertBrace(hbuf, ln, szLine, szLine1)
    InsBufLine(hbuf, ln + 1, "@szLine1@" # "else");
    ln = insertBrace(hbuf, ln+1, szLine, szLine1)
    searchForward()
    return
}

macro expandIfAndElseIf(hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    PutBufLine(hbuf, ln, szLine1 # "if (###)")
    ln = insertBrace(hbuf, ln, szLine, szLine1)
    InsBufLine(hbuf, ln + 1, "@szLine1@" # "else if (###)");
    ln = insertBrace(hbuf, ln+1, szLine, szLine1)
    InsBufLine(hbuf, ln + 1, "@szLine1@" # "else");
    ln = insertBrace(hbuf, ln+1, szLine, szLine1)
    searchForward()
    return
}

macro expandWhile (hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    SetBufSelText(hbuf, " (###)")
    insertBrace(hbuf, ln, szLine, szLine1)
    searchBackward()
}

macro expandDo (hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    InsBufLine(hbuf, ln + 1, "@szLine1@" # "{")
    InsBufLine(hbuf, ln + 2, "@szLine@" # "###");
    InsBufLine(hbuf, ln + 3, "@szLine1@" # "} while (###);")
    searchForward()
}

macro expandFor (hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    SetBufSelText(hbuf, " (###; ###; ###)")
    insertBrace(hbuf, ln, szLine, szLine1)
    searchBackward()
    searchBackward()
    searchBackward()
}

macro expandBrace (hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    DelBufLine(hbuf, ln)
    insertBrace(hbuf, ln-1, szLine, szLine1)
    searchForward()
}

macro expandDeclare (hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    DelBufLine(hbuf, ln);

    // 如果是h文件，打开对应的c文件
    szExtName = getFileExtName(GetBufName(hbuf));
    if (szExtName != "c")
    {
        szFileName = getFileName(GetBufName(hbuf), 0);
        szFileName = "@szFileName@.c";

        hCfile = OpenBuf(szFileName);
        if (hCfile == 0)
            stop;
    }
    else
    {
        hCfile = hbuf;
    }

    // 获取函数列表
    iSymbCnt = GetBufSymCount(hCfile);
    i = 0;
    while (i < iSymbCnt)
    {
        s = GetBufSymLocation(hCfile, i);

        if (strlen (s) > 0 && s.Type=="Function")
        {
            l = s.lnFirst;
            while (true)
            {
                szLine = GetBufLine(hCfile, l);
                if (-1 != strstr(szLine, ")"))
                {
                    InsBufLine(hbuf, ln, szLine # ";");
                    ln = ln + 1;
                    break;
                }
                else
                {
                    InsBufLine(hbuf, ln, szLine);
                    ln = ln + 1;
                }
                l = l + 1;
            }
            InsBufLine(hbuf, ln, "");
        }

        i = i + 1;
    }
}

macro expandDebug (hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    DelBufLine(hbuf, ln)
    InsBufLine(hbuf, ln + 0, "@szLine1@" # "#ifdef    MY_DEBUG_TEMP     /* MY_TEMP_DEL_THIS_LINE */")
    InsBufLine(hbuf, ln + 1, "@szLine1@" # "#else  /* MY_DEBUG_TEMP */  /* MY_TEMP_DEL_BLOCK_BELOW */")
    InsBufLine(hbuf, ln + 2, "@szLine1@" # "#endif /* MY_DEBUG_TEMP */  /* MY_TEMP_DEL_BLOCK_ABOVE */")
}

macro expandTemp (hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    DelBufLine(hbuf, ln)
    InsBufLine(hbuf, ln + 0, "@szLine1@" # "/* MY_TEMP_DEL_BLOCK_BELOW */")
    InsBufLine(hbuf, ln + 1, "@szLine1@" # "/* MY_TEMP_DEL_BLOCK_ABOVE */")
}

macro expandMain (hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    DelBufLine(hbuf, ln);
    InsBufLine(hbuf, ln + 0, "#include <stdio.h>");
    InsBufLine(hbuf, ln + 1, "");
    InsBufLine(hbuf, ln + 2, "int main(int argc, char* argv[])");
    InsBufLine(hbuf, ln + 3, "{");
    InsBufLine(hbuf, ln + 4, "    printf(\"Hello world!\\n\");");
    InsBufLine(hbuf, ln + 5, "");
    InsBufLine(hbuf, ln + 6, "    return 0;");
    InsBufLine(hbuf, ln + 7, "}");
    InsBufLine(hbuf, ln + 8, "");
}

macro expandCommentNewline(hbuf, ln, sel)
{
    szLine = GetBufLine(hbuf, ln);
    szTail = strmid(szLine, sel.ichLim, strlen(szLine));
    chTab = CharFromAscii(9);
    chSpace = CharFromAscii(32);
    ich = 0;
    while (szLine[ich] == chSpace || szLine[ich] == chTab)
    {
        ich = ich + 1
    }
    while (szLine[ich] != chSpace && szLine[ich] != chTab)
    {
        ich = ich + 1
    }
    while (szLine[ich] == chSpace || szLine[ich] == chTab)
    {
        ich = ich + 1
    }
    szSuffix = StrMid(szLine, 0, ich);
    PutBufLine(hbuf, ln, trimRight(strmid(szLine, 0, sel.ichLim)));
    InsBufLine(hbuf, ln + 1, szSuffix # trimString(szTail));
    SetBufIns (hbuf, ln + 1, ich)
}

macro insertBrace(hbuf, ln, szLine, szLine1)
{
    InsBufLine(hbuf, ln + 1, "@szLine1@" # "{");
    InsBufLine(hbuf, ln + 2, "@szLine@" # "###");
    InsBufLine(hbuf, ln + 3, "@szLine1@" # "}");
    return (ln + 3)
}

macro insertCase(hbuf, ln, szLine, szLine1, szCase)
{
    InsBufLine(hbuf, ln + 1, "@szLine1@" # "case @szCase@:");
    InsBufLine(hbuf, ln + 2, "@szLine1@" # "{");
    InsBufLine(hbuf, ln + 3, "@szLine@"  # "###");
    InsBufLine(hbuf, ln + 4, "@szLine@"  # "break;");
    InsBufLine(hbuf, ln + 5, "@szLine1@" # "}");
    return (ln + 5)
}

macro expandComment(hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    SetBufSelText(hbuf, " ### */")
    searchBackward()
}

macro expandCommentBlock(hbuf,wordinfo,szLine,szLine1,ln,sel)
{
    szCurLine = GetBufLine(hbuf, sel.lnFirst)
    i = 0
    while (i < wordinfo.ichLim - 2)
    {
        if (szCurLine[i] == "/" && szCurLine[i+1] == "*")
        {
            break
        }
        i = i + 1
    }

    if (i < wordinfo.ichLim - 2)
    {
        /* 该行有成对的注释符号，截取注释符号之间的内容，格式化注释 */
        szLeft =  StrMid(szCurLine,0, i)
        szContent = StrMid(szCurLine,i+2, wordinfo.ichLim - 2)
        szContent = trimString(szContent)
    }
    else
    {
        /* 该行没有成对的注释符号，要求用户输入注释 */
        if(wordinfo.ichLim > 70)
        {
            Msg("The right margine is small, Please use a new line")
            stop
        }
        szLeft =  StrMid(szCurLine,0,wordinfo.ichLim-2)
        lineLen = StrLen(szCurLine)
        kk = 0
        while(wordinfo.ichLim + kk < lineLen)
        {
            if((szCurLine[wordinfo.ichLim + kk] != " ")||(szCurLine[wordinfo.ichLim + kk] != "\t")
            {
                Msg("you must insert */ at the end of a line");
                stop
            }
            kk = kk + 1
        }
        szContent = Ask("Please input comment")
    }
    DelBufLine(hbuf, ln)
    szLeft = cat( szLeft, "/* ")
    commentContent(hbuf,ln,szLeft,szContent,1)
}

macro searchForward()
{
    LoadSearchPattern("###", 1, 0, 1);
    Search_Forward
}

macro searchBackward()
{
    LoadSearchPattern("###", 1, 0, 1);
    Search_Backward
}

macro commentContent (hbuf,ln,szPreStr,szContent,isEnd)
{
    szLeftBlank = szPreStr
    iLen = StrLen(szPreStr)
    k = 0
    while(k < iLen)
    {
        szLeftBlank[k] = " ";
        k = k + 1;
    }

    hNewBuf = newbuf("clip")
    if(hNewBuf == hNil)
        return
    SetCurrentBuf(hNewBuf)
    PasteBufLine (hNewBuf, 0)
    lnMax = GetBufLineCount( hNewBuf )
    szTmp = trimString(szContent)

    //判断如果剪贴板是0行时对于有些版本会有问题，要排除掉
    if(lnMax != 0)
    {
        szLine = GetBufLine(hNewBuf , 0)
        ret = strstr(szLine,szTmp)
        if(ret == 0)
        {
            /* 如果输入窗输入的内容是剪贴板的一部分说明是剪贴过来的取剪贴板中的内容 */
            szContent = trimString(szLine)
        }
        else
        {
            lnMax = 1
        }
    }
    else
    {
        lnMax = 1
    }
    szRet = ""
    nIdx = 0
    while ( nIdx < lnMax)
    {
        if(nIdx != 0)
        {
            szLine = GetBufLine(hNewBuf , nIdx)
            szContent = trimLeft(szLine)
               szPreStr = szLeftBlank
        }
        iLen = StrLen (szContent)
        szTmp = cat(szPreStr,"#");
        if( (iLen == 0) && (nIdx == (lnMax - 1))
        {
            InsBufLine(hbuf, ln, "@szTmp@")
        }
        else
        {
            i = 0
            //以每行75个字符处理
            while  (iLen - i > 75 - k )
            {
                j = 0
                while(j < 75 - k)
                {
                    iNum = szContent[i + j]
                    if( AsciiFromChar (iNum)  > 160 )
                    {
                       j = j + 2
                    }
                    else
                    {
                       j = j + 1
                    }
                    if( (j > 70 - k) && (szContent[i + j] == " ") )
                    {
                        break
                    }
                }
                if( (szContent[i + j] != " " ) )
                {
                    n = 0;
                    iNum = szContent[i + j + n]
                    //如果是中文字符只能成对处理
                    while( (iNum != " " ) && (AsciiFromChar (iNum)  < 160))
                    {
                        n = n + 1
                        if((n >= 3) ||(i + j + n >= iLen))
                             break;
                        iNum = szContent[i + j + n]
                    }
                    if(n < 3)
                    {
                        //分段后只有小于3个的字符留在下段则将其以上去
                        j = j + n
                        sz1 = StrMid(szContent,i,i+j)
                        sz1 = cat(szPreStr,sz1)
                    }
                    else
                    {
                        //大于3个字符的加连字符分段
                        sz1 = StrMid(szContent,i,i+j)
                        sz1 = cat(szPreStr,sz1)
                        if(sz1[StrLen(sz1)-1] != "-")
                        {
                            sz1 = cat(sz1,"-")
                        }
                    }
                }
                else
                {
                    sz1 = StrMid(szContent,i,i+j)
                    sz1 = cat(szPreStr,sz1)
                }
                InsBufLine(hbuf, ln, "@sz1@")
                ln = ln + 1
                szPreStr = szLeftBlank
                i = i + j
                while(szContent[i] == " ")
                {
                    i = i + 1
                }
            }
            sz1 = StrMid(szContent,i,iLen)
            sz1 = cat(szPreStr,sz1)
            if((isEnd == 1) && (nIdx == (lnMax - 1))
            {
                sz1 = cat(sz1," */")
            }
            InsBufLine(hbuf, ln, "@sz1@")
        }
        ln = ln + 1
        nIdx = nIdx + 1
    }
    closebuf(hNewBuf)
    return ln - 1
}

macro getWordFromString(hbuf,szLine,nBeg,nEnd,chBeg,chSeparator,chEnd)
{
    if((nEnd > StrLen(szLine) || (nBeg > nEnd))
    {
        return 0
    }
    nMaxLen = 0
    nIdx = nBeg
    //先定位到开始字符标记处
    while(nIdx < nEnd)
    {
        if(szLine[nIdx] == chBeg)
        {
            break
        }
        nIdx = nIdx + 1
    }
    nBegWord = nIdx + 1

    //用于检测chBeg和chEnd的配对情况
    iCount = 0

    nEndWord = 0
    //以分隔符为标记进行搜索
    while(nIdx < nEnd)
    {
        if(szLine[nIdx] == chSeparator)
        {
           szWord = StrMid(szLine,nBegWord,nIdx)
           szWord = trimString(szWord)
           nLen = StrLen(szWord)
           if(nMaxLen < nLen)
           {
               nMaxLen = nLen
           }
           AppendBufLine(hbuf,szWord)
           nBegWord = nIdx + 1
        }
        if(szLine[nIdx] == chBeg)
        {
            iCount = iCount + 1
        }
        if(szLine[nIdx] == chEnd)
        {
            iCount = iCount - 1
            nEndWord = nIdx
            if( iCount == 0 )
            {
                break
            }
        }
        nIdx = nIdx + 1
    }
    if(nEndWord > nBegWord)
    {
        szWord = StrMid(szLine,nBegWord,nEndWord)
        szWord = trimString(szWord)
        nLen = StrLen(szWord)
        if(nMaxLen < nLen)
        {
            nMaxLen = nLen
        }
        AppendBufLine(hbuf,szWord)
    }
    return nMaxLen
}

macro getFunctionDef(hbuf,symbol)
{
    return getLogicLine(hbuf, symbol.lnFirst, "{");
}

macro getLogicLine(hbuf, ln, lim)
{
    var szPhyLine;
    szPhyLine = ""

    var szLogicLine;
    szLogicLine = ""

    var isEnd;
    isEnd = 1;
    while (true)
    {
        szPhyLine = GetBufLine(hbuf, ln);

        var t;
        t = skipCommentFromString(szPhyLine, isEnd);

        isEnd = t.fIsEnd;
        szPhyLine = trimString(t.szContent);
        szLogicLine = cat(szLogicLine, szPhyLine);
        i = strstr(szLogicLine, lim);
        if (-1 != i)
        {
            szLogicLine = strmid(szLogicLine, 0, i);
            break;
        }
        ln = ln + 1;
    }

    return szLogicLine;
}

macro getFirstWord(szTemp)
{
    t = strtoken(szTemp, "C Token", 0);
    return t.szToken;
}

macro createBlankString(iCnt)
{
    s = ""
    i = 0;
    while (i < iCnt)
    {
        s = cat(s, " ");
        i = i + 1;
    }
    return s;
}

macro insertUtHeader(hbuf, ln, szFuncName, szCaseNum)
{
    szData = getDateStr();
    szAuther = getMyName();
    InsBufLine(hbuf, ln + 0,  "#define UT_@szFuncName@_@szCaseNum@");
    InsBufLine(hbuf, ln + 1,  "/*#######################################################################");
    InsBufLine(hbuf, ln + 2,  "#测试用例编号: UT_@szFuncName@_@szCaseNum@");
    InsBufLine(hbuf, ln + 3,  "#测  试  项  : @szFuncName@");
    InsBufLine(hbuf, ln + 4,  "#测试用例标题: ");
    InsBufLine(hbuf, ln + 5,  "#重 要 级 别 : 高/中/低");
    InsBufLine(hbuf, ln + 6,  "#预 置 条 件 : 1. ");
    InsBufLine(hbuf, ln + 7,  "#              2. ");
    InsBufLine(hbuf, ln + 8,  "#输       入 : 1. ");
    InsBufLine(hbuf, ln + 9,  "#              2. ");
    InsBufLine(hbuf, ln + 10, "#操 作 步 骤 : 执行测试用例");
    InsBufLine(hbuf, ln + 11, "#预 期 结 果 : 1. ");
    InsBufLine(hbuf, ln + 12, "#              2. ");
    InsBufLine(hbuf, ln + 13, "#完  成  人  : @szAuther@");
    InsBufLine(hbuf, ln + 14, "#日      期  : @szData@");
    InsBufLine(hbuf, ln + 15, "########################################################################*/");
}

macro insertUtBody(hbuf, ln, szFuncName, szCaseNum)
{
    InsBufLine(hbuf, ln + 0,  "TEST(UT_@szFuncName@, @szCaseNum@)");
    InsBufLine(hbuf, ln + 1,  "{");
    InsBufLine(hbuf, ln + 2,  "    /* Step1. 局部变量定义 */");
    InsBufLine(hbuf, ln + 3,  "");
    InsBufLine(hbuf, ln + 4,  "    /* Step2. 设置预置条件 */");
    InsBufLine(hbuf, ln + 5,  "");
    InsBufLine(hbuf, ln + 6,  "    /* Step3. 构造输入参数 */");
    InsBufLine(hbuf, ln + 7,  "");
    InsBufLine(hbuf, ln + 8,  "    /* Step4. 打桩 */");
    InsBufLine(hbuf, ln + 9,  "");
    InsBufLine(hbuf, ln + 10, "    /* Step5. 运行测试用例 */");
    InsBufLine(hbuf, ln + 11, "");
    InsBufLine(hbuf, ln + 12, "    /* Step6. 撤桩 */");
    InsBufLine(hbuf, ln + 13, "");
    InsBufLine(hbuf, ln + 14, "    /* Step7. 检查预期结果 */");
    InsBufLine(hbuf, ln + 15, "");
    InsBufLine(hbuf, ln + 16, "    /* Step8. 恢复测试环境 */");
    InsBufLine(hbuf, ln + 17, "");
    InsBufLine(hbuf, ln + 18, "}");
    InsBufLine(hbuf, ln + 19, "");
}

macro insertFuncHeader(hbuf, ln, szFunc, szMyName, newFunc)
{
    iIns = 0
    if(newFunc != 1)
    {
        symbol = GetSymbolLocationFromLn(hbuf, ln)
        if(StrLen(symbol) > 0)
        {
            hTmpBuf = openEmptyBuf("Tempbuf")

            //将文件参数头整理成一行并去掉了注释
            szLine = getFunctionDef(hbuf,symbol)
            iBegin = symbol.ichName
            //取出返回值定义
            szTemp = StrMid(szLine,0,iBegin)
            szTemp = trimString(szTemp)
            szRet =  getFirstWord(szTemp)
            if(symbol.Type == "Method")
            {
                szTemp = StrMid(szTemp,StrLen(szRet),StrLen(szTemp))
                szTemp = trimString(szTemp)
                if(szTemp == "::")
                {
                    szRet = ""
                }
            }
            if(toupper (szRet) == "MACRO")
            {
                //对于宏返回值特殊处理
                szRet = ""
            }
            //从函数头分离出函数参数
            nMaxParamSize = getWordFromString(hTmpBuf,szLine,iBegin,StrLen(szLine),"(",",",")")
            lnMax = GetBufLineCount(hTmpBuf)
            ln = symbol.lnFirst
            SetBufIns (hbuf, ln, 0)
        }
    }
    else
    {
        lnMax = 0
        szLine = ""
        szRet = ""
    }

    InsBufLine(hbuf, ln, "/*****************************************************************************")
    if( StrLen(szFunc)>0 )
    {
        InsBufLine(hbuf, ln+1, "    Func Name: @szFunc@")
    }
    else
    {
        InsBufLine(hbuf, ln+1, "    Func Name: ###")
    }

    szDate = getDateStr()
    InsBufLine(hbuf, ln+2, " Date Created: @szDate@ ")

    if(StrLen(szMyName)>0)
    {
       InsBufLine(hbuf, ln+3, "       Author: @szMyName@")
    }
    else
    {
       InsBufLine(hbuf, ln+3, "       Author: ###")
    }

    oldln = ln
    InsBufLine(hbuf, ln+4, "  Description:")
    szIns = "        Input: "
    if(newFunc != 1)
    {
        //对于已经存在的函数插入函数参数
        i = 0
        while ( i < lnMax)
        {
            szTmp = GetBufLine(hTmpBuf, i)
            nLen = StrLen(szTmp);
            szBlank = createBlankString(nMaxParamSize - nLen + 2)
            szTmp = cat(szTmp,szBlank)
            ln = ln + 1
            szTmp = cat(szIns,szTmp)
            InsBufLine(hbuf, ln+4, "@szTmp@")
            iIns = 1
            szIns = "               "
            i = i + 1
        }
        closebuf(hTmpBuf)
    }
    if(iIns == 0)
    {
            ln = ln + 1
            InsBufLine(hbuf, ln+4, "        Input: ")
    }
    InsBufLine(hbuf, ln+5, "       Output: ")
    InsBufLine(hbuf, ln+6, "       Return: @szRet@")
    InsBufLine(hbuf, ln+7, "      Caution: ")

    InsBufLine(hbuf, ln + 8,   "-----------------------------------------------------------------------------")
    InsBufLine(hbuf, ln + 9,   "   Modification History                                                      ")
    InsBufLine(hbuf, ln + 10,  "   DATE        NAME             DESCRIPTION                                  ")
    InsBufLine(hbuf, ln + 11,  "-----------------------------------------------------------------------------")
    InsBufLine(hbuf, ln + 12,  "                                                                             ")
    InsBufLine(hbuf, ln + 13,  "******************************************************************************/")

    if ((newFunc == 1) && (StrLen(szFunc)>0))
    {
        InsBufLine(hbuf, ln+14, "int @szFunc@(###)")
        InsBufLine(hbuf, ln+15, "{");
        InsBufLine(hbuf, ln+16, "    ###");
        InsBufLine(hbuf, ln+17, "}");
        searchForward()
    }
    hwnd = GetCurrentWnd()
    if (hwnd == 0)
        stop
    sel = GetWndSel(hwnd)
    sel.ichFirst = 0
    sel.ichLim = sel.ichFirst
    sel.lnFirst = ln + 14
    sel.lnLast = ln + 14
    szContent = Ask("Please input function description")
    setWndSel(hwnd,sel)
    DelBufLine(hbuf,oldln + 4)

    //显示输入的功能描述内容
    newln = commentContent(hbuf,oldln+4,"  Description: ",szContent,0) - 2
    ln = ln + newln - oldln
    if ((newFunc == 1) && (StrLen(szFunc)>0))
    {
        isFirstParam = 1

        //提示输入新函数的返回值
        szRet = Ask("Please input return value type")
        if(StrLen(szRet) > 0)
        {
            PutBufLine(hbuf, ln+4, "       Return: @szRet@")
            PutBufLine(hbuf, ln+12, "@szRet@ @szFunc@(###)")
            SetbufIns(hbuf,ln+12,StrLen(szRet)+StrLen(szFunc) + 2
        }
        szFuncDef = ""
        sel.ichFirst = StrLen(szFunc)+StrLen(szRet) + 2
        sel.ichLim = sel.ichFirst + 3
        //循环输入参数
        while (1)
        {
            szParam = ask("Please input parameter")
            szParam = trimString(szParam)
            szTmp   = cat(szIns,szParam)
            szParam = cat(szFuncDef,szParam)
            sel.lnFirst = ln + 12
            sel.lnLast = ln + 12
            setWndSel(hwnd,sel)
            sel.ichFirst = sel.ichFirst + StrLen(szParam)
            sel.ichLim = sel.ichFirst
            oldsel = sel
            if(isFirstParam == 1)
            {
                PutBufLine(hbuf, ln+2, "@szTmp@")
                isFirstParam  = 0
            }
            else
            {
                ln = ln + 1
                InsBufLine(hbuf, ln+2, "@szTmp@")
                oldsel.lnFirst = ln + 12
                oldsel.lnLast = ln + 12
            }
            SetBufSelText(hbuf,szParam)
            szIns = "               "
            szFuncDef = ", "
            oldsel.lnFirst = ln + 14
            oldsel.lnLast = ln + 14
            oldsel.ichFirst = 4
            oldsel.ichLim = 7
            setWndSel(hwnd,oldsel)
        }
    }
    return ln + 17

}

// ok
macro createHeadFile(hbuf)
{
    szName = getMyName()

    ln = 0

    szExtName = getFileExtName(GetBufName(hbuf))
    if (szExtName != "h")
    {
        szFileName = getFileName(GetBufName(hbuf), 0)
        szFileName = "@szFileName@.h"

        hOutbuf = openWndBuf(szFileName)

        SetCurrentBuf(hOutbuf)
    }
    else
    {
        szFileName = getFileName(GetBufName(hbuf), 1)
        hOutbuf = GetCurrentBuf()
    }

    szPreH = "__" # toupper (getFileName(szFileName, 0)) # "_H__"

    insertCpp(hOutbuf, 0)
    InsBufLine(hOutbuf, 0, "")
    InsBufLine(hOutbuf, 0, "#define @szPreH@")
    InsBufLine(hOutbuf, 0, "#ifndef @szPreH@")
    iTotalLn = GetBufLineCount (hOutbuf)
    InsBufLine(hOutbuf, iTotalLn, "")
    InsBufLine(hOutbuf, iTotalLn, "#endif /* @szPreH@ */")
    insertFileHead(hOutbuf)
}

macro createCFile(hbuf)
{
    szName = getMyName()

    ln = 0

    szExtName = getFileExtName(GetBufName(hbuf))
    if (szExtName != "c")
    {
        szFileName = getFileName(GetBufName(hbuf), 0)
        szFileName = "@szFileName@.c"

        hOutbuf = openWndBuf(szFileName)

        SetCurrentBuf(hOutbuf)
    }
    else
    {
        szFileName = getFileName(GetBufName(hbuf), 1)
        hOutbuf = GetCurrentBuf()
    }

    InsBufLine(hOutbuf, 0, "")
    InsBufLine(hOutbuf, 0, "")
    InsBufLine(hOutbuf, 0, "/* Function define */")
    InsBufLine(hOutbuf, 0, "")
    InsBufLine(hOutbuf, 0, "")
    InsBufLine(hOutbuf, 0, "/* Function declare */")
    InsBufLine(hOutbuf, 0, "")
    InsBufLine(hOutbuf, 0, "")
    InsBufLine(hOutbuf, 0, "/* Structure define */")
    InsBufLine(hOutbuf, 0, "")
    InsBufLine(hOutbuf, 0, "")
    InsBufLine(hOutbuf, 0, "/* Macro define */")
    InsBufLine(hOutbuf, 0, "")
    InsBufLine(hOutbuf, 0, "")
    InsBufLine(hOutbuf, 0, "/* Include header file */")
    InsBufLine(hOutbuf, 0, "")
    insertCpp(hOutbuf, 0)
    InsBufLine(hOutbuf, 0, "")
    insertFileHead(hOutbuf)
}

macro getSwitchVar(szLine)
{
    if( (szLine == "{") || (szLine == "}") )
    {
        return ""
    }
    ret = strstr(szLine,"#define" )
    if(ret != -1)
    {
        szLine = StrMid(szLine,ret + 8,StrLen(szLine))
    }
    szLine = trimLeft(szLine)
    nIdx = 0
    nLen = StrLen(szLine)
    while( nIdx < nLen)
    {
        if((szLine[nIdx] == " ") || (szLine[nIdx] == ",") || (szLine[nIdx] == "="))
        {
            szLine = StrMid(szLine,0,nIdx)
            return szLine
        }
        nIdx = nIdx + 1
    }
    return szLine
}

macro skipCommentFromString(szLine,isCommentEnd)
{
    RetVal = ""
    fIsEnd = 1
    nLen = StrLen(szLine)
    nIdx = 0
    while(nIdx < nLen )
    {
        //如果当前行开始还是被注释，或遇到了注释开始的变标记，注释内容改为空格?
        if( (isCommentEnd == 0) || (szLine[nIdx] == "/" && szLine[nIdx+1] == "*"))
        {
            fIsEnd = 0
            while(nIdx < nLen )
            {
                if(szLine[nIdx] == "*" && szLine[nIdx+1] == "/")
                {
                    szLine[nIdx+1] = " "
                    szLine[nIdx] = " "
                    nIdx = nIdx + 1
                    fIsEnd  = 1
                    isCommentEnd = 1
                    break
                }
                szLine[nIdx] = " "
                nIdx = nIdx + 1
            }

            //如果已经到了行尾终止搜索
            if(nIdx == nLen)
            {
                break
            }
        }

        //如果遇到的是//来注释的说明后面都为注释
        if(szLine[nIdx] == "/" && szLine[nIdx+1] == "/")
        {
            szLine = StrMid(szLine,0,nIdx)
            break
        }
        nIdx = nIdx + 1
    }
    RetVal.szContent = szLine;
    RetVal.fIsEnd = fIsEnd
    return RetVal
}

macro isInComment(hbuf, ln)
{
    LoadSearchPattern("/*", 0, 0, 0);
    Search_Backward
    hwnd = GetCurrentWnd ();
    s = GetWndSel (hwnd);
    if (s.lnFirst >= ln)
    {
        return false;
    }

    s = SearchInBuf (hbuf, "*/", s.lnFirst, s.ichFirst, 0, 0, 0);
    if ("" == s)
    {
        return false;
    }
    if (s.lnFirst < ln)
    {
        return false;
    }

    return true;
}

macro getMyName()
{
    szMyName = GetReg(MYNAME)
    if(StrLen( szMyName ) == 0)
    {
        szMyName = Ask("Please input your name:")
        SetReg(MYNAME, szMyName)
    }
    return szMyName
}

macro getWordLeftOfIch(ich, sz)
{
    wordinfo = "" // create a "wordinfo" structure

    chTab = CharFromAscii(9)

    // scan backwords over white space, if any
    ich = ich - 1;
    if (ich >= 0)
        while (sz[ich] == " " || sz[ich] == chTab)
        {
            ich = ich - 1;
            if (ich < 0)
                break;
        }

    // scan backwords to start of word
    ichLim = ich + 1;
    asciiA = AsciiFromChar("A")
    asciiZ = AsciiFromChar("Z")
    while (ich >= 0)
    {
        ch = toupper(sz[ich])
        asciiCh = AsciiFromChar(ch)

        if ((asciiCh < asciiA || asciiCh > asciiZ)
           && !IsNumber(ch)
           && ( ch != "#" && ch != "{" && ch != "/" && ch != "*"))
            break;

        ich = ich - 1;
    }

    ich = ich + 1
    wordinfo.szWord = StrMid(sz, ich, ichLim)
    wordinfo.ich = ich
    wordinfo.ichLim = ichLim;

    return wordinfo
}

macro getDateStr()
{
    SysTime  = GetSysTime(1)
    szYear   = SysTime.Year
    szMonth  = SysTime.month
    szDay    = SysTime.day

    return "@szYear@-@szMonth@-@szDay@"
}
