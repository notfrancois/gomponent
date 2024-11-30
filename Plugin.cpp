#include <sstream>

#include "Plugin.hpp"

void Plugin::load(const std::string& names)
{
    std::istringstream iss(names);
    std::string name;
    
    while (iss >> name) {
        #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
            handle = LoadLibraryA(("plugins/" + name + ".dll").c_str());

            if (handle == NULL)
            {
                DWORD error = GetLastError();
                std::stringstream errMsg;
                errMsg << "Failed to load " << name << ": error code " << error;

                throw std::runtime_error(errMsg.str());
            }
        #else
            handle = dlopen(("plugins/" + name + ".so").c_str(), RTLD_GLOBAL | RTLD_NOW);

            if (handle == NULL)
            {
                const char* errMsg = dlerror();

                if (errMsg != NULL)
                {
                    throw std::runtime_error("Failed to load " + name + ": " + std::string(errMsg));
                }
            }
        #endif
    }
}

void Plugin::unload()
{
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
        FreeLibrary((HMODULE)handle);
    #else
        dlclose(handle);
    #endif
}

void* Plugin::findCallback(const std::string& name)
{
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
        return (void*)GetProcAddress((HMODULE)handle, name.c_str());
    #else
        return dlsym(handle, name.c_str());
    #endif
}
