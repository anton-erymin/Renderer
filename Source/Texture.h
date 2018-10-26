#pragma once

#include <ostream>
#include <vector>

#include "Types.h"

class Texture {
public:
	Texture(const std::string &name, size_t width, size_t height, size_t numComponents, size_t bytesPerComponent);

    void Clear(const void *clearValues);

	void Set(size_t x, size_t y, size_t component, const void *value);
    void Set(size_t x, size_t y, const void *values, size_t numValues);
	void Set(size_t x, size_t y, const Color3 &value);
	void Set(size_t x, size_t y, const Color4 &value);

    void Get(size_t x, size_t y, size_t component, void *outValue);

	void SaveBmp(const std::string &fileName) const;

	size_t Width() const { return width; }
	size_t Height() const { return height; }

    const std::string &Name() const { return name; }

private:
	friend std::ostream &operator<<(std::ostream &os, const Texture &obj);

    const std::string name;

	const size_t width;
	const size_t height;
	const size_t numComponents;
	const size_t bytesPerComponent;

	std::vector<uint8_t> buffer;
};

std::ostream &operator<<(std::ostream &os, const Texture &obj);