#ifndef PINYINRESOURCE
#define PINYINRESOURCE

#include <string>
#include <map>
///
/// \brief 把资源文件转换成Map
///

class PinyinResource{

public:
    PinyinResource();
    ~PinyinResource();

    static void initPinyinResource(std::wstring resourceDir);

    static std::map<std::wstring, std::wstring>& getResource(std::wstring file, std::map<std::wstring, std::wstring>& map);

    static std::map<std::wstring, std::wstring>& getPinyinResource();

    static std::map<std::wstring, std::wstring>& getMutilPinyinResource();

    static std::map<std::wstring, std::wstring>& getChineseResource();
};



#endif // PINYINRESOURCE


