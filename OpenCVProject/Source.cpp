#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include<iostream>
#include<conio.h>           // may have to modify this line if not using Windows


using namespace cv;
using namespace std;

void faceDetection(Mat frame, string nameFrame);
void colorDetection(Mat frame, string nameFrame);

CascadeClassifier face_cascade;
String face_cascade_name = "haarcascade_frontalface_alt.xml";
///////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
	cv::VideoCapture capWebcam(0);            // kamera

	if (capWebcam.isOpened() == false) {                                // czy kamera jest 
		std::cout << "error: capWebcam not accessed successfully\n\n";     
		_getch();                                                          
		return(0);                                                          
	}


	Mat imgOriginal;        // input image
	Mat imgGrayscale;       // grayscale of input image
	Mat imgBlurred;         // intermediate blured image
	Mat imgCanny;           // Canny edge image

	capWebcam.set(CAP_PROP_FRAME_WIDTH, 640);  // ustawienie rozmiaru wyœwietlanego obrazu
	capWebcam.set(CAP_PROP_FRAME_HEIGHT, 480);

	char charCheckForEscKey = 0;

	if (!face_cascade.load(face_cascade_name)) { printf("--(!)Error loading face cascade\n"); return -1; };

	while (true) {            // wy³¹czenie programu na esc
		bool blnFrameReadSuccessfully = capWebcam.read(imgOriginal);            

		if (!blnFrameReadSuccessfully || imgOriginal.empty()) {                
			std::cout << "error: frame not read from webcam\n";                
			break;                                                              
		}


		cvtColor(imgOriginal, imgGrayscale, CV_BGR2GRAY);                   // convert to grayscale

		GaussianBlur(imgGrayscale,              // input image
			imgBlurred,                // output image
			cv::Size(5, 5),            // smoothing window width and height in pixels
			1.8);                      // sigma value, determines how much the image will be blurred

		Canny(imgBlurred,                       // input image
			imgCanny,                         // output image
			50,                               // low threshold
			100);                             // high threshold

		 rectangle(imgOriginal,
			Point(320, 400),
			Point(640, 480),                   //  D
			Scalar(0, 255, 255),
			2,
			8);
	
		rectangle(imgOriginal,
			Point(0, 480),
			Point(320, 400),                 //  C
			Scalar(0, 255, 255),
			2,
			8);

		rectangle(imgOriginal,
			Point(0, 320),
			Point(640, 400),                  // B
			Scalar(0, 255, 255),
			2,
			8);

		rectangle(imgOriginal,
			Point(320, 320),
			Point(640, 400),           // A
			Scalar(0, 255, 255),
			2,
			8);

		rectangle(imgOriginal,
			Point(0, 220),
			Point(640, 320),           // Pytanie
			Scalar(0, 255, 255),
			2,
			8);

		putText(imgOriginal, "Czy jest mozliwosc zdania z NAI?", Point(200, 280), CV_FONT_HERSHEY_COMPLEX, 0.4,
			Scalar(255, 255, 255), 1, 8);
		putText(imgOriginal, "A. Zalosne!!!", Point(100, 380), CV_FONT_HERSHEY_COMPLEX, 0.4,
			Scalar(255, 255, 255), 1, 8);
		putText(imgOriginal, "B. Zalosne!!!", Point(100, 450), CV_FONT_HERSHEY_COMPLEX, 0.4,
			Scalar(255, 255, 255), 1, 8);

		putText(imgOriginal, "C. Zalosne!!!", Point(350,380), CV_FONT_HERSHEY_COMPLEX, 0.4,
			Scalar(255, 255, 255), 1, 8);

		putText(imgOriginal, "D. Zalosne!!!", Point(350, 450), CV_FONT_HERSHEY_COMPLEX, 0.4,
			Scalar(255, 255, 255), 1, 8);

		faceDetection(imgOriginal, "imgOriginal");
		colorDetection(imgOriginal, "imgOriginal");
	      
											  // declare windows
		namedWindow("imgOriginal", CV_WINDOW_NORMAL);       // note: you can use CV_WINDOW_NORMAL which allows resizing the window
		namedWindow("imgCanny", CV_WINDOW_NORMAL); 
		// or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
												
															
															// CV_WINDOW_AUTOSIZE is the default


			imshow("imgOriginal", imgOriginal);
			imshow("imgCanny", imgCanny);

			
			if (waitKey(1) == 27)
				break;
		
		                // show windows
		                     //

		
	}   // end while
	
	return(0);
}

void faceDetection(Mat frame,string nameFrame)
{
	vector<Rect> faces;
	Mat frame_gray;
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	//-- Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
	for (size_t i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
		Mat faceROI = frame_gray(faces[i]);
		vector<Rect> eyes;

	}
	//-- Show what you got
	imshow(nameFrame, frame);
}

void colorDetection(Mat frame,string nameFrame) // funkcja do trackingu koloru
{
	Mat color;
	Mat range;
	namedWindow("Object Detection", CV_WINDOW_NORMAL);
	cvtColor(frame, color , COLOR_BGR2HSV);
	inRange(color, Scalar(0, 100, 100), Scalar(10, 255, 255), range);
	imshow(nameFrame, frame);
	imshow("Object Detecion", range);

	
}