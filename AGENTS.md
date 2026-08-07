# Working on Alchemy

Alchemy reconstructs *Golden Sun* (GBA) as byte-exact C. A change counts only
when the rebuilt bytes are identical to the released ROM.

There are 183 files under `tools/`. You need about eight of them. This page is
the whole entry point; everything else is reference you read when a gate sends
you there.

## The loop

1. **Pick an owner** from a queue below.
2. **Write C** into `semantic/<owner>.c` until it compiles and reads correctly.
3. **Close the residual** with the two sweeps.
4. **Adopt** it, which is what makes it count.
5. **Verify, then commit.**

```bash
bun run verify        # the gate: rebuilds the ROM and every overlay (slow, authoritative)
bun run test          # every tool's self-test, discovered not listed (~7s)
```

`verify` is the only authority. If it is green the work is real; if it is red
nothing else you believe matters.

## Picking work

```bash
bun tools/overlay_twins.ts --leads    # families: read one owner, transpose the rest
bun tools/overlay_twins.ts --unconverted
bun tools/check_sanctum.ts --queue    # compiler axis spent, shape axis never run
bun tools/decomp_queue.ts             # ranked single owners
```

Prefer `--leads`. A family of five costs one reading and pays out four
transpositions; a lone 200-byte owner pays out once.

## Closing a residual

Two independent axes. Exhausting one is not exhausting the search.

```bash
bun tools/mode_sweep.ts  semantic/<owner>.c   # compiler flags, source held fixed
bun tools/shape_sweep.ts semantic/<owner>.c   # source shapes, compiler held fixed
```

Both are bounded and deterministic, so "it found nothing" is a real result.
When they miss, escalate to the annealers -- `permute_v1.ts` for the main image,
`permute_overlay.ts` for overlays -- under the bounded-probe rule in
[HANDOVER.md](HANDOVER.md). Their measured yield is low, so they are a rescue
tool, not the engine: never run them unbounded and never promote a near-match.

If both bounded axes are spent and the owner is still not exact, seal it in
[SANCTUM.md](SANCTUM.md) so nobody re-derives the same floor.

## Adopting

Adoption is what moves the counter. The source must be **outside** `exact/`
when you adopt — `overlay_adopt.ts` assembles the overlay as a pre-flight
check, so a source already sitting in `exact/` fails on its own baseline.

```bash
bun tools/overlay_adopt.ts resource_3cb:053c --source /tmp/owner.c --span 68 --apply
bun run build:inventory && bun tools/full_c_progress.ts --write-report
```

Always `build:inventory` **before** `full_c_progress.ts`, or the metrics are
stale and the commit hook rejects you.

## Ledgers

| File | Owns |
|---|---|
| [LAWS.md](LAWS.md) | compiler behaviour we proved, with its evidence |
| [SANCTUM.md](SANCTUM.md) | owners withdrawn from routine attack, both axes spent |
| [PROVENANCE.md](PROVENANCE.md) | the clean-room boundary |
| `metrics/gs1-en-progress.json` | the headline byte counts |

## Hard rules

- **Never `asm(...)`** in C or headers, in any spelling, including fixed-register
  bindings and empty barriers. Byte equality never overrides this.
- **Never copy game-specific code** from another decompilation project. Tooling
  and methodology are reusable; function bodies, names, types and comments are
  not. See [PROVENANCE.md](PROVENANCE.md).
- **Commit subjects end with `[C exact/total bytes]`** matching the regenerated
  metrics. A deliberate regression needs `metrics: revert reduces Full-C`; a
  changed denominator needs `metrics: correct executable denominator`.
- **Never resolve a merge in `tools/alchemy_gcc.ts`, `tools/mode_sweep.ts` or the
  fork's `arm.c` by taking one side.** They are append-only registries of
  independent discoveries. Three-way merge against the merge-base, then resolve
  per entry. A deletion carries intent too.

## The compiler

`alchemy-gcc/` is our fork, not a third-party dependency; editing it is normal
work. `dist/` holds the built toolchain and every worktree symlinks to one
canonical copy, so all lanes compile with the same binary. Rebuilding means
re-pinning the cc1 digest in `tools/alchemy_gcc.ts` from a green verify.

Do not run `git submodule` commands from inside a worktree: it rewrites the
shared `core.worktree` and breaks git in the main checkout.
