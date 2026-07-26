# Laws

Source spellings and compiler behaviours that have each been paid for with a
byte-exact conversion or a measured dead end. Everything here is verified
against the live tree; nothing is inferred from reading the compiler.

This file exists because the working notes live under `work/`, which is
git-ignored. A law that only exists there is a law the next session rediscovers.

## Screen a target before drafting it

`grep 'mov\s*ip, pc' asm/<stem>.s`. A hit means the return address is preserved
in `ip`, and **neither approved compiler can emit that**: the fork's only two
Thumb indirect-call patterns produce `bl _call_via_rN`, and so does `old_agbcc`.
43 `c_candidate` regions carry it, including two of the most inviting by size
(`08097a10` at 68 B, `080b7f20` at 80 B). `0800070c` is a related case — it
tail-jumps to RAM through `ldr r3, [pc]` / `bx r3`.

## Control-flow shape

- **One result block means one condition, not a chain of early returns.** If the
  reference zeroes a result register after the entry work and has a single block
  that sets it, write one disjunction assigning a variable — `s32 result = 0;
  if (a || b || c) result = 1; return result;`. Four `if (…) return 1;`
  statements materialise the 1 inside the first arm and move everything after
  it. 080b27b0: 36 halfwords as early returns, byte-exact as one condition.

- **A dispatch that tests every value up front is a `switch`.** `cmp #1 / beq /
  cmp #2 / beq / b` is a small switch. An `if/else if` chain gives
  `cmp #1 / bne / body / cmp #2 / bne / body`. 0800c0f4: 36 halfwords as a
  chain, byte-exact as a switch.

- **A duplicated match body means loop rotation — write the rotated form.** Peel
  the first test out and duplicate the body, as GCC's own rotation produces.
  The natural `do { … break; } while` lets GCC hoist the match block's constants
  into callee-saved registers and enlarge the prologue. 08004278: 58 bytes out
  natural, 4 bytes explicit.

## Constants and widths

- **Storing a literal below word width goes through the pool.**
  `*(u16 *)P = 0;` emits `ldrh` from the constant pool; assigning through a
  same-width local emits `movs`. This one keeps recurring — it decided
  080b5b08, 080a9d3c and 080175c0.

- **An accumulator must be as wide as the arithmetic.** A `u16` running sum
  wraps every `+=` in `lsls #16 / lsrs #16`. The reference's plain `adds` means
  a 32-bit local narrowed only at the final store. 080060e8: 54 halfwords at
  124 B against 22 at exactly 116 B on that change alone.

- **Related pool constants get derived from each other.** GCC turns a second
  nearby constant into `subs r0, #4` off the first. If the reference loads both
  from the pool, spell them as distinct `Value_<addr>` externs. `Value_<8 hex>`
  links to that address; a trailing `_a`/`_b` names a second symbol for the same
  address, which is how the stock objects kept duplicate pool words alive.

- **A compound assignment picks a commutative operation's destination.**
  `control = 0x84000000 | words;` computes into the variable's register and
  copies; `control = 0x84000000; control |= words;` computes into the constant's
  register and the copy disappears. Operand order in the expression does
  nothing — GCC canonicalises. Worth 9 halfwords on 08021be0, and *harmful* on
  080052f4, so check both ways.

## Indexing

- **`array[index]` versus a walking pointer is a real choice.** Strength
  reduction rewrites an index into a pointer; where the reference keeps
  `ldrb r3, [r2, r6]` with the loop counter as the offset, route the stem
  through `-fno-strength-reduce` (080a9d3c). Where the reference walks, do not.
  080b90ac wants the element-index spelling and gets *worse* with the flag.

- **A register-offset load needs its displacement in its own local.** Inline,
  GCC emits a separate add; hoisted into an `s32 offset`, it emits the single
  `ldr r0, [r5, r3]`. 08011fd8 went 100 B to 96 B on that. The inverse also
  happens (080ae99c wants the separate add), so match what the reference does.

## DMA descriptors

- **Grouped DMA is a compiler mode, not a source shape.** `stmia rN!, {r0, r1,
  r2}` then `subs rN, #12` is `-mgrouped-dma-store`. The source is three locals
  assigned *first*, then three stores through a `u32 *`. Constants written
  inline lose the grouping, because the address materialisation lands between
  two stores and `arm_pre_reload` requires them adjacent.

- **Struct fields work; whole-struct assignment does not.** `dma->source = …`
  groups exactly like `dma[0] = …`; `*dma = (struct …){…}` lowers to `memcpy`,
  which the gate rejects as an unsupported external symbol.

- **Zero-fill descriptors take the address of an uninitialised local.**
  `u32 zero; u32 *source = &zero; *source = 0;` then the three stores. Sixteen
  never-attempted regions carry this fingerprint; two of them converted on the
  first draft.

- **Descriptor pointer and status pointer are two pointers.** When a region
  writes a descriptor then polls DMA3CNT, declare both and make only the polled
  one `volatile`. One volatile pointer for everything loses the grouping; one
  plain pointer for everything loses every descriptor but the last.

- **A VLA in a nested block, never `__builtin_alloca`.** A reference frame with
  *two* stack-pointer saves (`mov r7, sp` and `mov r8, sp`, restored in that
  order) is a variable-length array declared in a nested block, which emits both
  a function-level and a block-level save. `__builtin_alloca` emits only the
  first and cannot produce that frame.

## Compiler behaviours worth knowing

- **The pre-reload scheduler is inert.** 40 converted sources compile
  byte-identically with `-fschedule-insns` and `-fno-schedule-insns`. Every
  member of `UNSCHEDULED_SOURCES` is carried by `-fno-schedule-insns2` alone.

- **`arm_reorg` pulls split constants back together, and sometimes should not.**
  `-fno-thumb-contiguous-immediate` disables it. 080a1090 was four bytes out for
  a whole session on exactly this and is byte-exact with the flag.

- **Read the RTL dumps before proposing a mode.** On 080a1090 three modes were
  proposed by inspection and every one made it worse (4 bytes to 16, to 29, and
  no change). `-da` plus gating passes one at a time found the real cause in
  twenty minutes. The `.18.greg` and `.23.sched2` dumps show what each stage
  actually did; sched2 is frequently innocent.
