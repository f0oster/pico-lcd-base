#include "surface.h"





/*
*  Create a new surface and allocate memory
*/
Surface *surface_create (int width, int height) {
    Surface *surface = (Surface *)malloc(sizeof(Surface));
    surface->size = width * height;
    surface->width = width;
    surface->height = height;
    surface->pixels = (uint16_t *)malloc(width * height * 2);
    return surface;
}


/*
*  Unallocate the memory used by a surface
*/
void surface_destroy (Surface *surface) {
    free(surface->pixels);
    free(surface);
}


/*
*  Fills a surface with the specified 16-bit colour
*/
void surface_fill (Surface *surface, uint16_t colour) {
    colour = ((colour << 8) & 0xff00) | (colour >> 8);
    for (int i = 0; i < surface->size; i++) surface->pixels[i] = colour;
}


/*
*  Fills a surface with the specified r,g,b colour
*/
void surface_fill_rgb (Surface *surface, uint8_t r, uint8_t g, uint8_t b) {
    uint16_t colour = ((r / 8) << 11) + ((g / 8) << 6) + (b / 8);
    colour = ((colour << 8) & 0xff00) | (colour >> 8);
    for (int i = 0; i < surface->size; i++) surface->pixels[i] = colour;
}


/*
*  Copies a region of 'src' surface to an offset within 'dest' surface
*  Note that this doesnt scale ('destRect' width and height are unused)
*/
void surface_blit (Surface *dest, Surface *src, Rect *destRect, Rect *srcRect) {
    for (int y = 0; y < srcRect->h; y++) {
        int srcY = srcRect->y + y;
        if (srcY < 0 || srcY >= src->height) continue;
        int destY = destRect->y + y;
        if (destY < 0 || destY >= dest->height) continue;
        for (int x = 0; x < srcRect->w; x++) {
            int srcX = srcRect->x + x;
            if (srcX < 0 || srcX >= src->width) continue;
            int srcIdx = srcY * src->width + srcX;
            int destX = destRect->x + x;
            if (destX < 0 || destX >= dest->width) continue;
            int destIdx = destY * dest->width + destX;
            dest->pixels[destIdx] = src->pixels[srcIdx];
        }
    }
}


/*
*  Copies a region of 'src' surface to an offset within 'dest' surface, 
*  ignoring pixels in the 'src' that are of colour 'mask'.
*  Note that this doesnt scale ('destRect' width and height are unused)
*/
void surface_blit_mask (Surface *dest, Surface *src, Rect *destRect, Rect *srcRect, uint16_t mask) {
    for (int y = 0; y < srcRect->h; y++) {
        int srcY = srcRect->y + y;
        if (srcY < 0 || srcY >= src->height) continue;
        int destY = destRect->y + y;
        if (destY < 0 || destY >= dest->height) continue;
        for (int x = 0; x < srcRect->w; x++) {
            int srcX = srcRect->x + x;
            if (srcX < 0 || srcX >= src->width) continue;
            int srcIdx = srcY * src->width + srcX;
            int destX = destRect->x + x;
            if (destX < 0 || destX >= dest->width) continue;
            if (src->pixels[srcIdx] == mask) continue;
            int destIdx = destY * dest->width + destX;
            dest->pixels[destIdx] = src->pixels[srcIdx];
        }
    }
}


/*
*  Iterates 'src' and interprets non-space characters as 'colour' pixels, while all other
*  characters result in a 'mask' pixel colour. 
*/
void surface_load (Surface *dest, char *src, uint16_t len, uint16_t colour, uint16_t mask) {
    colour = ((colour << 8) & 0xff00) | (colour >> 8);
    mask = ((mask << 8) & 0xff00) | (mask >> 8);
    for (int i=0; i < len; i++) {
        dest->pixels[i] = src[i] == ' ' ? mask : colour;
    }
}

