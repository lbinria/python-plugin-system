#include "plugin_loader.h"

#include <iostream>
#include <stdexcept>

Plugin::Plugin(std::string pluginName) : name{pluginName} {

	// We should add module path to PYTHONPATH in order to retrieve module and their functions
    std::string python_code = std::string("import sys\nsys.path.insert(0,\"plugins/") + pluginName + std::string("\")");
    // std::string python_code = std::string("import sys\nsys.path.append(\"plugins/") + pluginName + std::string("\")"); // It doesn't works and I don't know why ...

    // Compile the Python code into a code object
    PyObject* code_object = Py_CompileString(python_code.c_str(), "<string>", Py_file_input);

    // Evaluate the code object using PyEval_EvalCode
    PyObject* globals = PyDict_New();
    PyObject* locals = PyDict_New();
    PyObject* result = PyEval_EvalCode(code_object, globals, locals);

	// std::cout << "eval:" << python_code << std::endl;


	// Load python module
	PyObject * pName = PyUnicode_DecodeFSDefault(pluginName.c_str());

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

	std::cout << "Try to load " << pluginName << std::endl;
	if (pModule == NULL) {
        PyErr_Print();
		throw std::runtime_error("Failed to load module");
	}

	// Bind functions
	PyObject * pLoadFunc = PyObject_GetAttrString(pModule, "load");
	pUnloadFunc = PyObject_GetAttrString(pModule, "unload");
	pStartFunc = PyObject_GetAttrString(pModule, "start");
	pUpdateFunc = PyObject_GetAttrString(pModule, "update");

	// Search for hooks in python plugin
	if (!(pLoadFunc && PyCallable_Check(pLoadFunc))) {
		if (PyErr_Occurred())
			PyErr_Print();
		
		throw std::runtime_error("Cannot find load function");
	}


	if (!(pUnloadFunc && PyCallable_Check(pUnloadFunc))) {
		if (PyErr_Occurred())
			PyErr_Print();
		
		throw std::runtime_error("Cannot find unload function");
	}

	if (!(pUpdateFunc && PyCallable_Check(pUpdateFunc))) {
		if (PyErr_Occurred())
			PyErr_Print();
		
		throw std::runtime_error("Cannot find update function");
	}


	// Call load function
	PyObject * pArgs = PyTuple_New(0);
	PyObject_CallObject(pLoadFunc, pArgs);
	Py_DECREF(pArgs);
	Py_XDECREF(pLoadFunc);


}

Plugin::~Plugin() {
	// Call unload function
	PyObject * pArgs = PyTuple_New(0);
	PyObject_CallObject(pUnloadFunc, pArgs);
	Py_DECREF(pArgs);
	Py_XDECREF(pUnloadFunc);

	Py_XDECREF(pStartFunc);
	Py_XDECREF(pUpdateFunc);

	Py_DECREF(pModule);
}

// TODO should return message that can be interpreted by the client
void Plugin::update() {
	// Call update
	PyObject * pArgs = PyTuple_New(0);
	PyObject_CallObject(pUpdateFunc, pArgs);
	Py_DECREF(pArgs);

}