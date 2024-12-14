#include "qrcode_engine.hpp"

ZXing::ImageView qrcode_write_string_barcode(std::string text, ZXing::BarcodeFormat format)
{
	auto writer = ZXing::MultiFormatWriter(format);
	auto matrix = writer.encode(text, 0, 0);
	auto bitmap = ZXing::ToMatrix<uint8_t>(matrix);


	return ZXing::ImageView(bitmap.data(), bitmap.width(), bitmap.height(), ZXing::ImageFormat::RGB, 0, 0);
}

std::string qrcode_read_string_barcode(ZXing::ImageView &image, ZXing::ReaderOptions opts)
{
	ZXing::Result result = ZXing::ReadBarcode(image, opts);

	return result.text();
}


