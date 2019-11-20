#ifndef CHARSETSELECTOR_H
#define CHARSETSELECTOR_H
#include <vector>
#include <memory>
#include "charset.h"

class CharsetSelector
{
public:
    CharsetSelector(const std::vector<std::wstring> filenames);
    bool open(const std::wstring &filename);
    bool close();
    const Charset* get();
    void move();
    void select(size_t);
    size_t pos();
private:
    std::vector<std::unique_ptr<Charset>> charsets;
    size_t current_charset;
};

#endif // CHARSETSELECTOR_H
