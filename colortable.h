#pragma once
#include <map>
#include <nana/paint/graphics.hpp>

class ColorTable {
	using colormap_t = std::map<int, nana::color>;
	colormap_t colormap;
public:
	using iterator = colormap_t::iterator;
	using const_iterator = colormap_t::const_iterator;

	ColorTable(int size)
	{
		for (size_t i = 0; i < size; i++)
		{
			/// Sets color with a HSL value.
			/// @param hue in range of [0, 360]
			/// @param saturation in range of [0, 1]
			/// @param lightness  in range of [0, 1]
			double hue = 240 / size * static_cast<double>(i);
			colormap[i] = nana::color().from_hsl(hue, 1.0, 0.5);
		}
	}

	iterator begin() { return colormap.begin(); }
	iterator end() { return colormap.end(); }
	const_iterator begin() const { return colormap.begin(); }
	const_iterator end() const { return colormap.end(); }
	const_iterator cbegin() const { return colormap.cbegin(); }
	const_iterator cend() const { return colormap.cend(); }

	nana::color at(const int& _KeyVal) const { return colormap.at(_KeyVal); }
};

