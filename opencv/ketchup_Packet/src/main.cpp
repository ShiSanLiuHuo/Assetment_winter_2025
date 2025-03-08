#include<opencv2/opencv.hpp>
#include<iostream>
using namespace cv;
using namespace std;


int main(){
    Mat img = imread("../imgs/Ketchup_Packet.png");
    if(img.empty()){
        cerr<<"Image no find!"<<endl;
        return -1;
    }

    Mat grayImage;
    cvtColor(img,grayImage,COLOR_BGR2GRAY);

    Mat edges;
    Canny(grayImage,edges,100,200);

    imwrite("ans.png",edges);
}