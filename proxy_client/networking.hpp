#pragma once
#include "pch.h"

using json = nlohmann::json;
static inline httplib::Client client("http://135.181.118.209:55244");
class proxy
{
public:

    std::string ip = "";
    std::string port = "";
    std::string proxyType = "";
    std::string ownerLogin = "";

    proxy(const std::string& ip, const std::string& port, const std::string& proxyType, const std::string& ownerLogin)
        : ip(ip), port(port), proxyType(proxyType), ownerLogin(ownerLogin)
    {
    }
};
class RestClient {
public:
    static inline std::string cookie = "";
    static inline std::mutex webMutex;

    static int login(const std::string& login, const std::string& pass) 
    {
        std::lock_guard<std::mutex> lock(webMutex);
        json loginData = {
            {"login", login},
            {"pass",pass},
            {"securityKey","L05oprV5xCafOb8lYJKmmykFXJWuWXbBa0MC5MproZIk3EnMeB"}
        };

        auto res = client.Post("/auth/Login", loginData.dump(), "application/json");
        if (res && res->status == 200) 
        {
            if (json::accept(res->body))
            {
                json data = json::parse(res->body);
                if (data["succes"] == "1")
                {
                    cookie = res->get_header_value("Set-Cookie");
                    std::string userType = data["userType"];
                    return std::stoi(userType);
                }
            }
            else
            {
                std::cout << res->body << std::endl;
            }
        }

        return -1;
    }
    static bool addProxy(const std::string& ip, const std::string& port, const std::string& proxyType)
    {
        std::lock_guard<std::mutex> lock(webMutex);
        json taskData = {
            {"ip", ip},
            {"port", port},
            {"proxyType",proxyType}
        };

        httplib::Headers headers = {
            {"Cookie", cookie}
        };

        auto res = client.Post("/rest/addProxy", headers, taskData.dump(), "application/json");
        return res && res->status == 200 && res->body == "1";
    }
    static bool deleteProxy(const std::string& ip)
    {
        std::lock_guard<std::mutex> lock(webMutex);
        json taskData = {
            {"ip", ip}
        };

        httplib::Headers headers = {
            {"Cookie", cookie}
        };

        auto res = client.Post("/rest/deleteProxy", headers, taskData.dump(), "application/json");
        return res && res->status == 200 && res->body == "1";
    }

    static bool modifyProxy(const std::string& ip,const int& duration)
    {
        std::lock_guard<std::mutex> lock(webMutex);
        json taskData = {
            {"ip", ip},
            {"duration",duration}
        };

        httplib::Headers headers = {
            {"Cookie", cookie}
        };

        auto res = client.Post("/rest/modifyProxy", headers, taskData.dump(), "application/json");
        return res && res->status == 200 && res->body == "1";
    }

    static std::vector<proxy> getMyProxies()
    {
        std::lock_guard<std::mutex> lock(webMutex);

        httplib::Headers headers = {
            {"Cookie", cookie}
        };
        auto res = client.Post("/rest/getMyProxies", headers, "", "application/json");
        std::vector<proxy> proxies;
        if (json::accept(res->body))
        {
            json data = json::parse(res->body);
            for (const auto& proxys : data)
            {
                proxies.push_back(proxy(proxys["ip"], proxys["port"], proxys["proxyType"],proxys["ownerLogin"]));
            }
        }
        return proxies;
    }

    static std::vector<proxy> getAllProxies()
    {
        std::lock_guard<std::mutex> lock(webMutex);

        httplib::Headers headers = {
            {"Cookie", cookie}
        };
        auto res = client.Post("/rest/getAllProxies", headers, "", "application/json");
        std::vector<proxy> proxies;
        if (json::accept(res->body))
        {
            json data = json::parse(res->body);
            for (const auto& proxys : data)
            {
                proxies.push_back(proxy(proxys["ip"], proxys["port"], proxys["proxyType"], proxys["ownerLogin"]));
            }
        }
        return proxies;
    }

};