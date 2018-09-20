#include "msocr.h"
#include <cstdio>
#include <string>
#include <locale>
#include <codecvt>

enum AddSpace {
    AddSpaceNever,
    AddSpaceAlphabet,
    AddSpaceCJK,
};

namespace {
AddSpace getAddSpaceStart(const wchar_t *word) {
    return word[0] < 0x2e80 ? AddSpaceAlphabet : AddSpaceCJK;
}

AddSpace getAddSpaceEnd(const wchar_t *word) {
    return word[wcslen(word)-1] < 0x2e80 ? AddSpaceAlphabet : AddSpaceCJK;
}

std::wstring fromString(const std::string& utf8Str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(utf8Str);
}

std::string toString(const std::wstring& utf16Str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(utf16Str);
}
} // namespace <anonymous>

std::string DoOCR(PTR engine, const uint8_t *lpBitmap, int width, int height) {
    // buf is standard RGBA8888 format bitmap, row by row
    PTR pResult = WrapperRecognizeImage(engine, lpBitmap, 948, 326);
    std::wstring wresult;
    
    //fprintf(stderr, "region count %d\n", WrapperGetResultRegionCount(pResult));
    for (int iRegion = 0; iRegion < WrapperGetResultRegionCount(pResult); iRegion++) {
        //fprintf(stderr, "region %d\n", iRegion);
        PTR pRegion = WrapperGetRegionWithIndex(pResult, iRegion);
        //fprintf(stderr, "line count %d\n", WrapperGetLineCount(pRegion));
        for (int iLine = 0; iLine < WrapperGetLineCount(pRegion); iLine++) {
            //fprintf(stderr, "  line %d\n", iLine);
            PTR pLine = WrapperGetLineWithIndex(pRegion, iLine);
            AddSpace previousWord = AddSpaceNever;
            
            //fprintf(stderr, "  word count %d\n", WrapperGetWordCount(pLine));
            for (int iWord = 0; iWord < WrapperGetWordCount(pLine); iWord++) {
                //fprintf(stderr, "    word %d:", iWord);
                PTR pWord = WrapperGetWordWithIndex(pLine, iWord);
                //fprintf(stderr, "%ls\n", WrapperGetText(pWord));
                const wchar_t *word = WrapperGetText(pWord);
                AddSpace currentWord = getAddSpaceStart(word);
                if (previousWord == AddSpaceNever || (previousWord == AddSpaceCJK && currentWord == AddSpaceCJK)) {
                    // add no space
                }
                else {
                    wresult += L" ";
                }

                previousWord = getAddSpaceEnd(word);
                wresult += word;
                WrapperDestroyOcrWord(pWord);
            }

            wresult += L"\n";
            WrapperDestroyOcrLine(pLine);

        }

        wresult += L"\n";
        WrapperDestroyOcrRegion(pRegion);
    }
    
    fprintf(stderr, "OCR %p Result: %ls\n", lpBitmap, wresult.c_str());
    WrapperDestroyOcrResut(pResult);
    
    return toString(wresult);
}

int main() {
    printf("ocr test started\n");

    // folder than contains orp file name MsOcrRes.orp (case-[in]sensitive depends on filesystem and OS);
    const wchar_t *path = L"/";

    auto pEngine = WrapperCreateOcrEngine(path);
    WrapperSetOcrLanguage(pEngine, MsOcrLanguageAutodetect);
    WrapperSetOcrTextOrientation(pEngine, MsOcrTextOrientationUp);

    FILE *f = fopen("/1.bitmap", "r");
    int width = 948;
    int height = 326;
    uint8_t *buf = (uint8_t*) malloc(4*width*height);
    fread(buf, 4, width*height, f);
    fclose(f);

    DoOCR(pEngine, buf, width, height);

    free(buf);
    WrapperDestroyOcrEngine(pEngine);
}

