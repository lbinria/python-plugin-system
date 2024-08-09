# This python module will be loaded as an add-on

# Load my C++ code
from ctypes import cdll, c_char_p
lib = cdll.LoadLibrary("plugins/smoother/libsmoother.so")

import imgui
# Define all method hooks for addon lifecycle
# These hook will be called by the client when needed

# Called when addon loaded
def load():
    print("load smoother plugin !")

# Called when addon unload
def unload():
    print("unload smoother plugin !")

def start():
    print("addon starts !")

def smooth(filename, n_iter):
    p_filename = c_char_p(filename.encode())
    p_lock_attr_name = c_char_p("".encode())
    p_result_path = c_char_p("".encode())
    return lib.smooth_c(p_filename, p_lock_attr_name, n_iter, p_result_path) # Return formatted results that can be read by client (here our modeler)

# Called each times interface need to draw (e.g imgui loop) 
# Draw interface
def update():
    filename = imgui.input_text("filename")
    n_iter = imgui.input_int("iterations")
    # n_iter = 1

    if imgui.button("Smooth !"):
        smooth(filename, n_iter)
