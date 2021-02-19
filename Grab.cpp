#include "Grab.h"
#include <pylon/BaslerUniversalInstantCamera.h>
#include "SampleImageCreator.h"
using namespace GenApi;
using namespace cv;
Grab::Grab(/* args */)
{
}

Grab::~Grab()
{
}

void ShowImage( IImage& image, const char* message = NULL)
{
#ifdef PYLON_WIN_BUILD
    // Display the image.
    Pylon::DisplayImage(1, image);
#endif

    if ( message)
    {
        cout << endl << message << " ";
    }

    // store state of cout 
    std::ios state(NULL);
    state.copyfmt(cout);

    const uint8_t* pBytes = reinterpret_cast<const uint8_t*>(image.GetBuffer());
    cout << endl << "First six bytes of the image: " << endl;
    for (unsigned int i = 0; i < 6; ++i)
    {
        cout << "0x" << hex << setfill('0') << setw(2) << unsigned(pBytes[i]) << " ";
    }
    cout << endl;

    // restore state of cout 
    cout.copyfmt(state);

    cerr << "Press Enter to continue." << endl;
    while( cin.get() != '\n');
}

Mat Grab::Camera_grab()
{
// The exit code of the sample application.
    //int exitCode = 0;
    
    //transfer the image for opencv
    CImageFormatConverter FormatConverter;
    FormatConverter.OutputPixelFormat = PixelType_BGR8packed;
    
    //stitch image
    vector<Mat>stitchimages; 

    CPylonImage pylonimage;
    // Before using any pylon methods, the pylon runtime must be initialized. 
    PylonInitialize();

    try
    {
        // Create an instant camera object with the camera device found first.
        //CInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice());
        CBaslerGigEInstantCamera camera(CTlFactory::GetInstance().CreateFirstDevice());
        cout << "Using device " << camera.GetDeviceInfo().GetModelName() << endl;
        //camera.RegisterConfiguration(new CAcquireContinuousConfiguration, RegistrationMode_ReplaceAll,Cleanup_Delete);
        //Configure here
        camera.Open();

        camera.Width.SetValue(3700,IntegerValueCorrection_Nearest);
        camera.Height.SetValue(4700,IntegerValueCorrection_Nearest);
       // camera.AcquisitionMode.SetValue("Continuous");
        camera.TriggerSelector.SetValue(TriggerSelector_LineStart);
        camera.TriggerMode.SetValue(TriggerMode_On);
        camera.TriggerSource.SetValue(TriggerSource_FrequencyConverter);
        camera.ExposureMode.SetValue(ExposureMode_Timed);
        camera.ExposureAuto.SetValue(ExposureAuto_Off);
        camera.ExposureTimeAbs.SetValue(1200);
        camera.ExposureTimeRaw.SetValue(12000);
        camera.FrequencyConverterInputSource.SetValue(FrequencyConverterInputSource_Line1);
        camera.FrequencyConverterMultiplier.SetValue(3);
        camera.FrequencyConverterPostDivider.SetValue(13);
    
        // The parameter MaxNumBuffer can be used to control the count of buffers
        // allocated for grabbing. The default value of this parameter is 10.
        camera.MaxNumBuffer = 10;
        // Start the grabbing of c_countOfImagesToGrab images.
        // The camera device is parameterized with a default configuration which
        // sets up free-running continuous acquisition.
        camera.StartGrabbing( c_countOfImagesToGrab);

        // This smart pointer will receive the grab result data.
        CGrabResultPtr ptrGrabResult;

        // Camera.StopGrabbing() is called automatically by the RetrieveResult() method
        // when c_countOfImagesToGrab images have been retrieved.
        int count=0;
        while ( camera.IsGrabbing())
        {
            // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
            camera.RetrieveResult(40000, ptrGrabResult, TimeoutHandling_ThrowException);

            // Image grabbed successfully?
            if (ptrGrabResult->GrabSucceeded())
            {
                // Access the image data.
                cout<<"Grab image" <<count<<endl; 
                FormatConverter.Convert(pylonimage,ptrGrabResult);
                //ShowImage( pylonimage, "Converted image.");
                cv::Mat src_mat = cv::Mat(ptrGrabResult->GetHeight(),ptrGrabResult->GetWidth(),CV_8UC3,(uint8_t *)pylonimage.GetBuffer());
                //imwrite("ImageGrabbed"+to_string(count)+".jpg",src_mat);
                stitchimages.push_back(src_mat.clone()); 
                count++;
#ifdef PYLON_WIN_BUILD
                // Display the grabbed image.
                Pylon::DisplayImage(1, ptrGrabResult);
#endif
            }
            else
            {
                cout << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << endl;
            }
        }
    }
    catch (const GenericException &e)
    {
        // Error handling.
        cerr << "An exception occurred." << endl
        << e.GetDescription() << endl;
        //exitCode = 1;
    }

    // Comment the following two lines to disable waiting on exit.
    cerr << endl << "Press enter to exit." << endl;
    while( cin.get() != '\n');

    // Releases all pylon resources. 
    PylonTerminate();  

    //Stitch all the image in the array
    Mat flipimage;
    //Mat middleimage1;
    vconcat(stitchimages[0],stitchimages[1],flipimage);
    //Mat result_image;
    //flip(flipimage,result_image,1);
    
    //imshow("Stitch",resultimage);
    // vector<int> compress_params;
    // compress_params.push_back(IMWRITE_PNG_COMPRESSION);
    // compress_params.push_back(0);
    //imwrite("StitchImage3.jpg",flipimage,compress_params);
    cout<<"Stitch Success"<<endl;

    return flipimage;
}




// int Grab::Carmera(int number,String_t names)
// {
//     int bools = 0;
//     // Before using any pylon methods, the pylon runtime must be initialized. 
//     PylonInitialize();
//     string images_names;
//     try
//     {
//         CDeviceInfo info;
// 		info.SetSerialNumber(names); 
//         Camera_t *mCamera;
// 		mCamera = new Camera_t(CTlFactory::GetInstance().CreateFirstDevice(info));

//         // Create an instant camera object with the camera device found first.
//         mCamera->Open();
//         mCamera->GevSCPSPacketSize.SetValue(6000);
//         mCamera->GevSCPD.SetValue(0);
//         // mCamera->LineSelector.SetValue(LineSelector_Line2);
//         // mCamera->LineSource.SetValue(LineSource_ExposureActive);
//         // mCamera->UserOutputSelector.SetValue(UserOutputSelector_UserOutput1);
//         // mCamera->UserOutputValue.SetValue(false);

//         // CInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice());

//         // Print the model name of the camera.
//         cout << "Using device " << mCamera->GetDeviceInfo().GetModelName() << endl;

//         // The parameter MaxNumBuffer can be used to control the count of buffers
//         // allocated for grabbing. The default value of this parameter is 10.
//         mCamera->MaxNumBuffer = 5;

//         // Start the grabbing of c_countOfImagesToGrab images.
//         // The camera device is parameterized with a default configuration which
//         // sets up free-running continuous acquisition.
//         mCamera->StartGrabbing( number);

//         // This smart pointer will receive the grab result data.
//         CGrabResultPtr ptrGrabResult;

//         // Camera.StopGrabbing() is called automatically by the RetrieveResult() method

//         // when c_countOfImagesToGrab images have been retrieved.
//         while ( mCamera->IsGrabbing())
//         {
//             // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
//             mCamera->RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);

//             // Image grabbed successfully?
//             if (ptrGrabResult->GrabSucceeded())
//             {
//                 // Access the image data.
//                 cout << "SizeX: " << ptrGrabResult->GetWidth() << endl;
//                 cout << "SizeY: " << ptrGrabResult->GetHeight() << endl;
//                 const uint8_t *pImageBuffer = (uint8_t *) ptrGrabResult->GetBuffer();
//                 cout << "Gray value of first pixel: " << (uint32_t) pImageBuffer[0] << endl << endl;
//                 images_names = names;
//                 images_names = "GrabbedImage"+images_names+".png";
//                 cout<<images_names<<endl;
//                 const char *na = images_names.c_str();
//                 CImagePersistence::Save( ImageFileFormat_Png, na, ptrGrabResult);
//                 bools = 1;
// #ifdef PYLON_WIN_BUILD
//                 // Display the grabbed image.
//                 Pylon::DisplayImage(1, ptrGrabResult);
// #endif
//             }
//             else
//             {
//                 bools = 0;
//                 cout << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << endl;
//             }
//         }
//     }
//     catch (const GenericException &e)
//     {
//         // Error handling.
//         cerr << "An exception occurred." << endl
//         << e.GetDescription() << endl;
//         bools = 0;
//         // exitCode = 1;
//     }

//     // Releases all pylon resources. 
//     //PylonTerminate();  
//     return bools;
// }





