#include <tuple>
#include <map>
#include <memory>
#include <chrono>
#include <iostream>
#include <numeric>

#include <spdlog/spdlog.h>

#include "parseCmdLine.h"
#include "rectangle.h"
#include "coordinates.h"
#include "colortable.h"
#include "mandelbrotGenerator.h"

int main(int argc, char* argv[]) {
	try
	{
		spdlog::info("Welcome to parallel mandelbrot!");
		auto [pictureCoordinates, viewport, iterations, nrOfRuns] = ParseCmdLine(argc, argv);

		spdlog::info("Picture size: width = {}, height = {}", pictureCoordinates.X, pictureCoordinates.Y);
		spdlog::info("Viewport: minX = {}, minY = {}, maxX = {}, maxY = {}", viewport.first.X, viewport.first.Y, viewport.second.X, viewport.second.Y);
		spdlog::info("Maximal number of iterations: {}", iterations);

		spdlog::info("Generator will be run {} times", nrOfRuns);

		std::vector<long long> runtimes;
		spdlog::info("Starting calculation...");
		std::unique_ptr<sf::VertexArray> vertexArray;

		for (size_t r = 1; r <= nrOfRuns; r++)
		{
			auto startTime = std::chrono::high_resolution_clock::now();

			vertexArray = GenerateMandelbrotSet(pictureCoordinates, viewport, iterations);

			auto endTime = std::chrono::high_resolution_clock::now();
			auto totalRuntime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
			runtimes.push_back(totalRuntime);

			spdlog::info("Run {} took {} milliseconds.", r, totalRuntime);
		}

		spdlog::info("");
		auto meanRuntime = std::accumulate(runtimes.begin(), runtimes.end(), 0) / static_cast<float>(nrOfRuns);
		spdlog::info("Calculation complete, mean runtime over {} runs was {} milliseconds", nrOfRuns, meanRuntime);

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
			window.draw(*vertexArray);

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