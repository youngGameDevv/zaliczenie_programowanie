#pragma once 
#include "../includes/json.hpp"
#include <trantor/utils/Date.h>
#include "../helper/helper.hpp"
using json = nlohmann::json;
#include "rest.h"

void rest::addProxy(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    auto resp = HttpResponse::newHttpResponse();
    if(!json::accept(req->bodyData()))
    {
        resp->setBody("404 not found");
        callback(resp);
        return;
    }
    json data = json::parse(req->bodyData());
    if(
        !data.contains("ip") ||
        !data.contains("port") ||
        !data.contains("proxyType"))
    {
        resp->setBody("404 not found");
        callback(resp);
        return;
    }
    else if(req->getSession()->get<std::string>("userType").empty() ||
            req->getSession()->get<std::string>("userType") != "999")
    {
        resp->setBody("404 not found");
        callback(resp);
        return;
    }
    else 
    {
        auto clientPtr = drogon::app().getDbClient();
        std::string ip = data["ip"];
        auto result = clientPtr->execSqlSync("SELECT * FROM proxies WHERE ip = ?", 
        ip);
        if(result.size() > 0)
        {
            resp->setBody("-1"); // proxy already exist
            callback(resp);
            return;
        }
        else
        {
            std::string proxyType = data["proxyType"];
            std::string port = data["port"];
            auto result = clientPtr->execSqlSync(
                "insert into proxies (ownerLogin,expire,ip,port,proxyType) VALUES (?,?,?,?,?)", 
                "free",
                trantor::Date::now().toDbString().c_str(),
                ip,
                port,
                proxyType);
            resp->setBody("1"); // succes
            callback(resp);
            return;
        }
    }
    callback(resp);
    return;
}
void rest::deleteProxy(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    auto resp = HttpResponse::newHttpResponse();
    if(!json::accept(req->bodyData()))
    {
        resp->setBody("404 not found");
        callback(resp);
        return;
    }
    
    json data = json::parse(req->bodyData());
    
    if(!data.contains("ip"))
    {
        resp->setBody("404 not found");
        callback(resp);
        return;
    }
    else if(req->getSession()->get<std::string>("userType").empty() ||
            req->getSession()->get<std::string>("userType") != "999")
    {
        resp->setBody("404 not found");
        callback(resp);
        return;
    }
    else 
    {
        auto clientPtr = drogon::app().getDbClient();
        std::string ip = data["ip"];
        auto result = clientPtr->execSqlSync(
            "SELECT * FROM proxies WHERE ip = ?", 
            ip);
        
        if(result.size() > 0)
        {
            auto deleteResult = clientPtr->execSqlSync(
                "DELETE FROM proxies WHERE ip = ?", 
                ip);
            resp->setBody("1"); 
            callback(resp);
            return;
        }
        else
        {
            resp->setBody("-1"); 
            callback(resp);
            return;
        }
    }
    callback(resp);
    return;
}
void rest::modifyProxy(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    auto resp = HttpResponse::newHttpResponse();
    if(!json::accept(req->bodyData()))
    {
        resp->setBody("404 not found");
        callback(resp);
        return;
    }
    json data = json::parse(req->bodyData());
    if( !data.contains("ip") ||
        !data.contains("duration"))
    {
        resp->setBody("404 not found");
        callback(resp);
        return;
    }
    else if(req->getSession()->get<std::string>("login").empty() ||
            req->getSession()->get<std::string>("userType").empty())
    {
        resp->setBody("404 not found");
        callback(resp);
        return;
    }
    else 
    {
        auto clientPtr = drogon::app().getDbClient();
        std::string ip = data["ip"];
        if(req->getSession()->get<std::string>("userType") == "999")
        {
            auto result = clientPtr->execSqlSync(
                "UPDATE proxies SET ownerLogin = ? , expire = ? WHERE ip = ?", 
                "free",
                trantor::Date::now().toDbString().c_str(),
                ip);
            resp->setBody("1");
            callback(resp);
            return;
        }
        int duration = data["duration"];
        auto result = clientPtr->execSqlSync(
            "SELECT * FROM proxies WHERE ip = ?", 
            ip);
        if(result.size() > 0)
        {

            auto time = trantor::Date::now();
            trantor::Date expiryDate = time.after(duration * 86400);
            auto result = clientPtr->execSqlSync(
                "UPDATE proxies SET ownerLogin = ? , expire = ? WHERE ip = ?", 
                req->getSession()->get<std::string>("login"),
                expiryDate.toDbString().c_str(),
                ip);
            resp->setBody("1"); // succes
            callback(resp);
            return;
        }
        else
        {
            resp->setBody("-1"); // proxy not exist
            callback(resp);
            return;
        }
    }
    callback(resp);
    return;
}

void rest::getMyProxies(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    auto resp = HttpResponse::newHttpResponse();
    if(req->getSession()->get<std::string>("login").empty())
    {
        resp->setBody("404 not found");
        callback(resp);
        return;
    }
    
    auto clientPtr = drogon::app().getDbClient();
    auto result = clientPtr->execSqlSync(
        "SELECT * FROM proxies WHERE ownerLogin = ? and expire > NOW()", 
        req->getSession()->get<std::string>("login"));
    json responseData = json::array();
    for (const auto& row : result)
    {
        json ipData;
        ipData["ip"] = row["ip"].as<std::string>(); 
        ipData["port"] = row["port"].as<std::string>();   
        ipData["proxyType"] = row["proxyType"].as<std::string>(); 
        ipData["expire"] = row["expire"].as<std::string>();   
        ipData["ownerLogin"] = "";
        responseData.push_back(ipData);
    }
    
    resp->setBody(responseData.dump());
    callback(resp);
}
void rest::getAllProxies(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
        auto resp = HttpResponse::newHttpResponse();
    if(req->getSession()->get<std::string>("login").empty())
    {
        resp->setBody("404 not found");
        callback(resp);
        return;
    }
    auto clientPtr = drogon::app().getDbClient();
    json responseData = json::array();
    std::string userType = req->getSession()->get<std::string>("userType");
    if(userType != "999")
    {
        auto result = clientPtr->execSqlSync(
                    "SELECT * FROM proxies where ownerLogin = ? or expire < NOW()",
                "free");
        for (const auto& row : result)
        {
            json ipData;
            ipData["ip"] = row["ip"].as<std::string>(); 
            ipData["port"] = row["port"].as<std::string>();   
            ipData["proxyType"] = row["proxyType"].as<std::string>(); 
            ipData["expire"] = row["expire"].as<std::string>();   
            ipData["ownerLogin"] = "";
            responseData.push_back(ipData);
        }
    }
    else
    {
        auto result = clientPtr->execSqlSync(
                    "SELECT * FROM proxies");
        for (const auto& row : result)
        {
            json ipData;
            ipData["ip"] = row["ip"].as<std::string>(); 
            ipData["port"] = row["port"].as<std::string>();   
            ipData["proxyType"] = row["proxyType"].as<std::string>(); 
            ipData["expire"] = row["expire"].as<std::string>();   
            ipData["ownerLogin"] = row["ownerLogin"].as<std::string>();  
            responseData.push_back(ipData);
        }
    }
        
    resp->setBody(responseData.dump());
    callback(resp);
}