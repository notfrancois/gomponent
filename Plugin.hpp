#pragma once

#include <string>
#include <sdk.hpp>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #include <Windows.h>
#else
    #include <dlfcn.h>
#endif

#include "Singleton.hpp"

class Plugin final : public Singleton<Plugin>
{
public:
    void load(const std::string& name);
    void unload();

    template <typename R, typename... Args>
    R call(const std::string& name, Args... args);

private:
    void* handle;
    FlatHashMap<std::string, void*> callbacks;

    void* findCallback(const std::string& name);
};

template <typename R, typename... Args>
R Plugin::call(const std::string& name, Args... args)
{
    auto it = callbacks.find(name);
    void* callbackAddr = nullptr;

    if (it == callbacks.end())
    {
        callbackAddr = findCallback(name);
        callbacks.emplace(name, callbackAddr);
    }
    else
    {
        callbackAddr = it->second;
    }

    typedef R (*CallbackType)(Args...);
    CallbackType callback = (CallbackType)callbackAddr;

    return (*callback)(std::forward<Args>(args)...);
}
