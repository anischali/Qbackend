#include "BarcodeFormat.h"
#include "BitMatrix.h"
#include "MultiFormatWriter.h"
#include "MultiFormatReader.h"
#include "BinaryBitmap.h"




using namespace ZXing;

Matrix<uint8_t> write_barcode(std::string text, ZXing::BarcodeFormat format)
{
	using namespace ZXing;

	auto writer = MultiFormatWriter(format);
	auto matrix = writer.encode(text, 0, 0);
	auto bitmap = ToMatrix<uint8_t>(matrix);


	return bitmap.copy();
}



Matrix<uint8_t> write_barcode(std::string text, ZXing::BarcodeFormat format)
{
	using namespace ZXing;

	auto writer = MultiFormatWriter(format);
	auto matrix = writer.encode(text, 0, 0);
	auto bitmap = ToMatrix<uint8_t>(matrix);


	return bitmap.copy();
}



std::string read_barcode(BinaryBitmap image, ReaderOptions opts)
{
	using namespace ZXing;

	auto reader = MultiFormatReader(opts);
	auto result = reader.read(image);

	return result.text();
}




