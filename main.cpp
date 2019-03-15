#include <tuple>
#include <map>

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

std::tuple<float, float> normalizeToViewRectangle(float pX, float pY, float minX, float minY, float maxX, float maxY) {
	return std::make_tuple(0.2f, 0.2f);
}

float calc_pix(float pX, float pY) {
	int minX = -2;
	int minY = -1;
	int maxX = 1;
	int maxY = 1;
	auto [cX, cY] = normalizeToViewRectangle(pX, pY, minX, minY, maxX, maxY);

//calcPix(px, py) {
//	cx, cy = normalizeToViewRectangle(px, py, minx, miny, maxx, maxy)
//		zx = cx;
//	zy = cy;
//	for (n = 0; n < maxIterations; n++) {
//		float x = (zx * zx - zy * zy) + cx;
//		float y = (zy * zx + zx * zy) + cy;
//		if ((x * x + y * y) > 4) {
//			// diverge , produce nice color
//			paint pixel px, py in a color depending on n
//				return;
//		}
//		zx = x;
//		zy = y;
//	}
//	paint pixel px, py black // in the set
//}
	return 0.0f;
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
		ColorTable ct(iterations);

		using namespace nana;

		form fm{ API::make_center(pictureCoordinates.X, pictureCoordinates.Y) };
		drawing dw(fm);

		dw.draw([&ct, iterations](paint::graphics & graph) {
			auto posX = 10;
			auto posY = 10;
			for (auto [pos, c] : ct)
			{
				graph.rectangle(rectangle{ posX, posY, 10, 10 }, true, c);
				posX += 10;
				posY += 10;
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