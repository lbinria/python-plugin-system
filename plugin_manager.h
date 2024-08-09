#pragma once
#include "plugin_loader.h"

#include <vector>
#include <memory>

struct PluginManager {

	PluginManager();
	~PluginManager();

	void init();
	std::vector<std::unique_ptr<Plugin>> plugins;
	// std::unique_ptr<Plugin> smoother = nullptr;

};