#ifndef DOUBLEARRAYTRIE_H
#define DOUBLEARRAYTRIE_H

#include <string>
#include <vector>

#include "pinyinresource.h"


class DoubleArrayTrie
{
public:
    DoubleArrayTrie();
    ~DoubleArrayTrie();

   int exactMatchSearch(std::wstring key);

   int exactMatchSearch(std::wstring key, int pos, int len, int nodePos);
   std::vector<int> commonPrefixSearch(std::wstring key);

   std::vector<int> commonPrefixSearch(std::wstring key, int pos, int len, int nodePos);

    // debug
    void dump();

    int build(std::vector<std::wstring> key);

    int build(std::vector<std::wstring> _key, int *_length, int *_value, int _keySize);

    void clear();

    int getUnitSize();

    int getSize();

    int getTotalSize();

    int getNonzeroSize();

public:
    int error_;
private:
    struct Node{
        int code;
        int depth;
        int left;
        int right;
    };

    std::vector<int> *check;
    std::vector<int> *base;
    std::vector<bool> *used;

    int size;
    int allocSize;
    std::vector<std::wstring> key;
    int keySize;
    int *length;
    int *value;
    int progress;
    int nextCheckPos;

private:
    /// \brief 重置数组大小
    /// \param newSize 大小数值
    /// \return
    int resize(int newSize);

    /// \brief 获取
    /// \param parent
    /// \param siblings
    /// \return
    int fetch(Node parent, std::vector<Node> &siblings);

    /// \brief 插入
    /// \param siblings
    /// \return
    int insert(std::vector<Node> siblings);
};

#endif // DOUBLEARRAYTRIE_H
