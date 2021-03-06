#include <stdlib.h>
#include <vips/vips.h>

int resizer_init(const int ncpu){
  if(VIPS_INIT("filestash")){
    return 1;
  }
  vips_concurrency_set(1);
  vips_cache_set_max(0);
  return 0;
}

int resizer_process(const char *input, const char *output, int size, int crop, int quality, int exif){
  VipsImage *img;
  int err;

  size = size > 4000 || size < 0 ? 1000 : size;
  crop = crop == 0 ? VIPS_INTERESTING_NONE : VIPS_INTERESTING_CENTRE;
  quality = quality > 100 || quality < 0 ? 80 : quality;
  exif = exif == 0 ? TRUE : FALSE;

  if(crop == VIPS_INTERESTING_CENTRE){
    // Generate a thumbnails: a square picture crop in the center
    err = vips_thumbnail(input, &img, size,
        "size", VIPS_SIZE_BOTH,
        "auto_rotate", TRUE,
        "crop", VIPS_INTERESTING_CENTRE,
        NULL
    );
  }else{
    // normal resize of an image with libvips
    err = vips_thumbnail(input, &img, size,
        "size", VIPS_SIZE_DOWN,
        "auto_rotate", TRUE,
        "crop", VIPS_INTERESTING_NONE,
        NULL
    );
  }
  if(err != 0){
    return err;
  }

  err = vips_jpegsave(img, output, NULL);
  g_object_unref(img);
  return err;
}
