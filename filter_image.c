#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    // TODO
   float sum = 0;
    for (int c = 0; c < im.c; c++) {
        for (int y = 0; y < im.h; y++) {
            for (int x = 0; x < im.w; x++) {
                sum += get_pixel(im, x, y, c);
            }
        }
    }

    for (int c = 0; c < im.c; c++) {
        for (int y = 0; y < im.h; y++) {
            for (int x = 0; x < im.w; x++) {
                float normalized_value = get_pixel(im, x, y, c) / sum;
                set_pixel(im, x, y, c, normalized_value);
            }
        }
    }
}

image make_box_filter(int w)
{
    // TODO
      image filter = make_image(w, w, 1);

    for (int y = 0; y < w; y++) {
        for (int x = 0; x < w; x++) {
            set_pixel(filter, x, y, 0, 1);
        }
    }

    l1_normalize(filter);

    return filter;
    // return make_image(1,1,1);
}

image convolve_image(image im, image filter, int preserve)
{
    // TODO
      assert(filter.c == 1 || filter.c == im.c);

    image result;
    if (preserve && filter.c == 1)
        result = make_image(im.w, im.h, im.c);
    else
        result = make_image(im.w, im.h, 1);

    int padding = filter.w / 2;

    for (int c = 0; c < result.c; c++) {
        for (int y = 0; y < result.h; y++) {
            for (int x = 0; x < result.w; x++) {
                float sum = 0;
                for (int fy = 0; fy < filter.h; fy++) {
                    for (int fx = 0; fx < filter.w; fx++) {
                        int im_x = x + fx - padding;
                        int im_y = y + fy - padding;

                        float filter_value = get_pixel(filter, fx, fy, 0);
                        float im_value = get_pixel(im, im_x, im_y, c);

                        sum += filter_value * im_value;
                    }
                }

                if (preserve && filter.c == 1) {
                    set_pixel(result, x, y, c, sum);
                } else {
                    set_pixel(result, x, y, 0, sum);
                }
            }
        }
    }

    return result;
   // return make_image(1,1,1);
}

image make_highpass_filter()
{
    // TODO
      image filter = make_image(3, 3, 1);

    set_pixel(filter, 0, 0, 0, 0);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 4);
    set_pixel(filter, 2, 1, 0, -1);
    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, -1);
    set_pixel(filter, 2, 2, 0, 0); 

    return filter;
   // return make_image(1,1,1);
}

image make_sharpen_filter()
{
    // TODO
     image filter = make_image(3, 3, 1);

    set_pixel(filter, 0, 0, 0, 0);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 5);
    set_pixel(filter, 2, 1, 0, -1);
    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, -1);
    set_pixel(filter, 2, 2, 0, 0);

    return filter;
    //return make_image(1,1,1);
}

image make_emboss_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);

    set_pixel(filter, 0, 0, 0, -2);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 1);
    set_pixel(filter, 2, 1, 0, 1);
    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, 1);
    set_pixel(filter, 2, 2, 0, 2);

    return filter;
   // return make_image(1,1,1);
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: The highpass filter enhances the high-frequency components of an image, resulting in sharpening and edge enhancement. 
//         It suppresses the low-frequency components and preserves the high-frequency details. Using the preserve option with the highpass filter would help retain the enhanced edges and details while reducing noise and artifacts.
//         Since the emboss filter is mainly used for its visual effect rather than preserving details or suppressing noise, the preserve option may not be necessary.
//         The sharpen filter can introduce noise and artifacts, so using the preserve option might not be ideal. It could potentially amplify the noise, leading to undesired results.  

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    // TODO
        int size = ceilf(6 * sigma);
    if (size % 2 == 0) size++; // Make sure size is odd

    image filter = make_image(size, size, 1);

    float sum = 0;
    int padding = size / 2;

    for (int y = 0; y < filter.h; y++) {
        for (int x = 0; x < filter.w; x++) {
            int dx = x - padding;
            int dy = y - padding;

            float exponent = -(dx * dx + dy * dy) / (2 * sigma * sigma);
            float value = expf(exponent) / (TWOPI * sigma * sigma);

            sum += value;

            set_pixel(filter, x, y, 0, value);
        }
    }

    for (int y = 0; y < filter.h; y++) {
        for (int x = 0; x < filter.w; x++) {
            float normalized_value = get_pixel(filter, x, y, 0) / sum;
            set_pixel(filter, x, y, 0, normalized_value);
        }
    }

    return filter;
   // return make_image(1,1,1);
}

image add_image(image a, image b)
{
    // TODO
        assert(a.w == b.w && a.h == b.h && a.c == b.c);

    image result = make_image(a.w, a.h, a.c);

    for (int c = 0; c < a.c; c++) {
        for (int y = 0; y < a.h; y++) {
            for (int x = 0; x < a.w; x++) {
                float value_a = get_pixel(a, x, y, c);
                float value_b = get_pixel(b, x, y, c);

                set_pixel(result, x, y, c, value_a + value_b);
            }
        }
    }

    return result;
    //return make_image(1,1,1);
}

image sub_image(image a, image b)
{
    // TODO
     assert(a.w == b.w && a.h == b.h && a.c == b.c);

    image result = make_image(a.w, a.h, a.c);

    for (int c = 0; c < a.c; c++) {
        for (int y = 0; y < a.h; y++) {
            for (int x = 0; x < a.w; x++) {
                float value_a = get_pixel(a, x, y, c);
                float value_b = get_pixel(b, x, y, c);

                set_pixel(result, x, y, c, value_a - value_b);
            }
        }
    }

    return result;
  //  return make_image(1,1,1);
}

image make_gx_filter()
{
    // TODO
      image filter = make_image(3, 3, 1);

    set_pixel(filter, 0, 0, 0, -1);
    set_pixel(filter, 1, 0, 0, 0);
    set_pixel(filter, 2, 0, 0, 1);
    set_pixel(filter, 0, 1, 0, -2);
    set_pixel(filter, 1, 1, 0, 0);
    set_pixel(filter, 2, 1, 0, 2);
    set_pixel(filter, 0, 2, 0, -1);
    set_pixel(filter, 1, 2, 0, 0);
    set_pixel(filter, 2, 2, 0, 1);

    return filter;
   // return make_image(1,1,1);
}

image make_gy_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);

    set_pixel(filter, 0, 0, 0, -1);
    set_pixel(filter, 1, 0, 0, -2);
    set_pixel(filter, 2, 0, 0, -1);
    set_pixel(filter, 0, 1, 0, 0);
    set_pixel(filter, 1, 1, 0, 0);
    set_pixel(filter, 2, 1, 0, 0);
    set_pixel(filter, 0, 2, 0, 1);
    set_pixel(filter, 1, 2, 0, 2);
    set_pixel(filter, 2, 2, 0, 1);

    return filter;
  // return make_image(1,1,1);
}

void feature_normalize(image im)
{
    // TODO
    float min_value = get_pixel(im, 0, 0, 0);
    float max_value = get_pixel(im, 0, 0, 0);

    for (int c = 0; c < im.c; c++) {
        for (int y = 0; y < im.h; y++) {
            for (int x = 0; x < im.w; x++) {
                float value = get_pixel(im, x, y, c);

                if (value < min_value)
                    min_value = value;
                if (value > max_value)
                    max_value = value;
            }
        }
    }

    float range = max_value - min_value;

    if (range == 0) {
        set_pixel(im,0,0,0,0);
    } else {
        for (int c = 0; c < im.c; c++) {
            for (int y = 0; y < im.h; y++) {
                for (int x = 0; x < im.w; x++) {
                    float value = get_pixel(im, x, y, c);
                    float normalized_value = (value - min_value) / range;
                    set_pixel(im, x, y, c, normalized_value);
                }
            }
        }
    }
}

image *sobel_image(image im)
{
    // TODO
     image *result = calloc(2, sizeof(image));

    image sobel_x = make_gx_filter();
    image sobel_y = make_gy_filter();

    image gx = convolve_image(im, sobel_x, 0);
    image gy = convolve_image(im, sobel_y, 0);

    result[0] = make_image(im.w, im.h, 1);
    result[1] = make_image(im.w, im.h, 1);

    for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
            float gx_value = get_pixel(gx, x, y, 0);
            float gy_value = get_pixel(gy, x, y, 0);

            float magnitude = sqrtf(gx_value * gx_value + gy_value * gy_value);
            float direction = atan2f(gy_value, gx_value);

            set_pixel(result[0], x, y, 0, magnitude);
            set_pixel(result[1], x, y, 0, direction);
        }
    }

    free_image(gx);
    free_image(gy);

    return result;
   // return calloc(2, sizeof(image));
}

image colorize_sobel(image im)
{
    // TODO
      image *sobel = sobel_image(im);
    image magnitude = sobel[0];
    image direction = sobel[1];

    feature_normalize(magnitude);

    image hsv = make_image(im.w, im.h, 3);

    for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
            float magnitude_value = get_pixel(magnitude, x, y, 0);
            float direction_value = get_pixel(direction, x, y, 0);

            set_pixel(hsv, x, y, 0, direction_value);
            set_pixel(hsv, x, y, 1, magnitude_value);
            set_pixel(hsv, x, y, 2, magnitude_value);
        }
    }
     
    image rgb = rgb_to_hsv(hsv);

    free_image(*sobel);
    free_image(magnitude);
    free_image(direction);
    free_image(hsv);

    return rgb;
   // return make_image(1,1,1);
}
