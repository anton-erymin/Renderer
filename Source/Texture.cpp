#include "Texture.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Algorithms.h"

Texture::Texture(const std::string &name, uint32_t width, uint32_t height, size_t numComponents, size_t bytesPerComponent, bool isNormalized)
	: name(name)
    , width(width)
	, height(height)
	, numComponents(numComponents)
	, bytesPerComponent(bytesPerComponent)
    , isNormalized(isNormalized) {
    buffer.resize(width * height * numComponents * bytesPerComponent, 0);
    assert(width > 0 && height > 0);
    assert(numComponents == 1 || numComponents == 3 || numComponents == 4);
    assert(bytesPerComponent == 4 || bytesPerComponent == 8);
}

Texture::Texture(const std::string &fileName, bool isSRGB) {
    assert(!fileName.empty());

    // Load image from file
    int x, y, n;
    unsigned char *data = nullptr;
    data = stbi_load(fileName.c_str(), &x, &y, &n, 0);

    if (!data) {
        throw std::runtime_error{"Cant load image"};
    }

    width = static_cast<uint32_t>(x);
    height = static_cast<uint32_t>(y);
    numComponents = n;
    bytesPerComponent = sizeof(float_t);
    name = fileName;
    buffer.resize(width * height * numComponents * bytesPerComponent);

    assert(width > 0 && height > 0);
    assert(numComponents == 1 || numComponents == 3 || numComponents == 4);

    // Copy data and transform into normalized [0;1]
    unsigned char *srcPtr = data;
    float_t *dstPtr = reinterpret_cast<float_t*>(buffer.data());
    for (size_t i = 0; i < width * height * numComponents; ++i) {
        *dstPtr++ = (float_t(*srcPtr++) / 255.0f);
    }
    stbi_image_free(data);

    isNormalized = true;

    // Decoding gamma if needed
    if (isSRGB) {
        GammaCorrection(*this, 2.2f);
    }
}

void Texture::Clear(const void *clearValues) {
    assert(clearValues);
    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            Set(i, j, clearValues);
        }
    }
}

void Texture::Set(size_t x, size_t y, size_t component, const void *value) {
    assert(component >= 0 && component < numComponents);
    assert(value);

    size_t i = (y * width + x) * (numComponents * bytesPerComponent) + component * bytesPerComponent;
    assert(i < buffer.size());

    float_t v = *((const float_t*)value);
    if (isNormalized) {
        v = Clamp(v, 0.0f, 1.0f);
    }

	std::memcpy(&buffer[i], &v, bytesPerComponent);
}

void Texture::Set(size_t x, size_t y, const void *values) {
    assert(values);

    size_t i = (y * width + x) * (numComponents * bytesPerComponent);
    assert(i < buffer.size());

    if (isNormalized) {
        for (size_t i = 0; i < numComponents; i++) {
            float_t *ptr = (float_t*)((char*)values + i * bytesPerComponent);
            *ptr = Clamp(*ptr, 0.0f, 1.0f);
        }
    }

    std::memcpy(&buffer[i], values, numComponents * bytesPerComponent);
}

void Texture::Set(size_t x, size_t y, const Vec3f &value) {
    assert(numComponents >= 3);
    Set(x, y, &value);
}

void Texture::Set(size_t x, size_t y, const Vec4f &value) {
    assert(numComponents >= 4);
    Set(x, y, &value);
}

void Texture::Get(size_t x, size_t y, size_t component, void *outValue) const {
    assert(component >= 0 && component < numComponents);
    assert(outValue);

    size_t i = (y * width + x) * (numComponents * bytesPerComponent) + component * bytesPerComponent;
    assert(i < buffer.size());

    *((float*)outValue) = *((float*)&buffer[i]);
}

void Texture::Get(size_t x, size_t y, void *outValues) const {
    assert(outValues);

    size_t i = (y * width + x) * (numComponents * bytesPerComponent);
    assert(i < buffer.size());

    std::memcpy(outValues, &buffer[i], numComponents * bytesPerComponent);
}

void Texture::SaveBmp(const std::string &fileName) const {
    assert(numComponents == 1 || numComponents == 3 || numComponents == 4);

	std::vector<uint8_t> imageData;
	imageData.resize(3 * width * height);

	auto srcPtr = buffer.data();
	auto dstPtr = imageData.data();
    float r = 0, g = 0, b = 0;

    for (size_t i = 0; i < width * height; i++) {
        if (numComponents == 1) {
            r = g = b = *((float*)srcPtr);
            srcPtr += bytesPerComponent;
        } else if (numComponents >= 3) {
            r = *((float*)srcPtr);
            srcPtr += bytesPerComponent;
            g = *((float*)srcPtr);
            srcPtr += bytesPerComponent;
            b = *((float*)srcPtr);
            srcPtr += bytesPerComponent;
        }
        if (numComponents == 4) {
            srcPtr += bytesPerComponent;
        }

        *dstPtr++ = uint8_t(r * 255.0);
        *dstPtr++ = uint8_t(g * 255.0);
        *dstPtr++ = uint8_t(b * 255.0);
    }

	stbi_write_bmp(fileName.empty() ? name.c_str() : fileName.c_str(), (int)width, (int)height, 3, imageData.data());
}

void Texture::Sample(float_t u, float_t v, void *outValue) const {
    assert(outValue);

    /*u = Clamp(u, 0.0f, 1.0f);
    v = Clamp(v, 0.0f, 1.0f);*/

    u = u - int(u);
    v = v - int(v);
    if (u < 0) {
        u = 1.0f + u;
    }
    if (v < 0) {
        v = 1.0f + v;
    }

    size_t x = static_cast<size_t>(u * (width - 1));
    size_t y = static_cast<size_t>(v * (height - 1));
    Get(x, y, outValue);
}

void Texture::CopyTo(Texture &dst) const {
    assert(IsCompatibleWith(dst));
    dst.buffer = buffer;
}

void Texture::MoveTo(Texture &dst) {
    assert(IsCompatibleWith(dst));
    dst.buffer = std::move(buffer);
}

std::ostream &operator<<(std::ostream &os, const Texture &obj) {
	return os << "Texture{" << obj.name.c_str() << ", w=" << obj.width << ", h=" << obj.height << ", c=" << obj.numComponents 
			  << ", bpc=" << obj.bytesPerComponent << "}";
}