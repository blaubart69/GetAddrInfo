#define  UNICODE
#define _UNICODE

#define WIN32_LEAN_AND_MEAN
#define STRICT
//#include <Windows.h>

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <Wspiapi.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#include <shellapi.h>

DWORD WriteString(LPCWSTR str, DWORD numberOfCharsToWrite);

//void rawmain()
int main(int argc, char*argv[])
{
    WSADATA wsaData;
    ADDRINFOA* result = NULL;
    int rc = 0;

    //int argc;
    //LPWSTR* argv = CommandLineToArgvA(GetCommandLineA(), &argc);
    if (argc != 2)
    {
        rc = ERROR_INVALID_PARAMETER;
    }
    else if ((rc = WSAStartup(MAKEWORD(2, 2), &wsaData))       != 0)
    { }
    else if ((rc = getaddrinfo(argv[1], NULL, NULL, &result)) != 0)
    { }
    else
    {
        char szIP[64];
		ADDRINFOA* ptr;

        for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
        {
            DWORD IPbuflen = 64;
            if ((rc = WSAAddressToStringA(
							(LPSOCKADDR)ptr->ai_addr
						, 	(DWORD)ptr->ai_addrlen
						, 	NULL
						, 	szIP
						, 	&IPbuflen)) != 0)
            {
                break;
            }

            //szIP[IPbuflen - 1] = '\r';
            //szIP[IPbuflen    ] = '\n';

			printf("%s\n", szIP);
			/*
            if ((rc = WriteString(szIP, IPbuflen + 1) != 0))
            {
                break;
            }*/
        }

        //FreeAddrInfoW(result);
		
    }
    
    WSACleanup();
    ExitProcess(rc);
}

