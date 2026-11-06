# Project Flow

## 1. Application starts

Files:

- [`src/main.cpp`](src/main.cpp)
- [`include/MainWindow.h`](include/MainWindow.h)
- [`src/MainWindow.cpp`](src/MainWindow.cpp)

Flow:

```text
main()
 └── Creates QApplication
 └── Creates MainWindow
 └── Shows the window
 └── Starts the Qt event loop
```

`MainWindow` creates the folder controls, scan button, threshold control,
progress bar, graph display, and details panel.

## 2. User adds folders

File:

- [`src/MainWindow.cpp`](src/MainWindow.cpp)

Functions:

- `MainWindow::addDirectory()` opens a folder dialog and adds the selected folder.
- `MainWindow::removeDirectory()` removes the selected folder.

## 3. User clicks Scan

File:

- [`src/MainWindow.cpp`](src/MainWindow.cpp)

Function:

- `MainWindow::scan()`

The main processing flow is:

```text
Selected folders
 └── scanDirectory()
      └── collectMetadata()
           └── buildGraph()
                └── setGraph()
                     └── kruskal()
                          └── computeLayout()
                               └── Draw graph
```

## 4. Recursively scan folders

Files:

- [`include/FileScanner.h`](include/FileScanner.h)
- [`src/FileScanner.cpp`](src/FileScanner.cpp)

Function:

- `scanDirectory()`

This function recursively visits the selected folders, finds regular files,
skips inaccessible files or folders, and sends every file to
`collectMetadata()`.

## 5. Collect file metadata

Files:

- [`include/MetadataCollector.h`](include/MetadataCollector.h)
- [`src/MetadataCollector.cpp`](src/MetadataCollector.cpp)
- [`include/FileNode.h`](include/FileNode.h)

Function:

- `collectMetadata()`

For each file, the project stores:

- File name
- File path
- Parent directory
- File extension
- File size
- Last modified time
- Filename stem

This information is stored in a `FileNode`. File contents are not read.

## 6. Tokenize file names

Files:

- [`include/Tokenizer.h`](include/Tokenizer.h)
- [`src/Tokenizer.cpp`](src/Tokenizer.cpp)

Important functions:

- `Tokenizer::lower()`
- `Tokenizer::stem()`
- `Tokenizer::tokens()`
- `Tokenizer::prefix()`

Example:

```text
"User_Profile_Final.cpp"

stem:   "user_profile_final"
tokens: ["user", "profile", "final"]
prefix: "use"
```

These tokens are used to find potentially related files.

## 7. Build candidate file pairs

Files:

- [`include/InvertedIndex.h`](include/InvertedIndex.h)
- [`src/InvertedIndex.cpp`](src/InvertedIndex.cpp)

The inverted index maps tokens to file IDs:

```text
"user"    → file 1, file 5
"profile" → file 1, file 8
```

Files sharing a token become candidate pairs. This avoids comparing every file
with every other file.

## 8. Calculate similarity

Files:

- [`include/SimilarityAnalyzer.h`](include/SimilarityAnalyzer.h)
- [`src/SimilarityAnalyzer.cpp`](src/SimilarityAnalyzer.cpp)

Similarity functions:

| Function | Purpose |
|---|---|
| `jaccard()` | Compares filename tokens |
| `editSimilarity()` | Compares filename spelling |
| `pathProximity()` | Compares directory paths |
| `metadataSimilarity()` | Compares extensions and file sizes |

## 9. Calculate the relationship score

Files:

- [`include/RelationshipScorer.h`](include/RelationshipScorer.h)
- [`src/RelationshipScorer.cpp`](src/RelationshipScorer.cpp)
- [`include/RelationshipEdge.h`](include/RelationshipEdge.h)

Function:

- `scorePair()`

The combined score uses filename token similarity, edit similarity, path
similarity, and metadata similarity.

```text
cost = 1.0 - score
```

An edge is added only when:

```text
score >= selected threshold
```

## 10. Construct the relationship graph

Files:

- [`include/GraphBuilder.h`](include/GraphBuilder.h)
- [`src/GraphBuilder.cpp`](src/GraphBuilder.cpp)

Function:

- `buildGraph()`

This function:

1. Assigns an ID to every file.
2. Generates filename tokens.
3. Adds tokens to the inverted index.
4. Creates candidate file pairs.
5. Scores every pair.
6. Keeps edges above the selected threshold.

Output:

```text
Graph
 ├── nodes: FileNode objects
 └── edges: RelationshipEdge objects
```

## 11. Create the Kruskal forest

Files:

- [`include/Kruskal.h`](include/Kruskal.h)
- [`src/Kruskal.cpp`](src/Kruskal.cpp)
- [`include/DisjointSet.h`](include/DisjointSet.h)
- [`src/DisjointSet.cpp`](src/DisjointSet.cpp)

Function:

- `kruskal()`

This function sorts edges by cost and uses the disjoint-set data structure to
select edges without creating cycles. The result is a minimum spanning forest.

A forest is used so unrelated file groups remain separate.

## 12. Calculate the graph layout

Files:

- [`include/GraphLayout.h`](include/GraphLayout.h)
- [`src/GraphLayout.cpp`](src/GraphLayout.cpp)

Function:

- `computeLayout()`

This function:

1. Finds disconnected clusters.
2. Applies force-directed positioning inside each cluster.
3. Separates clusters.
4. Packs clusters into the graph area.
5. Keeps the graph within fixed bounds.

## 13. Display the graph

Files:

- [`include/GraphView.h`](include/GraphView.h)
- [`src/GraphView.cpp`](src/GraphView.cpp)

Function:

- `GraphView::setGraph()`

This function runs Kruskal, calculates the layout, and draws:

- Edges as lines
- Files as circular nodes
- File names as labels

The user can zoom, pan, and select nodes or edges.

## 14. Show selected details

File:

- [`src/MainWindow.cpp`](src/MainWindow.cpp)

Functions:

- `showNode()` displays file name, extension, path, size, and modified time.
- `showEdge()` displays file names and all similarity scores.
- `openSelectedFile()` opens the selected file with the system default application.

## Complete flow

```text
main.cpp
   ↓
MainWindow
   ↓
User selects folders
   ↓
MainWindow::scan()
   ↓
FileScanner.cpp
   ↓
MetadataCollector.cpp
   ↓
Tokenizer.cpp
   ↓
GraphBuilder.cpp
   ↓
InvertedIndex.cpp
   ↓
RelationshipScorer.cpp
   ↓
SimilarityAnalyzer.cpp
   ↓
Graph edges created
   ↓
GraphView::setGraph()
   ↓
Kruskal.cpp + DisjointSet.cpp
   ↓
GraphLayout.cpp
   ↓
GraphView draws nodes and edges
   ↓
MainWindow shows selected details
```

The build and test targets are defined in
[`CMakeLists.txt`](CMakeLists.txt).
