#pragma once

#include <ostream>
#include <vector>

#include "Types.h"

class Texture {
public:
	Texture(const std::string &name, size_t width, size_t height, size_t numComponents, size_t bytesPerComponent);

	void Set(size_t x, size_t y, size_t component, const void *value);
    void Set(size_t x, size_t y, const void *values, size_t numValues);
	void Set(size_t x, size_t y, const Color3 &value);
	void Set(size_t x, size_t y, const Color4 &value);

	void SaveBmp(const std::string &fileName) const;

	size_t Width() const { return width_; }
	size_t Height() const { return height_; }

    const std::string &Name() const { return name_; }

private:
	friend std::ostream &operator<<(std::ostream &os, const Texture &obj);

    const std::string name_;

	const size_t width_;
	const size_t height_;
	const size_t numComponents_;
	const size_t bytesPerComponent_;

	std::vector<uint8_t> buffer_;
};

std::ostream &operator<<(std::ostream &os, const Texture &obj);