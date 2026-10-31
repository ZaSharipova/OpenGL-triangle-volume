#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <memory>
#include <utility>

namespace Interface {

class Window {
public:
    static constexpr int kWindowWidth = 800;
    static constexpr int kWindowHeight = 600;

    Window(int width, int height, const char* title);

    Window(Window&& other) = default;
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
    struct WindowDeleter {
        void operator()(GLFWwindow* window) const {
            if (window) {
                glfwDestroyWindow(window);
            }
        }
    };

    std::unique_ptr<GLFWwindow, WindowDeleter> window_;
    bool isFullScreen_ = false;
    bool cursorCaptured_ = true;

    int savedWidth_ = 0, savedHeight_ = 0;
    int savedX_ = 0, savedY_ = 0;
};

} // namespace Interface

#endif // WINDOW_HPP_
