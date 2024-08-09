#include "plugin_manager.h"
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "imgui_python_binding.h"

#include <iostream>
#include <filesystem>

PluginManager::PluginManager()  {
	std::cout << "create" << std::endl;
}

PluginManager::~PluginManager() {
	std::cout << "destroy" << std::endl;

}

void PluginManager::init() {

	// Expose C++ functions to python plugins through modules
	PyImport_AppendInittab(ImguiPythonBinding::moduleName, ImguiPythonBinding::PyInit_addon);

	// Init python interpreter
    Py_Initialize();

	// Import plugins
    for (auto const& dir_entry : std::filesystem::directory_iterator{"plugins"}) {
		auto pluginName = std::filesystem::path(dir_entry.path()).filename();
		plugins.push_back(std::move(std::make_unique<Plugin>(pluginName)));

	}

	// smoother = std::make_unique<Plugin>("smoother");

}
