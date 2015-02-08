/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <abort@digitalise.net> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return J. Dijkstra (02/08/2015)
 * ----------------------------------------------------------------------------
 *
 * Created to play RollerCoaster Tycoon in fullscreen on your monitor's native resolution.
 * This only works in conjunction with computerlife22's widescreen hack: (http://www.wsgf.org/dr/rollercoaster-tycoon)
 * For anyone that wants to start the game in fullscreen without this app, either create a bat file or add a CreateProcess API call (or patch the PE)
 */
#include "main.h"

int main(void)
{
	printf("Attempting to change the window properties (of '%s') to fullscreen...\n", WINDOW_NAME);

	HWND windowHandle = FindWindow(WINDOW_CLASS, WINDOW_NAME);
	if (windowHandle == NULL)
	{
		fprintf(stderr, "Could not find window %s (last win32 error: %d)\n", WINDOW_NAME, GetLastError());
		return -1;
	}

	resolutionInfo_t resolutionInfo = getMonitorInfo(windowHandle);
	if (resolutionInfo.error != 0)
	{
		fprintf(stderr, "Failed to retrieve monitor info (last win32 error: %d)\n", GetLastError());
		return -2;
	}

	if (!changeWindowToFullscreen(windowHandle, resolutionInfo))
	{
		fprintf(stderr, "Failed to change the window to fullscreen (last win32 error: %d)\n", GetLastError());
		return -3;
	}

	printf("Successfully changed the window properties to fullscreen\n");

	return 0;
}

resolutionInfo_t getMonitorInfo(HWND hwnd)
{
	assert(hwnd != NULL);
	HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO mInfo;
	mInfo.cbSize = sizeof(MONITORINFO);
	resolutionInfo_t resolutionInfo;

	if (!GetMonitorInfo(monitor, &mInfo))
	{
		resolutionInfo.error = 1;
	}
	else
	{
		resolutionInfo.width = mInfo.rcMonitor.right - mInfo.rcMonitor.left;
		resolutionInfo.height = mInfo.rcMonitor.bottom - mInfo.rcMonitor.top;
		resolutionInfo.error = 0;
	}

	return resolutionInfo;
}

BOOL changeWindowToFullscreen(HWND hwnd, resolutionInfo_t resolutionInfo)
{
	assert(hwnd != NULL);
	assert(resolutionInfo.width > 0);
	assert(resolutionInfo.height > 0);
	assert(resolutionInfo.error == 0);
	BOOL error = 0;

	// Retrieve current style of the window
	// No need to error handle, as this is not crucial
	LONG style = GetWindowLong(hwnd, GWL_STYLE);
	LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);

	// Add fullscreen properties to the current window style
	if (!SetWindowLong(hwnd, GWL_STYLE, style & ~(WS_CAPTION | WS_THICKFRAME))) error = 1;
	if (!SetWindowLong(hwnd, GWL_EXSTYLE, exStyle & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE))) error = 1;
	if (SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, resolutionInfo.width, resolutionInfo.height, SWP_SHOWWINDOW)) error = 1;
	
	// Not crucial to the change, so we ignore its error
	ShowWindow(hwnd, SW_MAXIMIZE);

	return error;
}