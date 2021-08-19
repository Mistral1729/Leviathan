//Include files
#include "tiger_engine.hpp"

namespace te 
{
    const char *vertex_shader_source = "#version 460 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    const char *fragment_shader_source = "#version 460 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";
}

//Main method
int main()
{
    //GLFW init and hint methods
    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //Create GLFW window
    //Pass glfwGetPrimaryMonitor() as GLFWmonitor pointer object in params to get fullscreen
    GLFWwindow* window = glfwCreateWindow(te::SCREEN_WIDTH, te::SCREEN_HEIGHT, "TigerEngine", NULL, NULL); 
    if (window == NULL)
    {
        std::cout << "ALERT : Failed to instantiate GLFW window." << '\n';
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, te::framebuffer_size_callback);

    //Load all OpenGL function pointers via glad
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "ALERT : Failed to initialise GLAD." << std::endl;
        return -1;
    }

    /**
     * Compile vertex shader source code
     * **/
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &te::vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cout << "ALERT : Vertex shader compilation unsuccessful.\n" << infoLog << std::endl;
    }
    
    /**
     * Compile fragment shader source code
     * **/
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &te::fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        std::cout << "ALERT : Fragment shader compilation unsuccessful.\n" << infoLog << std::endl;
    }

    /**
     * Link shader programs
     * **/
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        std::cout << "ALERT : Shader program linking unsuccessful.\n" << infoLog << std::endl;
    }
    glDeleteShader(vertex_shader); glDeleteShader(fragment_shader);
    
    /**
     * Vertex data and buffer setup, and vertex attribute config
     * **/
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    while (!glfwWindowShouldClose(window))
    {
        /**
         * Input
         */
        te::process_input(window);

        /**
         * Render
         */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /**
         * Swap buffers and poll I/O events
         */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}