#include "Sample.h"

Sample::Sample(image_rgb* image) :image(image), name("unknown.png"){
	ProcessImage();
};

Sample::Sample(image_rgb* image, string name) : image(image), name(name) {
	ProcessImage();
}

Sample::~Sample() {
	if (image) {
		delete image;
	}
}

// Process the image and calculates
// Pink for Gusts
// Black for WindSpeed
// Gray for direction
// DirectionChange comparing two following directions for changes.
void Sample::ProcessImage() {

	int previousDirection = 0;
	int imageHeight = (int)image->get_height();

	for (png::uint_32 w = 0; w < image->get_width(); w++) {

		// Analyze Image per Column

		png::uint_32 maxHPink = 0;
		int maxPink = 0;

		png::uint_32 minHBlack = 0;
		int minBlack = 255 * 3;

		png::uint_32 hGrey = -1;

		for (png::uint_32 h = 0; h < imageHeight; h++) {
			png::rgb_pixel pixel = image->get_pixel(w, h);
			int pinkValue = pixel.red - pixel.green;
			if (maxPink < pinkValue) {
				maxPink = pinkValue;
				maxHPink = h;
			}

			int blackValue = pixel.red + pixel.green + pixel.blue;
			if (minBlack > blackValue) {
				minBlack = blackValue;
				minHBlack = h;
			}

			if (pixel.red == 131 && pixel.green == 130 && pixel.blue == 130) {
				if (hGrey != -1) {
					cerr << "Found multiple Direction Pixels. You have to implement previous checks" << endl;
				}
				hGrey = h;
			}
		}
		// wind gust (red):
		int windGust = imageHeight - maxHPink;
		double normalizedWindGust = (double)windGust / (double)imageHeight;
		gusts.push_back(normalizedWindGust);

		// wind speed (black):
		int windSpeed = imageHeight - minHBlack;
		double normalizedWindSpeed = (double)windSpeed / (double)imageHeight;
		windSpeeds.push_back(normalizedWindSpeed);

		// wind direction (gray):
		int direction = previousDirection;
		if (hGrey == -1) {
			// cerr << "No direction found! Take Previous. " << name << endl;
		} else {
			direction = imageHeight - hGrey;
		}
		double normalizedDirection = (double)direction / (double)imageHeight;
		directions.push_back(normalizedDirection);

		// wind change (difference of two directions):
		if (w > 0) {
			// skip first row
			int delta = abs(direction - previousDirection);
			int deltaWrappedUp = abs(direction + (int)imageHeight - previousDirection);
			int deltaWrappedDown = abs(direction - (int)imageHeight - previousDirection);
			int deltaToUse = (delta < deltaWrappedUp) ? delta : deltaWrappedUp;
			if (deltaWrappedDown < deltaToUse) {
				deltaToUse = deltaWrappedDown;
			}

			double normalizedDelta = (double)deltaToUse / (double)imageHeight;
			changeDirections.push_back(normalizedDelta);			
			//cout << "delta: " << w << " -> " << deltaToUse << endl;
			//cout << "normalized: " << normalizedDelta << endl;
		}

		previousDirection = direction;
	}
}