#include <iostream>
#define PY_SSIZE_T_CLEAN
#include <Python.h>

// #include <dlfcn.h>
#include <stdexcept>

#include "imgui_python_binding.h"
#include "plugin_loader.h"
#include "plugin_manager.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <map>

GLFWwindow* window;
PluginManager pluginManager;


void init_bindings() {
    PyImport_AppendInittab(ImguiPythonBinding::moduleName, ImguiPythonBinding::PyInit_addon);
}

void cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

void init() {
    // Initialize GLFW and create a window
    glfwInit();
    window = glfwCreateWindow(800, 600, "Plugin loader example", NULL, NULL);
    glfwMakeContextCurrent(window);

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // try {
        pluginManager.init();
    // } catch (std::runtime_error e) {
    //     //cleanup();
    // }
}

// Main loop
void loop() {
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Create a ImGui window
        // ImGui::Text("This is a ImGui window!");
        
        // Call update functions of all loaded plugins
        for (auto&& p : pluginManager.plugins) {
            ImGui::Begin(p->name.c_str());
            p->update();
            ImGui::End();
        }


        // ImGui::Begin("MyWindow");
        // // ImGui::GetWindowDrawList()->AddRect(ImVec2(10, 10), ImVec2(10 + 60, 10 + 60), IM_COL32(255, 0, 0, 255));
        // ImGui::GetForegroundDrawList()->AddRect(ImVec2(50, 50), ImVec2(50 + 100, 50 + 100), IM_COL32(255, 0, 0, 255));
        // ImGui::End();

        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
}



int main(int argc, char *argv[]) {

    init();
    loop();
    cleanup();

    return 0;
}