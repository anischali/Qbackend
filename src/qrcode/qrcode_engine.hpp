#ifndef __QRCODE_ENGINE_HPP__
#define __QRCODE_ENGINE_HPP__
#include "BarcodeFormat.h"
#include "BitMatrix.h"
#include "MultiFormatWriter.h"
#include "MultiFormatReader.h"
#include "BinaryBitmap.h"
#include "ReadBarcode.h"


namespace qbackend::engines {
class qrcode_engine {

private:

public:
    qrcode_engine();
    ~qrcode_engine();


    ZXing::ImageView write(std::string text, ZXing::BarcodeFormat format);
    std::string read(ZXing::ImageView &image, ZXing::ReaderOptions opts);
};
};



#endif