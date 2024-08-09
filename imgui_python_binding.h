#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <dlfcn.h>
#include <stdexcept>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <map>

namespace ImguiPythonBinding {

    static std::map<std::string, std::string> input_texts;
    static std::map<std::string, int> input_ints;


    static PyObject* input_text(PyObject *self, PyObject *args) {
        
        const char * label;

        if (!PyArg_ParseTuple(args, "s", &label))
            return NULL;

        if (!input_texts.count(label)) {
            input_texts[label] = "";
            input_texts[label].reserve(512);
        }

        char *text = input_texts[label].data();

        ImGui::InputText(label, text, input_texts[label].capacity());

        return PyUnicode_FromString(text);
    }

    static PyObject* input_int(PyObject *self, PyObject *args) {
        
        const char * label;

        if (!PyArg_ParseTuple(args, "s", &label))
            return NULL;

        if (!input_ints.count(label))
            input_ints[label] = 0;

        int *i = &input_ints[label];

        ImGui::InputInt(label, i, 1, 5, sizeof(*i));

        return PyLong_FromLong(*i);
    }

    static PyObject* button(PyObject *self, PyObject *args) {
        
        const char * label;
        int x = 0, y = 0;

        if (!PyArg_ParseTuple(args, "s|(ii)", &label, &x, &y))
            return NULL;

        ImVec2 size(x, y);

        bool result = ImGui::Button(label, size);

        return PyBool_FromLong(result);
    }

    static PyMethodDef ImguiMethods[] = {
        {"button", button, METH_VARARGS, "Add button to ui"},
        {"input_text", input_text, METH_VARARGS, "Add input text to ui"},
        {"input_int", input_int, METH_VARARGS, "Add input int to ui"},
        {NULL, NULL, 0, NULL}
    };

    static PyModuleDef ImguiModule = {
        PyModuleDef_HEAD_INIT, "imgui", NULL, -1, ImguiMethods,
        NULL, NULL, NULL, NULL
    };

    static PyObject* PyInit_addon(void)
    {
        return PyModule_Create(&ImguiModule);
    }

    static constexpr const char * moduleName = "imgui";

}