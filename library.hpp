#ifndef LIBRARY_HPP
#define LIBRARY_HPP
#include <Windows.h>
#include <string>

namespace wn {


class Library
{
protected:
    HMODULE module;
public:
    Library():module(nullptr){}
    ~Library()
    {
        unload();
    }
    bool load(const std::string &fileName)
    {
        module = LoadLibraryA(fileName.c_str());
        if (module == nullptr) {
            return false;
        }
        return true;
    }
    void unload()
    {
        if (module != nullptr) {
            FreeLibrary(module);
            module = nullptr;
        }
        return;
    }

    template<typename Fn>
    Fn get(const std::string &funcName)
    {
        return static_cast<Fn>(GetProcAddress(module, funcName.c_str()));
    }
};

}

#endif // LIBRARY_HPP
