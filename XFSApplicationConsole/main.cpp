#include <iostream>
#include <tchar.h>
#include "XFSAPI.H"
#include "XFSPTR.H"

using namespace std;

int main()
{
	HRESULT hRes = -1;

	// ---------------------------------------------------------------------------------------------
	// XFS StartUp
	WFSVERSION wfsVersion;
	hRes = WFSStartUp(
		0x031E,				// Specifies the range of versions of the XFS Manager that the application can support
		&wfsVersion			// Pointer to the data structure that is to receive version support information and other details about the current XFS implementation(returned parameter).
	);
	cout << "XFS StartUp - " << hRes << endl;
	if (hRes != WFS_SUCCESS)
	{
		return -1;
	}

	// ---------------------------------------------------------------------------------------------
	// XFS Open
	CHAR serviceName[6] = "PTR30";
	HSERVICE hService = NULL;
	WFSVERSION wfsSrvcVersion;
	WFSVERSION wfsSPIVersion;
	hRes = WFSOpen(
		serviceName,			// Name of the service provider (same defined on regedit)
		WFS_DEFAULT_HAPP,		// The application handle to be associated with the session being opened. If this parameter is equal to WFS_DEFAULT_HAPP, the session is associated with the calling process as a whole
		NULL,					//  Points to a null-terminated string containing the application ID; the pointer may be NULL if theID is not used
		NULL,					// NULL turns off all tracing 
		WFS_INDEFINITE_WAIT,	// Number of milliseconds to wait for completion 
		0x031E,					// Specifies the range of versions of the service-specific interface that the application can support
		&wfsSrvcVersion,		// Pointer to the data structure that is to receive version support information and other details
		&wfsSPIVersion,			// Pointer to the data structure that is to receive version support information and (optionally) other details
		&hService				// Pointer to the service handle that the XFS Manager assigns to the service on a successful open; 
	);
	cout << "XFS Open - " << hRes << endl;
	if (hRes != WFS_SUCCESS)
	{
		return -1;
	}

	// ---------------------------------------------------------------------------------------------
	// XFS Register
	//hRes = WFSRegister(
	//	hService,						// Handle to the Service Provider as returned by WFSOpen
	//	SYSTEM_EVENTS | USER_EVENTS,    // The class(es) of events for which the application is registering
	//  hWndReg1						// The window handle which is to be registered to receive the specified messages.
	//);

	// ---------------------------------------------------------------------------------------------
	// XFS Execute
	int iPrintLen = 24;
	BYTE* bPrint = (LPBYTE)malloc(iPrintLen * sizeof(BYTE));
	memcpy(bPrint, "TESTE IMPRESSAO XFS APP", iPrintLen);

	WFSPTRRAWDATA wfsData;
	wfsData.ulSize = iPrintLen;
	wfsData.lpbData = bPrint;

	LPWFSRESULT lpWfsRes;
	hRes = WFSExecute(
		hService,				// hService received on WFSOpen
		WFS_CMD_PTR_RAW_DATA,	// command to be executed
		&wfsData,				// data passed based on command documentation
		WFS_INDEFINITE_WAIT,	// number of miliseconds to wait for completion
		&lpWfsRes				// Pointer to the pointer to the result data structure used to return the results of the execution. The Service Provider allocates the memory for this structure.
	);
	cout << "XFS Execute (Print) - " << hRes << " - " << lpWfsRes->hResult << endl;
	if (hRes != WFS_SUCCESS || lpWfsRes->hResult != WFS_SUCCESS)
	{
		return -1;
	}
	hRes = WFSFreeResult(lpWfsRes);		// Free pointer to WFSRESULT came from XFS Manager

	DWORD dwMediaControl = (WFS_PTR_CTRLCUT | WFS_PTR_CTRLEJECT);
	hRes = WFSExecute(
		hService,								// hService received on WFSOpen
		WFS_CMD_PTR_CONTROL_MEDIA,				// command to be executed
		(LPVOID)&dwMediaControl,				// data passed based on command documentation
		WFS_INDEFINITE_WAIT,					// number of miliseconds to wait for completion
		&lpWfsRes								// Pointer to the pointer to the result data structure used to return the results of the execution. The Service Provider allocates the memory for this structure.
	);
	cout << "XFS Execute (Eject) - " << hRes << " - " << lpWfsRes->hResult << endl;
	if (hRes != WFS_SUCCESS || lpWfsRes->hResult != WFS_SUCCESS)
	{
		return -1;
	}
	hRes = WFSFreeResult(lpWfsRes);		// Free pointer to WFSRESULT came from XFS Manager

	// ---------------------------------------------------------------------------------------------
	// XFS Close
	hRes = WFSClose(
		hService				// hService received on WFSOpen
	);
	cout << "XFS Close - " << hRes << endl;
	if (hRes != WFS_SUCCESS)
	{
		return -1;
	}

	// ---------------------------------------------------------------------------------------------
	// XFS CleanUp
	hRes = WFSCleanUp();		// CleanUp the session 
	cout << "XFS CleanUp - " << hRes << endl;

	return 0;
}