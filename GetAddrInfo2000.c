//#define WINVER _WIN32_WINNT_NT4

#define WIN32_LEAN_AND_MEAN
#define STRICT

//#include <Windows.h>

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <Wspiapi.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#include <shellapi.h>

#include <stdio.h>
#include <stdarg.h>

//DWORD WriteString(LPCWSTR str, DWORD numberOfCharsToWrite);

// missing in msvcrt.dll on Windows 2000
//_strcat_s
//_strcpy_s
//_strncpy_s
//_sprintf_s

int sprintf_s(char *buffer, size_t sizeOfBuffer, const char *format, ... )
{
	va_list  args;
	int written;
	UNREFERENCED_PARAMETER(sizeOfBuffer);
	
	va_start(args, format);
	written	= vsprintf(buffer, format, args);
	va_end(args);
	
	return written;
}

errno_t strcat_s(char *strDestination, size_t numberOfElements, const char *strSource)
{
	UNREFERENCED_PARAMETER(numberOfElements);
	strcat(strDestination, strSource);
	return 0;
}

errno_t strcpy_s(char *dest, rsize_t dest_size, const char *src)
{
	UNREFERENCED_PARAMETER(dest_size);
	strcpy(dest, src);
	return 0;
}

errno_t strncpy_s(char *strDest, size_t numberOfElements, const char *strSource, size_t count)
{
	UNREFERENCED_PARAMETER(numberOfElements);
	strncpy(strDest, strSource, count);
	return 0;
}


//void rawmain()
int main(int argc, char*argv[])
{
    WSADATA wsaData;
    ADDRINFOA* result = NULL;
    int rc = 0;

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

			printf("%s\n", szIP);
        }

        //FreeAddrInfoW(result);
		
    }
    
    WSACleanup();
    ExitProcess(rc);
}

