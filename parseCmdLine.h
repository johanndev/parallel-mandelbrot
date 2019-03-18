#pragma once
#include <tuple>

#include <tclap/CmdLine.h>

#include "coordinates.h"
#include "rectangle.h"

std::tuple<Coordinates<unsigned int>, Rect<float>, int, int, bool> ParseCmdLine(int argc, char** argv)
{
	TCLAP::CmdLine cmd("Parallel mandelbrot implementation by Johann Wimmer", ' ', "0.1", false);

	// Parse target picture size
	TCLAP::ValueArg<unsigned int> wArg("w", "width", "Width of the target picture", true, 0, "int");
	TCLAP::ValueArg<unsigned int> hArg("h", "height", "Height of the target picture", true, 0, "int");

	// Parse viewport rectangle arguments
	TCLAP::ValueArg<float> minXArg("a", "minx", "Minimal x coordinate of viewport", true, 0.0f, "float");
	TCLAP::ValueArg<float> minYArg("b", "miny", "Minimal y coordinate of viewport", true, 0.0f, "float");
	TCLAP::ValueArg<float> maxXArg("c", "maxx", "Maxmimal x coordinate of viewport", true, 0.0f, "float");
	TCLAP::ValueArg<float> maxYArg("d", "maxy", "Maxmimal y coordinate of viewport", true, 0.0f, "float");

	TCLAP::ValueArg<unsigned int> iterationsArg("i", "maxIt", "Maxmimal number of iterations", true, 0, "int");

	TCLAP::ValueArg<unsigned int> runsArg("r", "runs", "Number of runs", false, 1, "int");

	TCLAP::SwitchArg parallelArg("s", "single", "If specified, the generato will be run in single thread mode", cmd, true);

	cmd.add(wArg);
	cmd.add(hArg);

	cmd.add(minXArg);
	cmd.add(minYArg);
	cmd.add(maxXArg);
	cmd.add(maxYArg);

	cmd.add(iterationsArg);

	cmd.add(runsArg);

	cmd.parse(argc, argv);

	Coordinates<unsigned int> p = {
		wArg.getValue(),
		hArg.getValue()
	};

	Rect<float> r = {
		{minXArg.getValue(), minYArg.getValue()},
		{maxXArg.getValue(), maxYArg.getValue()},
	};

	return std::make_tuple(p, r, iterationsArg.getValue(), runsArg.getValue(), parallelArg.getValue());
}