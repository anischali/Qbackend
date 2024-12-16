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
    qrcode_engine() {};
    ~qrcode_engine() {};


    ZXing::Matrix<uint8_t> write(std::string text, ZXing::BarcodeFormat format);
    ZXing::Matrix<uint8_t> write(std::string text, ZXing::BarcodeFormat format, int width, int height);
    std::string read(ZXing::ImageView &image, ZXing::ReaderOptions opts);
    std::string read(ZXing::ImageView &image);
};
};



#endif