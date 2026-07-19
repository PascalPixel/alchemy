# Assembly boundary

Assembly is not one undifferentiated decompilation backlog. Every tracked
`asm/*.s` region is byte-verified, but its origin and long-term source form are
recorded separately. A function remains decompilation work until asm-free C
from the approved compiler reproduces it exactly. Linker products, runtime
thunks, fixed hardware entries, and proven deliberately assembled kernels stay
in assembly.

The current boundary is generated and checked without a ROM by
`bun tools/build_asm.ts --source-only`. After the latest exact-C checkpoint it
is:

| Classification | Files | Bytes | Long-term treatment |
|---|---:|---:|---|
| Linker long-call veneers | 330 | 2,640 | Keep assembly |
| `_call_via_rN` runtime thunk bundle | 1 | 56 | Keep assembly |
| BIOS/SWI wrapper bundles | 2 | 16 | Keep assembly |
| ROM dispatch table | 1 | 768 | Keep assembly |
| Relocated IWRAM payload | 1 | 5,120 | Keep structured assembly |
| IWRAM division veneers | 4 | 32 | Keep assembly |
| Fixed-point math and quarter-sine lookup module | 2 | 568 | Keep structured assembly/data |
| Compiler/assembler literal pool | 1 | 32 | Keep structured data |
| Relocated ARM runtime modules | 10 | 5,720 | Keep structured assembly |
| Relocated stack ARM kernel | 1 | 28 | Keep structured assembly |
| Shared-literal Thumb helper module | 1 | 40 | Keep structured assembly pending module-aware C |
| Mixed or misbounded code/data regions | 30 | 27,338 | Split before decompiling |
| Structured runtime relocation-helper module | 1 | 128 | Keep structured assembly |
| Proven parent-function fragments with pools | 9 | 1,646 | Merge when each owner is reconstructed |
| Proven multi-region function heads with pools | 9 | 3,820 | Merge with their continuations before exact C |
| Proven multi-region function continuations with pools | 7 | 2,928 | Merge with their function owners before exact C |
| Cross-function shared-literal module | 2 | 692 | Keep structured assembly pending module-aware C build |
| Proven deliberate performance modules | 2 | 954 | Keep assembly |
| Mixed-mode multiply helper | 1 | 16 | Keep assembly |
| Likely ordinary compiler output | 1,339 | 458,256 | Convert to exact C |
| **Total** | **1,754** | **510,798** | |

These counts describe files, not callable entries. `080000c0.s` bundles 96
fixed-width dispatch entries, `08006864.s` bundles two BIOS wrappers, and
`080072e4.s` bundles fourteen `_call_via_rN` thunks.

## Proven infrastructure assembly

- `080000c0.s` is a 96-entry ROM dispatch table. Each eight-byte entry has the
  fixed `ldr r4; bx r4; literal` form. Its width, literal placement, and
  clobber contract are not C semantics.
- The 330 long-call veneers are linker products with the exact
  `ldr r4, [pc]; bx r4; .4byte target` form. They are not Camelot functions and
  do not count as C debt.
- `080072e4.s` is the compiler runtime's fourteen-entry `_call_via_r0` through
  `_call_via_r13` table. The approved compiler emits calls to those symbols for
  ordinary C function pointers. The verifier and linker resolve them at a
  four-byte stride from `0x080072e4`.
- `08006864.s` and `08006870.s` contain direct BIOS software-interrupt
  wrappers. The fixed `swi`/`svc` instruction is not expressible in asm-free C.
- `080022ec.s` through `08002304.s` are four fixed veneers into the division
  runtime at `0x03000380..0x030003f0`. The adjacent ordinary routines now have
  their own boundaries and remain C targets.
- `0809b7e4.s` is the 32-byte PC-relative literal pool used by `0809b698`.
  The false function at `0809b7f8` has been removed; the real function at
  `0809b804` now builds exactly from C.

`asm/classification.json` is the machine-readable boundary. `build_asm.ts`
attaches its origin, retention decision, confidence, and evidence to every
built region and rejects changes to the proven category counts.

## Recovered fragment and pool boundaries

The smallest former mixed regions at `0800f1fa`, `0808b7b8`, `080d12a8`,
`080d5094`, `080d8258`, `080e1724`, `080e1a48`, `080e4ab8`, and `080e547c` are
not callable functions. Direct branches enter them while the parent function's
stack frame and high registers remain live, and each fragment branches back
into or returns through that parent. Their internal pools, adjacent pools, and
alignment are now explicit structured data. Authorship is unknown; they remain
assembly only until their complete owning functions can be reconstructed as
single units.

The former mixed regions at `080d765c`, `080dd9c0`, and `080ec100` are proven
callable function heads, not complete functions. Each begins with a complete
prologue, keeps its stack frame and saved high-register state live, and branches
across a local pool into a later continuation file. Direct calls prove the first
and third entries; a stored Thumb function pointer proves the second. Their
alignment and pools are now explicit, but exact C requires merging each whole
multi-region function first. Authorship remains unknown.

Four more callable heads at `0808b674`, `080e15e8`, `080e47b8`, and `080f4168`
now include their formerly omitted structured tails. The first, second, and
fourth branch into later files with their stack frames live; the third dispatches
through an explicit 101-entry table into its continuation fragments. The first
two corrected layouts match their approved Japanese counterparts after address
normalization. The fourth preserves the same layout with localized constants,
while the third has an identical code prefix and all 101 table targets relocate
by the same offset. This is boundary evidence only; authorship remains unknown.

Two additional former mixed regions at `0800ebec` and `080be378` are callable
heads with formerly omitted structured tails. `0800ebec` now includes its local
literal pool before branching into later continuations with its frame live.
`080be378` now exposes its 100-entry dispatch table, the final case body,
alignment, and local pool before control continues in later fragments. The
first corrected layout matches its approved Japanese counterpart after address
normalization. For the second, the non-pointer layout matches and all 100 table
entries relocate by the same offset. This is shared-boundary evidence only;
authorship remains unknown.

The former mixed regions at `080d76f0`, `080dda3c`, and `080ec190` are
continuations of those same three functions. They are entered by direct branch
with their owners' stack frames and high registers live, cross explicit local
pools, and branch onward into later continuation files. Their corresponding
normalized layouts occur intact in the approved Japanese ROM. This confirms
shared engine boundaries, not authorship; all three remain unknown-origin
assembly until their complete functions can be reconstructed.

The continuations at `0802691c`, `080de0d4`, and `080ec264` likewise use live
owner frames and continue into later files after explicit pools. The complete
corrected layouts for the latter two match the approved Japanese ROM after
normalization; the first retains 540 of 552 normalized bytes at its corresponding
layout, with the same control-flow boundary. This establishes structure only,
not authorship or original names.

`080e17c4` is a further continuation of the function headed at `080e15e8`.
It retains the owner's live frame across an internal pool and an indirect
arithmetic helper, completes the following loop tail, and branches into the
later `080e1a48` continuation. Its complete corrected layout matches the
approved Japanese ROM after address normalization. This establishes a shared
function boundary only, not authorship or an original name.

`08093fa0` proved to be a complete callable function followed only by two bytes
of alignment. After making that boundary explicit it returns to the ordinary
compiler-output category and remains an exact-C candidate. Its complete layout
also occurs intact in the approved Japanese ROM at the corresponding address.

`0800b168` and `080d5c48` likewise proved to be complete callable functions.
The former was missing only its trailing alignment, while the latter was
missing its adjacent literal pool. Both corrected layouts occur intact at the
corresponding approved Japanese addresses, so they return to the ordinary
compiler-output category as exact-C candidates. The comparison establishes
layout and boundary only, not authorship or original names.

`08006a78` is a compact runtime relocation-helper module. It contains two
direct-call entries, a four-byte Thumb helper template, and an installer that
copies the template by its source start and end addresses before storing the
relocated odd Thumb entry. Those source-position semantics are now represented
by labels rather than an opaque instruction word. The same complete layout is
present in the approved Japanese ROM, establishing a stable shared module but
not an original name or author. It remains structured assembly.

`0800230c.s` and `0800231c.s` form a fixed-point math and lookup module.
`0800231c.s` now owns the complete 256-entry unsigned halfword table at
`08002344`; every entry equals the rounded value of
`65536 * sin(pi * index / 512)`. The classification records the 40-byte Thumb
lookup routine and the 512-byte quarter-sine table as separate semantic
subregions even though their PC-relative relationship requires one source
unit. This mathematical identification does not establish an original symbol,
filename, or author.

The four files from `08002544.s` through `08002d5c.s` are caller-delimited ARM
routines copied before execution: two decompression formats, a transfer-command
executor, and a Thumb-BL relocation walker. Their instruction set and exact
copy bounds are proven, while the approved compiler emits Thumb code only.
They are therefore genuinely compiler-unproducible structured assembly rather
than ordinary C debt. Their program-versus-library origin and authorship remain
unknown.

`08002dd8.s` now owns both callable Thumb helpers at `08002dd8` and `08002df0`,
their intervening alignment, and the literal at `08002dfc` referenced across
the former file boundary. The obsolete standalone `08002df0.s` boundary is
removed. The merged source remains structured assembly pending a module-aware
C match; neither its layout nor its shared literal establishes authorship.

`080f9c44` is a callable 66-byte function followed by alignment and a local
literal pool. Its first two PC-relative loads share literals at the end of the
next function, so the present one-function-at-a-time C build cannot emit it
independently. A stored Thumb function pointer proves that the next function
starts at `080f9c90`, not at the former `080f9c9e` boundary; the equal branch
falls through from the recovered prefix into `080f9c9e`. The two corrected
files therefore remain one structured shared-literal module with unknown
authorship.

## Deliberate performance assembly

`080f9674.s` reconstructs the complete 932-byte sound-mixing entry copied from
ROM `0x080f9674` to IWRAM `0x03007000`. One callable entry deliberately switches
Thumb to ARM, returns to Thumb for channel setup, switches back to ARM for the
sample-buffer accumulation loops, and finally returns to Thumb for its epilogue.
Its stored odd function pointer, copy destination, exact mixed-mode boundaries,
and PC-relative mode switches prove both the run address and the instruction
sets. The approved Thumb-only compiler cannot emit this module.

The adjacent `080f9a18.s` clears 64 bytes with four unrolled `stmia`
instructions, temporarily holds `r4` in `ip`, and uses no stack frame. These
two schedules are direct evidence of deliberately assembled performance code,
although the ROM alone cannot prove whether Camelot wrote them or bundled them
with a sound library. This repository records that uncertainty instead of
inventing authorship.

The startup copy at `0800300c` identifies a second hot payload:
`0x08000770..0x08001b6f` is copied to `0x03000000..0x030013ff`. The first
`0x658` bytes now build exactly from `asm/08000770.s` at their IWRAM run
address. That source separates the ARM IRQ dispatcher, fixed-point
`smull`/`smlal` math kernels, computed-entry unrolled clearing, packed-bit
expansion, transforms, decompression, and compiler-runtime division helpers.
The remaining `0xda8` bytes at ROM `0x08000dc8..0x08001b6f` are the mixed-mode
audio path and remain private-ROM fallback until their Thumb/ARM boundaries
are reconstructed.

## Not assembly authorship evidence

Hardware-register literals, high-register allocation, `ldmia`/`stmia`, or an
isolated BIOS instruction do not by themselves prove hand assembly. Most
remaining files still look like ordinary optimized C and remain conversion
targets. Likewise, a failed register-pinning experiment says nothing about the
original source language.

The sound helpers around `080f9a80` demonstrate another structural boundary
problem. Entries at `080f9a98` and `080f9a9a` share one function tail, while
`080f9b10` branches directly into the middle of `080f9ac0` with live state.
Their present modules preserve those relationships and their alignment words;
they remain C debt unless a module-level asm-free compilation reproduces the
same entry and continuation layout.

Six unreachable files inside `0x08037464..0x08073808` had incoherent stack
restoration and impossible fall-through because the scanner had decoded packed
message bits as Thumb instructions. The semantic context-code and message-bank
reconstruction now replaces every one of them. This is a pipeline-level guard:
unreachable islands inside a consumer-proven data package remain data recovery
work and never become assembly or C merely because a disassembler accepts them.

## Acceptance rule

A region moves from `asm/` to `src/` only when its asm-free C compiles to the
exact bytes at the exact address. A region is retained as intentional assembly
only when its ABI, placement, instruction schedule, or hardware entry contract
provides positive evidence. Unknown or mixed regions stay explicitly unknown;
absence of a C match is never treated as proof of hand-written assembly.
