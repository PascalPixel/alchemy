# Alchemy

Alchemy reconstructs _Golden Sun_ (**TBS**, _The Broken Seal_) and _Golden Sun:
The Lost Age_ (**TLA**) as ordinary, readable C and editable assets that rebuild
the shipped games byte for byte. It follows a documented clean-room policy
modelled on long-running decompilations such as pret's, and records where each
file comes from. It takes no outside contributions until it reaches 100%.

Use `tbs` and `tla` in paths and target names. Japanese releases are the source
editions; localizations are measured differences. The production gate requires
a complete byte-identical TBS English build. Passing it does not mean all code
is C or that all twelve target ROMs have been rebuilt.

This file is the index of every rule. Read it whole, then read the topic files
the index below names before working in their domain.

## The two directives

1. **Never commit anything a pret-style decompilation would not publish.** The
   test in [TRACKING](.agents/TRACKING.md) decides every file. This directive
   outranks everything else, including the goal.
2. **Reconstruct the source Camelot most likely wrote and had on their disk**:
   files, folders, names, types, macros and build. Where the evidence says
   Camelot wrote C, we write C; where it says a shared macro with inline
   assembly, a hand-written `.s`, or a generated table with its writer and
   reader, we write that. DONE measures how much of the executable image such
   source explains.

Every rule in the topic files follows from these two and is kept because it
saved us from a specific mistake. When a rule and the evidence about what
Camelot wrote disagree, the evidence wins and the rule is repaired; evidence
never overrides directive 1.

## Always

- **Documents.** `README.md` introduces Alchemy, `TODO.md` holds open work, and
  this file with the topic files below holds every rule. `CLAUDE.md` is a
  symlink to this file. No other Markdown, plain-text, reStructuredText or
  AsciiDoc file may exist anywhere in the checkout, ignored `out/` and scratch
  included, and no prose guide hides inside JSON. A new domain is a new row
  here and its file under `.agents/`, in one commit. `make verify` enforces
  this. Upstream material keeps its own documents: the `agbcc` and `agscc`
  submodules and the compiler and binutils sources bootstrap downloads are
  exempt, and are never deleted or rewritten to satisfy this rule.
- **One home per rule.** State a rule in the one topic file that owns it and
  link to it from anywhere else. Code constraints live beside the code, owner
  results in the evidence records.
- **Output under `out/`.** Scores, work directories, reports and drafts go under
  ignored `out/`, never loose in the checkout root.
- **Scripts** are TypeScript on Bun or Rust, never Python or shell scripts. The
  only shell is the minimal `alchemy` and `psynergy` launchers and the Git
  hooks in `.hooks/`, each a few lines that immediately run the Rust tooling;
  keep them that way. Check [TOOLING](.agents/TOOLING.md) before writing a
  script.
- **Commits.** Stage explicit paths, never everything; run `make verify` on the
  staged tree; start the subject with the prefix `make progress-subject`
  prints. See [BUILD](.agents/BUILD.md).
- **Refusals are answers.** A failing gate, a refusing tool or a denied
  permission names a problem to fix, never one to route around.

## Topics

| File | Owns | Read before |
| --- | --- | --- |
| [TRACKING](.agents/TRACKING.md) | What may be tracked, private inputs, licensed code, the publication gate | Adding or generating any file |
| [EVIDENCE](.agents/EVIDENCE.md) | The clean room, admissible sources, the authority for each fact | Using any information that is not in this repository |
| [RECOVERY](.agents/RECOVERY.md) | Choosing owners, the short loop, residuals, extents and bindings, integration | Any decompilation or matching work |
| [COMPLETION](.agents/COMPLETION.md) | Exact C, DONE, retained assembly, veneer credit | Claiming, crediting or reporting progress |
| [COMPILER](.agents/COMPILER.md) | agscc and agbcc, routes and flags, forbidden tricks, compiler gaps | Touching a compiler or route, or when a match resists ordinary C |
| [SHARING](.agents/SHARING.md) | Editions, unit instances and twins, source shared by both games | Code used by more than one image, edition or game |
| [SOURCE](.agents/SOURCE.md) | Source layout, names, the Lunpa showcase standard | Naming, moving, creating or polishing source |
| [ASSETS](.agents/ASSETS.md) | Asset inputs and formats, compression, maps and the world assembly | Asset, map, image, sound or text work |
| [WORKING](.agents/WORKING.md) | Bounded batches, durable results, waves, deadlines, authority | Starting a batch of work |
| [BUILD](.agents/BUILD.md) | Setup, bootstrap, gates, commits and pushing | Building, verifying, committing or pushing |
| [TOOLING](.agents/TOOLING.md) | Every tool and command, the Alchemy and Psynergy boundary, the dashboard | Writing, changing or running any tool or script |
