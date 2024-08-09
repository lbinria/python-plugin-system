#pragma once 

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <string>

class Plugin {
	
	private:

	PyObject * pModule;
	PyObject * pUnloadFunc;
	PyObject * pStartFunc;
	PyObject * pUpdateFunc;

	public:

		Plugin(std::string pluginName);
		~Plugin();

		// Hooks
		void start();
		void update();

		std::string name;
		
};