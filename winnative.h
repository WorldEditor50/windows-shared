#ifndef WINNATIVE_H
#define WINNATIVE_H
/* windows */
#include <Windows.h>
#include <shellapi.h>
#include <DbgHelp.h>
#include <DbgModel.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <ShlObj.h>
/* stl */
#include <string>
#include <memory>
#include <algorithm>
#include <vector>

namespace WindowsNative {

/* string */
std::wstring stringToWString(const std::string &s);
std::string wstringToString(const std::wstring &s);
std::string gbkToUtf8(const std::string &gbk);
std::string utf8ToGbk(const std::string &utf8);
std::string asciiToUtf8(const std::string &ascii);
std::string utf8ToAscii(const std::string &utf8);

/* process */
class Proc
{
public:
unsigned long pid;
std::string name;
public:
Proc(){}
Proc(unsigned long id, const std::string &procName)
    :pid(id),name(procName){}
};

std::string findProcess(unsigned long pid);
unsigned long findProcess(const std::string &processName);
std::vector<Proc> getAllProcess();
/* window */
std::string getForegroundApp();
std::string getForegroundUwpApp();
int raise(const std::string &appName);
/* command line */
void exec(const std::string &cmd, bool visible);
void openUrl(const std::string &url, bool visible);
void execAsAdmin(const std::string &cmd);
/* privileges */
void elevateCurrentProcess();

}
#endif // WINNATIVE_H
