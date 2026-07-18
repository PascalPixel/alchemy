# Assembly boundary

Assembly is not one undifferentiated decompilation backlog. Every tracked
`asm/*.s` region is byte-verified, but its origin and long-term source form are
recorded separately. A function remains decompilation work until asm-free C
from the approved compiler reproduces it exactly. Linker products, runtime
thunks, fixed hardware entries, and proven deliberately assembled kernels stay
in assembly.

The current boundary, after the first Bun conversion batch, is:

| Classification | Files | Bytes | Long-term treatment |
|---|---:|---:|---|
| Linker long-call veneers | 292 | 2,336 | Keep assembly |
| `_call_via_rN` runtime thunk bundle | 1 | 56 | Keep assembly |
| BIOS/SWI wrapper bundles | 2 | 16 | Keep assembly |
| ROM dispatch table | 1 | 768 | Keep assembly |
| Mixed or misbounded code/data regions | 63 | 36,048 | Split before decompiling |
| Proven deliberate performance primitive | 1 | 22 | Keep assembly |
| Likely ordinary compiler output | 1,503 | 465,882 | Convert to exact C |
| Probable data misidentified as functions | 27 | 314 | Recover semantic data form |
| **Total** | **1,890** | **505,442** | |

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

The startup copy at `0800300c` also identifies a second hot payload:
`0x08000770..0x08001b6f` is copied to `0x03000000..0x030013ff`. Evidence inside
it includes the ARM IRQ dispatcher, fixed-point `smull`/`smlal` math kernels,
computed-entry unrolled clearing, packed-bit expansion, decompression, and a
mixed-mode audio path. Those routines are strong deliberate-assembly
candidates and must be reconstructed as structured assembly regions before
the private ROM fallback can be retired. Stereotyped division helpers inside
the payload remain compiler-runtime code, not game-specific optimization.

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
