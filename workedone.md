# Work Done

## 2026-11-04
- **Completed:** Scaffolded CMake + Qt 6 project; wrote all core modules (FileScanner, MetadataCollector, Tokenizer, InvertedIndex, SimilarityAnalyzer, RelationshipScorer, GraphBuilder, DisjointSet, Kruskal, GraphLayout) and Qt UI (MainWindow, GraphView, main.cpp); wrote 5 test suites.
- **Files:** `CMakeLists.txt`, `todo.md`, `workedone.md`, `include/*.h` (15 headers), `src/*.cpp` (15 sources), `tests/*.cpp` (5 tests + check.h)
- **Checks:** All 5 test suites pass under MSVC; CMake configures and builds cleanly with Qt 6.9.1 MSVC 2022 from `D:\Qt`; app.exe launches and shows an empty window.
- **Follow-up:** Qt installed via aqtinstall (prebuilt binaries) to `D:\Qt\6.9.1\msvc2022_64`; vcpkg source build was aborted (too slow, network-dependent).

## 2026-11-05
- **Completed:** Clean rebuild from scratch; ran all 5 test suites (all pass); launched app.exe smoke test (window opens successfully).
- **Files:** No source changes; verified `build/Release/` outputs.
- **Checks:** `ctest`-equivalent: all 5 test executables exit 0; app process starts and runs.
- **Follow-up:** None.

## 2026-11-05 (fixes)
- **Completed:** Fixed the scan crash caused by removing a folder while `scan()` was processing events; controls are now disabled and re-entrant scans are ignored. Reworked the force-directed layout to use a per-input ideal distance (`sqrt(area / nodes)`), capped forces, centering gravity, and a bounded/centered result. `GraphView` now fits the node cloud instead of long labels and clamps zoom. Added `tests/test_layout.cpp`.
- **Files:** `src/MainWindow.cpp`, `include/MainWindow.h`, `src/GraphLayout.cpp`, `src/GraphView.cpp`, `CMakeLists.txt`, `tests/test_layout.cpp`, `todo.md`, `workedone.md`.
- **Checks:** Release build succeeded. All 6 test suites pass. `sample_data` smoke test: 610 files, 8835 edges, 531 forest edges, layout bounded to ~862x850 (previously ~32414x29907); repeated scans, node/edge selection, re-entrant scan click, and remove-during-scan all completed without crashing. `app.exe` stayed running in a launch smoke test.
- **Follow-up:** Layout still uses the agreed O(n²) force-directed approach; scans with thousands of files will take longer but are now bounded visually.

## 2026-11-05 (graph polish)
- **Completed:** Stretched the positioned layout to the viewport aspect so the graph fills the panel width. Added label level-of-detail: all labels show for graphs of 100 nodes or fewer; larger graphs show only the selected node's label (or both labels for a selected edge). Labels ignore view zoom so they stay readable, and move to the left of nodes in the right half. Fixed a clear-time use-after-free by clearing label pointers before the scene.
- **Files:** `src/GraphView.cpp`, `include/GraphView.h`, `todo.md`, `workedone.md`.
- **Checks:** Release build succeeded; all 6 test suites pass. UI harness repeated scans, selections, re-entrant scan, and remove-during-scan without crashing on the 610-file sample. Screenshots confirmed the 610-node graph fills the panel without label clutter and the 12-node graph shows all labels.
- **Follow-up:** `build/Release/app.exe.old` is the previously running copy and can be deleted after that process is closed.

## 2026-11-05 (separate clusters)
- **Completed:** Made the layout component-aware. The Kruskal forest's connected components are detected with DSU, each component is force-laid-out independently, normalized to its own area (`sqrt(perNodeArea * count)`), and the components are shelf-packed (`2400` wide, `40` gap, `80` minimum box) before one global fit into ~1400x850. Components now render as clearly separate islands instead of one intermingled cloud.
- **Files:** `src/GraphLayout.cpp`, `todo.md`, `workedone.md`.
- **Checks:** Release build succeeded; all 6 test suites pass. `sample_data` (610 nodes, 8835 candidate edges, 531 forest edges, 79 components) lays out in ~100 ms within bounds `1400x683.9`. Offscreen UI harness re-ran repeated scans/selection, re-entrant scan, and remove-during-scan with no crashes; screenshot shows separated component islands.
- **Follow-up:** The largest components (219 and 67 nodes) remain dense when zoomed out; this is expected for the O(n²) force layout. Use pan/zoom and selection to inspect them.

## 2026-11-06 (open file from details)
- **Completed:** Selecting a graph node now shows an **Open File** button in the bottom details panel. The button opens the selected file with the system default application via `QDesktopServices`, and hides again when an edge is selected, the scene selection clears, or a new scan runs.
- **Files:** `src/MainWindow.cpp`, `src/GraphView.cpp`, `include/MainWindow.h`, `include/GraphView.h`, `README.md`, `workedone.md`.
- **Checks:** Release build succeeded; all 6 test suites pass; offscreen launch smoke test stayed running without crashing.
- **Follow-up:** None.
