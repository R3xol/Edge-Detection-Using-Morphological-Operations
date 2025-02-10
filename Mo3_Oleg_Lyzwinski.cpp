#include <iostream>
#include <string> 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
	const uchar MAX = 255;
	const uchar MIN = 0;

	//Wczytanie zdjęcia oraz sklonowanie
	string path = "B_1.jpg";
	Mat image = imread(path, IMREAD_GRAYSCALE);
	Mat image_binary = image.clone();

	//Definicja wymiarów obrzau oraz jego środka
	int width = image_binary.cols;
	int height = image_binary.rows;

	//Algorytm binaryzacji zdjęcia
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			
			if (image_binary.at<uchar>(h, w) > 127)
				image_binary.at<uchar>(h, w) = MAX;
			else
				image_binary.at<uchar>(h, w) = MIN;
		}
	}

	Mat image_erosion = image_binary.clone();
	Mat image_edges_erosion = image_binary.clone();
	Mat image_dilation = image_binary.clone();
	Mat image_edges_dilation = image_binary.clone();

	int count_erosion = 0, count_dilation = 0;

	//Przejście po o brazie w celu wykonania operacji morfologicznych
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {

			//Przejście po obrazie kożystając ze Structur Element Z4
			for (int H = h - 1; H <= h + 1; H++) {
				for (int W = w - 1; W <= w + 1; W++) {
					//Zabezpieczenie przed wyjściem poza krawędzie obrazu
					if (H >= 0 && W >= 0 && H < height && W < width) {
						//Sprawdzenie czy którykolwiek z pikseli SE jest biały oraz zliczenie ich
						if (image_binary.at<uchar>(H, W) == MAX)
							count_erosion++;
						//Sprawdzenie czy którykolwiek z pikseli SE jest czarny oraz zliczenie ich
						else
							count_dilation++;
					}

				}
			}
			//Definicja erozji (gdy któryś z pikseli w SE był biały - dany piksel staje się biały)
			if (count_erosion > 0) 
				image_erosion.at<uchar>(h, w) = MAX;
			//W przeciwnym razie zmieniamy na czarny
			else
				image_erosion.at<uchar>(h, w) = MIN;
			//Wyzerowanie zliczania
			count_erosion = 0;

			//Definicja dylatacji (gdy któryś z pikseli w SE był czarny - dany piksel staje się czarny)
			if (count_dilation > 0) 
				image_dilation.at<uchar>(h, w) = MIN;
			//W przeciwnym razie zmieniamy na biały
			else
				image_dilation.at<uchar>(h, w) = MAX;
			//Wyzerowanie zliczania
			count_dilation = 0;
		}
	}

	uchar diference_erosion = 0, diference_dilation = 0;

	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			//Obliczeniue różnicy między obrazem początkowym, a zerodowanym
			diference_erosion = image_erosion.at<uchar>(h, w) - image_binary.at<uchar>(h, w);
			if (diference_erosion == MAX)
				image_edges_erosion.at<uchar>(h, w) = MIN;
			else
				image_edges_erosion.at<uchar>(h, w) = MAX;
		}
	}
	
	
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			//Obliczeniue różnicy między obrazem początkowym, a po dylatacji
			diference_dilation = image_binary.at<uchar>(i, j) - image_dilation.at<uchar>(i, j);
			if (diference_dilation == MAX)
				image_edges_dilation.at<uchar>(i, j) = MIN;
			else
				image_edges_dilation.at<uchar>(i, j) = MAX;
		}
	}

	//zapis poprawionego obrazu do pliku
	imwrite("Binary_image.jpg", image_binary);
	imwrite("Edges_dilation.jpg", image_edges_dilation);
	imwrite("Edges_erosion.jpg", image_edges_erosion);

    return 0;
}


