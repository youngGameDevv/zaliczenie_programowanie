#pragma once

#include <drogon/HttpController.h>

using namespace drogon;


class rest : public drogon::HttpController<rest>
{
  public:
    METHOD_LIST_BEGIN

    METHOD_ADD(rest::addProxy, "/addProxy", Post);
    METHOD_ADD(rest::deleteProxy, "/deleteProxy", Post);
    METHOD_ADD(rest::modifyProxy, "/modifyProxy", Post);

    METHOD_ADD(rest::getMyProxies, "/getMyProxies", Post);
    METHOD_ADD(rest::getAllProxies, "/getAllProxies", Post);

    METHOD_LIST_END

    void addProxy(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void deleteProxy(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void modifyProxy(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void getMyProxies(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void getAllProxies(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
};

