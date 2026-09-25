#ifndef _INCLUDE_JPEGUTILS_HPP_
#define _INCLUDE_JPEGUTILS_HPP_

int jpgutl_decode_jpeg(unsigned char *jpeg_data_in, int jpeg_data_len,
                       unsigned int width, unsigned int height,
                       unsigned char *volatile img_out);
int jpgutl_put_yuv420p(unsigned char *dest_image, int image_size,
                       unsigned char *input_image, int width, int height,
                       int quality, Camera_t *cam, timespec *ts1,
                       ctx_coord *box);
int jpgutl_put_grey(unsigned char *dest_image, int image_size,
                    unsigned char *input_image, int width, int height,
                    int quality, Camera_t *cam, timespec *ts1, ctx_coord *box);
uint jpgutl_exif(u_char **exif, Camera_t *cam, timespec *ts_in1,
                 ctx_coord *box);

#endif /*  _INCLUDE_JPEGUTILS_HPP_ */
