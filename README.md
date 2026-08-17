<div align="center">

```
 _    _  ____  __    __       ____  _____  _____  __   
( \/\/ )( ___)(  )  (  )  ___(_  _)(  _  )(  _  )(  )  
 )    (  )__)  )(__  )(__(___) )(   )(_)(  )(_)(  )(__ 
(__/\__)(____)(____)(____)    (__) (_____)(_____)(____)
```

**A frameless, always-on-top desktop toolkit for Windows & Linux — sketchpad, notes, and calculator in one collapsible side panel.**

![C++](https://img.shields.io/badge/C%2B%2B-17-blue?logo=cplusplus&logoColor=white)
![Qt](https://img.shields.io/badge/Qt-6-41CD52?logo=qt&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%3E%3D3.10-064F8C?logo=cmake&logoColor=white)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux-lightgrey)

</div>

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Download](#download)
- [Tech Stack](#tech-stack)
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Linux](#linux)
  - [Windows](#windows)
- [Usage](#usage)
- [Running WellTool on Windows Startup](#running-welltool-on-windows-startup)
- [Keyboard Shortcuts](#keyboard-shortcuts)
- [Known Limitations](#known-limitations)
- [Roadmap](#roadmap)
- [License](#license)
- [Author](#author)

---

## Overview

**WellTool** is a lightweight, native desktop utility that lives quietly on the edge of your screen. Click the tab to slide it open, and you get instant access to a sketch canvas, a note-taking pad, and a scientific-style calculator — no separate apps, no window switching, no clutter.

It's built entirely with **Qt6 Widgets** and modern **C++17**, with a hand-written expression parser powering the calculator and a custom-drawn canvas engine behind the sketchpad — no third-party GUI or math libraries involved.

---

## Features

### 🎨 Canvas
- Freehand drawing with adjustable pen color and width
- Right-click to open a floating width slider
- Eraser mode with radius-based point removal
- Full **undo** support (`Ctrl+Z`) for both drawing and erasing
- Canvas auto-extends vertically as you draw near the bottom edge
- Export your sketch to a **PNG** file at any time

### 📝 Notes
- Simple, distraction-free text editor for quick notes
- Save / Open / Save As, with content persisted as `.md`
- **Text highlighting**: select any text and a small floating toolbar appears above your cursor — pick a color and it's applied as a highlight, right in place
- Highlighted formatting is preserved across save/reload

### 🧮 Calculator
- Custom recursive-descent expression parser (tokenizer + parser, written from scratch)
- Standard operators (`+ - * /`) with parentheses and correct precedence
- Built-in functions: `log`, `log10`, `sqrt`, `sin`, `cos`, `tan`, `pow`
- **User-defined functions** — define your own, e.g. `bmi(weight, height) = weight / height^2`, and reuse them like any built-in
- Custom functions persist across restarts, with a one-click reset

### 🎛️ Panel Behavior
- Frameless, always-on-top panel docked to the right edge of the screen
- Collapses to a slim tab when not in use, expands with a smooth fade/slide animation
- Freely draggable along the screen edge
- Resizable by dragging the bottom-left corner
- **Light / Dark theme** toggle, applied instantly via Qt stylesheets and remembered between sessions

---

## Download

Prebuilt Windows binaries (no need to install Qt or build anything yourself) are available on the [**Releases**](https://github.com/chees89/WellTool/releases) page — just grab the latest `.zip`, extract it anywhere, and run `WellTool.exe`.

> **A note on the Windows security warning:** when you run the exe, Windows SmartScreen may show a blue "Windows protected your PC" prompt, or your antivirus might flag it as unrecognized. This is expected and not a sign of anything malicious — it just means the executable isn't digitally signed with a code-signing certificate (those cost money and require a registered business to obtain, which isn't practical for a free personal utility like this one). If you see the prompt, click **"More info" → "Run anyway"**. The full source is right here in this repo if you'd like to check exactly what's being built and run.

---

## Tech Stack

| Layer            | Choice                                    |
|-------------------|--------------------------------------------|
| Language          | C++17                                      |
| UI Framework      | Qt6 (Widgets)                              |
| Build System      | CMake (≥ 3.10)                             |
| Persistence       | `QSettings` (Windows Registry / Linux config) |
| Styling           | Custom `.qss` stylesheets, bundled via Qt Resource System |
| Expression Engine | Hand-written tokenizer + recursive-descent parser |

---

## Project Structure

```
WellTool/
├── include/            # Header files
├── src/                # Implementation files
├── styles/              # dark.qss / light.qss theme sheets
├── icons/                # Application icon (logo.ico)
├── resources.qrc         # Qt resource manifest
└── CMakeLists.txt
```

---

## Getting Started

### Prerequisites

- **Qt6** (Widgets module)
- **CMake** 3.10 or newer
- A C++17-capable compiler (GCC, Clang, or MSVC)

### Linux

Install the required build tools and Qt6 development libraries first.

**Debian / Ubuntu:**
```bash
sudo apt update
sudo apt install build-essential cmake qt6-base-dev qt6-base-dev-tools
```

**Fedora:**
```bash
sudo dnf install gcc-c++ cmake qt6-qtbase-devel
```

**Arch Linux:**
```bash
sudo pacman -S base-devel cmake qt6-base
```

Then build:
```bash
git clone <your-repo-url>
cd WellTool
cmake -B build -S .
cmake --build build
./build/WellTool
```

### Windows

1. Install [Qt6](https://www.qt.io/download-qt-installer) (any recent MSVC or MinGW kit) and [CMake](https://cmake.org/download/).
2. Open the project folder in **Qt Creator** (recommended — it auto-detects the CMake kit), *or* build from the command line:
   ```powershell
   cmake -B build -S . -G "Ninja"
   cmake --build build
   ```
3. Run `WellTool.exe` from the `build` folder.

> **Note:** the built executable depends on the Qt6 runtime DLLs. If you're running it outside Qt Creator, either keep it on a machine with Qt6 installed, or deploy it with `windeployqt` first.

---

## Usage

- Click the thin tab on the right edge of the screen to **expand / collapse** the panel.
- Drag the tab up and down to reposition it along the screen edge.
- When expanded, drag the **bottom-left corner** to resize the panel.
- Switch between **Canvas**, **Notes**, **Calculator**, and **Themes** using the tabs at the top.

---

## Running WellTool on Windows Startup

Since WellTool is meant to sit quietly at the edge of your screen, you'll probably want it to launch automatically when you log in.

**Option 1 — Startup folder (simplest):**
1. Press `Win + R`, type `shell:startup`, hit Enter.
2. Copy a **shortcut** to `WellTool.exe` into the folder that opens.
3. Done — it'll launch automatically on your next login.

**Option 2 — Registry (for scripting / power users):**
```powershell
reg add "HKCU\Software\Microsoft\Windows\CurrentVersion\Run" /v WellTool /t REG_SZ /d "C:\Path\To\WellTool.exe" /f
```
To remove it later:
```powershell
reg delete "HKCU\Software\Microsoft\Windows\CurrentVersion\Run" /v WellTool /f
```

---

## Keyboard Shortcuts

| Shortcut         | Action                        |
|-------------------|-------------------------------|
| `Ctrl + Z`         | Undo last draw/erase (Canvas) |
| `Enter`            | Evaluate expression (Calculator) |

---

## Known Limitations

- Undo history currently applies only to the Canvas — Notes and Calculator don't have their own undo stacks.
- Notes with highlights are saved as HTML internally (so formatting survives reload inside WellTool), which means the `.md` file won't render as clean Markdown in external editors or on GitHub.
- Building from source requires the Qt6 runtime to be installed; this doesn't apply if you use the prebuilt zip from [Releases](https://github.com/chees89/WellTool/releases), which already bundles everything needed.

---

## Roadmap

- [x] Standalone Windows `.zip` release with bundled Qt6 DLLs — no separate Qt installation required
- [ ] Linux packaging (AppImage)
- [ ] Per-tab undo history (Notes, Calculator)

---

## Author

Built by **[chees89](https://github.com/chees89)** — a personal portfolio project built while developing a freelance C++ practice.
