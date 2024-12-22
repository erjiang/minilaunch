#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <chrono>
#include <iostream>
#include <algorithm>
#include "launchmath.h"
#include "units.h"

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
    std::string result_text;
    bool has_result = false;

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

        // Setup graphics context first
        gc = XCreateGC(display, window, 0, nullptr);
        XSetForeground(display, gc, BlackPixel(display, screen));

        // Then load and set the larger font
        Font font = XLoadFont(display, "-*-*-*-*-*-*-20-*-*-*-*-*-*-*");
        XSetFont(display, gc, font);

        // Center window
        int x = (DisplayWidth(display, screen) - 600) / 2;
        int y = (DisplayHeight(display, screen) - 40) / 2;
        XMoveWindow(display, window, x, y);

        // Setup input
        im = XOpenIM(display, nullptr, nullptr, nullptr);
        ic = XCreateIC(im, XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
                      XNClientWindow, window, XNFocusWindow, window, nullptr);

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

                    // Check for Ctrl+A to clear input
                    if ((event.xkey.state & ControlMask) && (key == XK_a || key == XK_A)) {
                        text_len = 0;
                        text[0] = 0;
                        redraw();
                        break;
                    }

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
    void checkForResult() {
        has_result = false;
        result_text.clear();

        if (text_len <= 0 || text[0] == '\0') {
            return;
        }

        std::string input(text, text_len);

        // Check if it's a math expression
        if ((std::isdigit(text[0]) || text[0] == '(') &&
            std::any_of(text, text + text_len,
            [](char c) { return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '(' || c == ')'; })) {

            // More thorough validation of the expression
            bool valid = true;

            // Check for incomplete expressions
            if (input.back() == '+' || input.back() == '-' ||
                input.back() == '*' || input.back() == '/' ||
                input.back() == '^') {
                return;
            }

            // Check for unmatched parentheses
            int parentheses = 0;
            for (char c : input) {
                if (c == '(') parentheses++;
                if (c == ')') parentheses--;
                if (parentheses < 0) {
                    valid = false;
                    break;
                }
            }
            if (parentheses != 0) valid = false;

            if (!valid) return;

            auto result = evaluateExpression(input);
            if (result.has_value()) {
                result_text = " = " + std::to_string(*result);
                has_result = true;
                std::cerr << "Math result: " << *result << std::endl;
            }
        }

        // Then check if it's a units conversion
        if (isUnitsConversion(input)) {
            size_t pos = input.find(" in ");
            if (pos != std::string::npos) {
                std::string fromUnit = input.substr(0, pos);
                std::string toUnit = input.substr(pos + 4);
                try {
                    auto result = convertUnits(fromUnit, toUnit);
                    if (result.has_value()) {
                        result_text = " = " + std::to_string(*result);
                        has_result = true;
                    }
                } catch (...) {
                    // Conversion failed
                }
            }
        }
    }

    void redraw() {
        checkForResult();
        XClearWindow(display, window);

        // Draw the main input text in black
        XSetForeground(display, gc, BlackPixel(display, screen));
        XDrawString(display, window, gc, 10, 25, text, text_len);

        // If we have a result, draw it in blue after the input
        if (has_result) {
            // Get the width of the input text
            int input_width = XTextWidth(XQueryFont(display, XGContextFromGC(gc)), text, text_len);

            // Set color to blue (RGB: 0, 0, 255)
            XSetForeground(display, gc, 0x0000FF);

            // Draw result after the input text with some padding
            int result_x = 10 + input_width + 10;  // 10px padding between input and result
            XDrawString(display, window, gc, result_x, 25,
                       result_text.c_str(), result_text.length());

            // Reset color back to black
            XSetForeground(display, gc, BlackPixel(display, screen));
        }
    }

    void execute_command() {
        // First check if it's a math expression
        if (std::any_of(text, text + text_len,
            [](char c) { return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '(' || c == ')'; })) {
            try {
                auto result = evaluateExpression(std::string(text, text_len));
                if (result.has_value()) {
                    // Convert result to string and display it
                    std::string resultStr = std::to_string(*result);
                    text_len = std::min(resultStr.length(), sizeof(text) - 1);
                    std::copy(resultStr.begin(), resultStr.begin() + text_len, text);
                    text[text_len] = 0;
                    redraw();
                    return;
                }
            } catch (...) {
                // If math evaluation fails, treat as normal command
            }
        }

        // If not a math expression or math failed, execute as command
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
