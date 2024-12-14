#ifndef __QRCODE_ENGINE_HPP__
#define __QRCODE_ENGINE_HPP__
#include "BarcodeFormat.h"
#include "BitMatrix.h"
#include "MultiFormatWriter.h"
#include "MultiFormatReader.h"
#include "BinaryBitmap.h"
#include "ReadBarcode.h"


ZXing::ImageView qrcode_write_string_barcode(std::string text, ZXing::BarcodeFormat format);
std::string qrcode_read_string_barcode(ZXing::ImageView &image, ZXing::ReaderOptions opts);


#endif