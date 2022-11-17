#include <iostream>
#include "opencv2/opencv.hpp"
#include <ctime>

enum RGBConsts
{
    MIN_RGB_VALUE = 0,
    MAX_RGB_VALUE = 255,
    COLOR_GROWTH_PARAMETER = 4
};

enum BlurConsts
{
    MIN_BLUR_VALUE = 1,
    MAX_BLUR_VALUE = 39,
    BLUR_GROWTH_PARAMETER = 6
};

enum ErosionConsts
{
    MIN_EROSION_VALUE = 1,
    MAX_EROSION_VALUE = 9,
    EROSION_GROWTH_PARAMETER = 2,
};

enum GeneralConsts
{
    NUMBER_OF_MILLISECONDS = 1000,
    COEFFICIENT = 1000
};

enum Directions
{
    TO_MIN_VALUE = 0,
    TO_MAX_VALUE = 1
};

bool IsOverflow(long long int value)
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

/* FOR: GaussianBlur(dest, dest, Size(weight, height), 0, 0) */
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

void ViewPercents(double percentValue)
{
    std::cout << percentValue * 100 << "%" << std::endl;
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
    int top = static_cast<int> (0.05 * frame.rows);
    int bottom = top;
    int left = static_cast<int> (0.05 * frame.cols);
    int right = left;
    int borderType = BORDER_CONSTANT;
    int red = MIN_RGB_VALUE, green = MIN_RGB_VALUE, blue = MIN_RGB_VALUE;
    int erosionSize = MIN_EROSION_VALUE;
    Directions directionOfRGB = TO_MAX_VALUE, directionOfErosion = TO_MAX_VALUE;

    long long int numberOfFrames = 0;
    struct timespec start_clock{}, end_clock{};
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_clock); // TODO: use chrono instead of clockgettime.

    std::chrono::time_point<std::chrono::high_resolution_clock> previousFrameTime(std::chrono::high_resolution_clock::now());
    std::chrono::time_point<std::chrono::high_resolution_clock> newFrameTime;

    std::chrono::time_point<std::chrono::high_resolution_clock> newReadTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> newTransformationTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> newOutputTime;

    double avgPercentOfRead = 0;
    double avgPercentOfTransformation = 0;
    double avgPercentOfOutput = 0;
    while (true)
    {
        newReadTime = std::chrono::high_resolution_clock::now();
        myCamera >> frame;
        assert(!frame.empty());
        std::chrono::duration<double> readTime = newReadTime - previousFrameTime;

        newTransformationTime = std::chrono::high_resolution_clock::now();
        Scalar value(red, green, blue);
        copyMakeBorder(frame, dest, top, bottom, left, right, borderType, value);
        flip(dest, dest, 1);
        Mat element = getStructuringElement(MORPH_CROSS, Size(2 * erosionSize + 1, 2 * erosionSize + 1), Point(2, 2));
        erode(dest, dest, element);
        std::chrono::duration<double> transformationTime = newTransformationTime - previousFrameTime;

        newOutputTime = std::chrono::high_resolution_clock::now();
        imshow("My Camera", dest);
        std::chrono::duration<double> outputTime = newOutputTime - previousFrameTime;

//        imshow("FPS test", frame);
        if (waitKey(NUMBER_OF_MILLISECONDS / COEFFICIENT) == static_cast<int> ('q'))
        {
            break;
        }

        if (IsOverflow(numberOfFrames))
        {
            break;
        }
        UpdateRGB(red, green, blue, directionOfRGB);
        UpdateErosion(erosionSize, directionOfErosion);

        newFrameTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = (newFrameTime - previousFrameTime);
        int FPS = static_cast<int> (1 / duration.count());
        previousFrameTime = newFrameTime;
        ++numberOfFrames;

        avgPercentOfRead += readTime.count() / duration.count();
        avgPercentOfTransformation += transformationTime.count() / duration.count();
        avgPercentOfOutput += outputTime.count() / duration.count();

        std::cout << "Frames per second: " << FPS << std::endl;
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &end_clock);
    double totalTime = static_cast<double> (end_clock.tv_sec - start_clock.tv_sec) + 0.000000001 * static_cast<double> (end_clock.tv_nsec - start_clock.tv_nsec);
    std::cout << std::endl << "AVERAGE FPS: " << static_cast<double> (numberOfFrames) / totalTime  << std::endl;

    std::cout << "AVERAGE PERCENT OF READ: ";
    ViewPercents(avgPercentOfRead / static_cast<double> (numberOfFrames));
    std::cout << "AVERAGE PERCENT OF TRANSFORMATION: ";
    ViewPercents(avgPercentOfTransformation / static_cast<double> (numberOfFrames));
    std::cout << "AVERAGE PERCENT OF OUTPUT: ";
    ViewPercents(avgPercentOfOutput / static_cast<double> (numberOfFrames));
    myCamera.release();

    return 0;
}
