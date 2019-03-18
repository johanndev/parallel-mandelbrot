#pragma once

class RGBColor {
public:
	unsigned char R;
	unsigned char G;
	unsigned char B;

	RGBColor(unsigned char r, unsigned char g, unsigned char b)
	{
		R = r;
		G = g;
		B = b;
	}

	bool Equals(RGBColor rgb)
	{
		return (R == rgb.R) && (G == rgb.G) && (B == rgb.B);
	}
};