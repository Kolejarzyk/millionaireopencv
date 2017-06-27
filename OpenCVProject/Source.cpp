#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include<iostream>
//#include<conio.h>           // may have to modify this line if not using Windows


using namespace cv;
using namespace std;

void faceDetection(Mat &frame, string nameFrame, vector<Rect> &faces);
void colorDetection(Mat &frame, string nameFrame, int low_r, int low_g, int low_b, int high_r, int high_g, int high_b, VideoCapture &capWebcam);
void firstQuestionFrame(Mat &frame);
void ocenaFrame();
void exitOrForwardFrame(Mat &frame, VideoCapture &capWebcam);

CascadeClassifier face_cascade;
String face_cascade_name = "haarcascade_frontalface_alt.xml";
///////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
	VideoCapture capWebcam(0);            // kamera

	if (capWebcam.isOpened() == false) {                                // czy kamera jest 
		cout << "error: capWebcam not accessed successfully\n\n";     
		//_getch();                                                          
		return(0);                                                          
	}

	int low_r = 100, low_g = 150, low_b = 0;
	int high_r = 140, high_g = 255, high_b = 255;
	vector<Rect> faces;
	Mat imgOriginal;        // input image
	Mat imgGrayscale;       // grayscale of input image
	Mat imgBlurred;         // intermediate blured image
	Mat imgCanny; 
	Mat frame1;          // Canny edge image
	double count = capWebcam.get(CV_CAP_PROP_FRAME_COUNT);
	capWebcam.set(CV_CAP_PROP_POS_FRAMES, count - 1);
	capWebcam.set(CAP_PROP_FRAME_WIDTH, 640);  // ustawienie rozmiaru wy�wietlanego obrazu
	capWebcam.set(CAP_PROP_FRAME_HEIGHT, 480);
    bool trues = true;
	char charCheckForEscKey = 0;

	if (!face_cascade.load(face_cascade_name)) { printf("--(!)Error loading face cascade\n"); return -1; };

	while (trues) {            // wy��czenie programu na esc
		bool blnFrameReadSuccessfully = capWebcam.read(imgOriginal);            

		if (!blnFrameReadSuccessfully || imgOriginal.empty()) {                
			std::cout << "error: frame not read from webcam\n";                
			break;                                                              
		}


		cvtColor(imgOriginal, imgGrayscale, CV_BGR2GRAY);                   // convert to grayscale
		namedWindow("imgOriginal", CV_WINDOW_NORMAL);       // note: you can use CV_WINDOW_NORMAL which allows resizing the window
		namedWindow("imgCanny", CV_WINDOW_NORMAL);

		GaussianBlur(imgGrayscale,              // input image
			imgBlurred,                // output image
			Size(5, 5),            // smoothing window width and height in pixels
			1.8);                      // sigma value, determines how much the image will be blurred

		Canny(imgBlurred,                       // input image
			imgCanny,                         // output image
			50,                               // low threshold
			100);                             // high threshold

		
		
		

		firstQuestionFrame(imgOriginal);
	    faceDetection(imgOriginal, "imgOriginal",faces);
		
		if (faces.size() == 0)
		{
			putText(imgOriginal, "Nie ma zawodnika", Point(50, 280), CV_FONT_HERSHEY_COMPLEX, 0.4,
				Scalar(255, 255, 255), 1, 8);
		}									
		imshow("imgOriginal", imgOriginal);
		imshow("imgCanny", imgCanny);
		Mat color;
		Mat redMask;
	
		cvtColor(imgOriginal, color, COLOR_BGR2HSV);
		
		inRange(color, Scalar(low_r, low_g, low_b), Scalar(high_r, high_g, high_b), redMask);
        imshow("Object Detecion", redMask);



				Vec3b hsv1 = color.at<Vec3b>(Point(100, 380)); // A
				int H1 = hsv1.val[0];
				int S1 = hsv1.val[1];
				int V1 = hsv1.val[2];


				if ((H1 >= low_r && H1 <= high_r) && (S1 >= low_g && S1 <= high_g) && (V1 >= low_b && V1 <= high_b))
				{
					waitKey(30);
					rectangle(imgOriginal,     // Po najechaniu koloru na ten punkt , zmienia si� 
						Point(320, 320),
						Point(640, 400),
						Scalar(255, 0, 0),
						-1,
						8);
				}

				Vec3b hsv2 = color.at<Vec3b>(Point(100, 450)); // B
				int H2 = hsv2.val[0];
				int S2 = hsv2.val[1];
				int V2 = hsv2.val[2];


				if ((H2 >= low_r && H2 <= high_r) && (S2 >= low_g && S2 <= high_g) && (V2 >= low_b && V2 <= high_b))
				{
					waitKey(30);
					rectangle(imgOriginal,     // Po najechaniu koloru na ten punkt , zmienia si� 
						Point(0, 320),
						Point(640, 400),
						Scalar(255, 0, 0),
						-1,
						8);
				}


				Vec3b hsv3 = color.at<Vec3b>(Point(350, 380)); // C
				int H3 = hsv3.val[0];
				int S3 = hsv3.val[1];
				int V3 = hsv3.val[2];


				if ((H3 >= low_r && H3 <= high_r) && (S3 >= low_g && S3 <= high_g) && (V3 >= low_b && V3 <= high_b))
				{			
					destroyWindow("imgOriginal");
					trues = false;
				    system("firefox http://edux.pjwstk.edu.pl");
				}


				Vec3b hsv4 = color.at<Vec3b>(Point(350, 450)); // D
				int H4 = hsv4.val[0];
				int S4 = hsv4.val[1];
				int V4 = hsv4.val[2];


				if ((H4 >= low_r && H4 <= high_r) && (S4 >= low_g && S4 <= high_g) && (V4 >= low_b && V4 <= high_b))
				{
					waitKey(30);
					rectangle(imgOriginal,     // Po najechaniu koloru na ten punkt , zmienia si� 
						Point(320, 320),
						Point(640, 400),
						Scalar(255, 0, 0),
						-1,
						8);
				}
				  // declare windows	
		// or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image											
													// CV_WINDOW_AUTOSIZE is the default
		
	
		
		if (waitKey(30) == 27)
		break;		
		
	}   // end while
	cout << "asdf\n"; 
	trues = true; 
	VideoCapture capWebcam1(1);            // kamera

	double count1 = capWebcam1.get(CV_CAP_PROP_FRAME_COUNT);
	capWebcam1.set(CV_CAP_PROP_POS_FRAMES, count - 1);
	capWebcam1.set(CAP_PROP_FRAME_WIDTH, 640);  // ustawienie rozmiaru wy�wietlanego obrazu
	capWebcam1.set(CAP_PROP_FRAME_HEIGHT, 480);
	if (capWebcam1.isOpened() == false) {                                // czy kamera jest 
		cout << "error: capWebcam not accessed successfully\n\n";     
		//_getch();                                                          
		return(0);                                                          
	}
    cout << "asdf\n"; 
	
    while (trues)
	{ 
				
		//Mat frame1;1
		bool success = capWebcam1.read(frame1);
		if (!success) {
			cout << "Cannot read  frame " << endl;
			break;
		}
          namedWindow("OcenaFrame", CV_WINDOW_AUTOSIZE);
cout << "asdf\n"; 

		rectangle(frame1,
			Point(320, 400),
			Point(640, 480),                   //  D
			Scalar(0, 255, 255),
			2,
			8);

		rectangle(frame1,
			Point(0, 480),
			Point(320, 400),                 //  C
			Scalar(0, 255, 255),
			2,
			8);

		rectangle(frame1,
			Point(0, 320),
			Point(640, 400),                  // B
			Scalar(0, 255, 255),
			2,
			8);

		rectangle(frame1,
			Point(320, 320),
			Point(640, 400),           // A
			Scalar(0, 255, 255),
			2,
			8);

		rectangle(frame1,
			Point(0, 220),
			Point(640, 320),           // Pytanie
			Scalar(0, 255, 255),
			2,
			8);

		putText(frame1, "Jaka ocena zdecydowanie ci sie nalezy?", Point(200, 280), CV_FONT_HERSHEY_COMPLEX, 0.4,
			Scalar(255, 255, 255), 1, 8);
		putText(frame1, "A. Na pewno nie 2 :P ", Point(100, 380), CV_FONT_HERSHEY_COMPLEX, 0.4,
			Scalar(255, 255, 255), 1, 8);
		putText(frame1, "B. 3 bedzie odpowiednie ", Point(100, 450), CV_FONT_HERSHEY_COMPLEX, 0.4,
			Scalar(255, 255, 255), 1, 8);

		putText(frame1, "C. 4 to chyba za duzo", Point(350, 380), CV_FONT_HERSHEY_COMPLEX, 0.4,
			Scalar(255, 255, 255), 1, 8);

		putText(frame1, "D. 5 to chyba dla ambitnych jest", Point(350, 450), CV_FONT_HERSHEY_COMPLEX, 0.4,
			Scalar(255, 255, 255), 1, 8);
        imshow("OcenaFrame", frame1);
		Mat color;
		Mat redMask;
		
		cvtColor(frame1, color, COLOR_BGR2HSV);
		inRange(color, Scalar(low_r, low_g, low_b), Scalar(high_r, high_g, high_b), redMask);

cout << "asdf\n"; 

		//for (int i = 0; i < color.rows; i++)
		//{
		//	for (int j = 0; j < color.cols; j++)
		//	{


				Vec3b hsv1 = color.at<Vec3b>(Point(100, 380)); // A
				int H1 = hsv1.val[0];
				int S1 = hsv1.val[1];
				int V1 = hsv1.val[2];


				if ((H1 >= low_r && H1 <= high_r) && (S1 >= low_g && S1 <= high_g) && (V1 >= low_b && V1 <= high_b))
				{

					rectangle(frame1,     // Po najechaniu koloru na ten punkt , zmienia si� 
						Point(320, 320),
						Point(640, 400),
						Scalar(255, 0, 0),
						-1,
						8);
				}

				Vec3b hsv2 = color.at<Vec3b>(Point(100, 450)); // B
				int H2 = hsv2.val[0];
				int S2 = hsv2.val[1];
				int V2 = hsv2.val[2];


				if ((H2 >= low_r && H2 <= high_r) && (S2 >= low_g && S2 <= high_g) && (V2 >= low_b && V2 <= high_b))
				{
					destroyWindow("OcenaFrame");
					exitOrForwardFrame(frame1, capWebcam1);
					break;
				}


				Vec3b hsv3 = color.at<Vec3b>(Point(350, 380)); // C
				int H3 = hsv3.val[0];
				int S3 = hsv3.val[1];
				int V3 = hsv3.val[2];


				if ((H3 >= low_r && H3 <= high_r) && (S3 >= low_g && S3 <= high_g) && (V3 >= low_b && V3 <= high_b))
				{

					rectangle(frame1,     // Po najechaniu koloru na ten punkt , zmienia si� 
						Point(0, 480),
						Point(320, 400),
						Scalar(255, 0, 0),
						-1,
						8);
				}


				Vec3b hsv4 = color.at<Vec3b>(Point(350, 450)); // D
				int H4 = hsv4.val[0];
				int S4 = hsv4.val[1];
				int V4 = hsv4.val[2];


				if ((H4 >= low_r && H4 <= high_r) && (S4 >= low_g && S4 <= high_g) && (V4 >= low_b && V4 <= high_b))
				{

					rectangle(frame1,     // Po najechaniu koloru na ten punkt , zmienia si� 
						Point(320, 320),
						Point(640, 400),
						Scalar(255, 0, 0),
						-1,
						8);



				}
	//		}
	//	}

		
	  if (waitKey(30) == 27) 
	  {cout << "przerwa"; 
	  break;
	  }	
	  
}

while (true)
	{

		double count = capWebcam.get(CV_CAP_PROP_FRAME_COUNT);
		capWebcam.set(CV_CAP_PROP_POS_FRAMES, count - 2);
		namedWindow("ExitFrame", CV_WINDOW_AUTOSIZE);
		Mat frame2;
		bool success = capWebcam.read(frame2);
		if (!success) {
			cout << "Cannot read  frame " << endl;
			break;
		}

		putText(frame2, "Zaliczyles juz przedmiot?", Point(200, 280), CV_FONT_HERSHEY_COMPLEX, 0.4,
			Scalar(255, 255, 255), 1, 8);
		putText(frame2, "A. Tak Dziekuje :D ", Point(100, 380), CV_FONT_HERSHEY_COMPLEX, 0.4,
			Scalar(255, 255, 255), 1, 8);
		putText(frame2, "B. Tutaj nie ma innej odopwiedzi tak zda�e� ", Point(100, 450), CV_FONT_HERSHEY_COMPLEX, 0.4,
			Scalar(255, 255, 255), 1, 8);

		rectangle(frame2,
			Point(0, 320),
			Point(640, 400),                  // B
			Scalar(0, 255, 255),
			2,
			8);

		rectangle(frame2,
			Point(320, 320),
			Point(640, 400),           // A
			Scalar(0, 255, 255),
			2,
			8);

		rectangle(frame2,
			Point(0, 220),
			Point(640, 320),           // Pytanie
			Scalar(0, 255, 255),
			2,
			8);
		Mat color;
		Mat redMask;
		int low_r = 100, low_g = 150, low_b = 0;
		int high_r = 140, high_g = 255, high_b = 255;

		cvtColor(frame2, color, COLOR_BGR2HSV);
		inRange(color, Scalar(low_r, low_g, low_b), Scalar(high_r, high_g, high_b), redMask);



				Vec3b hsv1 = color.at<Vec3b>(Point(100, 380)); // A
				int H1 = hsv1.val[0];
				int S1 = hsv1.val[1];
				int V1 = hsv1.val[2];


				if ((H1 >= low_r && H1 <= high_r) && (S1 >= low_g && S1 <= high_g) && (V1 >= low_b && V1 <= high_b))
				{

				   system("firefox http://edux.pjwstk.edu.pl");
                   destroyWindow("ExitFrame");
				   break;
				}

				Vec3b hsv2 = color.at<Vec3b>(Point(100, 450)); // B
				int H2 = hsv2.val[0];
				int S2 = hsv2.val[1];
				int V2 = hsv2.val[2];


				if ((H2 >= low_r && H2 <= high_r) && (S2 >= low_g && S2 <= high_g) && (V2 >= low_b && V2 <= high_b))
				{
                    system("firefox http://edux.pjwstk.edu.pl");
					
                    destroyWindow("ExitFrame");
					break;
				}

			//}
		//}
		imshow("ExitFrame", frame2);
		if (waitKey(30) == 0) break;

	}
cout << "pawel\n"; 

return (0);
}

void faceDetection(Mat &frame,string nameFrame, vector<Rect> &faces)
{
	
	
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

void ocenaFrame()
{ 
	VideoCapture capWebcam1(0);            // kamera

	if (capWebcam1.isOpened() == false) {                                // czy kamera jest 
		cout << "error: capWebcam not accessed successfully\n\n";     
		//_getch();                                                          
		//return(0);                                                          
	}

	

		
	

}

void firstQuestionFrame(Mat &frame)
{
	
	
		rectangle(frame,
			Point(320, 400),
			Point(640, 480),                   //  D
			Scalar(0, 255, 255),
			2,
			8);

		rectangle(frame,
			Point(0, 480),
			Point(320, 400),                 //  C
			Scalar(0, 255, 255),
			2,
			8);

		rectangle(frame,
			Point(0, 320),
			Point(640, 400),                  // B
			Scalar(0, 255, 255),
			2,
			8);

		rectangle(frame,
			Point(320, 320),
			Point(640, 400),           // A
			Scalar(0, 255, 255),
			2,
			8);

		rectangle(frame,
			Point(0, 220),
			Point(640, 320),           // Pytanie
			Scalar(0, 255, 255),
			2,
			8);

		putText(frame, "Czy jest mozliwosc zdania z NAI?", Point(200, 280), CV_FONT_HERSHEY_COMPLEX, 0.4,
			Scalar(255, 255, 255), 1, 8);
		putText(frame, "A. Ewidetnie nie!!!",Point(100, 380), CV_FONT_HERSHEY_COMPLEX, 0.4,
			Scalar(255, 255, 255), 1, 8);
		putText(frame, "B. Raczej nie!!!",Point(100, 450), CV_FONT_HERSHEY_COMPLEX, 0.4,
			Scalar(255, 255, 255), 1, 8);

		putText(frame, "C. Oczywiscie!!!",Point(350, 380), CV_FONT_HERSHEY_COMPLEX, 0.4,
			Scalar(255, 255, 255), 1, 8);

		putText(frame, "D. Nie wiem!!!",Point(350, 450), CV_FONT_HERSHEY_COMPLEX, 0.4,
			Scalar(255, 255, 255), 1, 8);

}
