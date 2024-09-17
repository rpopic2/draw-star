#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

static char *read_to_string(const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        printf("failed to open %s", path);
        return NULL;
    }
    fseek(file, 0L, SEEK_END);
    long len = ftell(file);
    fseek(file, 0L, SEEK_SET);

    char *buf = malloc(len);
    if (!buf) {
        printf("malloc failed");
        return NULL;
    }
    fread(buf, sizeof(char), len, file);
    fclose(file);

    return buf;
}

void print_err() {
    printf("%d\n", glGetError());
}

int main(void) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow *window = glfwCreateWindow(400, 400, "star", NULL, NULL);
    glfwMakeContextCurrent(window);

    gladLoadGL();
    printf("%s\n", glGetString(GL_VERSION));
    glViewport(0, 0, 400, 400);

    float rad54 = 54 * (M_PI / 180.0l);
    float rad18 = 18 * (M_PI / 180.0l);
    float rad36 = 36 * (M_PI / 180.0l);
    float rad306 = 54 * (M_PI / 180.0l);
    float a = 1.0f;

    float axcos306 = a * cosf(rad306);
    float axsin306 = a * sinf(rad306);
    float axcos18 = a * cosf(rad18);
    float axsin18 = a * sinf(rad18);

    float GOLDEN_RATIO = (1.0l + sqrt(5)) / 2.0l;
    float top_inner = axcos18 - (2 * axcos18 / (GOLDEN_RATIO + 1));

    float b = top_inner * (1 / sinf(rad36));

    GLfloat verticies[] = {
        -axcos306, -axsin306, 0.0f, 1.0f, 1.0f, 1.0f,
        axcos306, -axsin306, 0.0f, 1.0f, 1.0f, 1.0f,
        axcos18, axsin18, 0.0f, 1.0f, 1.0f, 1.0f,
        0.0f, a, 0.0f, 1.0f, 1.0f, 1.0f,
        -axcos18, axsin18, 0.0f, 1.0f, 1.0f, 1.0f,

        0.0f, -b, 0.0f, 0.0f, 0.0f, 0.7f,
        b * cosf(rad18), -b * sinf(rad18), 0.0f, 0.0f, 0.0f, 0.7f,
        top_inner, axsin18, 0.0f, 0.0f, 0.7f, 0.0f,
        -top_inner, axsin18, 0.0f, 0.7f, 0.0f, 0.0f,
        -b * cosf(rad18), -b * sinf(rad18), 0.0f, 0.0f, 0.7f, 0.7f,
    };
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    GLuint indicies[] = {
        0, 5, 9,
        1, 6, 5,
        2, 7, 6,
        3, 8, 7,
        4, 9, 8,

        9, 5, 6,
        8, 9, 6,
        8, 6, 7,
    };
    GLsizei indicies_cnt = sizeof(indicies) / sizeof(GLuint);
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    const char *vert_src = read_to_string("shader.vert");
    const char *frag_src = read_to_string("shader.frag");
    glShaderSource(vert, 1, &vert_src, NULL);
    glShaderSource(frag, 1, &frag_src, NULL);
    glCompileShader(vert);
    glCompileShader(frag);
    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    glDeleteShader(vert);
    glDeleteShader(frag);
    free((void *)vert_src);
    free((void *)frag_src);

    glUseProgram(program);
    glClearColor(0, 0, 0, 1);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, indicies_cnt, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(program);
    glfwDestroyWindow(window);
    glfwTerminate();
}

