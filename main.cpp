#include <tuple>
#include <map>
#include <memory>

#include <tclap/CmdLine.h>
#include "spdlog/spdlog.h"
#include <nana/gui.hpp>

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
		ColorTable ct (iterations);
		int i;
		std::vector<std::tuple<int, int, nana::color>> bitmap;

		for (int x = 0; x < pictureCoordinates.X; x++)
		{
			for (int y = 0; y < pictureCoordinates.Y; y++)
			{
				double zr = 0.;
				double zi = 0.;

				for (i = 0; i < iterations; i++) {

					// calculate next iteration
					double nextzr = zr * zr - zi * zi + scale(x, viewport.first.X, viewport.second.X, pictureCoordinates.X);
					double nextzi = 2 * zr * zi + scale(y, viewport.first.Y, viewport.second.Y, pictureCoordinates.Y);

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
 				bitmap.emplace_back(std::make_tuple(x, y, ct.at(i)));
			}
		}

		using namespace nana;

		form fm(API::make_center(pictureCoordinates.X, pictureCoordinates.Y),
			form::appear::decorate<
				form::appear::taskbar
			>());
		fm.caption(L"Parallel Mandelbrot - Johann Wimmer");

		//form fm{ API::make_center(pictureCoordinates.X, pictureCoordinates.Y) };
		drawing dw(fm);

		dw.draw([&ct, &bitmap](paint::graphics & graph) {
			for (auto [posX, posY, c] : bitmap) {
				graph.rectangle(rectangle{ posX, posY, 10, 10 }, true, c);
			}
		});

		dw.update();
		fm.show();
		::nana::exec();

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