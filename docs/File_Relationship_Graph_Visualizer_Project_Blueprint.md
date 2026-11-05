# File Relationship Graph Visualizer

## Project Specification and Implementation Blueprint

## 1. Project Overview

The **File Relationship Graph Visualizer** is a C++ desktop application
that identifies relationships between files using filesystem-level
information. It scans selected directories, collects metadata without
reading file contents, calculates similarity scores, constructs a
weighted relationship graph, and uses Kruskal's algorithm to generate a
minimum spanning forest (MSF). The MSF is the only graph displayed in
the Qt interface.

The application supports all file types. Database files are treated like
ordinary files; their tables, columns, schemas, and contents are not
inspected.

## 2. Problem Statement

Directories and nested folders may contain files related by names,
extensions, locations, or filesystem metadata. These relationships can
be difficult to identify manually. The application estimates
relationships from filesystem information and presents a simplified
connecting structure through a Kruskal minimum spanning forest.

## 3. Objectives

-   Scan one or more directories and discover files of any type.
-   Collect filesystem metadata without reading file contents.
-   Identify candidate relationships using filenames, extensions, paths,
    and available metadata.
-   Calculate similarity scores and combine them into relationship
    scores.
-   Construct a weighted graph and filter weak candidate relationships.
-   Generate a minimum spanning forest using Kruskal's algorithm.
-   Display only the Kruskal forest through a Qt interface.
-   Organize the codebase into multiple C++ source and header files.

## 4. Scope

### Included

-   Directory traversal and file discovery.
-   Filesystem metadata collection.
-   Filename tokenization, hashing, and an inverted index.
-   Jaccard similarity.
-   Levenshtein edit distance using Dynamic Programming.
-   Folder-path proximity scoring.
-   Optional file metadata similarity.
-   Weighted relationship scoring and thresholding.
-   Graph construction and connected components.
-   Kruskal minimum spanning forest and Disjoint-Set Union (DSU).
-   Force-directed positioning for visual layout.
-   Qt visualization and node/edge inspection.

### Excluded

-   Reading file contents.
-   Parsing database files or inspecting database tables and columns.
-   A separate full relationship graph view.
-   Barnes--Hut or quadtree force calculations.
-   Modifying, deleting, or renaming files.
-   Content-based similarity.

## 5. Supported File Information

The application can handle all file types because it does not depend on
their internal formats.

  Metadata             Purpose
  -------------------- ---------------------------------------------------
  Filename             Main input for filename similarity
  Extension            Identifies file type and supports type comparison
  Folder path          Estimates proximity in the directory hierarchy
  File size            Optional supporting similarity feature
  Last-modified time   Optional temporal proximity feature
  Creation time        Optional where consistently available

The recommended core metadata is filename, extension, path, file size,
and last-modified time. Creation time may be included if reliably
available on the target platform. Database files are treated like other
files; the application does not inspect their internal structures.

## 6. Technology Stack

  -----------------------------------------------------------------------
  Area                                Selected technology
  ----------------------------------- -----------------------------------
  Language                            C++

  Standard                            C++17

  GUI                                 Qt 6 Widgets

  Graph display                       `QGraphicsScene` and
                                      `QGraphicsView`

  Build system                        CMake

  File discovery                      C++ filesystem facilities

  Data structures                     Vectors, hash maps, hash sets,
                                      adjacency lists, DSU

  Similarity algorithms               Jaccard, Levenshtein edit distance

  Graph algorithm                     Kruskal minimum spanning forest

  Code organization                   Multiple `.cpp` and `.h` files
  -----------------------------------------------------------------------

## 7. System Workflow

1.  User selects one or more directories.
2.  The scanner traverses directories and discovers files.
3.  The metadata collector records permitted filesystem information.
4.  The tokenizer extracts tokens from filenames and relevant path
    components.
5.  The inverted index identifies candidate file pairs.
6.  Similarity measures are calculated for candidate pairs.
7.  The scoring module combines feature scores.
8.  The graph builder creates weighted edges that meet a threshold.
9.  Kruskal's algorithm produces a minimum spanning forest.
10. The layout module assigns positions to forest nodes.
11. The Qt interface displays the forest and supports inspection.

## 8. Architecture and Modules

### File Scanner

Traverses selected directories and discovers files. It does not read
file contents.

### Metadata Collector

Collects filename, extension, path, size, and last-modified time when
available. It normalizes metadata into a consistent internal
representation.

### Tokenizer and Indexer

Normalizes filenames and selected path components, splits them into
tokens, and builds an inverted index mapping tokens to file identifiers.

### Similarity Analyzer

Calculates Jaccard similarity, normalized Levenshtein similarity, path
proximity, and optional metadata similarity.

### Relationship Scorer

Combines feature scores using configurable weights and produces a final
relationship score.

### Graph Builder

Creates a weighted graph from file nodes and qualifying relationship
edges. It removes self-edges and duplicate undirected edges.

### Kruskal Module

Sorts edges by cost and uses DSU to select edges without creating
cycles. It produces a minimum spanning forest when the graph is
disconnected.

### Graph Layout

Assigns visual positions. A force-directed layout may attract connected
nodes and repel nodes from one another to reduce overlap. Positions are
updated iteratively until stabilization or a configured iteration limit.

### Qt Interface

Displays only the Kruskal forest using Qt Widgets and the graphics view
framework. Users can select nodes and edges to inspect their details.

## 9. Data Model

### File Node

Each file node contains: - Unique identifier. - Filename. - Extension. -
Normalized or absolute path. - File size, if available. - Last-modified
timestamp, if available. - Token collection.

A node represents a file, not its contents.

### Relationship Edge

Each candidate edge contains: - Source and destination node
identifiers. - Jaccard score. - Normalized edit-distance similarity. -
Path proximity score. - Optional metadata similarity. - Combined
relationship score. - Edge cost used by Kruskal.

The implementation must define how relationship strength is converted to
edge cost. If stronger relationships should be selected first, the cost
definition must support that objective consistently.

### Graph Representation

-   Vector of file nodes.
-   Vector of weighted edges.
-   Adjacency list for graph traversal.
-   Hash maps for node lookup.
-   Hash sets for token and candidate-pair tracking.
-   DSU for Kruskal's algorithm.

## 10. Algorithms

### 10.1 Directory Traversal with DFS

1.  Start from each selected directory.
2.  Visit the current directory.
3.  Record files found.
4.  Recursively or iteratively visit subdirectories.
5.  Handle inaccessible paths and filesystem errors without stopping the
    entire scan.

### 10.2 Filename Tokenization

Normalize and split filenames using separators such as spaces,
underscores, hyphens, and periods. Remove empty tokens and optionally
filter common low-information tokens. Preserve the original filename for
display.

### 10.3 Hashing and Inverted Index

An inverted index maps each token to the files containing it. Files
sharing tokens become candidate pairs. This avoids needlessly comparing
every possible pair when the index can identify likely matches.

Example:

``` text
"report" -> [file_1, file_4, file_8]
"final"  -> [file_1, file_2]
"image"  -> [file_3, file_7]
```

### 10.4 Jaccard Similarity

For token sets A and B:

\[ J(A,B)=`\frac{|A\cap B|}{|A\cup B|}`{=tex} \]

The score is between 0 and 1. A value of 1 means identical token sets; 0
means no shared tokens.

### 10.5 Levenshtein Edit Distance with Dynamic Programming

Levenshtein distance is the minimum number of insertions, deletions, and
substitutions needed to transform one string into another. Dynamic
Programming calculates the distance.

A normalized similarity can be defined as:

\[ S\_{edit}(a,b)=1-`\frac{D(a,b)}{\max(|a|,|b|)}`{=tex} \]

For two empty strings, define similarity as 1 to avoid division by zero.
The score ranges from 0 to 1.

### 10.6 Folder-Path Proximity

Path proximity estimates how close two files are in the directory
hierarchy. One possible method compares their common directory prefix or
shared ancestor depth. The selected formula must be normalized,
documented, and used consistently.

### 10.7 Metadata Similarity

Optional supporting features include: - Extension match or similarity. -
File-size proximity. - Last-modified-time proximity.

Metadata should be supporting evidence and should not dominate filename
or path evidence unless the chosen weights explicitly specify that.

### 10.8 Weighted Relationship Score

A weighted score can combine the features:

\[ S(A,B)=w_J S_J+w_E S_E+w_P S_P+w_M S_M \]

Where the terms represent Jaccard, edit-distance, path, and metadata
scores. Weights should be non-negative and sum to 1. If a feature is
unavailable, the implementation should define how remaining weights are
adjusted.

Retain an edge only when its score meets the configured threshold.

### 10.9 Graph Construction

1.  Create a vertex for every discovered file.
2.  Evaluate candidate pairs.
3.  Calculate their relationship scores.
4.  Apply the threshold.
5.  Store qualifying weighted edges.
6.  Prevent duplicate undirected edges and self-edges.

The resulting graph may have multiple connected components.

### 10.10 Kruskal Minimum Spanning Forest

Kruskal's algorithm is the only graph-selection algorithm used for the
displayed graph.

1.  Sort qualifying edges by the defined edge cost.
2.  Initialize every vertex as a separate DSU set.
3.  Process edges in sorted order.
4.  Select an edge if its endpoints belong to different sets.
5.  Merge those sets.
6.  Skip an edge if its endpoints are already in the same set.
7.  Continue until the forest is complete or all edges have been
    processed.

For a connected graph, the result is a minimum spanning tree. For a
disconnected graph, the result is a minimum spanning forest.

### 10.11 Disjoint-Set Union

DSU supports: - `find` to identify a set representative. - `unite` to
merge sets. - Path compression. - Union by rank or size.

It prevents cycles in the selected forest.

### 10.12 Force-Directed Layout

The layout positions the selected forest for viewing. Connected nodes
may attract one another while nodes repel one another to reduce overlap.
Position updates continue until the layout stabilizes or reaches a
configured iteration limit. The layout does not change Kruskal's edge
selection.

## 11. User Interface

### Main Window

-   Application title.
-   Directory selection.
-   Scan/analyze action.
-   Progress and status display.
-   Graph visualization area.
-   Optional score-weight and threshold settings.

### Graph View

-   Displays only the Kruskal minimum spanning forest.
-   Files appear as nodes and selected relationships as edges.
-   Supports panning and zooming.
-   Shows filenames near nodes.
-   Allows node and edge selection for inspection.

### File Details

When a node is selected, show filename, extension, path, file size, and
last-modified time where available. Do not display file contents or
database internals.

### Edge Details

When an edge is selected, show connected filenames, combined
relationship score, component scores, and the edge cost used by Kruskal.

## 12. Suggested Multi-File Project Structure

``` text
FileRelationshipGraphVisualizer/
├── CMakeLists.txt
├── README.md
├── include/
│   ├── FileNode.h
│   ├── RelationshipEdge.h
│   ├── FileScanner.h
│   ├── MetadataCollector.h
│   ├── Tokenizer.h
│   ├── InvertedIndex.h
│   ├── SimilarityAnalyzer.h
│   ├── RelationshipScorer.h
│   ├── GraphBuilder.h
│   ├── DisjointSet.h
│   ├── Kruskal.h
│   ├── GraphLayout.h
│   ├── GraphView.h
│   └── MainWindow.h
├── src/
│   ├── main.cpp
│   ├── FileScanner.cpp
│   ├── MetadataCollector.cpp
│   ├── Tokenizer.cpp
│   ├── InvertedIndex.cpp
│   ├── SimilarityAnalyzer.cpp
│   ├── RelationshipScorer.cpp
│   ├── GraphBuilder.cpp
│   ├── DisjointSet.cpp
│   ├── Kruskal.cpp
│   ├── GraphLayout.cpp
│   ├── GraphView.cpp
│   └── MainWindow.cpp
└── tests/
    ├── test_tokenizer.cpp
    ├── test_similarity.cpp
    ├── test_graph_builder.cpp
    └── test_kruskal.cpp
```

The exact split may evolve, but scanning, scoring, graph algorithms,
layout, and UI should remain separate responsibilities.

## 13. Module Responsibility Summary

  Module               Responsibility
  -------------------- ----------------------------------------
  FileScanner          Discover files
  MetadataCollector    Collect filesystem metadata
  Tokenizer            Normalize and tokenize names/paths
  InvertedIndex        Map tokens to file identifiers
  SimilarityAnalyzer   Calculate similarity features
  RelationshipScorer   Combine scores
  GraphBuilder         Construct weighted graph
  DisjointSet          Track sets for Kruskal
  Kruskal              Generate minimum spanning forest
  GraphLayout          Position graph nodes
  GraphView            Render graph elements
  MainWindow           Coordinate user interface and workflow

## 14. Error Handling

Handle inaccessible directories, permission errors, files disappearing
during scans, unavailable metadata, empty directories, a single-file
scan, no qualifying edges, disconnected components, long paths, and
large scans. Report errors or status to the user without crashing the
application.

## 15. Testing Plan

### File Scanner

-   Different file extensions.
-   Nested directories.
-   Empty directory.
-   Inaccessible directory.
-   Verify file contents are not read.

### Tokenizer and Index

-   Spaces, underscores, hyphens, and extensions.
-   Case normalization.
-   Correct token-to-file mapping.
-   Candidate-pair deduplication.

### Similarity

-   Identical and disjoint token sets.
-   Similar filenames with small spelling differences.
-   Empty strings.
-   Different paths.
-   Missing metadata.

### Graph Builder and Kruskal

-   No qualifying edges.
-   Single edge.
-   Cyclic graph.
-   Disconnected graph.
-   Equal edge costs.
-   Empty and single-vertex graphs.
-   Duplicate edge prevention.

### Interface

-   Directory selection and analysis.
-   Forest rendering.
-   Zoom and pan.
-   Node metadata inspection.
-   Edge score inspection.

## 16. Evaluation Criteria

-   Correct file discovery across nested directories.
-   Consistent metadata collection.
-   Correct similarity calculations.
-   Correct weighted graph construction.
-   Correct Kruskal minimum spanning forest.
-   Clear forest visualization.
-   Useful node and edge inspection.
-   Stable handling of filesystem errors.
-   Maintainable multi-file source organization.

## 17. Limitations

-   Relationships are inferred from metadata, not file contents.
-   Files related by content but unrelated by names or paths may not be
    connected.
-   Similar filenames do not guarantee identical purpose.
-   Metadata availability may differ by operating system and filesystem.
-   The minimum spanning forest retains a connecting subset of
    qualifying relationships, not every edge in the candidate graph.
-   Visual appearance depends on layout settings.
-   Database schemas and internal records are not inspected.

## 18. Expected Outcome

The completed application will scan selected directories, infer file
relationships from filesystem-level metadata, construct a weighted
graph, and display a Kruskal minimum spanning forest through a Qt 6
interface. The codebase will be divided into multiple C++17 source and
header files, with distinct modules for scanning, metadata collection,
similarity analysis, graph construction, Kruskal's algorithm, layout,
and visualization. File contents remain unread and unchanged.
