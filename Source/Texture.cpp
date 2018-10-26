#include "Texture.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Texture::Texture(const std::string &name, size_t width, size_t height, size_t numComponents, size_t bytesPerComponent)
	: name(name)
    , width(width)
	, height(height)
	, numComponents(numComponents)
	, bytesPerComponent(bytesPerComponent)
	, buffer(width * height * numComponents * bytesPerComponent) {}

void Texture::Clear(const void *clearValues) {
    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            Set(i, j, clearValues, numComponents);
        }
    }
}

void Texture::Set(size_t x, size_t y, size_t component, const void *values) {
	auto i = (y * width + x) * (numComponents * bytesPerComponent) + component * bytesPerComponent;
    assert(i < buffer.size());
	std::memcpy(&buffer[i], values, bytesPerComponent);
}

void Texture::Set(size_t x, size_t y, const void *values, size_t numValues) {
    auto i = (y * width + x) * (numComponents * bytesPerComponent);
    assert(i < buffer.size());
    std::memcpy(&buffer[i], values, numValues * bytesPerComponent);
}

void Texture::Set(size_t x, size_t y, const Color3 &value) {
	if (numComponents < 3) {
		throw std::logic_error("Must be at least 3 components texture");
	}
    Set(x, y, &value, 3);
}

void Texture::Set(size_t x, size_t y, const Color4 &value) {
	if (numComponents < 4) {
		throw std::logic_error("Must be at least 4 components texture");
	}
    Set(x, y, &value, 4);
}

void Texture::Get(size_t x, size_t y, size_t component, void *outValue) {
    auto i = (y * width + x) * (numComponents * bytesPerComponent) + component * bytesPerComponent;
    assert(i < buffer.size());
    *((float*)outValue) = *((float*)&buffer[i]);
}

void Texture::SaveBmp(const std::string &fileName) const {
    assert(numComponents == 1 || numComponents == 3 || numComponents == 4);

	std::vector<uint8_t> imageData;
	imageData.resize(3 * width * height);

	auto srcPtr = buffer.data();
	auto dstPtr = imageData.data();
    float r, g, b;

    for (size_t i = 0; i < width * height; i++) {
        if (numComponents == 1) {
            r = g = b = *((float*)srcPtr);
            srcPtr += bytesPerComponent;
        } else if (numComponents == 3) {
            r = *((float*)srcPtr);
            srcPtr += bytesPerComponent;
            g = *((float*)srcPtr);
            srcPtr += bytesPerComponent;
            b = *((float*)srcPtr);
            srcPtr += bytesPerComponent;
        }

        *dstPtr++ = uint8_t(r * 255.0);
        *dstPtr++ = uint8_t(g * 255.0);
        *dstPtr++ = uint8_t(b * 255.0);
    }

	stbi_write_bmp(fileName.empty() ? name.c_str() : fileName.c_str(), (int)width, (int)height, 3, imageData.data());
}

std::ostream &operator<<(std::ostream &os, const Texture &obj) {
	return os << "Texture{" << obj.name.c_str() << ", w=" << obj.width << ", h=" << obj.height << ", c=" << obj.numComponents 
			  << ", bpc=" << obj.bytesPerComponent << "}";
}