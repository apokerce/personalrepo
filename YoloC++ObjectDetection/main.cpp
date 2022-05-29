#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <cstring>
#include <opencv2/dnn.hpp>
#include "AhmetPokerce.h"

//main.exe -- camera {device number 0/1/2} --label {person/car/airplane/etc.} – save_image

int main(int argc, char* kwargs[])
{
    Queue frames(1000);
    Queue FramesToSave(1000);
    int Saveimage_index = 0;
    // Options are created to check the input format
    const char* Option1 = "c"; const char* Option2 = "v";
    bool saveCond = CheckSaveRequest(kwargs);
    // Arrange Labels and Network
    cv::dnn::Net net = ArrangeNetwork();
    std::vector<std::string> classname = FetchClassNames();

    if(kwargs[1][2] == Option1[0])
    {
        cv::VideoCapture cap;
        // atoi converts strings to integer
        cap.open(atoi(kwargs[2]), cv::CAP_ANY);
        // Create output video writer
        cv::VideoWriter video("outcpp.avi", cv::VideoWriter::fourcc('M','J','P','G'), 5, cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)));

        while(true)
        {   
            
            std::thread thread1(VideoCapture,std::ref(frames), cap);
            if(thread1.joinable())
                thread1.detach();
            std::thread thread2(ObjectDetection, std::ref(frames), std::ref(FramesToSave), classname, net, kwargs[4]);
            if(thread2.joinable())
                thread2.join();
            // if condition fails video will not be recorded.
            if(!FramesToSave.QEmpty())
                cv::imshow("image", FramesToSave.QFront());
            // Press escape to exit, press 0/1/2 to switch to other camera
            int Key = cv::waitKey(1);
            if(Key == 27)
                break;
            else if(Key == 48)
                cap.open((Key-48), cv::CAP_ANY);
            else if(Key == 49)
                cap.open((Key-48), cv::CAP_ANY);
            else if(Key == 50)
                cap.open((Key-48), cv::CAP_ANY);
            // press lower case "a" to save the image
            if(Key == 97 && !FramesToSave.QEmpty())
            {
                std::string name = "image" + std::to_string(Saveimage_index++) + ".jpg";
                cv::imwrite(name, FramesToSave.QFront());
            }
            if(saveCond == true)
            {
                std::thread thread3(VideoWrite, std::ref(FramesToSave), video);
                if(thread3.joinable())
                    thread3.detach();
            }
            else
                cv::Mat Empty = FramesToSave.Qdelete();
        }
        
    }
    else if(kwargs[1][2] == Option2[0])
    {
        cv::VideoCapture cap;
        std::string input(kwargs[2]);
        cap.open(input, cv::CAP_ANY);
        cv::VideoWriter video("outcpp.avi", cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)));

	  
        while(true)
        {   
            std::thread thread1(VideoCapture,std::ref(frames), cap);
            thread1.join();
            std::thread thread2(ObjectDetection, std::ref(frames), std::ref(FramesToSave), classname, net, kwargs[4]);
            thread2.join();
            // if condition fails video will not be recorded.
            if(!FramesToSave.QEmpty())
                cv::imshow("image", FramesToSave.QFront());
            // Press escape to exit, press 0/1/2 to switch to other camera
            int Key = cv::waitKey(1);
            if(Key == 27)
                break;
            // press lower case "a" to save the image
            if(Key == 97 && !FramesToSave.QEmpty())
            {
                std::string name = "image" + std::to_string(Saveimage_index++) + ".jpg";
                cv::imwrite(name, FramesToSave.QFront());
            }
            if(saveCond == true)
            {
                std::thread thread3(VideoWrite, std::ref(FramesToSave), video);
                thread3.join();
            }
            else
                cv::Mat Empty = FramesToSave.Qdelete();
        }

    }
    
    return 0;
}