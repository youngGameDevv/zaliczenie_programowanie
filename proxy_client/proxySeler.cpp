#pragma once
#include "pch.h"
#include "UI.h"


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	//AllocConsole();
//	FILE* f;
//	freopen_s(&f, "CONOUT$", "w", stdout);
	UI::Render();
}

