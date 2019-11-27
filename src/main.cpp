#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "Transform.h"
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <cmath>

const int WIDTH = 600;
const int HEIGHT = 400;

using namespace std;
using namespace cv;

static void prepare(const String &path, vector<String> &files)
{
    vector<String> extensions{".jpg", ".jPg", ".jPG", ".jpG",
                              ".Jpg", ".JPg", ".JPG", ".JpG", ".png", ".pNg", ".pNG", ".pnG",
                              ".Png", ".PNg", ".PNG", ".PnG", ".avi", ".aVi", ".aVI", ".avI",
                              ".Avi", ".AVi", ".AVI", ".AvI", ".mp4", ".Mp4", ".MP4", ".mP4"};
    for (auto ext : extensions)
    {
        vector<String> tmp;
        glob(path + "*" + ext, tmp, false);
        files.insert(files.end(), tmp.begin(), tmp.end());
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        cout << "usage: MyMakeVideo directory" << '\n';
        return -1;
    }
    String path = argv[1];
    vector<String> files;

    // prepare the files
    prepare(path, files);

    // empty files error dectection
    if (files.empty())
    {
        cerr << "no files in the current directory!" << '\n';
        return -1;
    }

    VideoCapture video(files.back());

    // input video open error detection
    if (!video.isOpened())
    {
        cerr << "Fail to open the video...!" << '\n';
        destroyWindow("MyVideo");
        return -1;
    }

    VideoWriter out("./out.mp4", VideoWriter::fourcc('m', 'p', '4', 'v'), video.get(CAP_PROP_FPS), Size(WIDTH, HEIGHT));

    // output video open error detection
    if (!out.isOpened())
    {
        cerr << "Could not open the output video file for write\n";
        destroyWindow("MyVideo");
        return -1;
    }

    namedWindow("MyVideo", WINDOW_NORMAL);
    moveWindow("MyVideo", 400, 200);
    Transform t(out);
    Mat pre;
    default_random_engine e(time(0));

    // header
    int center_x = WIDTH / 2;
    int center_y = HEIGHT / 2;
    Mat head = Mat::zeros(Size(WIDTH, HEIGHT), CV_8UC3);
    imshow("MyVideo", head);
    out << head;
    waitKey(1000);
    for (int i = 0; i < 360; i++)
    {
        // Point t0(15 * (2 * sin(i * M_PI / 180) - sin(2 * i * M_PI / 180)) + center_x, -15 * (2 * cos(i * M_PI / 180) - cos(2 * i * M_PI / 180)) + center_y);
        // Point t1(15 * (2 * sin((i + 1) * M_PI / 180) - sin(2 * (i + 1) * M_PI / 180)) + center_x, -15 * (2 * cos((i + 1) * M_PI / 180) - cos(2 * (i + 1) * M_PI / 180)) + center_y);
        // Point t0(50 * (2 * cos(i * M_PI / 180) - cos(2 * i * M_PI / 180)) + center_x, 50 * (2 * sin(i * M_PI / 180) - sin(2 * i * M_PI / 180)) + center_y);
        // Point t1(50 * (2 * cos((i + 1) * M_PI / 180) - cos(2 * (i + 1) * M_PI / 180)) + center_x, 50 * (2 * sin((i + 1) * M_PI / 180) - sin(2 * (i + 1) * M_PI / 180)) + center_y);
        Point t0(50 * (-2 * sin(i * M_PI / 180) + cos(2 * i * M_PI / 180)) + center_x, 50 * (2 * cos(i * M_PI / 180) - sin(2 * i * M_PI / 180)) + center_y);
        Point t1(50 * (-2 * sin((i + 1) * M_PI / 180) + cos(2 * (i + 1) * M_PI / 180)) + center_x, 50 * (2 * cos((i + 1) * M_PI / 180) - sin(2 * (i + 1) * M_PI / 180)) + center_y);
        line(head, t0, t1, Scalar(0, 0, 255), 2, LINE_8);
        imshow("MyVideo", head);
        out << head;
        waitKey(50);
    }

    for (int i = 0; i < files.size() - 1; i++)
    {
        Mat img = imread(files[i]);
        resize(img, img, Size(WIDTH, HEIGHT));
        if (i != 0)
        {
            // choose changes randomly
            switch (e() % 5)
            {
            case 0:
                t.window_slides_transform(img, pre, 10, 10);
                break;
            case 1:
                t.window_shades_transform(img, pre, 10, 1);
                break;
            case 2:
                t.rtl_transform(img, pre, 10);
                break;
            case 3:
                t.circle_expand_transform(img, pre, 10);
                break;
            case 4:
                t.growth_transform(img, pre, 10, 10);
                break;
            default:
                break;
            }
            pre = img.clone();
            waitKey(100);
        }
        else
        {
            pre = img.clone();
            out << img;
            imshow("MyVideo", img);
            waitKey(100);
        }
    }

    Mat frame;

    // plus video
    bool flag = true;
    while (true)
    {
        video >> frame;
        if (frame.empty())
            break;
        resize(frame, frame, Size(WIDTH, HEIGHT));
        putText(frame, "3160102243", Point(50, HEIGHT - 50), FONT_HERSHEY_SCRIPT_COMPLEX, 1, Scalar(255, 255, 0));
        putText(frame, "Chao Liang", Point(50, HEIGHT - 20), FONT_HERSHEY_SCRIPT_COMPLEX, 1, Scalar(255, 255, 0));
        if (flag)
        {
            t.growth_transform(frame, pre, 10, 10);
            flag = false;
        }
        else
        {
            out << frame;
            imshow("MyVideo", frame);
            waitKey(1000 / video.get(CAP_PROP_FPS));
        }
    }
    video.release();
    waitKey(0);
    destroyWindow("MyVideo");
    return 0;
}