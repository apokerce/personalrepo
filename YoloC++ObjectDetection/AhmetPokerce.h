#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "AhmetPOKERCE2.h"



class Queue
{   
    private:
        int frontt,rear,countt,max_size_q;
        cv::Mat qlist[1000];
    public:
        Queue(int m);
        void Qinsert(cv::Mat frame);
        cv::Mat Qdelete(void);
        cv::Mat QFront(void) const;
        int Qlength(void) const {return countt;}
        int QEmpty(void) const{return (countt==0);}
        int QFull(void) const {return countt==max_size_q;}
};



Queue::Queue(int m)
{   frontt = 0;
    rear = 0;
    countt = 0;
    max_size_q = m;  
}



void Queue::Qinsert(cv::Mat frame)
{   if (QFull())
    {   std::cerr<<"Queue overflow"<<std::endl;
        exit(1);
    }
    countt++;
    qlist[rear] = frame;
    rear=(rear+1)%1000;   
}



cv::Mat Queue::Qdelete(void)
{   cv::Mat temp;
    if(QEmpty())
    {
        return temp;
    }
    temp = qlist[frontt];
    countt--;
    frontt = (frontt+1)%max_size_q;
    return temp;    
}


cv::Mat Queue::QFront(void) const
{
    return qlist[frontt];
}


void VideoCapture(Queue& frame, cv::VideoCapture cap)
{
    // Capture video frames and push into queue
    cv::Mat frame1;
    cap.read(frame1);   
    frame.Qinsert(frame1);
}







void ObjectDetection(Queue& Framess, Queue& SaveFrames, std::vector<std::string> class_names, cv::dnn::Net network, std::string label)
{
    const int inputWidth = 416; const int inputHeight = 416;

    cv::Mat frame, blob;
    if(!Framess.QEmpty())
    {
        frame = Framess.Qdelete();
        cv::dnn::blobFromImage(frame, blob, 1/255.0, cv::Size(inputWidth, inputHeight), cv::Scalar(0,0,0), true, false);
        network.setInput(blob);
        std::vector<cv::Mat> outputs;
        // names defines the layer where the forwarding will end
        network.forward(outputs, getLastLayerNames(network)); 
        ProcessData_DrawBoxes(frame, outputs, class_names, label);
        // Convert frame to CV_8U format to save
        cv::Mat detectedFrame;
        frame.convertTo(detectedFrame, CV_8U);
        SaveFrames.Qinsert(detectedFrame);
    }
    else
        return;
        

}

void VideoWrite(Queue& FramesForVideo, cv::VideoWriter video)
{
    if(!FramesForVideo.QEmpty())
    {
        cv::Mat Frame_ = FramesForVideo.Qdelete();
        video.write(Frame_);
    }

}




std::vector<std::string> FetchClassNames()
{
    std::vector<std::string> classNames;
    // Read class names
    std::string classFile = "coco.names";
    std::ifstream names1(classFile.c_str());
    std::string line;
    while(std::getline(names1, line))
    {
        classNames.push_back(line);
    }
    return classNames;
    
}



cv::dnn::Net ArrangeNetwork()
{
    // Read Config file and weights
    std::string modelConfig = "yolov3.cfg";
    std::string modelWeights = "yolov3.weights";

    // Load Yolo Network
    cv::dnn::Net network = cv::dnn::readNetFromDarknet(modelConfig, modelWeights);
    network.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    return network;
}

bool CheckSaveRequest(char* kwarg[])
{   
    if(std::strcmp(kwarg[5], "true") == 0)
        return true;
    else if(std::strcmp(kwarg[5], "--save_image") == 0)
    {
        if(std::strcmp(kwarg[6], "true") == 0)
            return true;
    }
        
    else 
        return false;
}