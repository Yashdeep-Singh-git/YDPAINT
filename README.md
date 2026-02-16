# PaintLite -- Micro Vector Editor (SVG)

A custom vector graphics editor developed for COP 290.\
This application allows users to create, modify, and manage vector
diagrams using a restricted subset of the SVG format.

------------------------------------------------------------------------

## 🛠 Build & Run Instructions

### Requirements

-   Qt6 (Community Edition)
-   CMake
-   Make / GCC (or compatible compiler)

### Build Steps

``` bash
mkdir build
cd build
cmake ..
make
```

### Run

``` bash
./PaintLite
```

------------------------------------------------------------------------

## 📂 Project Structure

    .
    ├── CMakeLists.txt              # Build configuration
    ├── include/                    # Header files
    │   ├── gui/                    # UI component headers
    │   ├── models/                 # Shape class definitions
    │   └── utils/                  # Parser utilities
    ├── src/                        # Implementation files
    │   ├── gui/                    # Canvas and MainWindow logic
    │   ├── models/                 # Shape implementations
    │   └── utils/                  # SVG parser logic
    ├── resources/                  # Icons and UI assets
    ├── resources.qrc               # Qt resource file
    └── README.md                   # Project documentation

------------------------------------------------------------------------

## 🎨 Features & Usage

### Toolbar Functions

**Select Tool** - Click to select the top-most object. - Move:
Left-click and drag. - Resize: Right-click and drag.

**Shape Tools** - Circle - Rectangle - Rounded Rectangle - Line -
Hexagon - Freehand (manual drawing) - Text (insert text objects)

**Attributes** - Stroke Color (set before drawing or after selecting) -
Fill Color (set before drawing or after selecting) - Stroke Width (set
before drawing or after selecting) - Corner Radius (for Rounded
Rectangles; adjustable before and after selection)

------------------------------------------------------------------------

### Menubar

**File** - New -- Clears canvas - Open -- Loads SVG file - Save As --
Saves current diagram as SVG - Exit -- Closes application

If there are unsaved changes, the system prompts to save before
switching or exiting.

**Edit** - Cut (Ctrl + X) - Copy (Ctrl + C) - Paste (Ctrl + V) - Delete
(Delete / Backspace) - Undo (Ctrl + Z) - Redo (Ctrl + Y or Ctrl +
Shift + Z)

------------------------------------------------------------------------
