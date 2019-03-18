#include <tuple>
#include <map>
#include <memory>

#include <tclap/CmdLine.h>
#include <spdlog/spdlog.h>
#include <SFML/Graphics.hpp>
#include <omp.h>

#include "rectangle.h"
#include "coordinates.h"
#include "colortable.h"

std::tuple<Coordinates<int>, Rect<float>, int> ParseCmdLine(int argc, char** argv)
{
	TCLAP::CmdLine cmd("Parallel mandelbrot implementation by Johann Wimmer", ' ', "0.1", false);

	// Parse target picture size
	TCLAP::ValueArg<int> wArg("w", "width", "Width of the target picture", true, 0, "int");
	TCLAP::ValueArg<int> hArg("h", "height", "Height of the target picture", true, 0, "int");

	// Parse viewport rectangle arguments
	TCLAP::ValueArg<float> minXArg("a", "minx", "Minimal x coordinate of viewport", true, 0.0f, "float");
	TCLAP::ValueArg<float> minYArg("b", "miny", "Minimal y coordinate of viewport", true, 0.0f, "float");
	TCLAP::ValueArg<float> maxXArg("c", "maxx", "Maxmimal x coordinate of viewport", true, 0.0f, "float");
	TCLAP::ValueArg<float> maxYArg("d", "maxy", "Maxmimal y coordinate of viewport", true, 0.0f, "float");

	TCLAP::ValueArg<int> iterationsArg("i", "maxIt", "Maxmimal number of iterations", true, 0, "int");

	cmd.add(wArg);
	cmd.add(hArg);

	cmd.add(minXArg);
	cmd.add(minYArg);
	cmd.add(maxXArg);
	cmd.add(maxYArg);

	cmd.add(iterationsArg);

	cmd.parse(argc, argv);

	Coordinates<int> p = {
		wArg.getValue(),
		hArg.getValue()
	};

	Rect<float> r = {
		{minXArg.getValue(), minYArg.getValue()},
		{maxXArg.getValue(), maxYArg.getValue()},
	};

	return std::make_tuple(p, r, iterationsArg.getValue());
}

// Scales the current coordinate to the output canvas
double scale(int p, double p0, double p1, int range) {
	return p0 + p * (p1 - p0) / range;
}

int main(int argc, char* argv[]) {
	try
	{
		spdlog::info("Welcome to parallel mandelbrot!");
		auto [pictureCoordinates, viewport, iterations] = ParseCmdLine(argc, argv);

		spdlog::info("Picture size: width = {}, height = {}", pictureCoordinates.X, pictureCoordinates.Y);
		spdlog::info("Viewport: minX = {}, minY = {}, maxX = {}, maxY = {}", viewport.first.X, viewport.first.Y, viewport.second.X, viewport.second.Y);
		spdlog::info("Maximal number of iterations: {}", iterations);

		// Generate color table based on the number of iterations
		ColorTable colorTable(iterations);
		int i;
		int index;
		double nextzr;
		double nextzi;

		double zr;
		double zi;

		sf::Vertex vertex;
		sf::VertexArray vertexArray(sf::Points, pictureCoordinates.X * pictureCoordinates.Y);

		spdlog::info("Starting calculation...");

#pragma omp parallel \
  shared (iterations, pictureCoordinates, viewport, colorTable, vertexArray) \
  private (index, i, nextzr, nextzi, zr, zi, vertex)
		{
#pragma omp for
			for (index = 0; index < pictureCoordinates.X * pictureCoordinates.Y; index++)
			{
				int x = index % pictureCoordinates.X;
				int y = index / pictureCoordinates.X;
				zr = 0.;
				zi = 0.;

				for (i = 0; i < iterations; i++) {

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

				// Set the pixel at the current position
				if (i == iterations) {
					i -= 1;
				}
				vertex.position = sf::Vector2f(x, y);
				vertex.color = colorTable.at(i);
#pragma omp critical
				{
					vertexArray.append(vertex);
				}
			}
		}

		spdlog::info("Calculation complete!");

		sf::RenderWindow window(sf::VideoMode(pictureCoordinates.X, pictureCoordinates.Y), "Parallel Mandelbrot - Johann Wimmer", sf::Style::Titlebar | sf::Style::Close);

		// run the program as long as the window is open
		while (window.isOpen())
		{
			// check all the window's events that were triggered since the last iteration of the loop
			sf::Event event;
			while (window.pollEvent(event))
			{
				// "close requested" event: we close the window
				if (event.type == sf::Event::Closed)
					window.close();
			}
			// draw the vertex array to the screen
			window.draw(vertexArray);

			// end the current frame
			window.display();
		}

	}
	catch (TCLAP::ArgException & e)
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	}
	catch (std::exception & e)
	{
		std::cerr << "error: " << e.what() << "\n";
	}

}