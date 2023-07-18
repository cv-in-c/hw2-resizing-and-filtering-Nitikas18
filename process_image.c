#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
    
    if (x < 0){
        x = 0;
    }
    else if (x >= im.w){
        x = im.w - 1;
    }

    if (y < 0){
        y = 0;
    }
    else if (y >= im.h){
        y = im.h - 1;
    }

    if (c < 0){
        c = 0;
    }
    else if (c >= im.c){
        c = im.c - 1;
    }

    int index = c * im.w * im.h + y * im.w + x;

    return im.data[index];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
     if (x < 0 || x >= im.w || y < 0 || y >= im.h || c < 0 || c >= im.c) {
        return;
        }
        int index = c * im.w * im.h + y * im.w + x;
        im.data[index] = v;

}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    int num_pixels = im.w * im.h * im.c;

    memcpy(copy.data, im.data, sizeof(float) * num_pixels);

    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    for (int y = 0; y < im.h; y++) {
            
            for (int x = 0; x < im.w; x++) {
                float R = get_pixel(im, x, y, 0);
                float G = get_pixel(im, x, y, 1);
                float B = get_pixel(im, x, y, 2);

          
                float Y = 0.299f * R + 0.587f * G + 0.114f * B;
                set_pixel(gray, x, y, 0, Y);
        }
    }
     return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    for (int y = 0; y < im.h; y++) {
            
            for (int x = 0; x < im.w; x++) {
                float pxvl = get_pixel(im, x, y, c);
                pxvl= pxvl + v;

                set_pixel(im, x, y, c, pxvl);
        }
    }

}

void clamp_image(image im)
{
    // TODO Fill this in
      
    for (int c = 0; c < im.c; c++) {
        for (int y = 0; y < im.h; y++) {
            for (int x = 0; x < im.w; x++) {
              
                float pxvl = get_pixel(im, x, y, c);

                if(pxvl<0){
                    pxvl=0;
                }
                else if(pxvl>1){
                    pxvl=1;
                }
                else{
                    pxvl=pxvl;
                }

            
                set_pixel(im, x, y, c, pxvl);
            }
        }
    }


}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

typedef struct {
    double r; 
    double g; 
    double b; 
} RGBColor;

typedef struct {
    double h; 
    double s;  
    double v;  
} HSVColor;

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    RGBColor rgb;
    HSVColor hsv;
    for (int y = 0; y < im.h; y++) {
            
            for (int x = 0; x < im.w; x++) {
                rgb.r = get_pixel(im, x, y, 0);
                rgb.g = get_pixel(im, x, y, 1);
                rgb.b = get_pixel(im, x, y, 2);
                 
                hsv.v = three_way_max(rgb.r,rgb.g,rgb.b);
                float m= three_way_min(rgb.r,rgb.g,rgb.b);
                float C= hsv.v-m;
                hsv.s ;
                if(hsv.v==0){
                    hsv.s=0;
                }
                else{
                    hsv.s=C/hsv.v;
                }
                
                float  H;
                if(C==0){H=0;}
                else if(hsv.v==rgb.r){
                    H=(rgb.g-rgb.b)/C;
                }
                else if(hsv.v==rgb.g){
                    H=((rgb.b-rgb.r)/C) +2;
                }
                else{
                    H=((rgb.r-rgb.g)/C) +4;
                }
                
                if(H<0){
                    hsv.h=(H/6) + 1;
                }
                else{
                    hsv.h= H/6;
                }
            }
    }        

}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    RGBColor rgb;
    HSVColor hsv;

     for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
            hsv.h = get_pixel(im, x, y, 0);
            hsv.s = get_pixel(im, x, y, 1);
            hsv.v = get_pixel(im, x, y, 2);
            
            double C = hsv.v * hsv.s;
            double X = C * (1 - fabs(fmod(6 * hsv.h, 2) - 1));
            double m = hsv.v - C;
            
            double r, g, b;
            if (hsv.h >= 0 && hsv.h < 1) {
                r = C;
                g = X;
                b = 0;
            } else if (hsv.h >= 1 && hsv.h < 2) {
                r = X;
                g = C;
                b = 0;
            } else if (hsv.h >= 2 && hsv.h < 3) {
                r = 0;
                g = C;
                b = X;
            } else if (hsv.h >= 3 && hsv.h < 4) {
                r = 0;
                g = X;
                b = C;
            } else if (hsv.h >= 4 && hsv.h < 5) {
                r = X;
                g = 0;
                b = C;
            } else {
                r = C;
                g = 0;
                b = X;
            }
            
            rgb.r = r + m;
            rgb.g = g + m;
            rgb.b = b + m;
            
            set_pixel(im, x, y, 0, rgb.r);
            set_pixel(im, x, y, 1, rgb.g);
            set_pixel(im, x, y, 2, rgb.b);
        }
    }
}
