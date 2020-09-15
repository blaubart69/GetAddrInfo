#define  UNICODE
#define _UNICODE

#define WIN32_LEAN_AND_MEAN
#define STRICT
//#include <Windows.h>

#include <winsock2.h>
#include <Ws2tcpip.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#include <shellapi.h>

DWORD WriteString(LPCWSTR str, DWORD numberOfCharsToWrite);

void rawmain()
{
    WSADATA wsaData;
    ADDRINFOW* result = NULL;
    int rc = 0;

    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (argc != 2)
    {
        rc = ERROR_INVALID_PARAMETER;
    }
    else if ((rc = WSAStartup(MAKEWORD(2, 2), &wsaData))       != 0)
    { }
    else if ((rc = GetAddrInfoW(argv[1], NULL, NULL, &result)) != 0)
    { }
    else
    {
        WCHAR szIP[64];
		ADDRINFOW* ptr;

        for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
        {
            DWORD IPbuflen = 64;
            if ((rc = WSAAddressToStringW((LPSOCKADDR)ptr->ai_addr, (DWORD)ptr->ai_addrlen, NULL, szIP, &IPbuflen)) != 0)
            {
                break;
            }

            szIP[IPbuflen - 1] = L'\r';
            szIP[IPbuflen    ] = L'\n';

            if ((rc = WriteString(szIP, IPbuflen + 1) != 0))
            {
                break;
            }
        }

        FreeAddrInfoW(result);
    }
    
    WSACleanup();
    ExitProcess(rc);
}
DWORD WriteString(LPCWSTR str, DWORD numberOfCharsToWrite)
{
    HANDLE  hStdOut;
    DWORD   written;
    int     bytesWritten;
    LPSTR   multibyteBuffer = NULL;
    int     multibyteBufferSize;
    DWORD   rc = 0;

    if ((hStdOut = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE) 
    {
        rc = ERROR_INVALID_HANDLE;
    }
    else if (GetFileType(hStdOut) == FILE_TYPE_CHAR)
    {
        if (!WriteConsole(hStdOut, str, numberOfCharsToWrite, &written, NULL))
        {
            rc = GetLastError();
        }
    }
    else if ((multibyteBuffer = (LPSTR)HeapAlloc(GetProcessHeap(), 0, multibyteBufferSize = numberOfCharsToWrite * 3)) == NULL)
    {
        rc = ERROR_NOT_ENOUGH_MEMORY;
    }
    else if ((bytesWritten = WideCharToMultiByte(
                          GetConsoleOutputCP()      // codepage
                        , 0                         // dwFlags [in]
                        , str                       // lpWideCharStr [in]
                        , numberOfCharsToWrite      // cchWideChar [in]
                        , multibyteBuffer           // lpMultiByteStr [out, optional]
                        , multibyteBufferSize       // cbMultiByte [in]
                        , NULL                      // lpDefaultChar[in, optional]
                        , NULL)) == 0) 
    {
        rc = GetLastError();
    }
    else if ( ! WriteFile(hStdOut, multibyteBuffer, bytesWritten, &written, NULL) )
    {
        rc = GetLastError();
    }

    if (multibyteBuffer != NULL)
    {
        HeapFree(GetProcessHeap(), 0, multibyteBuffer);
    }

    return rc;
}
