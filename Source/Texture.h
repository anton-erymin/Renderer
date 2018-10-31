#pragma once

#include <ostream>
#include <vector>

#include "Math.h"

class Texture {
public:
    Texture() = default;
	Texture(const std::string &name, uint32_t width, uint32_t height, size_t numComponents, size_t bytesPerComponent, bool isNormalized = true);
    Texture(const std::string &fileName, bool isSRGB = false);

    void Clear(const void *clearValues);

	void Set(size_t x, size_t y, size_t component, const void *value);
    void Set(size_t x, size_t y, const void *values);
	void Set(size_t x, size_t y, const Vec3f &value);
	void Set(size_t x, size_t y, const Vec4f &value);
    void Get(size_t x, size_t y, size_t component, void *outValue) const;
    void Get(size_t x, size_t y, void * outValues) const;

	void SaveBmp(const std::string &fileName) const;

    void Sample(float_t u, float_t v, void *outValue) const;

	uint32_t Width() const { return width; }
    uint32_t Height() const { return height; }

    const std::string &Name() const { return name; }
    size_t GetNumComponents() const { return numComponents; }
    size_t GetBytesPerComponent() const { return bytesPerComponent; }
    bool IsNormalized() const { return isNormalized; }

    bool IsCompatibleWith(const Texture &texture) const { 
        return width == texture.width && 
               height == texture.height && 
               numComponents == texture.numComponents && 
               bytesPerComponent == texture.bytesPerComponent && 
               isNormalized == texture.isNormalized && 
               buffer.size() == texture.buffer.size(); 
    }

    float_t GetAspectRatio() const { return static_cast<float_t>(width) / static_cast<float_t>(height); }

    void CopyTo(Texture &dst) const;
    void MoveTo(Texture &dst);

private:
	friend std::ostream &operator<<(std::ostream &os, const Texture &obj);

    std::string name;

	uint32_t width;
	uint32_t height;
	size_t numComponents;
	size_t bytesPerComponent;

    bool isNormalized;

	std::vector<uint8_t> buffer;
};

std::ostream &operator<<(std::ostream &os, const Texture &obj);