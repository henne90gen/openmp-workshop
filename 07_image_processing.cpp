
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

struct Image {
  std::string fileName;
  uint32_t width = 0;
  uint32_t height = 0;
  uint32_t channels = 3;
  std::vector<uint8_t> pixels;
};

#pragma pack(push, 1)
struct BmpFileHeader {
  uint16_t identifier = 0x4D42;
  uint32_t fileSizeInBytes;
  uint16_t reserved0 = 0;
  uint16_t reserved1 = 0;
  uint32_t pixelOffset = 54;
};

struct BmpInfoHeader {
  uint32_t size = 40;
  int32_t width;
  int32_t height;
  uint16_t numPlanes = 1;
  uint16_t bitsPerPixel = 24;
  uint32_t compression = 0;
  uint32_t imageSize = 0;
  uint32_t xPixelsPerM;
  uint32_t yPixelsPerM;
  uint32_t colorsUsed;
  uint32_t importantColors = 0;
};

struct BmpFile {
  BmpFileHeader fileHeader;
  BmpInfoHeader infoHeader;
  uint8_t *pixels = nullptr;
};
#pragma pack(pop)

uint32_t getRowSize(uint32_t width, uint32_t bitsPerPixel) {
  return static_cast<int>(std::ceil((bitsPerPixel * width) / 32)) * 4;
}

std::ostream &operator<<(std::ostream &os, BmpFile &f) {
  size_t fileHeaderSize = sizeof(f.fileHeader);
  size_t infoHeaderSize = sizeof(f.infoHeader);
  os.write(reinterpret_cast<const char *>(&f.fileHeader), fileHeaderSize);
  os.write(reinterpret_cast<const char *>(&f.infoHeader), infoHeaderSize);
  uint32_t pixelSize = f.fileHeader.fileSizeInBytes - f.fileHeader.pixelOffset;
  os.write(reinterpret_cast<const char *>(f.pixels), pixelSize);
  return os;
}

bool writeBmp(const Image &image) {
  BmpFileHeader fileHeader = {};

  BmpInfoHeader infoHeader = {};
  infoHeader.width = image.width;
  infoHeader.height = image.height;
  infoHeader.xPixelsPerM = 0;
  infoHeader.yPixelsPerM = 0;
  infoHeader.colorsUsed = 0;

  int rowSize = getRowSize(image.width, infoHeader.bitsPerPixel);
  fileHeader.fileSizeInBytes = fileHeader.pixelOffset + image.height * rowSize;

  const auto pBuf = const_cast<uint8_t *>(image.pixels.data());
  BmpFile bmpFile = {
      .fileHeader = fileHeader,
      .infoHeader = infoHeader,
      .pixels = pBuf,
  };

  std::ofstream file(image.fileName, std::ios::binary);
  file << bmpFile;
  return true;
}

std::istream &operator>>(std::istream &is, BmpFile &f) {
  is.read(reinterpret_cast<char *>(&f.fileHeader), sizeof(f.fileHeader));
  is.read(reinterpret_cast<char *>(&f.infoHeader), sizeof(f.infoHeader));
  int64_t diff = f.infoHeader.size - sizeof(BmpInfoHeader);
  if (diff > 0) {
    is.seekg(diff, std::ios_base::cur);
  }
  uint32_t pixelSize = f.fileHeader.fileSizeInBytes - f.fileHeader.pixelOffset;
  f.pixels = reinterpret_cast<uint8_t *>(std::malloc(pixelSize));
  is.read(reinterpret_cast<char *>(f.pixels), pixelSize);
  return is;
}

bool readBmp(Image &image) {
  BmpFile bmpFile = {};

  std::ifstream file(image.fileName, std::ios::binary);
  file >> bmpFile;

  image.width = bmpFile.infoHeader.width;
  image.height = bmpFile.infoHeader.height;
  image.channels = bmpFile.infoHeader.bitsPerPixel / 8;

  unsigned int pixelSize = image.width * image.height * image.channels;
  image.pixels =
      std::vector<uint8_t>(bmpFile.pixels, bmpFile.pixels + pixelSize);

  return true;
}

void edit_image(Image &image) {
#pragma omp parallel for
  for (int i = 0; i < image.height * image.height; i++) {
    image.pixels[i * 3 + 0] -= 0;
    image.pixels[i * 3 + 1] -= 0;
    image.pixels[i * 3 + 2] -= 50;
  }
}

int main() {
  Image image = {.fileName = "../test.bmp"};
  readBmp(image);

  std::cout << "Read image with size " << image.width << "x" << image.height
            << std::endl;

  edit_image(image);

  image.fileName = "../test_result.bmp";
  writeBmp(image);
  return 0;
}
