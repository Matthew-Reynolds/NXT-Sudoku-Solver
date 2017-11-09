/**********************************************************************
Filename: PC_FileIO.c
Date: November 5, 2017
File Version: 1.3

ASCII conversion functions so that files read or written on a PC can be
used on the Lego EV3 or NXT bricks.  Whitespace (space, tab, enter) is needed
between values.

The functions available are:
bool openWritePC(TFileHandle & fout, char* name);
bool openWritePC(TFileHandle & fout, char* name, word fileSize); // file size can be specified for NXT
bool openReadPC(TFileHandle & fin, char* name);
bool closeFilePC(TFileHandle & fileHandle);
bool writeCharPC(TFileHandle & fout, byte charmsg);
bool writeEndlPC(TFileHandle & fout);
bool writeTextPC(TFileHandle & fout, string const & textmsg);
bool writeLongPC(TFileHandle & fout, long number);
bool writeFloatPC(TFileHandle & fout, string const & numFormat, float number);
bool writeFloatPC(TFileHandle & fout, float number);
bool readCharPC(TFileHandle & fin, char & charmsg);
bool readTextPC(TFileHandle & fin, string & result); // a maximum of 20 characters can be read
bool readIntPC(TFileHandle & fin, int & number);
bool readFloatPC(TFileHandle & fin, float & number);

History
Ver  Date       Comment
1.3  Nov  5/17  if platform is not specified, default is now EV3
1.2  Nov  7/16  file status is now returned as a bool; added EV3 functionality;
                flag is used to select between NXT functions and EV3; added
                test program to be part of file
1.1  Nov 19/15  added PCReadChar which ignores whitespace when reading characters
1.0  Jul  2/15  original release

**********************************************************************/
#pragma SystemFile
#ifndef PC_FILEIO
#define PC_FILEIO

// if platform is not specified, default is EV3
#ifndef _EV3FILEIO
#define _EV3FILEIO 1
#endif

#define _MAIN_PCFILEIO 1  // 1 for test main program, 0 for use in other programs

// ASCII characters
const byte _NULL = 0;
const byte _TAB = 9;
const byte _LF = 10;
const byte _CR = 13;

// define equivalents for EV3 and NXT platforms
#if _EV3FILEIO
typedef int TFileHandle;
typedef bool TFileIOResult;
#endif

// status flag
#if _EV3FILEIO
const TFileIOResult _IO_OKAY = true;
#else
const TFileIOResult _IO_OKAY = (TFileIOResult)0;
#endif

bool openWritePC(TFileHandle & fout, char* name, word fileSize=1000)
{
    TFileIOResult status = _IO_OKAY;
#if _EV3FILEIO
    fout = fileOpenWrite(name);
#else
    Delete(name, status);
    OpenWrite(fout, status, name, fileSize);
#endif
    return status == _IO_OKAY;
}

bool openReadPC(TFileHandle & fin, char* name)
{
    TFileIOResult status = _IO_OKAY;
#if _EV3FILEIO
    fin = fileOpenRead(name);
#else
    word fileSize = 0;
    OpenRead(fin, status, name, fileSize);
#endif
    return status == _IO_OKAY;
}

bool closeFilePC(TFileHandle & fileHandle)
{
    TFileIOResult status = _IO_OKAY;
#if _EV3FILEIO
    status = fileClose(fileHandle);
#else
    Close(fileHandle, status);
#endif
    return status == _IO_OKAY;
}

bool writeCharPC(TFileHandle & fout, byte charmsg)
{
    TFileIOResult status = _IO_OKAY;
#if _EV3FILEIO
    status = fileWriteChar(fout, charmsg);
#else
    WriteByte(fout, status, charmsg);
#endif
    return status == _IO_OKAY;
}

bool writeEndlPC(TFileHandle & fout)
{
    bool statusOK = writeCharPC(fout, _CR);  // both CR and LF are needed for simple editors (e.g. Notepad)
    if (statusOK)
        statusOK = writeCharPC(fout, _LF);
    return statusOK;
}

bool writeTextPC(TFileHandle & fout, string const & textmsg)
{
    TFileIOResult status = _IO_OKAY;
#if _EV3FILEIO
    int msglen = strlen(textmsg) + 1;  // add 1 to include the zero terminator
    status = fileWriteData(fout, textmsg, msglen);
#else
    WriteText(fout, status, textmsg);
#endif
    return status == _IO_OKAY;
}

bool writeLongPC(TFileHandle & fout, long number)
{
    string outputString;
    stringFormat(outputString, "%d", number);
    return writeTextPC(fout, outputString);
}

bool writeFloatPC(TFileHandle & fout, string const & numFormat, float number)
{
    string outputString;
    stringFormat(outputString, numFormat, number);
    return writeTextPC(fout, outputString);
}

bool writeFloatPC(TFileHandle & fout, float number)
{
    return writeFloatPC(fout, "%f", number);
}

bool readBytePC(TFileHandle & fin, byte & charmsg)
{
    TFileIOResult status = _IO_OKAY;
#if _EV3FILEIO
    status = fileReadChar(fin, &charmsg);
#else
    ReadByte(fin,status, charmsg);
#endif
    return status == _IO_OKAY;
}

bool _isWhiteSpace(char c)
{
    return c == _NULL || c == ' ' || c == _TAB || c == _CR || c == _LF;
}

bool readCharPC(TFileHandle & fin, char & charmsg)
{
    // ignore leading whitespace or NULLs
    bool status = true;
    do
        status = readBytePC(fin, charmsg);
    while (status && _isWhiteSpace(charmsg));
    return status;
}

bool readTextPC(TFileHandle & fin, string & result) // a maximum of 20 characters can be read
{
    const int MAX_SIZE = 20;
    byte msg[MAX_SIZE+1];  // add one to size so can be null terminated

    // ignore leading whitespace or NULLs
    bool statusChar = readCharPC(fin, msg[0]);

    // get string, first byte is already available from above if statusChar is true
    int n = 0;
    while(statusChar && n < MAX_SIZE && !_isWhiteSpace(msg[n])) // characters will be lost if string size exceeds 20
    {
        n++;
        statusChar = readBytePC(fin, msg[n]);
    }

    bool status = true;
    if (n > 0)
    {
        msg[n] = _NULL;  // null terminate the string
        stringFromChars(result, &msg[0]);
        status = true;
    }
    else
    {
        result = "";
        status = false;
    }
    return status;
}

bool readIntPC(TFileHandle & fin, int & number)
{
    string numstr;
    bool status = readTextPC(fin, numstr);
    if (status)
        number = atoi(numstr);
    return status;
}

bool readFloatPC(TFileHandle & fin, float & number)
{
    string numstr;
    bool status = readTextPC(fin, numstr);
    if (status)
        number = atof(numstr);
    return status;
}




#endif
/**********************************************************************
Copyright(c) 2015-2016 C.C.W. Hulls, P.Eng.
Students, staff, and faculty members at the University of Waterloo
are granted a non-exclusive right to copy, modify, or use this
software for non-commercial teaching, learning, and research purposes
provided the author(s) are acknowledged except for as noted below.
**********************************************************************/
