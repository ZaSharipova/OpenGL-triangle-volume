#ifndef SHADERS_H_
#define SHADERS_H_

const char* trianglefVertexShaderSrc = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "layout (location = 2) in float aFlag;\n"
    "out float vFlag;"
    "out vec3 vFlagPos;\n"
    "out vec3 vNormal;\n"
    "uniform mat4 model;"
    "uniform mat4 view;"
    "uniform mat4 projection;"

    "void main() {\n"
    "   vFlag = aFlag;\n"
    "   vNormal = aNormal;\n"
    "   vFlagPos = vec3(model * vec4(aPos, 1.0));\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0);"
    "}";

const char* trianglefFragmentShaderSrc = "#version 330 core\n"
    "in float vFlag;\n"
    "in vec3 vFlagPos;\n"
    "in vec3 vNormal;\n"
    "out vec4 FlagColor;\n"

    "void main() {\n"
    "   vec3 lightPos = vec3(0.0, 5.0, 5.0);\n"
    "   vec3 lightColor = vec3(1.0);\n"
    "   float ambientStrength = 0.2;\n"
    "   vec3 ambient = ambientStrength * lightColor;\n"

    "   vec3 N = normalize(vNormal);\n"
    "   vec3 L = normalize(lightPos - vFlagPos);\n"
    "   float diff = max(dot(N, L), 0.0);\n"
    "   vec3 diffuse = diff * lightColor;\n"
    "   vec3 color = vFlag > 0 ? vec3(1.0, 0.2, 0.2) : vec3(0.2, 0.6, 1.0);\n"
    "   vec3 result = (ambient + diffuse) * color;\n"
    "   FlagColor = vec4(result, 1.0);"
    "}";

const char* cubeVertexShaderSrc = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform mat4 model;"
    "uniform mat4 view;"
    "uniform mat4 projection;"

    "void main() {\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0);"
    "}";

const char* cubeFragmentShaderSrc = "#version 330 core\n"
    "out vec4 FlagColor;\n"

    "void main() {\n"
    "   FlagColor = vec4(1.0, 1.0, 1.0, 1.0);"
    "}";

#endif // SHADERS_H_
