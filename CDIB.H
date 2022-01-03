//
//
//	Исходники класса CDib взяты из примеров к книге 
//	"Основы Visual C++ версия 4" Дэвид Дж. Круглински 
//													
//	К исходному классу была добавлена новая функция CreateRegion,
//	основанная на коде другой функции BitmapToRegion (за подробной
//	информацией смотри реализацию функции CreateRegion), а также еще
//	несколько других функций. В результате использование данного
//	класса почти не отличается от CBitmap
//	
//

#ifndef __CDIB_H__D301EDEF_3AB3_4154_9D57_4CA576C7EF5D__
#define __CDIB_H__D301EDEF_3AB3_4154_9D57_4CA576C7EF5D__

// cdib.h
// new version for WIN32

class CDib : public CObject
{
    enum Alloc {noAlloc, crtAlloc,
                heapAlloc}; // applies to BITMAPINFOHEADER
    DECLARE_SERIAL(CDib)
public:
    LPVOID  m_lpvColorTable;
    HBITMAP m_hBitmap;
    LPBYTE  m_lpImage;  // starting address of DIB bits
    LPBITMAPINFOHEADER m_lpBMIH; // buffer containing the
                                 //  BITMAPINFOHEADER
private:
    HGLOBAL m_hGlobal; // for external windows we need to free
                       // could be allocated by this class or
                       //  allocated externally
    Alloc m_nBmihAlloc;
    Alloc m_nImageAlloc;
    DWORD m_dwSizeImage; // of bits -- not BITMAPINFOHEADER
                         //  or BITMAPFILEHEADER
    int m_nColorTableEntries;

    HANDLE m_hFile;
    HANDLE m_hMap;
    LPVOID m_lpvFile;
    HPALETTE m_hPalette;
public:
    CDib();
    CDib(CSize size, int nBitCount);  // builds BITMAPINFOHEADER
    ~CDib();
    int GetSizeImage() {return m_dwSizeImage;}
    int GetSizeHeader()
        {return sizeof(BITMAPINFOHEADER) +
                sizeof(RGBQUAD) * m_nColorTableEntries;}
    CSize GetDimensions();
    BOOL AttachMapFile(const char* strPathname, BOOL bShare = FALSE);
    BOOL CopyToMapFile(const char* strPathname);
    BOOL AttachMemory(LPVOID lpvMem, BOOL bMustDelete = FALSE,
        HGLOBAL hGlobal = NULL);
    BOOL Draw(CDC* pDC, CPoint origin,
        CSize size); // until we implement CreateDibSection
    HBITMAP CreateSection(CDC* pDC = NULL);
    UINT UsePalette(CDC* pDC, BOOL bBackground = FALSE);
    BOOL MakePalette();
    BOOL SetSystemPalette(CDC* pDC);
    BOOL Compress(CDC* pDC,
        BOOL bCompress = TRUE); // FALSE means decompress
    HBITMAP CreateBitmap(CDC* pDC);
    BOOL Read(CFile* pFile);
    BOOL Write(CFile* pFile);
    void Serialize(CArchive& ar);
    void Empty();

	//new
	HRGN CreateRegion(COLORREF cTransparentColor, COLORREF cTolerance=0x101010);	
	void LoadBitmap(UINT nIDResource);
	BOOL Draw(CDC* pDC, CPoint destOrigin, CSize destSize, CPoint srcOrigin, CSize srcSize);
private:
    void DetachMapFile();
    void ComputePaletteSize(int nBitCount);
    void ComputeMetrics();
};

#endif