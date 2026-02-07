#include "Window.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Model Environment";

// ImGui Joints
std::vector<Joint*> Window::joints;

// Objects to render
//Cube* Window::cube;
Skeleton* Window::skel;
Skin* Window::skin;

// Camera Properties
Camera* Cam;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;

// Constructors and desctructors
bool Window::initializeProgram() {
    // Create a shader program with a vertex shader and a fragment shader.
    shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

    // Check the shader program.l
    if (!shaderProgram) {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

    return true;
}

bool Window::initializeObjects(const char* skelFile, const char* skinFile) {
    // Load skeleton
    skel = new Skeleton();
    skin = new Skin();

	//const char* skinFileName = "C:/Users/Aslan/Classes/2025-2026/Winter/CSE_169/project_1/skins/tube.skin";
    std::string skelFilePath = std::string("C:/Users/Aslan Chan/Classes/2025-2026/CSE_169_project_1/cse169_project_1/models/") + skelFile;
    skel->Load(skelFilePath.c_str());

    if (skinFile != nullptr) {
	    std::string skinFilePath = std::string("C:/Users/Aslan Chan/Classes/2025-2026/CSE_169_project_1/cse169_project_1/models/") + skinFile;
        skin->Load(skinFilePath.c_str());
    }
    else {
        skin = nullptr;
    }

    /*if (filename != "") {
        skel->Load(filename);
        skin->Load(skinFileName);
    }
    else {
    }*/

    return true;
}

void Window::cleanUp() {
    // Deallcoate the objects.
    delete skel;

    if (skin != nullptr) {
        delete skin;
    }

    // Delete the shader program.
    glDeleteProgram(shaderProgram);
}

// for the Window
GLFWwindow* Window::createWindow(int width, int height) {
    // Initialize GLFW.
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }

    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create the GLFW window.
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

    // Check if the window could not be created.
    if (!window) {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }

    // Make the context of the window.
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewInit();

    // Set swap interval to 1.
    glfwSwapInterval(0);

    // set up the camera
    Cam = new Camera();
    Cam->SetAspect(float(width) / float(height));

    // initialize the interaction variables
    LeftDown = RightDown = false;
    MouseX = MouseY = 0;

    // Call the resize callback to make sure things get drawn immediately.
    Window::resizeCallback(window, width, height);

    return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height) {
    Window::width = width;
    Window::height = height;
    // Set the viewport size.
    glViewport(0, 0, width, height);

    Cam->SetAspect(float(width) / float(height));
}

// update and draw functions
void Window::idleCallback() {
    // Perform any updates as necessary.
    Cam->Update();

    skel->Update();

    if (skin != nullptr) {
	    skin->Update();
    }
}

void Window::displayCallback(GLFWwindow* window) {
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the object.
    if (skin != nullptr) {
	    skin->Draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
    }
    else {
        skel->Draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
    }

    renderImGui(window);

    // Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
}

void Window::renderImGui(GLFWwindow* window) {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        /*static float f1 = 0.0f;
        static float f2 = 0.5f;
        std::vector<char*> jointNames = {"Joint 1", "Joint 2"};
        std::vector<float*> jointValAdd = {&f1, &f2};*/

        ImGui::Begin("Skeleton Rigging");

        //ImGui::SetWindowSize(ImVec2(200.0f, 100.0f));

        for (int i = 0; i < joints.size(); i++)
        {
            /*
            bool canMoveX = (joints[i]->dof[0]->min > -100000. || joints[i]->dof[0]->max < 100000.)
                                && (joints[i]->dof[0]->max - joints[i]->dof[0]->min != 0);
            bool canMoveY = (joints[i]->dof[1]->min > -100000. || joints[i]->dof[1]->max < 100000.)
                                && (joints[i]->dof[1]->max - joints[i]->dof[1]->min != 0);
            bool canMoveZ = (joints[i]->dof[2]->min > -100000. || joints[i]->dof[2]->max < 100000.)
                                && (joints[i]->dof[2]->max - joints[i]->dof[2]->min != 0);
            */

            if(ImGui::CollapsingHeader(joints[i]->name.c_str())) {
                //if(canMoveX) {
                    std::string xLabel = "x##";
                    xLabel.append(std::to_string(i));
                    ImGui::SliderFloat(xLabel.c_str(), &joints[i]->dof[0]->value, joints[i]->dof[0]->min, joints[i]->dof[0]->max);
                //}

                //if(canMoveY) {
                    std::string yLabel = "y##";
                    yLabel.append(std::to_string(i));
                    ImGui::SliderFloat(yLabel.c_str(), &joints[i]->dof[1]->value, joints[i]->dof[1]->min, joints[i]->dof[1]->max);
                //}

                //if(canMoveZ) {
                    std::string zLabel = "z##";
                    zLabel.append(std::to_string(i));
                    ImGui::SliderFloat(zLabel.c_str(), &joints[i]->dof[2]->value, joints[i]->dof[2]->min, joints[i]->dof[2]->max);
                //}
            }
            
        }

        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// helper to reset the camera
void Window::resetCamera() {
    Cam->Reset();
    Cam->SetAspect(float(Window::width) / float(Window::height));
}

// callbacks - for Interaction
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    /*
     * TODO: Modify below to add your key callbacks.
     */

    // Check for a key press.
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;

            case GLFW_KEY_R:
                resetCamera();
                break;

            default:
                break;
        }
    }
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    ImGuiIO& io = ImGui::GetIO();
    if(io.WantCaptureMouse) return;

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        LeftDown = (action == GLFW_PRESS);
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        RightDown = (action == GLFW_PRESS);
    }
}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {
    int maxDelta = 100;
    int dx = glm::clamp((int)currX - MouseX, -maxDelta, maxDelta);
    int dy = glm::clamp(-((int)currY - MouseY), -maxDelta, maxDelta);

    MouseX = (int)currX;
    MouseY = (int)currY;

    // Move camera
    // NOTE: this should really be part of Camera::Update()
    if (LeftDown) {
        const float rate = 1.0f;
        Cam->SetAzimuth(Cam->GetAzimuth() + dx * rate);
        Cam->SetIncline(glm::clamp(Cam->GetIncline() - dy * rate, -90.0f, 90.0f));
    }
    if (RightDown) {
        const float rate = 0.005f;
        float dist = glm::clamp(Cam->GetDistance() * (1.0f - dx * rate), 0.01f, 1000.0f);
        Cam->SetDistance(dist);
    }
}