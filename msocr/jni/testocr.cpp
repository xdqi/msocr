#include "msocr.h"

int main() {
    printf("ocr test started\n");
    
    // folder than contains orp file name MsOcrRes.orp (case-[in]sensitive depends on filesystem and OS);
    const wchar_t *path = L"/"; 

    auto pEngine = WrapperCreateOcrEngine(path);
    WrapperSetOcrLanguage(pEngine, MsOcrLanguageChineseSimplified);
    WrapperSetOcrTextOrientation(pEngine, MsOcrTextOrientationUp);
    
    FILE *f = fopen("/1.bitmap", "r");
    uint8_t *buf = (uint8_t*)malloc(4*948*326);
    fread(buf, 4, 948*326, f);
    fclose(f);
    
    // buf is standard RGBA8888 format bitmap, row by row
    uint64_t pResult = WrapperRecognizeImage(pEngine, buf, 948, 326);
    
    printf("region count %d\n", WrapperGetResultRegionCount(pResult));
    for (int iRegion = 0; iRegion < WrapperGetResultRegionCount(pResult); iRegion++) {
        printf("region %d\n", iRegion);
        uint64_t pRegion = WrapperGetRegionWithIndex(pResult, iRegion);
        printf("line count %d\n", WrapperGetLineCount(pRegion));
        for (int iLine = 0; iLine < WrapperGetLineCount(pRegion); iLine++) {
            printf("  line %d\n", iLine);
            uint64_t pLine = WrapperGetLineWithIndex(pRegion, iLine);
            
            printf("  word count %d\n", WrapperGetWordCount(pLine));
            for (int iWord = 0; iWord < WrapperGetWordCount(pLine); iWord++) {
                printf("    word %d:", iWord);
                uint64_t pWord = WrapperGetWordWithIndex(pLine, iWord);
                printf("%ls\n", WrapperGetText(pWord));
                
                WrapperDestroyOcrWord(pWord);
            }
            
            WrapperDestroyOcrLine(pLine);
            
        }
        
        WrapperDestroyOcrRegion(pRegion);
    }
    
    WrapperDestroyOcrResut(pResult);
    
    free(buf);
    WrapperDestroyOcrEngine(pEngine);
}

