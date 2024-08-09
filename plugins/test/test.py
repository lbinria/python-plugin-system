# This python module will be loaded as an add-on

import imgui
# Define all method hooks for addon lifecycle
# These hook will be called by the client when needed

# Called when addon loaded
def load():
    print("load test plugin !")

# Called when addon unload
def unload():
    print("unload test plugin !")

def start():
    print("addon starts !")

# Called each times interface need to draw (e.g imgui loop) 
# Draw interface
def update():
    if imgui.button("Yop !"):
        print("Hello world !")