#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <cmath>
#include <iostream>

class Transform
{
private:
    cv::VideoWriter &o;

public:
    Transform(cv::VideoWriter &out) : o(out) {}
    Transform() = delete;
    Transform(const Transform &) = delete;
    Transform(const Transform &&) = delete;
    void setVideoWriter(cv::VideoWriter &out)
    {
        o = out;
    }
    void window_slides_transform(const cv::Mat &a, cv::Mat &b, int x_num, int y_num);
    void window_shades_transform(const cv::Mat &a, cv::Mat &b, int y_num, int scale);
    void rtl_transform(const cv::Mat &a, cv::Mat &b, int scale);
    void circle_expand_transform(const cv::Mat &a, cv::Mat &b, int scale);
};