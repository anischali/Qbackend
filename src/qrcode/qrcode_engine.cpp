#include "qrcode_engine.hpp"


using namespace qbackend::engines;

ZXing::Matrix<uint8_t> qrcode_engine::write(std::string text, ZXing::BarcodeFormat format)
{
	return write(text, format, 512, 512);
}


ZXing::Matrix<uint8_t> qrcode_engine::write(std::string text, ZXing::BarcodeFormat format, int width, int height)
{
	auto writer = ZXing::MultiFormatWriter(format);
	auto matrix = writer.encode(text, width, height);
	auto bitmap = ZXing::ToMatrix<uint8_t>(matrix);


	return bitmap;
}

std::string qrcode_engine::read(ZXing::ImageView &image, ZXing::ReaderOptions opts)
{
	ZXing::Result result = ZXing::ReadBarcode(image, opts);

	return result.text();
}




std::string qrcode_engine::read(ZXing::ImageView &image) {
	ZXing::ReaderOptions opts;

	opts.setTextMode(ZXing::TextMode::HRI);
	opts.setEanAddOnSymbol(ZXing::EanAddOnSymbol::Read);

	return read(image, opts);
}