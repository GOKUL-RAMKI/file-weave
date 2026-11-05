# File Relationship Graph Visualizer

A C++17 / Qt 6 desktop application that scans folders, scores files by name, path, and
metadata similarity, and visualizes the strongest relationships as a graph. The graph is
built from a **Kruskal minimum spanning forest**, so disconnected groups of files appear as
separate clusters instead of one tangled cloud.

## Requirements

### Required software

| Dependency | Version | Notes |
| --- | --- | --- |
| Windows | 10 or 11 | The project is developed and tested on Windows. |
| Visual Studio 2022 Build Tools | 17.x | Install the **Desktop development with C++** workload (MSVC v143 + Windows SDK). |
| CMake | 3.21 or newer | Must be available on `PATH`, or use the copy bundled with Visual Studio. |
| Qt | 6.x, MSVC 2022 64-bit build | Tested with **Qt 6.9.1 `msvc2022_64`**. Only the `Widgets` module is used. |

No third-party C++ libraries are required beyond Qt and the C++ standard library.

### Optional software

- **Git** for cloning the repository.
- **Ninja** if you prefer the Ninja generator instead of Visual Studio.
- **Qt Creator** if you prefer an IDE workflow.

## Installing the dependencies

### 1. Visual Studio 2022 Build Tools

1. Download the [Visual Studio 2022 Build Tools](https://visualstudio.microsoft.com/downloads/).
2. In the installer, select **Desktop development with C++**.
3. Make sure these components are included:
   - MSVC v143 C++ build tools
   - Windows 10 or Windows 11 SDK
   - C++ CMake tools for Windows (optional, but provides `cmake`)

### 2. CMake

Install from [cmake.org](https://cmake.org/download/) and enable **Add CMake to the system
PATH**. Verify with:

```powershell
cmake --version
```

### 3. Qt 6

Install Qt 6 using either method below.

**Option A — Qt online installer (recommended)**

1. Download the [Qt online installer](https://www.qt.io/download-qt-installer).
2. Select **Qt 6.x → MSVC 2022 64-bit**.
3. Install to a known location, for example `D:\Qt\6.9.1\msvc2022_64`.

**Option B — aqtinstall**

```powershell
pip install aqtinstall
aqt install-qt windows desktop 6.9.1 win64_msvc2022_64 -O D:\Qt
```

The resulting path should contain `bin\windeployqt.exe` and `lib\cmake\Qt6`.

## Building

Open **x64 Native Tools Command Prompt for VS 2022** (or call `vcvars64.bat` first), then
from the project root:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH="D:/Qt/6.9.1/msvc2022_64"
cmake --build build --config Release --parallel
```

Replace the `CMAKE_PREFIX_PATH` value with your Qt installation path.

If you are using a normal PowerShell window, initialize the MSVC environment first:

```powershell
cmd /c "call ""C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"" && cmake -S . -B build -G ""Visual Studio 17 2022"" -A x64 -DCMAKE_PREFIX_PATH=""D:/Qt/6.9.1/msvc2022_64"""
cmd /c "call ""C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"" && cmake --build build --config Release --parallel"
```

On Windows, the build automatically runs `windeployqt` after linking, so the required Qt
DLLs and plugins are placed next to `app.exe`.

## Running

```powershell
.\build\Release\app.exe
```

### Using the app

1. Click **Add Folder** and choose a folder to scan.
2. Click **Scan**. The app scans the folder recursively and builds the relationship graph.
3. Adjust the threshold to control how similar two files must be before they are connected.
4. Pan with the mouse drag / scrollbars and zoom with the mouse wheel.
5. Click a node or edge to inspect its details. On large graphs, labels are shown only for
   the selected node or edge to keep the view readable.
6. Use **Remove** to remove the selected folder from the list.

Disconnected groups in the Kruskal forest are laid out and packed as separate clusters.

## Tests

After building, run:

```powershell
ctest --test-dir build -C Release --output-on-failure
```

The suites cover the scanner, tokenizer, similarity scoring, graph builder, Kruskal forest,
and layout bounds.

## Project layout

```text
.
├── CMakeLists.txt
├── README.md
├── include/            Public headers for the core library and Qt views
├── src/                Core algorithms and Qt UI implementation
├── tests/              Unit tests registered with CTest
├── sample_data/        610-file sample tree used for manual and stress testing
└── docs/               Project blueprint and design notes
```

The `build/` directory is generated and is not part of the repository.

## Known limitations

- The force-directed layout is O(n²) by design (no Barnes–Hut or quadtree). Very large
  folders still take longer to lay out, though the result is bounded to the viewport.
- Large clusters (hundreds of nodes) remain dense when fully zoomed out. Use zoom, pan, and
  node selection to inspect them.
- Only filenames, paths, and filesystem metadata are analyzed; file contents are never read.
