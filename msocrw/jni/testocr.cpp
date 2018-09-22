#include "msocr.h"

#include <cstdio>
#include <string>
#include <locale>
#include <codecvt>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

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

PTR globalEngine = 0;

std::string DoOCR(const uint8_t *lpBitmap, int width, int height) {
    // buf is standard RGBA8888 format bitmap, row by row
    PTR pResult = WrapperRecognizeImage(globalEngine, lpBitmap, width, height);
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

cv::Mat DecodeImage(const uint8_t *lpImage, size_t szImage) {
    auto input = std::vector<uint8_t>(szImage);
    memcpy(input.data(), lpImage, szImage);

    auto inputImage = cv::imdecode(input, cv::IMREAD_UNCHANGED);
    cv::Mat outputImage;
    if (inputImage.channels() == 4) {
        cv::cvtColor(inputImage, outputImage, cv::COLOR_BGRA2RGBA);
    } else if (inputImage.channels() == 3) {
        cv::cvtColor(inputImage, outputImage, cv::COLOR_BGR2RGBA);
    } else if (inputImage.channels() == 1) {
        cv::cvtColor(inputImage, outputImage, cv::COLOR_GRAY2RGBA);
    } else {
        printf("image not supported");
    }

    return outputImage;
}

std::string DoOCR(const uint8_t *lpImage, size_t szImage) {
    auto img = DecodeImage(lpImage, szImage);
    return DoOCR(img.data, img.cols, img.rows);
}

void Init(const wchar_t *path)
{
    // path: the path of folder than contains orp file name "MsOcrRes.orp"
    globalEngine = WrapperCreateOcrEngine(path);
    WrapperSetOcrLanguage(globalEngine, MsOcrLanguageAutodetect);
    WrapperSetOcrTextOrientation(globalEngine, MsOcrTextOrientationUp);
}

void __attribute__((destructor))
ocrDestroy(void)
{
    if (globalEngine) {
        WrapperDestroyOcrEngine(globalEngine);
    }
}

} // namespace <anonymous>

extern "C" {
    void ocr_init(const char *path) {
        Init(fromString(path).c_str());
    }

    char *ocr_recognize_bitmap(const uint8_t *lpBitmap, int width, int height) {
        const auto &str = DoOCR(lpBitmap, width, height);
        char *ret = (char *)malloc(str.size() + 1);
        strcpy(ret, str.c_str());
        return ret;
    }

    char *ocr_recognize_image(const uint8_t *lpImage, size_t szImage) {
        const auto &str = DoOCR(lpImage, szImage);
        char *ret = (char *)malloc(str.size() + 1);
        strcpy(ret, str.c_str());
        return ret;
    }

    void ocr_free(char *result) {
        free(result);
    }
}
