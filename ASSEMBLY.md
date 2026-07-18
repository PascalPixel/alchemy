# Assembly boundary

Assembly is not one undifferentiated decompilation backlog. Every tracked
`asm/*.s` region is byte-verified, but its origin and long-term source form are
recorded separately. A function remains decompilation work until asm-free C
from the approved compiler reproduces it exactly. Linker products, runtime
thunks, fixed hardware entries, and proven deliberately assembled kernels stay
in assembly.

The current boundary, after the sixteenth exact-C checkpoint and full IWRAM reconstruction, is:

| Classification | Files | Bytes | Long-term treatment |
|---|---:|---:|---|
| Linker long-call veneers | 292 | 2,336 | Keep assembly |
| `_call_via_rN` runtime thunk bundle | 1 | 56 | Keep assembly |
| BIOS/SWI wrapper bundles | 2 | 16 | Keep assembly |
| ROM dispatch table | 1 | 768 | Keep assembly |
| Relocated IWRAM payload | 1 | 5,120 | Keep structured assembly |
| IWRAM division veneers | 4 | 32 | Keep assembly |
| Deliberate fixed-point math primitives | 2 | 56 | Keep assembly |
| Compiler/assembler literal pool | 1 | 32 | Keep structured data |
| Mixed or misbounded code/data regions | 35 | 29,972 | Split before decompiling |
| Structured runtime relocation-helper module | 1 | 128 | Keep structured assembly |
| Proven parent-function fragments with pools | 9 | 1,646 | Merge when each owner is reconstructed |
| Proven multi-region function heads with pools | 7 | 2,256 | Merge with their continuations before exact C |
| Proven multi-region function continuations with pools | 6 | 2,284 | Merge with their function owners before exact C |
| Cross-function shared-literal module | 2 | 692 | Keep structured assembly pending module-aware C build |
| Proven deliberate performance primitive | 1 | 22 | Keep assembly |
| Likely ordinary compiler output | 1,357 | 458,086 | Convert to exact C |
| Probable data misidentified as functions | 27 | 314 | Recover semantic data form |
| **Total** | **1,749** | **503,816** | |

These counts describe files, not callable entries. `080000c0.s` bundles 96
fixed-width dispatch entries, `08006864.s` bundles two BIOS wrappers, and
`080072e4.s` bundles fourteen `_call_via_rN` thunks.

## Proven infrastructure assembly

- `080000c0.s` is a 96-entry ROM dispatch table. Each eight-byte entry has the
  fixed `ldr r4; bx r4; literal` form. Its width, literal placement, and
  clobber contract are not C semantics.
- The 292 long-call veneers are linker products with the exact
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

`08093fa0` proved to be a complete callable function followed only by two bytes
of alignment. After making that boundary explicit it returns to the ordinary
compiler-output category and remains an exact-C candidate. Its complete layout
also occurs intact in the approved Japanese ROM at the corresponding address.

`08006a78` is a compact runtime relocation-helper module. It contains two
direct-call entries, a four-byte Thumb helper template, and an installer that
copies the template by its source start and end addresses before storing the
relocated odd Thumb entry. Those source-position semantics are now represented
by labels rather than an opaque instruction word. The same complete layout is
present in the approved Japanese ROM, establishing a stable shared module but
not an original name or author. It remains structured assembly.

`080f9c44` is a callable 66-byte function followed by alignment and a local
literal pool. Its first two PC-relative loads share literals at the end of the
next function, so the present one-function-at-a-time C build cannot emit it
independently. A stored Thumb function pointer proves that the next function
starts at `080f9c90`, not at the former `080f9c9e` boundary; the equal branch
falls through from the recovered prefix into `080f9c9e`. The two corrected
files therefore remain one structured shared-literal module with unknown
authorship.

## Deliberate performance assembly

`080f9a18.s` is the presently proven file-level example. It lies inside the
`0x080f9674..0x080f9a73` payload copied to `0x03007000..0x030073ff` for the
sound path. It clears 64 bytes with four unrolled `stmia` instructions,
temporarily holds `r4` in `ip`, and uses no stack frame. Both its IWRAM
placement and instruction schedule are direct evidence of deliberately
assembled performance code rather than an accidental compiler mismatch.

The wider relocated payload contains mixed Thumb/ARM channel processing and
sample-buffer stores. It is deliberately assembled code, although the ROM
alone cannot prove whether Camelot wrote it or bundled it with a sound library.
This repository records that uncertainty instead of inventing authorship.

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

Twenty-seven unreachable files above `0x0803e976` have incoherent stack
restoration, impossible fall-through, undefined instructions, or no credible
callers. Their headers mark them as probable data false positives. They do not
count as deliberate assembly or legitimate C functions; semantic data recovery
must replace the provisional instruction spelling.

## Acceptance rule

A region moves from `asm/` to `src/` only when its asm-free C compiles to the
exact bytes at the exact address. A region is retained as intentional assembly
only when its ABI, placement, instruction schedule, or hardware entry contract
provides positive evidence. Unknown or mixed regions stay explicitly unknown;
absence of a C match is never treated as proof of hand-written assembly.
