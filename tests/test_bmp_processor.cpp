#include "bmp_processor.hpp"
#include "app_runner.hpp"
#include "gtest/gtest.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static const int NUM_OF_THREADS = 6;

// Tests basic execution with valid input and output files.
TEST(BMPProcessorTest, BasicTest) {
    std::vector<std::string> args = {"test", "images/lenna.bmp", "output.bmp"};
    EXPECT_NE(RunApp(args), 0);
}

// Tests the crop filter with valid dimensions.
TEST(BMPProcessorTest, CropFilterTest) {
    std::vector<std::string> args = {"test", "images/lenna.bmp", "output.bmp", "-crop", "1000", "1000"};
    EXPECT_EQ(RunApp(args), 0);
}

// Verifies error handling for a non-existent input file.
TEST(BMPProcessorTest, InvalidFileTest) {
    std::vector<std::string> args = {"test", "nonexistent.bmp", "output.bmp"};
    EXPECT_NE(RunApp(args), 0);
}

// Verifies error handling when filter parameters are incomplete.
TEST(BMPProcessorTest, InvalidArgumentsTest) {
    std::vector<std::string> args = {"test", "images/lenna.bmp", "output.bmp", "-crop"};
    EXPECT_NE(RunApp(args), 0);
}

// Verifies the grayscale filter by ensuring all RGB channels are equal.
TEST(BMPProcessorTest, GrayscaleFilterTest) {
    std::vector<std::string> args = {"test", "images/lenna.bmp", "output_gs.bmp", "-gs"};
    EXPECT_EQ(RunApp(args), 0);

    BMPProcessor output_processor("output_gs.bmp", "temp_output.bmp", NUM_OF_THREADS);
    std::ifstream output_file("output_gs.bmp", std::ios::binary);
    EXPECT_TRUE(output_file.is_open());

    const int offset = 54;
    output_file.seekg(offset, std::ios::beg);

    char r = 0;
    char g = 0;
    char b = 0;
    while (output_file.get(r) && output_file.get(g) && output_file.get(b)) {
        EXPECT_EQ(r, g);
        EXPECT_EQ(g, b);
    }

    output_file.close();
}

// Verifies error handling for missing parameters on the crop filter.
TEST(BMPProcessorTest, DopInvalidArgumentsTest) {
    std::vector<std::string> args = {"test", "images/lenna.bmp", "output.bmp", "-crop"};
    EXPECT_NE(RunApp(args), 0);
}

// Verifies the negative filter by checking if pixel values are correctly inverted.
TEST(BMPProcessorTest, NegativeFilterTest) {
    std::vector<std::string> args = {"test", "images/flag.bmp", "output_neg.bmp", "-neg"};
    EXPECT_EQ(RunApp(args), 0);

    BMPProcessor original_processor("images/flag.bmp", "temp_orig.bmp", NUM_OF_THREADS);
    BMPProcessor neg_processor("output_neg.bmp", "temp_neg.bmp", NUM_OF_THREADS);

    const auto &orig_data = original_processor.GetImageData();
    const auto &neg_data = neg_processor.GetImageData();

    ASSERT_EQ(orig_data.size(), neg_data.size());

    for (size_t i = 0; i < orig_data.size(); ++i) {
        EXPECT_EQ(neg_data[i], static_cast<uint8_t>(255 - orig_data[i])) << "Mismatch at pixel index " << i;
    }
}

// Tests sequential application of multiple different filters.
TEST(BMPProcessorTest, MultipleFiltersTest) {
    std::vector<std::string> args = {
        "test", "images/lenna.bmp", "output_multiple_filters.bmp", "-crop", "500", "500", "-gs", "-neg"};
    EXPECT_EQ(RunApp(args), 0);
    EXPECT_TRUE(std::filesystem::exists("output_multiple_filters.bmp"));
}

// Verifies error handling when a filter is provided without its required parameters.
TEST(BMPProcessorTest, MissingFilterParametersTest) {
    std::vector<std::string> args = {"test", "images/lenna.bmp", "output_missing_params.bmp", "-crop"};
    EXPECT_NE(RunApp(args), 0);
}

// Verifies error handling when a filter is given unexpected extra parameters.
TEST(BMPProcessorTest, ExtraFilterParametersTest) {
    std::vector<std::string> args = {"test", "images/lenna.bmp", "output_extra_params.bmp", "-gs", "100", "200"};
    EXPECT_NE(RunApp(args), 0);
}

// Tests execution of image processing with multithreading.
TEST(BMPProcessorTest, MultithreadingTest) {
    std::vector<std::string> args = {"test", "images/lenna.bmp", "output_multithread.bmp", "-gs"};
    EXPECT_EQ(RunApp(args), 0);
    EXPECT_TRUE(std::filesystem::exists("output_multithread.bmp"));
}

// Verifies the sharpen filter alters the original image.
TEST(BMPProcessorTest, SharpenFilterTest) {
    std::vector<std::string> args = {"test", "images/lenna.bmp", "output_sharp.bmp", "-sharp"};
    ASSERT_EQ(RunApp(args), 0);

    BMPProcessor original_processor("images/lenna.bmp", "temp_orig.bmp", NUM_OF_THREADS);
    BMPProcessor sharp_processor("output_sharp.bmp", "temp_sharp.bmp", NUM_OF_THREADS);

    const auto &orig_data = original_processor.GetImageData();
    const auto &sharp_data = sharp_processor.GetImageData();

    ASSERT_EQ(orig_data.size(), sharp_data.size());

    bool images_differ = false;
    for (size_t i = 0; i < orig_data.size(); ++i) {
        if (orig_data[i] != sharp_data[i]) {
            images_differ = true;
            break;
        }
    }
    EXPECT_TRUE(images_differ);
}

// Tests combining crop, sharpen, and negative filters in one run.
TEST(BMPProcessorTest, MultipleFiltersWithSharpenTest) {
    std::vector<std::string> args = {
        "test", "images/lenna.bmp", "output_sharp_multiple.bmp", "-crop", "500", "500", "-sharp", "-neg"};
    EXPECT_EQ(RunApp(args), 0);
    EXPECT_TRUE(std::filesystem::exists("output_sharp_multiple.bmp"));
}