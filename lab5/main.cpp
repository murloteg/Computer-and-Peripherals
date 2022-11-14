#include <iostream>
#include "opencv2/opencv.hpp"
#include <ctime>

enum Consts
{
    MIN_RGB_VALUE = 0,
    MAX_RGB_VALUE = 255,
    MIN_BLUR_VALUE = 1,
    MAX_BLUR_VALUE = 39,
    MIN_EROSION_VALUE = 1,
    MAX_EROSION_VALUE = 9,
    COLOR_GROWTH_PARAMETER = 4,
    BLUR_GROWTH_PARAMETER = 6,
    EROSION_GROWTH_PARAMETER = 2
};

enum Directions
{
    TO_MIN_VALUE = 0,
    TO_MAX_VALUE = 1
};

inline bool IsOverflow(long long int value)
{
    return (value == LLONG_MAX);
}

void UpdateRGB(int& red, int& green, int& blue, Directions& direction)
{
    if (direction == TO_MAX_VALUE)
    {
        if (red >= MAX_RGB_VALUE)
        {
            red = green = blue = MAX_RGB_VALUE;
            direction = TO_MIN_VALUE;
            return;
        }
        red += COLOR_GROWTH_PARAMETER;
        green += COLOR_GROWTH_PARAMETER;
        blue += COLOR_GROWTH_PARAMETER;
    }

    else
    {
        if (red <= MIN_RGB_VALUE)
        {
            red = green = blue = MIN_RGB_VALUE;
            direction = TO_MAX_VALUE;
            return;
        }
        red -= COLOR_GROWTH_PARAMETER;
        green -= COLOR_GROWTH_PARAMETER;
        blue -= COLOR_GROWTH_PARAMETER;
    }
}

void UpdateErosion(int& erosionSize, Directions& direction)
{
    if (direction == TO_MAX_VALUE)
    {
        if (erosionSize >= MAX_EROSION_VALUE)
        {
            erosionSize = MAX_EROSION_VALUE;
            direction = TO_MIN_VALUE;
            return;
        }
        erosionSize += EROSION_GROWTH_PARAMETER;
    }

    else
    {
        if (erosionSize <= MIN_EROSION_VALUE)
        {
            erosionSize = MIN_EROSION_VALUE;
            direction = TO_MAX_VALUE;
            return;
        }
        erosionSize -= EROSION_GROWTH_PARAMETER;
    }
}

/* FOR: GaussianBlur(dest, dest, Size(weight, height), 0, 0); */
void UpdateBlur(int& weight, int& height, Directions& direction)
{
    if (direction == TO_MAX_VALUE)
    {
        if (weight >= MAX_BLUR_VALUE)
        {
            weight = height = MAX_BLUR_VALUE;
            direction = TO_MIN_VALUE;
            return;
        }
        height += BLUR_GROWTH_PARAMETER;
        weight += BLUR_GROWTH_PARAMETER;
    }

    else
    {
        if (weight <= MIN_BLUR_VALUE)
        {
            weight = height = MIN_BLUR_VALUE;
            direction = TO_MAX_VALUE;
            return;
        }
        height = (height - BLUR_GROWTH_PARAMETER >= MIN_BLUR_VALUE) ?  height - BLUR_GROWTH_PARAMETER : MIN_BLUR_VALUE;
        weight = (weight - BLUR_GROWTH_PARAMETER >= MIN_BLUR_VALUE) ?  weight - BLUR_GROWTH_PARAMETER : MIN_BLUR_VALUE;
    }
}

using namespace cv;
int main()
{
    Mat frame;
    Mat dest;
    VideoCapture myCamera(0);

    if (!myCamera.isOpened())
    {
        return -1;
    }
    myCamera.read(frame);

    long long int numberOfFrames = 0;
    struct timespec start_clock{}, end_clock{};
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_clock);
    int top = static_cast<int> (0.05 * frame.rows);
    int bottom = top;
    int left = static_cast<int> (0.05 * frame.cols);
    int right = left;
    int borderType = BORDER_CONSTANT;

    int red = MIN_RGB_VALUE, green = MIN_RGB_VALUE, blue = MIN_RGB_VALUE;
    int erosionSize = MIN_EROSION_VALUE;
    Directions directionOfRGB = TO_MAX_VALUE, directionOfErosion = TO_MAX_VALUE;
    while (myCamera.read(frame))
    {
        myCamera >> frame;
        Scalar value(red, green, blue);
        copyMakeBorder(frame, dest, top, bottom, left, right, borderType, value);
        flip(dest, dest, 1);
        Mat element = getStructuringElement(MORPH_CROSS, Size(2 * erosionSize + 1, 2 * erosionSize + 1), Point(2, 2));
        erode(dest, dest, element);
        imshow("My Camera", dest);
        if (waitKey(1000 / 20) == static_cast<int> ('q'))
        {
            break;
        }

        if (IsOverflow(numberOfFrames))
        {
            break;
        }
        UpdateRGB(red, green, blue, directionOfRGB);
        UpdateErosion(erosionSize, directionOfErosion);
        ++numberOfFrames;
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &end_clock);
    double totalTime = static_cast<double> (end_clock.tv_sec - start_clock.tv_sec) + 0.000000001 * static_cast<double> (end_clock.tv_nsec - start_clock.tv_nsec);
    std::cout << "FPS: " << static_cast<double> (numberOfFrames) / totalTime  << std::endl;
    myCamera.release();

    return 0;
}
