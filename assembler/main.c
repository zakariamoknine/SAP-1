#include <X11/Xlib.h>

int main(int argc, char** argv)
{
    Display* display = XOpenDisplay(NULL);
    int screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);
    
    Window window = XCreateSimpleWindow(display, root, 10, 10, 800, 800, 1, BlackPixel(display, screen), WhitePixel(display, screen));

    XSelectInput(display, window, ExposureMask | KeyPressMask);

    XMapWindow(display, window);

    int running = 1;
    XEvent event;
    while (running)
    {
        XNextEvent(display, &event);
        if (event.type == Expose)
        {

        }
        if (event.type == KeyPress)
        {
            running = 0;
        }
    }

    XCloseDisplay(display);
}
