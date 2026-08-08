//! Native compiler-plan layer.
//!
//! WHY THIS CRATE EXISTS. The former compiler module was moved to Rust one
//! layer at a time so that each layer could be proved equivalent before the
//! next one moved. `alchemy-routing` owns which flags a source gets,
//! `alchemy-symbols` owns symbol shaping, and `alchemy-bundle` owns which
//! compiler binaries are approved and how the gcc296 driver is invoked. This
//! crate owns the layer above all three: given a target, a routing source, an
//! input and an output, it decides which compiler family applies, which driver
//! serves it, whether a separate preprocessing step is needed, and what argv
//! each step receives.
//!
//! It covers TypeScript lines 3594 through 3790 -- `CompilerFamily`,
//! `CompilerFlagMutations`, `SourceToAssemblyPlanOptions`, `CompilerCommandStep`,
//! `SourceToAssemblyPlan`, `mutatedCompilerFlags`, `inferredPreprocessedOutput`,
//! `sourceToAssemblyPlan`, `directPreprocessorCommand`, `directCompilerCommand`
//! and `directCompilerCommandForSource`. It stops cleanly before the
//! toolchain-free guards (`callbackArityLint`, line 3789), which are a lint
//! layer over `alchemy-symbols` rather than part of planning.
//!
//! TWO STANDING HAZARDS.
//!
//! ORDER IS BEHAVIOUR. Every sequence in this crate is an argv or a chain of
//! ternary tests, and gcc is later-flag-wins: moving one element changes the
//! machine code that comes out. There is no `HashMap` and no `HashSet` anywhere
//! in this crate, and there must not be one -- an unordered container here
//! would not fail a test, it would silently produce a different binary.
//!
//! THE DEFECTS ARE REPRODUCED, NOT REPAIRED. Six behaviours in this slice are
//! wrong on their own terms, and each is carried over exactly, marked with a
//! PORT NOTE and pinned by a test that asserts the wrong answer. Every sweep on
//! record was measured through them; correcting one here while the TypeScript
//! still ships it would make the two implementations disagree, which is the one
//! outcome this port cannot have. Fix them in both languages at once, or not at
//! all.
//!
pub mod nodepath;
pub mod plan;
