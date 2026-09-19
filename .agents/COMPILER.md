# Compiler

## One compiler, one flag set

Use the approved `agbcc` and `agscc` submodules and bundle. Game code uses
canonical GCC 2.96; recorded prebuilt-library families have their own fixed
routes established by provenance, not score. Commands live in
`tools/alchemy/src/compiler/routing.rs`.

GCC source and compiler modifications belong in the licensed `agscc`
submodule, never as source patches in this repository. Pascal approved moving
the existing TLA lowering into `agscc` behind `-mgs2` on 2026-09-13. Both games
use the same compiler bundle; only TLA game code enables that option, which
keeps its upstream spelling. The TLA lowering is a reconstruction, not a claim
to recovered historical compiler source.

**Apart from that TLA option, do not modify agscc unless restoring historical
stock GCC 2.96 shipped by Red Hat.** Every change, including diagnostic or
host-port changes, requires Pascal's approval, a specific historical release
and source or vendor-patch evidence, and proof of restored fidelity. Better
matching, determinism, version strings or an approved pin are not historical
provenance or authorization.

Only Pascal may authorize changes to pins, executable hashes, family routes or
output transformations. A different shipped compiler family requires evidence
across that family. Do not weaken guards or copy another project's workarounds.

## Tricks are not matches

No per-function routing or flags, invented flags, fixed-register variables,
empty barriers, forced scheduling, output patches or selected lucky runs.
Never force registers, scheduling or compiler output.

A match that needs a scheduling trick is not a match: `volatile` storage on
ordinary RAM, a dummy or duplicate store, a store overwritten before it is
read, a `do { } while (0)` or empty-block barrier. Such an owner is withdrawn,
in a showcase or not. It stays in its listing as a recorded compiler gap, with
a non-credited `compiler_scheduling_module` row in
`semantic/overlay-assembly.json` and its proof in the dossier, and DONE drops
by its bytes. `volatile` stays legitimate where the hardware demands it: I/O
registers at `0x04xxxxxx`, palette and video memory, interrupt handlers and the
state they share with the main loop, such as the sound engine's.

Inline assembly is admitted only as a shared macro header the evidence shows
Camelot used (a construct the approved compiler provably cannot emit, recurring
across otherwise compiler-shaped C bodies), never per function and never to
steer the compiler.

The reviewed `Dma_Set` body in `games/THE BROKEN SEAL/INCLUDE/DMA.H`
implements the fixed register interface of the recurring DMA construct. Its
internal register bindings belong to that evidenced assembly interface; they
do not authorize fixed registers in callers or other C. The ordinary-source
gate admits only the complete token-pinned shared body, including its
instructions, operands and clobbers, and still checks every surrounding raw
and preprocessed token. Changing the header does not silently expand that
admission. The evidence and remaining family live in the
`thumb_multiple_transfer_module` record in `raw/classification.json`.

## The toolchain

The approved bundle uses GNU GAS 2.10 for compiler output and unit slices,
including alignment fill. Modern binutils handles retained syntax, symbols and
linking with the recorded integer and soft-float ABI. Do not suppress ABI
mismatches. Cache identity includes the full executable bundle. The restored
bundle is admitted on Apple Silicon macOS; other hosts need source builds and
verification. Keep third-party source untouched.

Uppercase `.C` is compiled as C, never inferred as C++, without changing the
approved route or optimization flags.
