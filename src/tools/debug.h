/* Debugger by H3nt4iBoY v1.3

- Everything in one .h
- Supports 3 modes: file, screen, no debug.
- Can be switched in runtime
- Uses 'cout' style
*/

#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#include <fstream>
#include <iostream>
#include <string>

#define DEBUG_CLASS_VERSION "1.3"
#define DEBUG_TO_FILE   1
#define DEBUG_TO_SCREEN 2
#define DEBUG_NO_DEBUG  3

class Debug
{
private:
    std::string debfile = "debug.txt";
    int mode = DEBUG_TO_FILE;
    bool file_close=true;
    std::ofstream outp;

    std::string lastModificationTag;
    bool writeDebugs=true;

public:
    Debug(){ }
    Debug(int _mode, std::string deb_file="debug.txt")
    {
        mode=_mode;
        debfile=deb_file;
        if(mode==DEBUG_TO_FILE) outp.open(debfile.c_str(), std::ofstream::app);
    }
    ~Debug(){ outp.close(); }

    void setMode(int _modde)      { mode=_modde; }
    void setWriteDebugs(bool _val){ writeDebugs=_val; }
    void closeFile(bool val)      { file_close=val; }
    void setLastModifTag(std::string tag){ lastModificationTag=tag; }
    void setDoublePrecision(int val)
    {
        std::cout.precision(val);
        outp.precision(val);
    }

    std::string getLastModif(){ return lastModificationTag; }
    bool getWriteDebugs()     { return writeDebugs; }

    template<typename T>
    Debug& operator<<(const T val);
};

template<typename T>
Debug& Debug::operator<<(const T val)
{
    if(mode==DEBUG_TO_FILE && writeDebugs)
    {
        if(file_close || !outp.is_open()) outp.open(debfile.c_str(), std::ofstream::app);
        outp<<val;
        if(file_close) outp.close();
    }
    else if(mode==DEBUG_TO_SCREEN && writeDebugs) std::cout<<val;

    return *this;
}


extern Debug deb; //default debugger


#endif // DEBUG_H_INCLUDED
