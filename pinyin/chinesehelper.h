#ifndef CHINESEHELPER_H
#define CHINESEHELPER_H

#include <string>

/**
* @brief 汉字简繁转换类
*/
class ChineseHelper
{
public:
    ChineseHelper();
    ~ChineseHelper();

    static void initChineseHelper();

    /// \brief 将单个繁体字转换为简体字
    /// \param c 需要转换的繁体字
    /// \return 转换后的简体字
    static wchar_t convertToSimplifiedChinese(wchar_t c);

    /// \brief 将单个简体字转换为繁体字
    /// \param c 需要转换的简体字
    /// \return 转换后的繁字体
    static wchar_t convertToTraditionalChinese(wchar_t c);

    /// \brief 将繁体字转换为简体字
    /// \param str 需要转换的繁体字
    /// \return 转换后的简体体
    static std::wstring convertToSimplifiedChinese(std::wstring str);

    /// \brief 将简体字转换为繁体字
    /// \param str 需要转换的简体字
    /// \return 转换后的繁字体
    static std::wstring convertToTraditionalChinese(std::wstring str);

    /// \brief 判断某个字符是否为繁体字
    /// \param c 需要判断的字符
    /// \return 是繁体字返回true，否则返回false
    static bool isTraditionalChinese(wchar_t c);

    /// \brief 判断某个字符是否为汉字
    /// \param c 需要判断的字符
    /// \return 是汉字返回true，否则返回false
    static bool isChinese(wchar_t c);

    /// \brief 判断字符串中是否包含中文
    /// \param str 字符串
    /// \return 包含汉字返回true，否则返回false
    static bool containsChinese(std::wstring str);
};
#endif // CHINESEHELPER_H
