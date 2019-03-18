#pragma once
#include <map>
#include <SFML/Graphics.hpp>

#include "hsl.h"
#include "rgb.h"

static float HueToRGB(float v1, float v2, float vH) {
	if (vH < 0)
		vH += 1;

	if (vH > 1)
		vH -= 1;

	if ((6 * vH) < 1)
		return (v1 + (v2 - v1) * 6 * vH);

	if ((2 * vH) < 1)
		return v2;

	if ((3 * vH) < 2)
		return (v1 + (v2 - v1) * ((2.0f / 3) - vH) * 6);

	return v1;
}

static RGBColor HSLToRGB(HSLColor hsl) {
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;

	if (hsl.S == 0)
	{
		r = g = b = (unsigned char)(hsl.L * 255);
	}
	else
	{
		float v1, v2;
		float hue = (float)hsl.H / 360;

		v2 = (hsl.L < 0.5) ? (hsl.L * (1 + hsl.S)) : ((hsl.L + hsl.S) - (hsl.L * hsl.S));
		v1 = 2 * hsl.L - v2;

		r = (unsigned char)(255 * HueToRGB(v1, v2, hue + (1.0f / 3)));
		g = (unsigned char)(255 * HueToRGB(v1, v2, hue));
		b = (unsigned char)(255 * HueToRGB(v1, v2, hue - (1.0f / 3)));
	}

	return RGBColor(r, g, b);
}

class ColorTable {
	using colormap_t = std::map<int, sf::Color>;
	colormap_t colormap;
public:
	using iterator = colormap_t::iterator;
	using const_iterator = colormap_t::const_iterator;

	ColorTable(unsigned int size)
	{
		for (size_t i = 0; i < size; i++)
		{
			auto hsl = HSLColor(240.0f / size * i, 1.0, 0.5);

			auto rgb = HSLToRGB(hsl);
			colormap[i] = sf::Color(rgb.R, rgb.G, rgb.B);
		}
	}

	iterator begin() { return colormap.begin(); }
	iterator end() { return colormap.end(); }
	const_iterator begin() const { return colormap.begin(); }
	const_iterator end() const { return colormap.end(); }
	const_iterator cbegin() const { return colormap.cbegin(); }
	const_iterator cend() const { return colormap.cend(); }

	sf::Color at(const int& _KeyVal) const {
		return colormap.at(_KeyVal);
	}
};
