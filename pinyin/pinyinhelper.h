#ifndef PINYINHELPER_H
#define PINYINHELPER_H

#include "pinyinresource.h"
#include "doublearraytrie.h"
#include "pinyinformat.h"
#include "chinesehelper.h"
#include <string>

class PinyinHelper
{
public:
    PinyinHelper();
    ~PinyinHelper();

    static void initPinYinHelper();
    /**
    * 将单个汉字转换为相应格式的拼音
    * @param c 需要转换成拼音的汉字
    * @param pinyinFormat 拼音格式
    * @return 汉字的拼音
    */
    static std::vector<std::wstring> convertToPinyinArray(wchar_t c, PinyinFormat pinyinFormat);

    /**
    * 将单个汉字转换成带声调格式的拼音
    * @param c 需要转换成拼音的汉字
    * @return 字符串的拼音
    */
    static std::vector<std::wstring> convertToPinyinArray(wchar_t c);

    /**
    * 将字符串转换成相应格式的拼音
    * @param str 需要转换的字符串
    * @param separator 拼音分隔符
    * @param pinyinFormat 拼音格式
    * @return 字符串的拼音
    * @throws PinyinException
    */
    static std::wstring convertToPinyinString(std::wstring str, std::wstring separator, PinyinFormat pinyinFormat);

    /**
    * 将字符串转换成带声调格式的拼音
    * @param str 需要转换的字符串
    * @param separator 拼音分隔符
    * @return 转换后带声调的拼音
    * @throws PinyinException
    */
    static std::wstring convertToPinyinString(std::wstring str, std::wstring separator);

    /**
    * 判断一个汉字是否为多音字
    * @param c 汉字
    * @return 判断结果，是汉字返回true，否则返回false
    */
    static bool hasMultiPinyin(wchar_t c);


    /**
    * 获取字符串对应拼音的首字母
    * @param str 需要转换的字符串
    * @return 对应拼音的首字母
    * @throws PinyinException
    */
    static std::wstring getShortPinyin(std::wstring str);

private:
    /**
    * @brief 将带声调格式的拼音转换为数字代表声调格式的拼音
    * @param pinyinArrayString 带声调格式的拼音
    * @return 数字代表声调格式的拼音
    */
    static std::vector<std::wstring> convertWithToneNumber(std::wstring pinyinArrayString);

    /**
    * @brief 将带声调格式的拼音转换为不带声调格式的拼音
    * @param pinyinArrayString 带声调格式的拼音
    * @return 不带声调的拼音
    */
    static std::vector<std::wstring> convertWithoutTone(std::wstring pinyinArrayString);

    /**
    * 将带声调的拼音格式化为相应格式的拼音
    *
    * @param pinyinString
    *            带声调的拼音
    * @param pinyinFormat
    *           拼音格式:   WITH_TONE_NUMBER--数字代表声调，
    *                      WITHOUT_TONE--不带声调，
    *                      WITH_TONE_MARK--带声调
    * @return 格式转换后的拼音
    */
    static std::vector<std::wstring> formatPinyin(std::wstring pinyinString, PinyinFormat pinyinFormat);


};

#endif // PINYINHELPER_H
