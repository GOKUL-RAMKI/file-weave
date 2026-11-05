# TODO

## Milestone 1 — Project Setup
- [x] Create the CMake and Qt 6 project.
- [x] Confirm the application builds and opens an empty window.

## Milestone 2 — File Discovery
- [x] Scan selected directories with `std::filesystem`.
- [x] Collect filename, extension, path, size, and last-modified time.
- [x] Verify scanner test (nested, empty, mixed extensions, inaccessible path).

## Milestone 3 — Similarity and Graph
- [x] Implement filename tokenization.
- [x] Implement the inverted index and candidate-pair deduplication.
- [x] Implement Jaccard, Levenshtein, path proximity, and metadata similarity.
- [x] Implement weighted scoring and thresholding.
- [x] Build the weighted graph without self-edges or duplicates.
- [x] Verify tokenizer, similarity, and graph tests.

## Milestone 4 — Kruskal Forest
- [x] Implement DSU with path compression and union by rank.
- [x] Implement Kruskal's algorithm.
- [x] Verify the forest on connected, disconnected, cyclic, and equal-cost graphs.

## Milestone 5 — Visualization
- [x] Implement force-directed positioning.
- [x] Display the Kruskal forest in Qt with pan and zoom.
- [x] Support node and edge inspection.
- [x] Scale repulsion with input size and bound the final layout so nodes stay visible.
- [x] Fit the view to the node cloud instead of long filename labels.
- [x] Stretch the layout to the viewport aspect so the graph uses the panel width.
- [x] Hide labels for large graphs and show them for the selected node or edge.
- [x] Lay out each Kruskal-forest connected component separately and pack the components into distinct islands.

## Milestone 6 — Verification
- [x] Run all tests.
- [x] Verify the project builds from a clean build directory.
- [x] Run a manual scan smoke test.
- [x] Verify `sample_data` (610 files) scans, builds, lays out, and renders repeatedly without crashing.
- [x] Prevent control changes from re-entering or corrupting an in-progress scan.
