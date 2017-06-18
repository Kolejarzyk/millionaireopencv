#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include<iostream>
#include<conio.h>           // may have to modify this line if not using Windows


using namespace cv;
using namespace std;

void faceDetection(Mat &frame, string nameFrame);
void colorDetection(Mat &frame, string nameFrame, int low_r, int low_g, int low_b, int high_r, int high_g, int high_b);

CascadeClassifier face_cascade;
String face_cascade_name = "haarcascade_frontalface_alt.xml";
///////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
	VideoCapture capWebcam(0);            // kamera

	if (capWebcam.isOpened() == false) {                                // czy kamera jest 
		cout << "error: capWebcam not accessed successfully\n\n";     
		_getch();                                                          
		return(0);                                                          
	}

	int low_r = 0, low_g = 100, low_b = 100;
	int high_r = 10, high_g = 255, high_b = 255;

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
		putText(imgOriginal, "A. Oczywiœcie!!!", Point(100, 380), CV_FONT_HERSHEY_COMPLEX, 0.4,
			Scalar(255, 255, 255), 1, 8);
		putText(imgOriginal, "B. Raczej nie!!!", Point(100, 450), CV_FONT_HERSHEY_COMPLEX, 0.4,
			Scalar(255, 255, 255), 1, 8);

		putText(imgOriginal, "C. Ewidetnie nie!!!", Point(350,380), CV_FONT_HERSHEY_COMPLEX, 0.4,
			Scalar(255, 255, 255), 1, 8);

		putText(imgOriginal, "D. Raczej nie!!!", Point(350, 450), CV_FONT_HERSHEY_COMPLEX, 0.4,
			Scalar(255, 255, 255), 1, 8);

		faceDetection(imgOriginal, "imgOriginal");
		colorDetection(imgOriginal, "imgOriginal",low_r,low_g,low_b,high_r,high_g,high_b);
	      
											  // declare windows
		namedWindow("imgOriginal", CV_WINDOW_NORMAL);       // note: you can use CV_WINDOW_NORMAL which allows resizing the window
		namedWindow("imgCanny", CV_WINDOW_NORMAL); 
		// or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
												
															
															// CV_WINDOW_AUTOSIZE is the default

		imshow("imgOriginal", imgOriginal);
		imshow("imgCanny", imgCanny);

			
		if (waitKey(1) == 27)
		break;		
		
	}   // end while
	while (true)
	{
		double count = capWebcam.get(CV_CAP_PROP_FRAME_COUNT); 
		capWebcam.set(CV_CAP_PROP_POS_FRAMES, count - 1); 
		namedWindow("MyVideo", CV_WINDOW_AUTOSIZE);


		Mat frame;
		bool success = capWebcam.read(frame);
		if (!success) {
			cout << "Cannot read  frame " << endl;
			break;
		}	

		imshow("MyVideo", frame);
		if (waitKey(0) == 27) break;
	}	

	
	return(0);
}

void faceDetection(Mat &frame,string nameFrame)
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
	}
	//-- Show what you got
	imshow(nameFrame, frame);
}

void colorDetection(Mat &frame,string nameFrame,int low_r,int low_g, int low_b, int high_r ,int high_g,int high_b) // funkcja do trackingu koloru
{

	

	Mat color;
	Mat redMask;
	namedWindow("Object Detection", CV_WINDOW_NORMAL);
	cvtColor(frame, color , COLOR_BGR2HSV);
	inRange(color, Scalar(low_r, low_g, low_b), Scalar(high_r, high_g, high_b), redMask);
	
	
	
	
		for (int i = 0; i < frame.rows; i++)
		{
			for (int j = 0; j < frame.cols; j++)
			{
				Vec3b colour = redMask.at<Vec3b>(Point(100, 380));
				if ((colour.val[0] >= low_r  && colour.val[0] <= high_r)  && (colour.val[1] >= low_g && colour.val[1] <= high_g) && (colour.val[2] >= low_b && colour.val[2] <= high_b))
				{

					rectangle(frame,     // Po najechaniu koloru na ten punkt , zmienia siê 
						Point(320, 320),
						Point(640, 400),
						Scalar(255, 255, 255),
						-1,
						8);
					break;
					updateWindow("imgOriginal");
				}
			}
		
	}

	imshow("Object Detecion", redMask);
}


