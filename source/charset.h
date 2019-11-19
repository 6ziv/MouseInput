#ifndef CHARSET_H
#define CHARSET_H
#include <vector>
#include <string>
#include <cchar.h>
class charset
{
public:
    bool open(std::wstring filename);
    void openDefault();
    size_t CountCatalogs();
    const std::wstring& GetCatalogTag(size_t catalog);
    const std::wstring& GetCatalogDescription(size_t catalog);
    const std::vector<wchar_t>& GetCatalog(size_t catalog);

private:
    std::vector<std::pair<std::pair<std::wstring,std::wstring>,size_t>> Cats;
    std::vector<std::vector<wchar_t>> Chars;
};

#endif // CHARSET_H
