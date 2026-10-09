#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
public:
    static constexpr int kWindowWidth = 800;
    static constexpr int kWindowHeight = 600;

    Window(int width, int height, const char *title);

    ~Window() {
        Destroy();
    }

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    GLFWwindow* GetWindow();
    int& GetSavedWidth();
    int& GetSavedHeight();
    int& GetSavedX();
    int& GetSavedY();
    bool& GetIsFullScreen();

    void SetCursorCaptured(bool captured);
    bool IsCursorCaptured() const;

    void ToggleFullScreen();

private:
    void Destroy() {
        if (window_) {
            glfwDestroyWindow(window_);
            window_ = nullptr;
            glfwTerminate();
        }
    }

    GLFWwindow* window_ = nullptr;
    bool isFullScreen_ = false;
    bool cursorCaptured_ = true;

    int savedWidth_ = 0, savedHeight_ = 0;
    int savedX_ = 0, savedY_ = 0;
};

#endif // WINDOW_HPP_
