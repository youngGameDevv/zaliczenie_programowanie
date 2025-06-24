#pragma once
#include "pch.h"
#include "Drawing.h"
#pragma warning(disable : 4996)
LPCSTR Drawing::lpWindowName = "hshsdbxbdsh";
ImVec2 Drawing::vWindowSize = { 600, 600 };
ImGuiWindowFlags Drawing::WindowFlags = 0;
ImGuiTabBarFlags Drawing::tabFlags = 0;
bool Drawing::bDraw = true;
void Drawing::Active()
{
	bDraw = true;
}

bool Drawing::isActive()
{
	return bDraw == true;
}

void Drawing::Draw(int x, int y)
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.11f, 0.11f, 0.14f, 0.94f));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.20f, 0.20f, 0.24f, 0.85f));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.30f, 0.85f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.33f, 0.33f, 0.38f, 0.85f));
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.25f, 0.25f, 0.30f, 0.85f));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.33f, 0.33f, 0.38f, 0.85f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.90f, 0.90f, 0.90f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.33f, 0.33f, 0.33f, 0.50f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 4));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 5));
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);
	ImGui::Begin(imguiName, &bDraw, WindowFlags | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    static char username[64] = "";
    static char password[64] = "";
	{
		if (userType < 0)
		{
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 300) * 0.5f);

            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.9f, 1.0f, 1.0f));
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Login").x) * 0.5f);
            ImGui::Text("Login");
            ImGui::PopStyleColor();

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::Spacing();

            ImGui::Spacing();

            ImGui::Text("Username:");
            ImGui::SetNextItemWidth(-1); 
            ImGui::InputText("##username", username, sizeof(username));

            ImGui::Spacing();

            ImGui::Text("Password:");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##password", password, sizeof(password));
			ImGui::Spacing();
			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 120) * 0.5f);

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 0.85f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.7f, 0.3f, 0.85f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.5f, 0.15f, 0.85f));

			if (ImGui::Button("Login", ImVec2(120, 30)))
			{
				userType = RestClient::login(username, password);
			}

			ImGui::PopStyleColor(3);

		}
		else
		{
			if (refreshProxies)
			{
				refreshProxies = false;
				proxies.clear();
				proxies = RestClient::getAllProxies();
				if (userType == 1)
				{
					myProxies.clear();
					myProxies = RestClient::getMyProxies();
				}
			}
			if (userType == 1) // normal user
			{
				ImGui::Text("mode: user");
				ImGui::SameLine();
				if (ImGui::Button("refresh"))
				{
					refreshProxies = true;
				}
				ImGui::Text("all proxies");
				ImGui::Text("ip");
				ImGui::SameLine();
				ImGui::Text("port");
				ImGui::SameLine();
				ImGui::Text("proxyType");
				for (auto i : proxies)
				{
					ImGui::Text(i.ip.c_str());
					ImGui::SameLine();
					ImGui::Text(i.port.c_str());
					ImGui::SameLine();
					ImGui::Text((i.proxyType == "1") ? "socks5" : "https");
					ImGui::SameLine();
					if (ImGui::Button(("buy##" + i.ip).c_str()))
					{
						RestClient::modifyProxy(i.ip,30);
						refreshProxies = true;
					}
				}

				ImGui::Text("my proxies");
				ImGui::Text("ip");
				ImGui::SameLine();
				ImGui::Text("port");
				ImGui::SameLine();
				ImGui::Text("proxyType");
				for (auto i : myProxies)
				{
					ImGui::Text(i.ip.c_str());
					ImGui::SameLine();
					ImGui::Text(i.port.c_str());
					ImGui::SameLine();
					ImGui::Text((i.proxyType == "1") ? "socks5" : "https");
				}
			}
			else // admin 
			{
				ImGui::Text("mode: admin");
				ImGui::SameLine();
				if (ImGui::Button("add proxy"))
				{
					ImGui::OpenPopup("Add New proxy");
				}
				ImGui::SameLine();
				if (ImGui::Button("refresh"))
				{
					refreshProxies = true;
				}
				ImGui::Text("ip");
				ImGui::SameLine();
				ImGui::Text("port");
				ImGui::SameLine();
				ImGui::Text("proxyType");
				ImGui::SameLine();
				ImGui::Text("owner");
				for (auto i : proxies)
				{
					ImGui::Text(i.ip.c_str());
					ImGui::SameLine();
					ImGui::Text(i.port.c_str());
					ImGui::SameLine();
					ImGui::Text((i.proxyType == "1") ? "socks5" : "https");
					ImGui::SameLine();
					ImGui::Text(i.ownerLogin.c_str());
					ImGui::SameLine();
					if (ImGui::Button(("delete##"+i.ip).c_str()))
					{
						RestClient::deleteProxy(i.ip);
						refreshProxies = true;
					}
					ImGui::SameLine();
					if (ImGui::Button(("free proxy##"+i.ip).c_str()))
					{
						RestClient::modifyProxy(i.ip, 0);
						refreshProxies = true;
					}
				}

				if (ImGui::BeginPopupModal("Add New proxy", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
				{
					static char ip[100] = "";
					static char port[100] = "";
					static char proxyType[100] = "";

					ImGui::Text("ip");
					ImGui::Spacing();
					ImGui::InputText("ip###ip1", ip, sizeof(ip));
					ImGui::Spacing();

					ImGui::Text("port");
					ImGui::Spacing();
					ImGui::InputText("port###port", port, sizeof(port));
					ImGui::Spacing();

					ImGui::Text("proxyType");
					ImGui::Spacing();
					ImGui::InputText("proxyType###proxtype1", proxyType, sizeof(proxyType));
					ImGui::Spacing();

					if (ImGui::Button("add"))
					{
						RestClient::addProxy(ip, port, proxyType);
						refreshProxies = true;
						ImGui::CloseCurrentPopup();
					}
				}
			}
		}
	}
	ImGui::PopStyleVar(4);
	ImGui::PopStyleColor(8);
	ImGui::End();
}
