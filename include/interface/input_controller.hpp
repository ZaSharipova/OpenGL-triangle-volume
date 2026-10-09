#ifndef INPUT_CONTROLLER_HPP_
#define INPUT_CONTROLLER_HPP_

#include "camera.hpp"
#include "window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class InputController {
public:
    void ProcessInput(Window& window, Camera& camera);
    void ProcessMouse(Camera& camera, double xpos, double ypos);

private:
    void ProcessFullScreenToggle(Window& window, const int nowFState);
    void ProcessMakeCursorFree(Window& window, const int nowTabState);
    void ResetMouse();

    float lastX_ = 400.0f;
    float lastY_ = 300.0f;
    bool firstMouse_ = true;
    float mouseSensitivity_ = 0.1f;

    int prevFState_ = GLFW_RELEASE;
    int prevTabState_ = GLFW_RELEASE;
};

#endif // INPUT_CONTROLLER_HPP_
