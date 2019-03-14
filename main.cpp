#include <tuple>

#include <tclap/CmdLine.h>
#include "spdlog/spdlog.h"

#include "rectangle.h"
#include "coordinates.h"

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

int main(int argc, char* argv[]) {
	try
	{
		spdlog::info("Welcome to parallel mandelbrot!");
		auto [pictureCoordinates, viewport, iterations] = ParseCmdLine(argc, argv);

		spdlog::info("Picture size: width = {}, height = {}", pictureCoordinates.X, pictureCoordinates.Y);
		spdlog::info("Viewport: minX = {}, minY = {}, maxX = {}, maxY = {}", viewport.first.X, viewport.first.Y, viewport.second.X, viewport.second.Y);
		spdlog::info("Maximal number of iterations: {}", iterations);


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