#pragma once
#include "coordinates.h"

template <typename T>
struct Rect {
	Coordinates<T> first;
	Coordinates<T> second;
};