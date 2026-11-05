# Development Guidelines — File Relationship Graph Visualizer

This file is the working agreement for all coding agents contributing to the **File Relationship Graph Visualizer**. Read it before making changes and follow it throughout development.

## 1. Project Goal

Build the agreed C++17 desktop application using Qt 6 Widgets and CMake. The application scans files using filesystem-level metadata, estimates relationships, constructs a weighted graph, and displays **only the Kruskal minimum spanning forest**.

The application must not read file contents or inspect database tables, columns, schemas, or records.

## 2. Keep the Implementation Focused

- Implement only features required by the project specification and current milestone.
- Do not add unrelated features, extra frameworks, design patterns, abstractions, or “clever” optimizations without a clear project need.
- Do not expand the scope on your own. If a requirement is unclear or a change would increase scope, ask before implementing it.
- Prefer the simplest correct implementation that is understandable and maintainable.
- Avoid speculative future-proofing.
- Do not add dependencies unless they are necessary and approved.
- Do not silently change the selected technology stack or algorithm choices.

## 3. Code Style

- Use clear, simple variable and function names.
- Prefer short names when their meaning is obvious, such as `file`, `path`, `score`, `edge`, `node`, `index`, and `count`.
- Avoid cryptic names, excessive abbreviations, and unnecessarily long names.
- Use consistent naming throughout the codebase.
- Keep functions focused on one task.
- Avoid unnecessary comments. Add comments only when the reason or behavior is not obvious.
- Do not write code that is more complicated than the task requires.
- Follow the existing formatting and style in the repository. If no style exists, use consistent indentation and braces.
- Do not use macros or templates merely to save a few lines.
- Do not duplicate logic when a small, clear shared function is appropriate.

## 4. Multi-File Organization

Do not put the whole application in one file. Separate code by responsibility, following the existing project structure where possible:

- File discovery and metadata collection.
- Tokenization and indexing.
- Similarity calculations and scoring.
- Graph construction.
- DSU and Kruskal.
- Graph layout.
- Qt graph rendering and main window.

Use headers for declarations and source files for implementations. Avoid creating extra files that do not have a clear purpose. Keep module boundaries practical rather than overly elaborate.

## 5. Functional Constraints

- Support files of all types by using filesystem-level information.
- Treat database files as ordinary files. Do not inspect database internals.
- Do not open files to read their contents for similarity analysis.
- Use the agreed similarity methods: Jaccard similarity, Levenshtein edit distance with Dynamic Programming, path proximity, and agreed metadata features.
- Use a weighted relationship score and the configured threshold.
- Use Kruskal’s algorithm and DSU to create the minimum spanning forest.
- Display the Kruskal forest only; do not add a separate full-graph display.
- Use the agreed force-directed layout only for positioning the displayed graph.
- Do not add Barnes–Hut, quadtree, or other unapproved algorithms.
- Do not modify, delete, or rename scanned files.

## 6. Task Tracking Files

Keep these two files current in the repository root.

### `todo.md` — Milestones and Remaining Work

Use `todo.md` as the plan for work that has not yet been completed.

- Organize tasks by milestone.
- Use Markdown checkboxes: `- [ ]` for pending and `- [x]` for completed.
- Keep tasks concrete and verifiable.
- Do not mark a task complete until its implementation is present and the relevant checks have passed.
- Add a task only if it belongs to the approved project scope.
- If a task is blocked, state the reason next to it.
- Do not use this file as a chronological activity log.

Suggested format:

```md
# TODO

## Milestone 1 — Project Setup
- [ ] Create the CMake and Qt 6 project.
- [ ] Confirm the application builds.

## Milestone 2 — File Discovery
- [ ] Scan selected directories.
- [ ] Collect permitted filesystem metadata.

## Milestone 3 — Similarity and Graph
- [ ] Implement tokenization and inverted index.
- [ ] Implement similarity calculations.
- [ ] Build the weighted graph.

## Milestone 4 — Kruskal Forest
- [ ] Implement DSU.
- [ ] Implement Kruskal’s algorithm.
- [ ] Verify the forest on connected and disconnected test graphs.

## Milestone 5 — Visualization
- [ ] Implement graph positioning.
- [ ] Display the Kruskal forest in Qt.
- [ ] Support node and edge inspection.

## Milestone 6 — Verification
- [ ] Run relevant tests.
- [ ] Verify the project builds from a clean build directory.
```

Adapt this list to the actual repository and approved requirements. Do not treat this example as permission to add features.

### `workedone.md` — Work Log

Use `workedone.md` as a concise chronological record of completed work. Keep the filename exactly `workedone.md`.

For each meaningful work session or completed task, record:
- Date.
- What was changed.
- Files added or modified.
- Tests or build checks run and their result.
- Any blocker or follow-up, if applicable.

Do not log every tiny edit. Do not claim a test passed unless it was actually run. Keep entries factual and brief.

Suggested format:

```md
# Work Done

## YYYY-MM-DD
- **Completed:** Implemented the file scanner.
- **Files:** `FileScanner.h`, `FileScanner.cpp`
- **Checks:** Build succeeded; scanner test passed.
- **Follow-up:** None.
```

If a check was not run, write `Not run`. If it failed, record the failure accurately.

## 7. Agent Workflow

Before starting a task:

1. Read this guideline file.
2. Read `todo.md` and `workedone.md`.
3. Inspect the relevant existing code before editing.
4. Identify the smallest implementation that completes the task.
5. Check that the task belongs to the approved project scope.

While working:

1. Make focused changes.
2. Reuse existing code where appropriate.
3. Avoid unrelated refactoring.
4. Do not overwrite another module’s responsibility without a reason.
5. Keep names and interfaces consistent.

After working:

1. Build or run the relevant tests when possible.
2. Fix issues introduced by the change.
3. Update `todo.md` for completed, pending, or blocked work.
4. Append a factual entry to `workedone.md`.
5. Summarize the changes and checks in the final response.
6. Mention any unverified behavior or known issue.

## 8. Git and Change Safety

- Do not discard, overwrite, or revert existing user changes.
- Do not run destructive commands unless explicitly approved.
- Avoid broad formatting changes unrelated to the task.
- Do not commit or push unless requested.
- Keep changes small enough to review.

## 9. Definition of Done

A task is complete only when:

- The requested behavior is implemented within scope.
- The code follows these guidelines.
- Relevant build or tests have been run, or their absence is stated.
- `todo.md` reflects the task’s status.
- `workedone.md` contains a concise factual record.
- No unrelated features or changes were added.

## 10. When to Ask

Ask the user before proceeding if:
- A requirement is ambiguous and affects implementation.
- A proposed solution adds scope or a dependency.
- A change conflicts with the project specification.
- A decision could alter the algorithm, output, or architecture.

Do not pause for routine implementation details that can be resolved by following the existing code and these guidelines.
