#pragma once
#ifndef DRAWING_H
#define DRAWING_H
#include "pch.h"
#include "networking.hpp"
//#include "networking.hpp"

class Drawing
{
public:
	static LPCSTR lpWindowName;
	static ImVec2 vWindowSize;
	static ImGuiWindowFlags WindowFlags;
	static bool bDraw;
	static ImGuiTabBarFlags tabFlags;
	static void Active();
	static bool isActive();
	static void Draw(int x, int y);
	static inline int userType = -1;
	static inline LPCSTR imguiName;
	static inline std::vector<proxy> proxies;
	static inline std::vector<proxy> myProxies;
	static inline bool refreshProxies = true;
};

#endif
