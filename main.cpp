#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <chrono>
#include <iostream>

class MinimalLauncher {
private:
    Display* display;
    Window window;
    XIC ic;
    XIM im;
    GC gc;
    int screen;
    char text[256] = {0};
    int text_len = 0;
    std::chrono::steady_clock::time_point start_time;

public:
    MinimalLauncher() {
        start_time = std::chrono::steady_clock::now();

        display = XOpenDisplay(nullptr);
        screen = DefaultScreen(display);

        // Create window
        window = XCreateSimpleWindow(
            display, RootWindow(display, screen),
            0, 0, 600, 40,  // x, y, width, height
            1, BlackPixel(display, screen),
            WhitePixel(display, screen));

        // Set window properties
        XSelectInput(display, window, ExposureMask | KeyPressMask);
        XStoreName(display, window, "Launch");

        // Center window
        int x = (DisplayWidth(display, screen) - 600) / 2;
        int y = (DisplayHeight(display, screen) - 40) / 2;
        XMoveWindow(display, window, x, y);

        // Setup input
        im = XOpenIM(display, nullptr, nullptr, nullptr);
        ic = XCreateIC(im, XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
                      XNClientWindow, window, XNFocusWindow, window, nullptr);

        // Setup graphics context
        gc = XCreateGC(display, window, 0, nullptr);
        XSetForeground(display, gc, BlackPixel(display, screen));

        // Show window
        XMapWindow(display, window);

        auto end_time = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            end_time - start_time).count();
        std::cerr << "UI ready in " << duration << "ms\n";
    }

    void run() {
        XEvent event;
        KeySym key;
        Status status;
        char buf[20];

        while (true) {
            XNextEvent(display, &event);

            switch (event.type) {
                case Expose:
                    redraw();
                    break;

                case KeyPress:
                    if (XFilterEvent(&event, None))
                        break;

                    int len = XmbLookupString(ic, &event.xkey, buf, 20, &key, &status);

                    if (key == XK_Return) {
                        if (text_len > 0) {
                            execute_command();
                            return;
                        }
                    } else if (key == XK_Escape) {
                        return;
                    } else if (key == XK_BackSpace) {
                        if (text_len > 0) text_len--;
                        redraw();
                    } else if (len == 1 && text_len < 255) {
                        text[text_len++] = buf[0];
                        text[text_len] = 0;
                        redraw();
                    }
                    break;
            }
        }
    }

    ~MinimalLauncher() {
        XDestroyIC(ic);
        XCloseIM(im);
        XFreeGC(display, gc);
        XDestroyWindow(display, window);
        XCloseDisplay(display);
    }

private:
    void redraw() {
        XClearWindow(display, window);
        XDrawString(display, window, gc, 10, 25, text, text_len);
    }

    void execute_command() {
        if (fork() == 0) {
            setsid();
            execl("/bin/sh", "sh", "-c", text, nullptr);
            exit(0);
        }
    }
};

int main() {
    MinimalLauncher launcher;
    launcher.run();
    return 0;
}
