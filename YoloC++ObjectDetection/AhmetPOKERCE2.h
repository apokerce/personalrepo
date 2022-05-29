#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>



std::vector<std::string> getLastLayerNames(const cv::dnn::Net& net)
{
    std::vector<std::string> names_temp;
    //getUnconnected returns last layers
    std::vector<int> outLayers = net.getUnconnectedOutLayers(); 
    //Get all layers
    std::vector<std::string> layersNames = net.getLayerNames();
    // Get the names of the output layers in names
    names_temp.resize(outLayers.size());
    for (int i = 0; i < outLayers.size(); ++i)
        names_temp[i] = layersNames[outLayers[i] - 1];
    return names_temp;
}



void ProcessData_DrawBoxes(cv::Mat& Frame, const std::vector<cv::Mat>&outputss, std::vector<std::string> classes, std::string label)
{
    const float nonMaxSuppressionThresh = 0.4;
    const float confThresh = 0.5;
    std::vector<int> Ids;
    std::vector<float> Confidence;
    std::vector<cv::Rect> BoundingBoxes;
    /* Extract predictions with confidence higher than threshold. */
    for(int i=0;i<outputss.size(); i++)
    {
        float* data = (float*)outputss[i].data;
        for(int j=0;j<outputss[i].rows;j++)
        {
            cv::Mat scores = outputss[i].row(j).colRange(5, outputss[i].cols);
            cv::Point ClassIds;
            double confidence;
            // This function returns the location of maximum score
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
    
    // Apply non maximum supression
    std::vector<int> indexes;
    cv::dnn::NMSBoxes(BoundingBoxes, Confidence, confThresh, nonMaxSuppressionThresh, indexes);
    for(int i=0;i<indexes.size();i++)
    {
        // Take suppressed index
        int id = indexes[i];
        int classId = Ids[id];
        cv::Rect box = BoundingBoxes[id];
        // Check any label given or not
        if(std::strcmp(label.c_str(), classes[classId].c_str()) == 0)
        {   
            // Create rectangle bounding box
            cv::rectangle(Frame, cv::Point(box.x, box.y), cv::Point((box.x + box.width), (box.y + box.height)), cv::Scalar(0,0,255), 3);
            std::string label = cv::format("%.2f", Confidence[id]) + ":" + classes[classId];
            int baseline;
            // Put label name
            cv::Size labelsize = cv::getTextSize(label, cv::FONT_HERSHEY_COMPLEX, 0.5, 1, &baseline);
            int top = cv::max(box.y, labelsize.height);
            cv::rectangle(Frame, cv::Point(box.x, box.y - std::round(1.5*labelsize.height)), cv::Point(box.x + std::round(1.5*labelsize.width), box.y + baseline), cv::Scalar(0,0,0), cv::FILLED);
            cv::putText(Frame, label, cv::Point(box.x, box.y), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255,255,255),1);
        }
        else if(std::strcmp(label.c_str(), ("--save_image")) == 0)
        {
            // Create rectangle bounding box
            cv::rectangle(Frame, cv::Point(box.x, box.y), cv::Point((box.x + box.width), (box.y + box.height)), cv::Scalar(0,0,255), 3);
            std::string label = cv::format("%.2f", Confidence[id]) + ":" + classes[classId];
            int baseline;
            // Put label name
            cv::Size labelsize = cv::getTextSize(label, cv::FONT_HERSHEY_COMPLEX, 0.5, 1, &baseline);
            int top = cv::max(box.y, labelsize.height);
            cv::rectangle(Frame, cv::Point(box.x, box.y - std::round(1.5*labelsize.height)), cv::Point(box.x + std::round(1.5*labelsize.width), box.y + baseline), cv::Scalar(0,0,0), cv::FILLED);
            cv::putText(Frame, label, cv::Point(box.x, box.y), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255,255,255),1);
        }
     }
    
}



