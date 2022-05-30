Download yolo.weights from https://pjreddie.com/media/files/yolov3.weights

While compiling the code a thread problem might occur to pass the thread error use the following compiling which include -pthread flag

    g++ main.cpp -o output -std=c++11 `pkg-config --cflags --libs opencv` -pthread

__To use the code:__

__For all labels__

    ./output --camera 0 --label --save_image true

__For only one label__

    ./output --camera 0 --label "type here" --save_image true

You can change the camera by changing the number coming after --camera or you can change by pressing 0/1/2 while streaming video. If you want to take the image of the time that is being streamed press lower case "a". 

__For all labels__

    ./output --video "video_name.extension" --label --save_image true

__For only one label__

    ./output --video "video_name.extension" --label "type here" --save_image true
