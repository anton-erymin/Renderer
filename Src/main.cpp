#include <iostream>

#include "Texture.h"
#include "types.h"

struct RasterizerVertex {
    Vec2i    coords;
    float_t *attributes;
};

void DrawTriangle(RasterizerVertex v1, RasterizerVertex v2, RasterizerVertex v3, size_t numAttributes, Texture &target) {
    Vec2i &x1 = v1.coords;
    Vec2i &x2 = v2.coords;
    Vec2i &x3 = v3.coords;

    // Sort vertices
	if (x1.y > x2.y) {
		std::swap(v1, v2);
	}
	if (x1.y > x3.y) {
		std::swap(v1, v3);
	}
	if (x2.y > x3.y) {
		std::swap(v2, v3);
	}

	if (x1.y == x3.y) {
		return;
	}

    // Side 1 from v1 - v3
    // Side 2 from v1 - v2
    // Side 3 from v2 - v3
    const float_t sideHeight1 = float_t(x3.y - x1.y);
    const float_t sideHeight2 = float_t(x2.y - x1.y);
    const float_t sideHeight3 = float_t(x3.y - x2.y);

	const float_t dx1 = float_t(x3.x - x1.x) / sideHeight1;
	const float_t dx2 = float_t(x2.x - x1.x) / sideHeight2;
	const float_t dx3 = float_t(x3.x - x2.x) / sideHeight3;

    // For each attribute we have 3 its own slope interpolation coeffs for each triangle side
    std::vector<float_t> attrCoeffs1, attrCoeffs2, attrCoeffs3;
    attrCoeffs1.resize(numAttributes);
    attrCoeffs2.resize(numAttributes);
    attrCoeffs3.resize(numAttributes);

    for (size_t i = 0; i < numAttributes; ++i) {
        attrCoeffs1[i] = float_t(v3.attributes[i] - v1.attributes[i]) / sideHeight1;
        attrCoeffs2[i] = float_t(v2.attributes[i] - v1.attributes[i]) / sideHeight2;
        attrCoeffs3[i] = float_t(v3.attributes[i] - v2.attributes[i]) / sideHeight3;
    }

    std::vector<float_t> attrsInterpolated(numAttributes);
    std::vector<float_t> attrsInterpolatedLeft(numAttributes);
    std::vector<float_t> attrsInterpolatedRight(numAttributes);

	for (auto y = x1.y; y <= x3.y; ++y) {
		const bool second = y > x2.y;

		int32_t xl = static_cast<int32_t>(x1.x + dx1 * (y - x1.y));
		int32_t xr = static_cast<int32_t>(second ? x2.x + dx3 * (y - x2.y) : x1.x + dx2 * (y - x1.y));

        for (size_t i = 0; i < numAttributes; ++i) {
            attrsInterpolatedLeft[i]  = v1.attributes[i] + attrCoeffs1[i] * (y - x1.y);
            attrsInterpolatedRight[i] = second ? v2.attributes[i] + attrCoeffs3[i] * (y - x2.y) 
                                               : v1.attributes[i] + attrCoeffs2[i] * (y - x1.y);
        }

		if (xl > xr) {
			std::swap(xl, xr);
            std::swap(attrsInterpolatedLeft, attrsInterpolatedRight);
		}

        const float_t lineLength = float_t(xr - xl);

        // attrsInterpolatedRight will contain interpolation coeffs along the horizontal line
        for (size_t i = 0; i < numAttributes; ++i) {
            attrsInterpolatedRight[i] = (attrsInterpolatedRight[i] - attrsInterpolatedLeft[i]) / lineLength;
        }

		for (auto x = xl; x <= xr; ++x) {
			if (x >= 0 && x < target.Width() && y >= 0 && y < target.Height()) {

                for (size_t i = 0; i < numAttributes; ++i) {
                    attrsInterpolated[i] = 
                        attrsInterpolatedLeft[i] + attrsInterpolatedRight[i] * float_t(x - xl);
                }

				target.Set(x, y, Color3{ attrsInterpolated[0], attrsInterpolated[1], attrsInterpolated[2] });
			}
		}
	}
}

int main() {
	Texture screen{ "Screen", 1024, 768, 3, sizeof(float_t) };

    float_t attrs1[3] = { 1.0f, 0.0f, 0.0f };
    float_t attrs2[3] = { 0.0f, 1.0f, 0.0f };
    float_t attrs3[3] = { 0.0f, 0.0f, 1.0f };

	DrawTriangle({ 10, 10, attrs1 }, { 300, 0, attrs2 }, { 700, 400, attrs3 }, 3, screen);

	screen.SaveBmp("screen.bmp");

	LOG(screen);

	system("screen.bmp");
	std::cin.get();
	return 0;
}