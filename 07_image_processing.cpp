
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
  fileHeader.fileSizeInBytes =
      fileHeader.pixelOffset + image.width * image.height * image.channels;

  BmpInfoHeader infoHeader = {};
  infoHeader.width = image.width;
  infoHeader.height = image.height;
  infoHeader.xPixelsPerM = 0;
  infoHeader.yPixelsPerM = 0;
  infoHeader.colorsUsed = 0;

  int rowSize = static_cast<int>(
                    std::ceil((infoHeader.bitsPerPixel * image.width) / 32)) *
                4;
  uint8_t *pBuf = nullptr;
  if (rowSize == image.width * 3) {
    pBuf = const_cast<uint8_t *>(image.pixels.data());
  } else {
    // we have to create a copy of the pixels to account for additional padding
    // that is necessary at the end of each pixel row

    int pixelsSize = rowSize * image.height;
    pBuf = reinterpret_cast<uint8_t *>(std::malloc(pixelsSize));
    std::memset(pBuf, 0, pixelsSize);

#if 0
#pragma omp parallel for
    for (int row = 0; row < image.height; row++) {
      int pBufRowIndex = row * rowSize;
      int pixelsRowIndex = row * image.width;
      memcpy(pBuf + pBufRowIndex, image.pixels.data() + pixelsRowIndex,
             image.width * 3);
    }
#else
    for (int row = 0; row < image.height; row++) {
      for (int col = 0; col < image.width; col++) {
        int pBufIndex = row * rowSize + col * 3;
        int pixelsIndex = (row * image.width + col) * 3;
        pBuf[pBufIndex + 0] = image.pixels[pixelsIndex + 0];
        pBuf[pBufIndex + 1] = image.pixels[pixelsIndex + 1];
        pBuf[pBufIndex + 2] = image.pixels[pixelsIndex + 2];
      }
    }
#endif
  }

  BmpFile bmpFile = {
      fileHeader,
      infoHeader,
      pBuf,
  };

  std::ofstream file(image.fileName, std::ios::binary);
  file << bmpFile;
  return true;
}

std::istream &operator>>(std::istream &is, BmpFile &f) {
  is.read(reinterpret_cast<char *>(&f.fileHeader), sizeof(f.fileHeader));
  is.read(reinterpret_cast<char *>(&f.infoHeader), sizeof(f.infoHeader));
  int diff = f.infoHeader.size - sizeof(BmpInfoHeader);
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

  int rowSize = static_cast<int>(std::ceil(
                    (bmpFile.infoHeader.bitsPerPixel * image.width) / 32)) *
                4;
  if (rowSize == image.width * 3) {
    unsigned int pixelSize = image.width * image.height * image.channels;
    image.pixels =
        std::vector<uint8_t>(bmpFile.pixels, bmpFile.pixels + pixelSize);
  } else {
    // we have to create a copy of the pixels to account for additional padding
    // that is necessary at the end of each pixel row

    int pixelsSize = rowSize * image.height;
    image.pixels =
        std::vector<uint8_t>(image.width * image.height * image.channels);

    for (int row = 0; row < image.height; row++) {
      for (int col = 0; col < image.width; col++) {
        int pBufIndex = row * rowSize + col * 3;
        int pixelsIndex = (row * image.width + col) * 3;
        image.pixels[pixelsIndex + 0] = bmpFile.pixels[pBufIndex + 0];
        image.pixels[pixelsIndex + 1] = bmpFile.pixels[pBufIndex + 1];
        image.pixels[pixelsIndex + 2] = bmpFile.pixels[pBufIndex + 2];
      }
    }
  }

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
