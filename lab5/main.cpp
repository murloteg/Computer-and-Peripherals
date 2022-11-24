#include <iostream>
#include "opencv2/opencv.hpp"

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
    MIN_DELAY = 1
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

void ViewPercents(std::vector<double>& stats, int& vectorIndex)
{
    std::cout << stats[vectorIndex] * 100 << "%" << std::endl;
    ++vectorIndex;
}

void ViewStats(std::vector<double>& stats, double avgTimeOfIteration, long long int numberOfFrames)
{
    avgTimeOfIteration /= static_cast<double> (numberOfFrames);
    for (double& item : stats)
    {
        item /= static_cast<double> (numberOfFrames);
    }

    for (double& item : stats)
    {
        item /= avgTimeOfIteration;
    }

    int vectorIndex = 0;
    std::cout << "1) AVERAGE PERCENTAGE OF READ: ";
    ViewPercents(stats, vectorIndex);
    std::cout << "2) AVERAGE PERCENTAGE OF TRANSFORMATION: ";
    ViewPercents(stats, vectorIndex);
    std::cout << "3) AVERAGE PERCENTAGE OF OUTPUT: ";
    ViewPercents(stats, vectorIndex);
    std::cout << "4) AVERAGE PERCENTAGE OF OTHER: ";
    ViewPercents(stats, vectorIndex);
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
    int top = static_cast<int> (0.08 * frame.rows);
    int bottom = top;
    int left = static_cast<int> (0.08 * frame.cols);
    int right = left;
    int borderType = BORDER_CONSTANT;
    int red = MIN_RGB_VALUE, green = MIN_RGB_VALUE, blue = MIN_RGB_VALUE;
    int erosionSize = MIN_EROSION_VALUE;
    Directions directionOfRGB = TO_MAX_VALUE, directionOfErosion = TO_MAX_VALUE;

    long long int numberOfFrames = 0;
    std::chrono::time_point<std::chrono::high_resolution_clock> startFrameTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endFrameTime;

    std::chrono::time_point<std::chrono::high_resolution_clock> startReadTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endReadTime;

    std::chrono::time_point<std::chrono::high_resolution_clock> startTransformationTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTransformationTime;

    std::chrono::time_point<std::chrono::high_resolution_clock> startOutputTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endOutputTime;

    std::chrono::time_point<std::chrono::high_resolution_clock> startOtherTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endOtherTime;

    double avgTimeOfRead = 0;
    double avgTimeOfTransformation = 0;
    double avgTimeOfOutput = 0;
    double avgTimeOfOther = 0;
    double avgTimeOfIteration = 0;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTotalTime(std::chrono::high_resolution_clock::now());
    while (!frame.empty())
    {
        startFrameTime = std::chrono::high_resolution_clock::now();
        startReadTime = std::chrono::high_resolution_clock::now();
        myCamera >> frame;
        endReadTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> readTime = endReadTime - startReadTime;

        startTransformationTime = std::chrono::high_resolution_clock::now();
        Scalar value(red, green, blue);
        copyMakeBorder(frame, dest, top, bottom, left, right, borderType, value);
        flip(dest, dest, 1);
        Mat element = getStructuringElement(MORPH_CROSS, Size(2 * erosionSize + 1, 2 * erosionSize + 1), Point(2, 2));
        erode(dest, dest, element);
        UpdateRGB(red, green, blue, directionOfRGB);
        UpdateErosion(erosionSize, directionOfErosion);
        endTransformationTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> transformationTime = endTransformationTime - startTransformationTime;
        startOutputTime = std::chrono::high_resolution_clock::now();
        imshow("The frame after transformation", dest);
//        imshow("The frame without transformation", frame);
        endOutputTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> outputTime = endOutputTime - startOutputTime;

        startOtherTime = std::chrono::high_resolution_clock::now();
        if (waitKey(MIN_DELAY) == static_cast<int> ('q'))
        {
            break;
        }

        if (IsOverflow(numberOfFrames))
        {
            break;
        }
        endOtherTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> otherTime = endOtherTime - startOtherTime;

        avgTimeOfRead += readTime.count();
        avgTimeOfTransformation += transformationTime.count();
        avgTimeOfOutput += outputTime.count();
        avgTimeOfOther += otherTime.count();
        ++numberOfFrames;

        endFrameTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = (endFrameTime - startFrameTime);
        int FPS = static_cast<int> (1 / duration.count());
        avgTimeOfIteration += duration.count();
        std::cout << "Frames per second: " << FPS << std::endl;
    }

    std::chrono::time_point<std::chrono::high_resolution_clock> endTotalTime(std::chrono::high_resolution_clock::now());
    std::chrono::duration<double> totalTime = (endTotalTime - startTotalTime);
    std::cout << std::endl << "======[AVERAGE FPS]======: " << static_cast<double> (numberOfFrames) / (totalTime.count()) << std::endl;
    std::vector<double> stats = {avgTimeOfRead, avgTimeOfTransformation, avgTimeOfOutput, avgTimeOfOther};
    ViewStats(stats, avgTimeOfIteration, numberOfFrames);
    myCamera.release();

    return 0;
}
