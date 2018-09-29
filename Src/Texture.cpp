#include "texture.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Texture::Texture(const std::string &name, size_t width, size_t height, size_t numComponents, size_t bytesPerComponent)
	: name_(name)
    , width_(width)
	, height_(height)
	, numComponents_(numComponents)
	, bytesPerComponent_(bytesPerComponent)
	, buffer_(width * height * numComponents * bytesPerComponent) {}

void Texture::Set(size_t x, size_t y, size_t component, const void *values) {
	auto i = (y * width_ + x) * (numComponents_ * bytesPerComponent_) + component * bytesPerComponent_;
	if (i >= buffer_.size()) {
		throw std::out_of_range{"i"};
	}
	std::memcpy(&buffer_[i], values, bytesPerComponent_);
}

void Texture::Set(size_t x, size_t y, const void *values, size_t numValues) {
    auto i = (y * width_ + x) * (numComponents_ * bytesPerComponent_);
    if (i >= buffer_.size()) {
        throw std::out_of_range{"i"};
    }
    std::memcpy(&buffer_[i], values, numValues * bytesPerComponent_);
}

void Texture::Set(size_t x, size_t y, const Color3 &value) {
	if (numComponents_ < 3) {
		throw std::logic_error("Must be at least 3 components texture");
	}
    Set(x, y, &value, 3);
}

void Texture::Set(size_t x, size_t y, const Color4 &value) {
	if (numComponents_ < 4) {
		throw std::logic_error("Must be at least 3 components texture");
	}
    Set(x, y, &value, 4);
}

void Texture::SaveBmp(const std::string &fileName) const {
	std::vector<unsigned char> image;
	image.resize(3 * width_ * height_);

	auto srcPtr = buffer_.data();
	auto dstPtr = image.data();
	for (size_t i = 0; i < 3 * width_ * height_; ++i) {
		*dstPtr++ = (unsigned char)(*((float*)srcPtr) * 255);
		srcPtr += bytesPerComponent_;
	}

	stbi_write_bmp(fileName.empty() ? name_.c_str() : fileName.c_str(), (int)width_, (int)height_, 3, image.data());
}

std::ostream &operator<<(std::ostream &os, const Texture &obj) {
	return os << "Texture{" << obj.name_.c_str() << ", w=" << obj.width_ << ", h=" << obj.height_ << ", c=" << obj.numComponents_ 
			  << ", bpc=" << obj.bytesPerComponent_ << "}";
}