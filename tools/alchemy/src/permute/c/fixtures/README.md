# Permuter fixtures

These fixtures use independently authored synthetic inputs. No game source, ROM or imported test corpus is included.

- parser-cases.tsv: 47 translation units combining types, declarators, initializers, operators, statements, extensions and typedef shadowing. Columns: case name, C, formatted C, preorder node kinds with constant types.
- parser-invalid.tsv: 12 syntax and namespace failures.
- parser-unsupported.tsv: unnamed scalar members and pragma expressions. Rejection tables store case name, input C and a short description.
- shared-declarations.tsv: 12 cases covering shared structures, unions, enums, pointers, arrays, callbacks and nested declarations.
- search-scenarios.tsv: 18 original draft/target pairs, including an unreachable target, finite choices, ranges, shared fragments, once-only writes, line swaps, ignored regions and randomized call ordering. The harness uses both games, both score algorithms and multiple workers. Saved results are recompiled and checked for instruction equality.
- .c, .expected and .nodes: complete translation-unit formatting and traversal checks. Trailing spaces use `\x20`; the final blank line uses a `\x0a` marker, decoded before comparison.
- mutations*.tsv: individual passes, successive walks, retry counts, region selection, replay, side effects and variable-access analysis across the synthetic mutations-*.c programs.
- rng*.tsv: signed and large integer seeds, sampling and independent control/mutation streams.
- output-*.tsv: filtering, deduplication, source diffs and assembly hashes.
- levenshtein-alignments.tsv and scoring-algorithms.tsv: equal spans and final penalties, including long inputs and normalization options.

TSV cells escape backslashes, tabs, newlines and carriage returns. Walk fixtures retain failed attempts and terminal errors. Expected results check the recorded scenarios; they do not establish exhaustive grammar support, complete command-line compatibility or a measured improvement in matching productivity.
