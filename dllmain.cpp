// dllmain.cpp : Defines the entry point for the DLL application.
/////////////////////////////////////////////////////////////////////////////

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include "./curl.h"

//@see http://pages.cs.wisc.edu/~tlabonne/memleak.html
#include "CrtDbg.h"
void activateMemoryLeakChecks(void) {
#	ifndef NDEBUG
#	ifndef _CRTDBG_MAP_ALLOC
#		pragma message("Define _CRTDBG_MAP_ALLOC for more detailed CRT memory leak report!")
#	endif
	int flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	flags |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(flags);
#	endif
}

void reportMemoryLeaks(void) {
#	ifndef NDEBUG
	_CrtDumpMemoryLeaks();
#	endif
}
/////////////////////////////////////////////////////////////////////////////

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
#if 0		//only required in standalone C programs
		khp("", -1);	//force initialize kdb+ internal memory system
#endif
		setm(1);		//allow symbols to be created in threads other than the main thread
		activateMemoryLeakChecks();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		m9();
		reportMemoryLeaks();
		break;
	}
	return TRUE;
}