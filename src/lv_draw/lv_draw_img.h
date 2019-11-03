/**
 * @file lv_draw_img.h
 *
 */

#ifndef LV_DRAW_IMG_H
#define LV_DRAW_IMG_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lv_draw.h"
#include "lv_img_decoder.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      MACROS
 **********************/

#define LV_IMG_BUF_SIZE_TRUE_COLOR(w, h) ((LV_COLOR_SIZE / 8) * w * h)
#define LV_IMG_BUF_SIZE_TRUE_COLOR_CHROMA_KEYED(w, h) ((LV_COLOR_SIZE / 8) * w * h)
#define LV_IMG_BUF_SIZE_TRUE_COLOR_ALPHA(w, h) (LV_IMG_PX_SIZE_ALPHA_BYTE * w * h)

/*+ 1: to be sure no fractional row*/
#define LV_IMG_BUF_SIZE_ALPHA_1BIT(w, h) ((((w / 8) + 1) * h))
#define LV_IMG_BUF_SIZE_ALPHA_2BIT(w, h) ((((w / 4) + 1) * h))
#define LV_IMG_BUF_SIZE_ALPHA_4BIT(w, h) ((((w / 2) + 1) * h))
#define LV_IMG_BUF_SIZE_ALPHA_8BIT(w, h) ((w * h))

/*4 * X: for palette*/
#define LV_IMG_BUF_SIZE_INDEXED_1BIT(w, h) (LV_IMG_BUF_SIZE_ALPHA_1BIT(w, h) + 4 * 2)
#define LV_IMG_BUF_SIZE_INDEXED_2BIT(w, h) (LV_IMG_BUF_SIZE_ALPHA_2BIT(w, h) + 4 * 4)
#define LV_IMG_BUF_SIZE_INDEXED_4BIT(w, h) (LV_IMG_BUF_SIZE_ALPHA_4BIT(w, h) + 4 * 16)
#define LV_IMG_BUF_SIZE_INDEXED_8BIT(w, h) (LV_IMG_BUF_SIZE_ALPHA_8BIT(w, h) + 4 * 256)

/**********************
 *      TYPEDEFS
 **********************/
typedef struct {
    lv_color_t res_color;
    lv_opa_t res_opa;

    const void * src;
    lv_coord_t src_w;
    lv_coord_t src_h;
    lv_coord_t pivot_x;
    lv_coord_t pivot_y;
    lv_coord_t pivot_x_256;
    lv_coord_t pivot_y_256;
    lv_img_dsc_t img_dsc;
    int32_t sinma;
    int32_t cosma;
    int16_t angle;
    lv_color_t color;
    lv_img_cf_t cf;

    uint8_t chroma_keyed :1;
    uint8_t has_alpha :1;
    uint8_t native_color :1;

}lv_img_rotate_dsc_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Draw an image
 * @param coords the coordinates of the image
 * @param mask the image will be drawn only in this area
 * @param src pointer to a lv_color_t array which contains the pixels of the image
 * @param style style of the image
 * @param opa_scale scale down all opacities by the factor
 */
void lv_draw_img(const lv_area_t * coords, const lv_area_t * mask, const void * src, const lv_style_t * style,
                 uint16_t angle, lv_opa_t opa_scale);

/**
 * Get the type of an image source
 * @param src pointer to an image source:
 *  - pointer to an 'lv_img_t' variable (image stored internally and compiled into the code)
 *  - a path to a file (e.g. "S:/folder/image.bin")
 *  - or a symbol (e.g. LV_SYMBOL_CLOSE)
 * @return type of the image source LV_IMG_SRC_VARIABLE/FILE/SYMBOL/UNKNOWN
 */
lv_img_src_t lv_img_src_get_type(const void * src);


/**
 * Get the color of an image's pixel
 * @param dsc an image descriptor
 * @param x x coordinate of the point to get
 * @param y x coordinate of the point to get
 * @param color the color of the image. In case of `LV_IMG_CF_ALPHA_1/2/4/8` this color is used.
 * Not used in other cases.
 * @return color of the point
 */
lv_color_t lv_img_buf_get_px_color(lv_img_dsc_t * dsc, lv_coord_t x, lv_coord_t y, lv_color_t color);


/**
 * Get the alpha value of an image's pixel
 * @param dsc pointer to an image descriptor
 * @param x x coordinate of the point to set
 * @param y x coordinate of the point to set
 * @return alpha value of the point
 */
lv_opa_t lv_img_buf_get_px_alpha(lv_img_dsc_t * dsc, lv_coord_t x, lv_coord_t y);


void lv_img_rotate_init(lv_img_rotate_dsc_t * dsc, int16_t angle, const void * src, lv_coord_t src_w, lv_coord_t src_h,
                        lv_img_cf_t cf, lv_coord_t pivot_x, lv_coord_t pivot_y, lv_color_t color);

bool lv_img_get_px_rotated(lv_img_rotate_dsc_t * dsc, lv_coord_t x, lv_coord_t y);

/**
 * Set the color of a pixel of an image. The alpha channel won't be affected.
 * @param dsc pointer to an image descriptor
 * @param x x coordinate of the point to set
 * @param y x coordinate of the point to set
 * @param c color of the point
 */
void lv_img_buf_set_px_color(lv_img_dsc_t * dsc, lv_coord_t x, lv_coord_t y, lv_color_t c);

/**
 * Set the alpha value of a pixel of an image. The color won't be affected
 * @param dsc pointer to an image descriptor
 * @param x x coordinate of the point to set
 * @param y x coordinate of the point to set
 * @param opa the desired opacity
 */
void lv_img_buf_set_px_alpha(lv_img_dsc_t * dsc, lv_coord_t x, lv_coord_t y, lv_opa_t opa);

/**
 * Set the palette color of an indexed image. Valid only for `LV_IMG_CF_INDEXED1/2/4/8`
 * @param dsc pointer to an image descriptor
 * @param id the palette color to set:
 *   - for `LV_IMG_CF_INDEXED1`: 0..1
 *   - for `LV_IMG_CF_INDEXED2`: 0..3
 *   - for `LV_IMG_CF_INDEXED4`: 0..15
 *   - for `LV_IMG_CF_INDEXED8`: 0..255
 * @param c the color to set
 */
void lv_img_buf_set_palette(lv_img_dsc_t * dsc, uint8_t id, lv_color_t c);

/**
 * Get the pixel size of a color format in bits
 * @param cf a color format (`LV_IMG_CF_...`)
 * @return the pixel size in bits
 */
uint8_t lv_img_color_format_get_px_size(lv_img_cf_t cf);

/**
 * Check if a color format is chroma keyed or not
 * @param cf a color format (`LV_IMG_CF_...`)
 * @return true: chroma keyed; false: not chroma keyed
 */
bool lv_img_color_format_is_chroma_keyed(lv_img_cf_t cf);

/**
 * Check if a color format has alpha channel or not
 * @param cf a color format (`LV_IMG_CF_...`)
 * @return true: has alpha channel; false: doesn't have alpha channel
 */
bool lv_img_color_format_has_alpha(lv_img_cf_t cf);

/**
 * Allocate an image buffer in RAM
 * @param w width of image
 * @param h height of image
 * @param cf a color format (`LV_IMG_CF_...`)
 * @return an allocated image, or NULL on failure
 */
lv_img_dsc_t *lv_img_buf_alloc(lv_coord_t w, lv_coord_t h, lv_img_cf_t cf);

/**
 * Free an allocated image buffer
 * @param dsc image buffer to free
 */
void lv_img_buf_free(lv_img_dsc_t *dsc);

/**
 * Get the memory consumption of a raw bitmap, given color format and dimensions.
 * @param w width
 * @param h height
 * @param cf color format
 * @return size in bytes
 */
uint32_t lv_img_buf_get_img_size(lv_coord_t w, lv_coord_t h, lv_img_cf_t cf);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_TEMPL_H*/
