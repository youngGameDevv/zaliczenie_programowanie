#pragma once

#include <drogon/HttpController.h>

using namespace drogon;


class auth : public drogon::HttpController<auth>
{
  public:
    METHOD_LIST_BEGIN
    METHOD_ADD(auth::Register, "/register", Post);
    METHOD_ADD(auth::Login, "/login", Post);

    METHOD_LIST_END
    void Register(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void Login(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
};

