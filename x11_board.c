#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int x11_fd;
fd_set in_fds;
struct timeval tv;
void write_to(char * x)
{
  FILE *file = fopen("coordinates", "a");
  if (file == NULL) {
    perror("Error opening file");
    return;
  }
  fprintf(file, x);
  fclose(file);
}

void read_png_file(const char *filename, unsigned char **image_data, int *width, int *height) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) { perror("File opening failed"); return; }
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);
    if (setjmp(png_jmpbuf(png))) { perror("PNG error"); return; }

    png_init_io(png, fp);
    png_read_info(png, info);

    *width = png_get_image_width(png, info);
    *height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    if (bit_depth == 16) png_set_strip_16(png);
    if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    png_set_gray_to_rgb(png);
    png_read_update_info(png, info);

    *image_data = malloc(4 * (*width) * (*height));
    png_bytep *row_pointers = malloc(sizeof(png_bytep) * (*height));
    for (int y = 0; y < *height; y++) {
        row_pointers[y] = *image_data + y * (*width) * 4;
    }
    png_read_image(png, row_pointers);
    fclose(fp);
    png_destroy_read_struct(&png, &info, NULL);
    free(row_pointers);
}

int main(int argc, char *argv[]) {
    unsigned char *image_data;
    int width, height;
    read_png_file("board.png", &image_data, &width, &height);
    Display *dpy = XOpenDisplay(NULL);
    if (!dpy) { fprintf(stderr, "Cannot open display\n"); return 1; }
    int screen = DefaultScreen(dpy);
    Window root = RootWindow(dpy, screen);
    Window win = XCreateSimpleWindow(dpy, root, 0, 0, width, height, 1, BlackPixel(dpy, screen), WhitePixel(dpy, screen));
    XSelectInput(dpy, win, ExposureMask | KeyPressMask | ButtonPressMask);
    Visual *visual = DefaultVisual(dpy, screen);
    int depth = DefaultDepth(dpy, screen);
    XMapWindow(dpy, win);
    XImage *ximage = XCreateImage(dpy, visual, depth, ZPixmap, 0, (char *)image_data, width, height, 32, 0);
    XEvent e;
    x11_fd = ConnectionNumber(dpy);
    int done = 0;
    int i;
    char text[10];
  KeySym mykey;
  char s[64];
    while(done == 0) {
        FD_ZERO(&in_fds);
        FD_SET(x11_fd, &in_fds);
        tv.tv_usec = 100000;
        tv.tv_sec = 0;
        int num_ready_fds = select(x11_fd + 1, &in_fds, NULL, NULL, &tv);
        if (num_ready_fds > 0)
          {
            while(XPending(dpy))
              XNextEvent(dpy, &e);
            switch(e.type){
            case ButtonPress:
              sprintf(s, "%i, %i", e.xbutton.x, e.xbutton.y);
              write_to(s);
              break;
            case KeyPress:
              i = XLookupString(&e.xkey, text, 10, &mykey, 0);
              if(i==1 && text[0]=='q') done = 1;
              if(i==1 && text[0]=='s') write_to("score\n");
              if(i==1 && text[0]=='u') write_to("undo\n");
              if(i==1 && text[0]==27) done = 1;//esc key
              break;
            }
          }
        else if (num_ready_fds == 0){
          read_png_file("board.png", &image_data, &width, &height);
          ximage = XCreateImage(dpy, visual, depth, ZPixmap, 0, (char *)image_data, width, height, 32, 0);
          XPutImage(dpy, win, DefaultGC(dpy, screen), ximage, 0, 0, 0, 0, width, height);
        } else
            printf("An error occured!\n");
    }
    write_to("quit\n");
    XDestroyImage(ximage);  // Frees image_data too
    XCloseDisplay(dpy);
    return 0;
}
    
