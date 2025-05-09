# stationfiftyone

**stationfiftyone** is a project to develop a modular indoor (and later outdoor) environmental monitoring station.  
The station is intended to measure **CO₂ levels, temperature, humidity, and more**, and present that data in a compact, clear format — directly on an RGB LED matrix.

In the **first stage**, the focus is on bringing up a **Waveshare RGB LED Matrix Panel** with a resolution of **96×48 pixels** and a **1/24 scan rate**, driven directly from a **Raspberry Pi 5** using custom bitbanging logic.

<pre> 
stationfiftyone/
├── .vscode/
│   └── c_cpp_properties.json
│   └── launch.json
│   └── tasks.json
├── src/
│   └── defines.h
│   └── hub75_driver.cpp
│   └── hub75_driver.h
│   └── main.cpp
│   └── panel_config.cpp
│   └── panel_config.h
├── Makefile
└── README.md 
</pre>
