#pragma once
// MS OCR APIs
#include <cstdint>

#ifdef __LP64__
#define PTR unsigned long long
#else
#define PTR uint64_t
#endif

#pragma mark - OcrEngine
// return OcrEngine
PTR WrapperCreateOcrEngine(const wchar_t *path);
enum MsOcrLanguage {
    MsOcrLanguageInvalid,
    MsOcrLanguageAutodetect,
    MsOcrLanguageChineseSimplified,
    MsOcrLanguageChineseTraditional,
    MsOcrLanguageCzech,
    MsOcrLanguageDanish,
    MsOcrLanguageDutch,
    MsOcrLanguageEnglish,
    MsOcrLanguageFinnish,
    MsOcrLanguageFrench,
    MsOcrLanguageGerman,
    MsOcrLanguageGreek,
    MsOcrLanguageHungarian,
    MsOcrLanguageItalian,
    MsOcrLanguageJapanese,
    MsOcrLanguageKorean,
    MsOcrLanguageNorwegian,
    MsOcrLanguagePolish,
    MsOcrLanguagePortuguese,
    MsOcrLanguageRussian,
    MsOcrLanguageSpanish,
    MsOcrLanguageSwedish,
    MsOcrLanguageTurkish
};
void WrapperSetOcrLanguage(PTR hOcrEngine, int lang); // to maintain ABI
enum MsOcrTextOrientation {
    MsOcrTextOrientationUp,
    MsOcrTextOrientationRight,
    MsOcrTextOrientationDown,
    MsOcrTextOrientationLeft
};
void WrapperSetOcrTextOrientation(PTR hOcrEngine, int ori); // to maintain ABI
void WrapperDestroyOcrEngine(PTR hOcrEngine);
// returns OcrResult
PTR WrapperRecognizeImage(PTR hOcrEngine,const uint8_t *lpBitmap,int width,int height);

#pragma mark - OcrResult
// type MsOcrLanguage
int WrapperGetResultLanguage(PTR hOcrResult);
// type MsOcrTextOrientation
int WrapperGetResultTextOrientation(PTR hOcrResult);

int WrapperGetResultRegionCount(PTR hOcrResult);
void WrapperGetResultTextAngleWithDetected(PTR hOcrResult,double &textAngle,bool &textAngleDetected);
// returns OcrRegion
PTR WrapperGetRegionWithIndex(PTR hOcrResult,int index);
void WrapperDestroyOcrResut(PTR hOcrResult);

#pragma mark - OcrRegion
int WrapperGetLineCount(PTR hOcrRegion);
// returns OcrLine
PTR WrapperGetLineWithIndex(PTR hOcrRegion,int index);
void WrapperGetBoundingRectFromRegion(PTR hOcrRegion,int &left,int &top,int &width,int &height);
void WrapperDestroyOcrRegion(PTR hOcrRegion);

#pragma mark - OcrLine
int WrapperGetWordCount(PTR hOcrLine);
// returns OcrWord
PTR WrapperGetWordWithIndex(PTR hOcrLine,int index);
void WrapperGetBoundingRectFromLine(PTR hOcrLine,int &left,int &top,int &width,int &height);
bool WrapperGetLineIsVertical(PTR hOcrLine);
void WrapperDestroyOcrLine(PTR hOcrLine);

#pragma mark - OcrWord
void WrapperGetConfidence(PTR hOcrWord);
wchar_t *WrapperGetText(PTR hOcrWord);
void WrapperGetBoundingRectFromWord(PTR hOcrWord,int &left,int &top,int &width,int &height);
void WrapperDestroyOcrWord(PTR hOcrWord);
