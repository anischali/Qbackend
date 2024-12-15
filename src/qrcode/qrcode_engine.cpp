#include "qrcode_engine.hpp"


using namespace qbackend::engines;

ZXing::Matrix<uint8_t> qrcode_engine::write(std::string text, ZXing::BarcodeFormat format)
{
	return write(text, format, 512, 512);
}


ZXing::Matrix<uint8_t> qrcode_engine::write(std::string text, ZXing::BarcodeFormat format, int height, int width)
{
	auto writer = ZXing::MultiFormatWriter(format);
	auto matrix = writer.encode(text, 512, 512);
	auto bitmap = ZXing::ToMatrix<uint8_t>(matrix);


	return bitmap;
}

std::string qrcode_engine::read(ZXing::ImageView &image, ZXing::ReaderOptions opts)
{
	ZXing::Result result = ZXing::ReadBarcode(image, opts);

	return result.text();
}


