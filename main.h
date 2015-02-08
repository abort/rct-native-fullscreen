/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <abort@digitalise.net> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return J. Dijkstra (02/08/2015)
 * ----------------------------------------------------------------------------
 */
#ifndef MAIN_H_
#define MAIN_H_

#define STRICT
#define WIN32_LEAN_AND_MEAN

#define WINDOW_NAME "RollerCoaster Tycoon"
#define WINDOW_CLASS "RollerCoaster Tycoon"

#include <assert.h>
#include <Windows.h>
#include <stdio.h>

typedef struct {
	LONG width;
	LONG height;
	BOOL error;	// error flag
} resolutionInfo_t;


resolutionInfo_t getMonitorInfo(HWND hwnd);
BOOL changeWindowToFullscreen(HWND hwnd, resolutionInfo_t resolutionInfo);

#endif