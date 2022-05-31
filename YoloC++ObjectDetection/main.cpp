#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <cstring>
#include <opencv2/dnn.hpp>
#include "AhmetPokerce.h"

// getLastLayerNames method is required since we need to tell the network until the which layer the forwarding the input will be
std::vector<std::string> getLastLayerNames(const cv::dnn::Net& net);

// This function draws the bounding boxes, takes the data coming from the network and process it
void ProcessData_DrawBoxes(cv::Mat& Frame, const std::vector<cv::Mat>&outputss, std::vector<std::string> classes, std::string label);

// Read from camera is made in a function so that it can be send to a thread for multiprocessing
void VideoCapture(Queue& frame, cv::VideoCapture cap);

// Forwarding the input images into network is made in the ObjectDetection function and taken in another thread
void ObjectDetection(Queue& Framess, Queue& SaveFrames, std::vector<std::string> class_names, cv::dnn::Net network, std::string label);

// In VideoWrite function frames are taken from a queue which is used to store the frames that will be used in video
void VideoWrite(Queue& FramesForVideo, cv::VideoWriter video);

// Class names that are available in Yolo is fetch using below function
std::vector<std::string> FetchClassNames();

// Network arrangement like reading the config files and last layer information is made in below function
cv::dnn::Net ArrangeNetwork();

// The information about whether the video will be saved is checked in below function
bool CheckSaveRequest(char* kwarg[]);


int main(int argc, char* kwargs[])
{
    /* frames queue is created to store the image frames that are taken from camera or video 
       FramesToSave queue is used for storing the frames that will be used in video since I 
       pick the images in object detection from frames queue VideoWrite function may take wrong frame */ 
    Queue frames(1000);
    Queue FramesToSave(1000);

    int Saveimage_index = 0; // Variable to differentiate the images that will be saved during streaming the video
    const char* Option1 = "c"; const char* Option2 = "v"; // Option1 and Option2 is used to understand whether the streaming will be made from video or camera
    
    bool saveCond = CheckSaveRequest(kwargs); // saveCond controls the streaming video will be saved or not
    
    // Network arrangment and fetching available class names made here since both will be used in both camera and video input
    cv::dnn::Net net = ArrangeNetwork();
    std::vector<std::string> classname = FetchClassNames();

    // User input is checked if input is chosen to be camera process is starting else if video else part starts
    if(kwargs[1][2] == Option1[0])
    {
        // Taken frames from camera is started using cap.open
        cv::VideoCapture cap;
        cap.open(atoi(kwargs[2]), cv::CAP_ANY);
        // Create output video writer
        cv::VideoWriter video("outcpp.avi", cv::VideoWriter::fourcc('M','J','P','G'), 5, cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)));

        while(true)
        {   
            // a thread is created for video capturing from camera std::ref was required in my compiler version 
            std::thread thread1(VideoCapture,std::ref(frames), cap);
            thread1.detach();

            // a thread is created for objectdetection phase
            std::thread thread2(ObjectDetection, std::ref(frames), std::ref(FramesToSave), classname, net, kwargs[4]);
            thread2.join();

            // This junction of code is used to show the user real time flow of the video
            if(!FramesToSave.QEmpty())
                cv::imshow("image", FramesToSave.QFront());

            // This part controls whether user wants to stop the process, change camera or take screenshot
            int Key = cv::waitKey(1);  // Controls if any key is pressed
            if(Key == 27)   // Press ESC for stop
                break;
            else if(Key == 48)   // Press 0 for internal camera
                cap.open((Key-48), cv::CAP_ANY);
            else if(Key == 49)   // Press 1 for external camera
                cap.open((Key-48), cv::CAP_ANY);
            else if(Key == 50)   // Press 2 for another external camera
                cap.open((Key-48), cv::CAP_ANY);
            if(Key == 97 && !FramesToSave.QEmpty()) // Press lower case "a" to save the image
            {
                // Image name is changing with number not to overlap with previously saved image
                std::string name = "image" + std::to_string(Saveimage_index++) + ".jpg";
                cv::imwrite(name, FramesToSave.QFront());
            }

            if(saveCond == true)
            {
                /* VideoWrite function deletes the frames that are used for saving video if condition fails else part is there to
                   not to take screenshot of the same image since above part only picks the frame in the queue */
                std::thread thread3(VideoWrite, std::ref(FramesToSave), video);
                thread3.detach();
            }
            else
                cv::Mat Empty = FramesToSave.Qdelete();
        }
        
    }
    else if(kwargs[1][2] == Option2[0]) 
    {
        // video name is converted to string since cap.open takes string argument fails when kwargs directly given
        cv::VideoCapture cap;
        std::string input(kwargs[2]);
        cap.open(input, cv::CAP_ANY);

        // Create output video writer
        cv::VideoWriter video("outcpp.avi", cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)));

	  
        while(true)
        {   
            // a thread is created for video capturing from camera std::ref was required in my compiler version 
            std::thread thread1(VideoCapture,std::ref(frames), cap);
            thread1.join();

            // a thread is created for objectdetection phase
            std::thread thread2(ObjectDetection, std::ref(frames), std::ref(FramesToSave), classname, net, kwargs[4]);
            thread2.join();

            // This junction of code is used to show the user real time flow of the video
            if(!FramesToSave.QEmpty())
                cv::imshow("image", FramesToSave.QFront());

            // This part controls whether user wants to stop the process or take screenshot
            int Key = cv::waitKey(1);
            if(Key == 27)     // Press ESC for stop
                break;
            if(Key == 97 && !FramesToSave.QEmpty())  // Press lower case "a" to save the image
            {
                std::string name = "image" + std::to_string(Saveimage_index++) + ".jpg";
                cv::imwrite(name, FramesToSave.QFront());
            }

            if(saveCond == true)
            {
                /* Same purpose as in the camera streaming */
                std::thread thread3(VideoWrite, std::ref(FramesToSave), video);
                thread3.join();
            }
            else
                cv::Mat Empty = FramesToSave.Qdelete();
        }

    }
    return 0;
}


std::vector<std::string> getLastLayerNames(const cv::dnn::Net& net)
{
    /* The forwarding process needs to know the layer where the process ends this function takes
       the last layer names and store in a string which is given to forwarding */

    std::vector<std::string> names_temp;
    std::vector<int> outLayers = net.getUnconnectedOutLayers(); 
    std::vector<std::string> layersNames = net.getLayerNames();
    names_temp.resize(outLayers.size());
    for (int i = 0; i < outLayers.size(); ++i)
        names_temp[i] = layersNames[outLayers[i] - 1];
    return names_temp;
}


void ProcessData_DrawBoxes(cv::Mat& Frame, const std::vector<cv::Mat>&outputss, std::vector<std::string> classes, std::string label)
{
    // This function take outputs that are given by the network and draw the bounding boxes of interests which determined by the user 
       

    const float nonMaxSuppressionThresh = 0.4; // Confidence below this value will be deleted directly in the NMS part
    const float confThresh = 0.5;              // Confidence score below this constant will now be shown
    std::vector<int> Ids;                      // Ids vector is used to store the detected class ID's
    std::vector<float> Confidence;             // This vector is storing the confidence scores that will be shown to user
    std::vector<cv::Rect> BoundingBoxes;       // Stores the bounding box coordinates

    /* In the loop below all the predictions are checked and the predictions' information that are exceeding
       the predetermined confidence threshold are saved in the vectors that are defined above */
    for(int i=0;i<outputss.size(); i++)
    {
        float* data = (float*)outputss[i].data;
        for(int j=0;j<outputss[i].rows;j++)
        {
            cv::Mat scores = outputss[i].row(j).colRange(5, outputss[i].cols);
            cv::Point ClassIds;
            double confidence;
            // This function returns the location of maximum score that is predicted by the last layer
            cv::minMaxLoc(scores, 0, &confidence, 0, &ClassIds);
            if(confidence > confThresh)
            {
                // Store coordinates of bounding boxes and arrange according to input size
                int width = (int)(data[2]*Frame.cols);
                int height = (int)(data[3]*Frame.rows);
                int left = (int)(data[0]*Frame.cols) - (width / 2);
                int top = (int)(data[1]*Frame.rows) - (height / 2);
                Ids.push_back(ClassIds.x);
                Confidence.push_back((float)confidence);
                BoundingBoxes.push_back(cv::Rect(left, top, width, height));
            }
            data += outputss[i].cols;
        }
    }
    
    // Non max supression is used to get rid of the overlaying bounding boxes that are belong to the same object
    // indexes each corresponding an object is returned by the function
    std::vector<int> indexes;
    cv::dnn::NMSBoxes(BoundingBoxes, Confidence, confThresh, nonMaxSuppressionThresh, indexes);

    /* In this loop, indexes are scanned and user input is checked whether all objects will be shown 
       or user requested a specific class will be shown */
    for(int i=0;i<indexes.size();i++)
    {
        // Index, class index and bounding box coordinates are extracted
        int id = indexes[i];
        int classId = Ids[id];
        cv::Rect box = BoundingBoxes[id];

        /* If there exists a special request from the user only that classes will be drawn so input and predicted class Id is checked
           If a request is made label will have requested label otherwise --save_image string will be in label so all objects shown*/
        if(std::strcmp(label.c_str(), classes[classId].c_str()) == 0)
        {   
            // A rectangle is drawn on the frame using the bounding box coordinates 
            cv::rectangle(Frame, cv::Point(box.x, box.y), cv::Point((box.x + box.width), (box.y + box.height)), cv::Scalar(0,0,255), 3);
            // A string is created using confidence level and name of the object
            std::string label = cv::format("%.2f", Confidence[id]) + ":" + classes[classId];
            // getTextSize returns the size of the text in pixels
            int baseline;
            cv::Size labelsize = cv::getTextSize(label, cv::FONT_HERSHEY_COMPLEX, 0.5, 1, &baseline);
            // A rectangle is drawn for label and label name is put using putText
            cv::rectangle(Frame, cv::Point(box.x, box.y - std::round(1.5*labelsize.height)), cv::Point(box.x + std::round(1.5*labelsize.width), box.y + baseline), cv::Scalar(0,0,0), cv::FILLED);
            cv::putText(Frame, label, cv::Point(box.x, box.y), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255,255,255),1);
        }
        else if(std::strcmp(label.c_str(), ("--save_image")) == 0)
        {
            // A rectangle is drawn on the frame using the bounding box coordinates 
            cv::rectangle(Frame, cv::Point(box.x, box.y), cv::Point((box.x + box.width), (box.y + box.height)), cv::Scalar(0,0,255), 3);
            // A string is created using confidence level and name of the object
            std::string label = cv::format("%.2f", Confidence[id]) + ":" + classes[classId];
            // getTextSize returns the size of the text in pixels
            int baseline;
            cv::Size labelsize = cv::getTextSize(label, cv::FONT_HERSHEY_COMPLEX, 0.5, 1, &baseline);
            // A rectangle is drawn for label and label name is put using putText
            cv::rectangle(Frame, cv::Point(box.x, box.y - std::round(1.5*labelsize.height)), cv::Point(box.x + std::round(1.5*labelsize.width), box.y + baseline), cv::Scalar(0,0,0), cv::FILLED);
            cv::putText(Frame, label, cv::Point(box.x, box.y), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255,255,255),1);
        }
     }
    
}


void VideoCapture(Queue& frame, cv::VideoCapture cap)
{
    // cap.read reads the camera frame and write to frame1 then its pushed to queue as its requeired
    cv::Mat frame1;
    cap.read(frame1);   
    frame.Qinsert(frame1);
}


void ObjectDetection(Queue& Framess, Queue& SaveFrames, std::vector<std::string> class_names, cv::dnn::Net network, std::string label)
{
    // The yolo version that I used takes the images as 416x416, these are used in resizing the input image
    const int inputWidth = 416; const int inputHeight = 416;

    cv::Mat frame, blob;
    if(!Framess.QEmpty())
    {
        /* Frame is taken from queue, resized and normalized that given to the networks input
           outputs are taken as vector and network is forwarded meaning outputs according to input is taken than
           it is send ProcessData_DrawBoxes function to draw bounding boxes */
        frame = Framess.Qdelete();
        cv::dnn::blobFromImage(frame, blob, 1/255.0, cv::Size(inputWidth, inputHeight), cv::Scalar(0,0,0), true, false);
        network.setInput(blob);
        std::vector<cv::Mat> outputs;
        network.forward(outputs, getLastLayerNames(network)); // getLastLayerNames returns the name of the last layer
        ProcessData_DrawBoxes(frame, outputs, class_names, label);
        // Convert frame to CV_8U format for OpenCV to be able to save video
        cv::Mat detectedFrame;
        frame.convertTo(detectedFrame, CV_8U);
        SaveFrames.Qinsert(detectedFrame); // Queue where video frames are saved
    }
    else
        return;       
}


void VideoWrite(Queue& FramesForVideo, cv::VideoWriter video)
{
    /* This function takes the saved frames and using video.write OpenCV creates the video */
    if(!FramesForVideo.QEmpty())
    {
        cv::Mat Frame_ = FramesForVideo.Qdelete();
        video.write(Frame_);
    }
}


std::vector<std::string> FetchClassNames()
{
    /* This function reads a file where the class names which are available for Yolo and stores in a vector */
    std::vector<std::string> classNames;
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
    // This function reads Config file and weights than creates a Yolo network
    std::string modelConfig = "yolov3.cfg";
    std::string modelWeights = "yolov3.weights";

    // Yolo network is created using below syntax and function is returned with network
    cv::dnn::Net network = cv::dnn::readNetFromDarknet(modelConfig, modelWeights);
    network.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    return network;
}


bool CheckSaveRequest(char* kwarg[])
{   
    // User input is controlled if a save request is made condition is returned
    if(std::strcmp(kwarg[5], "true") == 0) // If label request is not made save request stored in fifth member of array
        return true;
    else if(std::strcmp(kwarg[5], "--save_image") == 0) 
    {
        if(std::strcmp(kwarg[6], "true") == 0)  // If label request is made save request stored in sixth member of array
            return true;
    }
        
    else 
        return false;
}
