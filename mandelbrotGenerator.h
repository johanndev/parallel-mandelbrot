#pragma once

#include <memory>

#include <SFML/Graphics.hpp>
#include <omp.h>

#include "colortable.h"
#include "coordinates.h"
#include "rectangle.h"

// Scales the current coordinate to the output canvas
double scale(int p, double p0, double p1, int range) {
	return p0 + p * (p1 - p0) / range;
}

static std::unique_ptr<sf::VertexArray> GenerateMandelbrotSet(
	const Coordinates<unsigned int> pictureCoordinates,
	const Rect<float> viewport,
	const unsigned int maxIterations)
{
	unsigned int i;
	unsigned int x;
	unsigned int y;
	double nextzr;
	double nextzi;

	double zr;
	double zi;

	sf::Vertex vertex;
	auto vertexArray = std::make_unique<sf::VertexArray>(sf::Points, pictureCoordinates.X * pictureCoordinates.Y);
	auto colorTable = ColorTable(maxIterations);

#pragma omp parallel \
  shared (maxIterations, pictureCoordinates, viewport, colorTable, vertexArray) \
  private (x, y, i, nextzr, nextzi, zr, zi, vertex) 
	{
#pragma omp for
		for (x = 0; x < pictureCoordinates.X; x++)
		{
			for (y = 0; y < pictureCoordinates.Y; y++)
			{
				zr = 0.;
				zi = 0.;

				for (i = 0; i < maxIterations; i++) {
					// calculate next iteration
					nextzr = zr * zr - zi * zi + scale(x, viewport.first.X, viewport.second.X, pictureCoordinates.X);
					nextzi = 2 * zr * zi + scale(y, viewport.first.Y, viewport.second.Y, pictureCoordinates.Y);

					// are we done?
					if ((nextzr * nextzr + nextzi * nextzi) > 4) {
						break;
					}
					zr = nextzr;
					zi = nextzi;
				}
				vertex.position = sf::Vector2f(x, y);
				// Set the pixel at the current position
				// Decrement the index if we hit max iterations
				// to avoid out-of-bounds access
				auto color = i == maxIterations ? i - 1 : i;
				vertex.color = colorTable.at(color);
#pragma omp critical
				{
					vertexArray->append(vertex);
				}
			}
		}
	}

	return vertexArray;
}
