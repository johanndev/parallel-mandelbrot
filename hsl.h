#pragma once

class HSLColor {
public:
	int H;
	float S;
	float L;

	HSLColor(int h, float s, float l)
	{
		H = h;
		S = s;
		L = l;
	}

	bool Equals(HSLColor hsl)
	{
		return (H == hsl.H) && (S == hsl.S) && (L == hsl.L);
	}
};
