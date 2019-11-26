#include "Transform.h"
void Transform::window_slides_transform(const cv::Mat &a, cv::Mat &b, int x_num, int y_num)
{
    const uchar *p;
    uchar *q;
    for (int i = 0; i < b.rows; i += b.rows / x_num)
    {
        for (int j = 0; j < b.cols; j += b.cols / y_num)
        {
            for (int m = 0; m < b.rows / y_num; m++)
            {
                p = a.ptr<uchar>(i + m);
                q = b.ptr<uchar>(i + m);
                for (int n = 0; n < b.cols / x_num; n++)
                {
                    for (int k = 0; k < b.channels(); k++)
                    {
                        q[(j + n) * b.channels() + k] = p[(j + n) * b.channels() + k];
                    }
                }
            }
            o << b;
            cv::imshow("MyVideo", b);
            cv::waitKey(50);
        }
    }
}

void Transform::window_shades_transform(const cv::Mat &a, cv::Mat &b, int y_num, int scale)
{
    const uchar *p;
    uchar *q;
    for (int i = 0; i < b.rows / y_num; i += scale)
    {
        for (int j = 0; j < b.rows; j += b.rows / y_num)
        {
            for (int k = 0; k < scale; k++)
            {
                p = a.ptr<uchar>(j + i + k);
                q = b.ptr<uchar>(j + i + k);
                for (int m = 0; m < b.cols * b.channels(); m++)
                {
                    q[m] = p[m];
                }
            }
        }
        o << b;
        cv::imshow("MyVideo", b);
        cv::waitKey(50);
    }
}

// right -> left
void Transform::rtl_transform(const cv::Mat &a, cv::Mat &b, int scale)
{
    const uchar *p;
    uchar *q;
    for (int i = b.cols; i >= 0; i -= b.cols / scale)
    {
        for (int j = 0; j < b.rows; j++)
        {
            p = a.ptr<uchar>(j);
            q = b.ptr<uchar>(j);
            for (int k = i; k < b.cols; k++)
            {
                for (int m = 0; m < b.channels(); m++)
                {
                    q[k * b.channels() + m] = p[(k - i) * b.channels() + m];
                }
            }
        }
        o << b;
        cv::imshow("MyVideo", b);
        cv::waitKey(50);
    }
}

void Transform::circle_expand_transform(const cv::Mat &a, cv::Mat &b, int scale)
{
    int center_x = b.cols / 2;
    int center_y = b.rows / 2;
    int r = scale;
    double max_r = pow(center_x, 2) + pow(center_y, 2);
    const uchar *p;
    uchar *q;
    while (pow(r, 2) < max_r)
    {
        for (int i = center_y - r; i <= center_y + r; i++)
        {
            if (i < 0 || i >= b.rows)
                continue;
            p = a.ptr<uchar>(i);
            q = b.ptr<uchar>(i);
            for (int j = center_x - r; j <= center_x + r; j++)
            {
                if (j < 0 || j >= b.cols)
                    continue;
                if (pow(i - center_y, 2) + pow(j - center_x, 2) <= pow(r, 2))
                {
                    for (int k = 0; k < b.channels(); k++)
                    {
                        q[j * b.channels() + k] = p[j * b.channels() + k];
                    }
                }
            }
        }
        o << b;
        cv::imshow("MyVideo", b);
        r += scale;
        cv::waitKey(50);
    }
}

void Transform::growth_transform(const cv::Mat &a, cv::Mat &b, int x_num, int y_num)
{
    std::default_random_engine e(time(0));
    std::vector<int> pos;
    for (int i = 0; i < x_num * y_num; i++)
        pos.push_back(i);
    std::shuffle(pos.begin(), pos.end(), e);
    const uchar *p;
    uchar *q;
    for (int i = 0; i < pos.size(); i++)
    {
        int index = pos[i];
        int x = index % x_num;
        int y = index / x_num;
        for (int m = y * b.rows / y_num; m < (y + 1) * b.rows / y_num; m++)
        {
            p = a.ptr<uchar>(m);
            q = b.ptr<uchar>(m);
            for (int n = x * b.cols / x_num; n < (x + 1) * b.cols / x_num; n++)
            {
                for (int k = 0; k < b.channels(); k++)
                {
                    q[n * b.channels() + k] = p[n * b.channels() + k];
                }
            }
        }
        o << b;
        cv::imshow("MyVideo", b);
        cv::waitKey(50);
    }
}