#pragma once
#include "../includes/json.hpp"
#include "auth.h"
#include <trantor/utils/Date.h>
using json = nlohmann::json;
void auth::Register(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    auto resp = HttpResponse::newHttpResponse();
    if(!json::accept(req->bodyData()))
    {
        resp->setBody("404 not found");
        callback(resp);
        return;
    }
    json data = json::parse(req->bodyData());
    if(!data.contains("userType") || 
        !data.contains("login") || 
        !data.contains("pass") ||
        data["securityKey"] != "L05oprV5xCafOb8lYJKmmykFXJWuWXbBa0MC5MproZIk3EnMeB")
    {
        resp->setBody("404 not found");
        callback(resp);
        return;
    }
    else
    {
        auto clientPtr = drogon::app().getDbClient();
        std::string userType =  data["userType"];
        std::string login =  data["login"];
        std::string pass =  data["pass"];
        auto result = clientPtr->execSqlSync("SELECT * FROM users WHERE login = ?", login);
        if (!result.empty()) 
        {
            resp->setBody("-1"); // login already exist
            callback(resp);
            return;
        }
        clientPtr->execSqlAsyncFuture(
            "insert into users (userType,login,pass) VALUES (?,?,?)",
            userType,
            login,
            pass);
        resp->setBody("1");
        callback(resp);
        return;
    }
}
void auth::Login(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    auto resp = HttpResponse::newHttpResponse();
    if(!json::accept(req->bodyData()))
    {
        resp->setBody("404 not found");
        callback(resp);
        return;
    }
    json data = json::parse(req->bodyData());
    if(!data.contains("login") || 
        !data.contains("pass") ||
        data["securityKey"] != "L05oprV5xCafOb8lYJKmmykFXJWuWXbBa0MC5MproZIk3EnMeB")
    {
        resp->setBody("404 not found");
        callback(resp);
        return;
    }
    else
    {
        auto clientPtr = drogon::app().getDbClient();
        std::string login =  data["login"];
        std::string pass =  data["pass"];
        auto result = clientPtr->execSqlSync(
            "select * from users where login = ? and pass = ?",
            login,
            pass);
        json response;
        if(result.size() > 0 )
        {
            response["userType"] = result[0]["userType"].as<std::string>();
            response["succes"] = "1";
            req->session()->insert("login",login);
            req->session()->insert("userType",result[0]["userType"].as<std::string>());
        }
        else
        {
            response["succes"] = "0";
        }
        resp->setBody(response.dump());
    }
    callback(resp);
    return;
}