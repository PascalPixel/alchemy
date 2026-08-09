# Current status

Snapshot: 2026-08-09. This is the live status surface. Dated investigations and
their original measurements are archived under [`docs/history/`](docs/history/).

## Progress

- **Exact C:** 320,326 / 1,347,254 executable bytes (**23.78%**).
- **Public DONE:** **28%**, combining Exact C with audited permanent assembly.
- **Resource 373:** 10,230 / 24,954 bytes DONE (**41.00%**): 9,398 Exact C
  bytes plus 832 permanent-assembly bytes. Five owners were newly adopted in
  this round, and 7 semantic owners remain.
- Exact C is the contributor progress counter. DONE answers the public
  completion question by also including code deliberately retained as permanent
  assembly. Semantic C, function counts, and queue sizes are diagnostics.

## Working loop

1. Pick an owner with the dispatcher commands `semantic_queue` or
   `overlay_twins --leads`.
2. Reconstruct and review the owner in `semantic/` until its boundaries,
   control flow, data flow, calls, and side effects are credible.
3. Use witnessed sibling shapes and bounded deterministic source searches for a
   small residual. Search compiler modes only when the residual supports it.
4. Use `tools/alchemy-permuter` only as a bounded rescue search. Candidates must
   preserve C dependencies and compile to the owner; a near-match is not an
   adoption.
5. Adopt an exact result through the owner-specific gate: `integrate_matches`
   for main-image drafts or `overlay_adopt` for code overlays. Ensure the
   diagnostic inventory exists with `make inventory` before overlay adoption,
   then regenerate metrics, run verification, and commit with the required
   progress-bearing subject.

The authoritative gate is `make verify`. During iteration, use `make test`,
`make lint`, `make progress`, and the focused build stages documented in
[`AGENTS.md`](AGENTS.md).

## Tooling state

The root Makefile is the workflow facade: its `dispatch-*` targets invoke the
dispatcher registry, while its hyphenated build and check targets invoke the
native implementations directly. The completed audit leaves 128 Rust crates
and 121 binary targets. Nine
dispatcher groups expose 99 public commands; all 22 remaining binaries are
explicitly classified as internal diagnostics, support/self-test hosts, or
benches. The native runner executes 112 runtime self-tests, including every
public command. Architecture rejects an unclassified or stale binary and the
documentation gate checks every public command against
[`docs/TOOLS.md`](docs/TOOLS.md).

One genuine orphan, `text-bg`, was removed after its behavior was proved to be
owned by active asset crates. The other library and internal crates have real
consumers or demonstrated diagnostic roles. The full trial and rationale are
recorded in
[`docs/history/2026-08-09-native-tool-audit.md`](docs/history/2026-08-09-native-tool-audit.md).

Recent operational findings are reflected in the native tools: the permuter is
dependency-aware, bounded, and restricted to safe output cleanup; compiler
corpus selection uses `exact/*.c`; semantic and remaining-owner tools work from
the repository root; overlay scans reject invalid input instead of hiding scan
errors; and the dashboard supports both localhost and the configured LAN host
on port 4649.

## Immediate focus

Finish the seven remaining `resource_373` owners through reconstruction and
small, evidenced residual searches. Keep the exact adoption boundary auditable,
preserve the clean-room rules in [`PROVENANCE.md`](PROVENANCE.md), and record
reusable compiler behavior in [`LAWS.md`](LAWS.md) and exhausted search axes in
[`SANCTUM.md`](SANCTUM.md).
