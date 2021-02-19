#include "TCPServer.h"
#include "Grab.h"
#include "line2Dup.h"
#include <chrono>

int main(int argc, char* argv[])
{
    // The exit code of the sample application.
    int exitCode = 0;

    Grab grab;
    cv::Mat resultimage;
    //Grab the image
    resultimage=grab.Camera_grab();

    //vector<int> compress_params;
    // compress_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
    // compress_params.push_back(0);
    imwrite("StitchImage12.bmp",resultimage);

    //Match and compute the center


    //grab.Carmera(5,);
    //TCPServer tcp;
    // while(1)
    // {
    //     tcp.setup(10003);
    //     string biaos = tcp.receive();
    //     cout<<"biaos is "<<biaos<<endl;
    // }   
    return exitCode;
}