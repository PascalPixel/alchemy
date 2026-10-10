**TBS Thumb with stock GCC 2.95.2 — source-rewrite trial, 10 October 2026**

Stock 2.95.2 did not replace the July 2000 Thumb compiler in this trial. Twelve
manually rewritten modules produced no exact whole-module matches. A bounded
source search also found none. More decisively, the trial identified a short
leaf-function stack pattern that the July backend explains and the stock 2.95.2
backend does not reproduce under the fixed game configuration.

This answers a gap in the [broad compiler study](study.md): compiling C already
shaped for 2.96 with an older compiler is not sufficient to reject that compiler.
Here the source was changed, and the recurring obstacle was checked separately
with small C probes, backend source and native bytes. The recommendation remains
July-era GCC for Thumb and stock 2.95.2 for the demonstrated ARM routines.
No production source, compiler, option, route, rule or credit changed.

**Frozen trial.** The source state is glyph `7bad69deed` (only the broad study's
report differs from its source baseline). The compiler is the untouched stock
GCC 2.95.2 `thumb-coff` cc1 from the earlier pinned 32-bit build, SHA-256
`3ce52facc98ece7b678a2b2b22caf37bd1a880358e3da9ed9478a6aa93ecc974`.
Every game-code candidate uses exactly:

```
-O2 -mthumb-interwork -fcall-used-r4
```

The stock release has separate ARM and Thumb backends; this is its Thumb backend.
The production preprocessor and assembler are kept fixed. Edition definitions
and includes follow the existing build; GCC-version preprocessing macros use
2.9 for this trial. The broad study found no game-source uses of those version
macros. Candidate output is never rewritten.

Twelve ordinary-source translation units cover 33 functions and 1,684 bytes of
English reference text, including literal pools. They were chosen for distinct
mechanisms and manageable manual reconstruction, not as a random sample. The
two battle modules were inspected before registration. This is an exploratory
trial, not a new independent validation set or a whole-game match percentage.

| Case | Mechanism | Reference text bytes | Baseline exact | Manual rewrite exact | Search exact |
|---|---|---:|---|---|---|
| battle | Stack array passed to two services | 62 | No | No | No |
| classify | Effect classification and a call | 62 | No | No | No |
| facing | Field loads, angle calculation and clamping | 88 | No | No | No |
| cells | Stores to scene-state fields | 72 | No | No | No |
| party | Level-up loop with stack result storage | 60 | No | No | No |
| item | Table indexing, equipment tests and calls | 106 | No | No | No |
| outputs | Free-list removal, insertion and initialization | 164 | No | No | No |
| palette | Component loop and indirect division call | 84 | No | No | No |
| nodes | Linked-list traversal | 44 | No | No | No |
| lz | Byte stream, nested loops and back references | 144 | No | No | No |
| debug | Volatile input polling and conditional updates | 172 | No | No | No |
| curve | Sixteen sample/interpolation/table functions | 626 | No | No | No |

The manual rewrites use ordinary types, pointer locals, explicit countdowns,
early returns, structured loops, separated field loads and arithmetic, and small
inline interpolation helpers. They preserve the intended behavior and module
interfaces; they are unadopted drafts, not verified replacements. Existing
hardware/input volatility remains where required. No new volatile, fixed-register
variable, inline assembly, empty barrier or compiler patch was introduced.

Equality compares all nonempty allocated sections, their flags, alignment, size
and bytes, and symbolic relocations. Whole-module equality is the acceptance
test. Halfword edit distance only guides exploration: a smaller distance, equal
size, or an individual unchanged function does not count as a module match.
Complete symbol tables and linked-ROM placement still require adoption checks.

**Bounded search and its limits.** Alchemy's existing C parser, normalizer,
printer, RNG and mutation implementations were reused in an isolated study
adapter. Both starting forms for all twelve modules survived parse/normalize/
print with identical allocated sections and relocations under 2.95.2: 24/24
controls. All functions remain in each translation unit.

Each search pass allowed 256 attempts per module, 3,072 attempts in total,
starting from the original C, the manual rewrite and the best scored candidate.
One to three source transformations were applied per attempt with a recorded
deterministic seed. Attempts are not necessarily unique compiler outputs.

- The broad exploratory pass compiled 2,971 candidates, rejected 99 at
  compilation/assembly and skipped two attempts that found no applicable
  mutation. It found no exact module. It includes declaration and
  statement reordering, which can create invalid source hypotheses: one apparent
  improvement used an uninitialized pointer. Such lower scores are not reported
  as successful rewrites or usable near matches.
- A separate conservative pass excludes those reorderings, temporary insertion,
  type changes, variable expansion/removal and helper extraction. It explores
  blocks, pointer/member expression forms, arithmetic factoring, commutation,
  equivalent comparison/assignment forms and condition reversal. It compiled
  3,063 candidates and rejected nine. It also found no exact module. Automatically
  generated candidates still require semantic review before any adoption.

An initial adapter run omitted the normalizer required by Alchemy's mutation API
and stopped. It is retained as a setup failure and excluded from these totals.
The corrected adapter's normalization test and the reused parser/mutation/RNG
tests pass: 27 tests. The binary audit's data-exclusion and map tests also pass.

The bounded search cannot establish that every possible C rewrite fails. Its
negative result is strengthened by the following independent backend evidence.

**A short leaf function saves lr in TBS.** `NodeChain_GetNodeAtCount` is a
44-byte module whose native code starts with `push {lr}`, traverses a list using
short branches and low registers, and ends with `pop {r1}; bx r1`. It has no
function call, far branch, callee-saved-register save, stack local or inline asm.
Its C object is actually linked and this same complete text is present in all
six verified TBS editions, including Japanese.

Stock 2.95.2 omits that push/pop and returns through lr. This is not only a result
of our existing C. A small unrelated probe shows the same backend distinction:

```c
int SelectSign(int value)
{
    if (value < 0)
        return 2 * value;
    return value + 1;
}
```

With the fixed flags, July GCC emits `push {lr}` and `pop {r1}; bx r1`; 2.95.2
emits no frame and returns through `bx lr`. Conditional-expression, local-result
and goto versions were tested too, along with list traversal and a mask function.

The source explanation is specific:

- In 2.95.2, `gcc/config/arm/thumb.c` computes the low saved-register mask and
  adds lr when that mask is nonzero, the function contains a call, or
  `far_jump_used_p()` finds a final branch requiring BL. Its branch descriptions
  in `thumb.md` distinguish short branches, long B sequences and far BL sequences.
  `leaf_function_p()` in `final.c` scans remaining calls. Under the fixed
  configuration, a final short leaf with no low saved registers satisfies none
  of those conditions for the extra lr save.
- July's unified backend, `gcc/config/arm/arm.c:thumb_far_jump_used_p`, caches a
  possible far jump before final branch lengths settle. Once set, the decision
  stays set even if the final branch is short. Its own comments describe an
  unnecessary lr push/pop as a possible result. That behavior explains the
  native pattern without adding a device to the C.

The binary screen found 272 game-labelled English TBS functions of at most
256 bounded bytes starting with `push {lr}`, with no decoded BL or additional
push: 161 resident and 111 overlay functions. It reuses the broad study's
symbol bounds, mapping symbols and literal/jump-table exclusion. This is a
screening count, not 272 separate incapability proofs: ownership of raw code can
include unidentified libraries, and other handwritten/call conventions require
individual review. The six-edition menu function is the direct, ordinary-C anchor.

**Stock options do not provide the missing frame.** The probe grid tests both
compilers at the fixed base plus O1, Os, O0, preserved frame pointer, TPCS frames,
TPCS leaf frames, caller/callee super-interworking, interworking off, and
`-fcall-saved-lr`: 22 successful compiler/profile runs. This is a separate
capability experiment, not per-file flags for the game trial.

On 2.95.2, the normal optimized profiles still omit the short leaf's lr save.
Keeping a frame pointer adds r7 and frame setup; TPCS leaf frames add a 16-byte
backtrace structure; callee super-interworking adds an ARM entry sequence.
Those produce additional native instructions that the target does not have.
Making lr call-saved does not add the wanted leaf frame. No tested stock option
recovers the target shape. Profiling is not a solution: it adds instrumentation.

**Register preference is another family-wide difference.** The old Thumb target
does not define the unified backend's register allocation order. July explicitly
prefers r3, r2, r1, r0 for low scratch registers, and r10 before r9 among the
relevant high registers. The trial sees these differences in field accesses,
masks and palette interpolation. Source lifetimes can affect allocation, so
this alone is not an impossibility claim. It explains why many source changes
remain far from a match even when their operations are the same.

**Decision.** Do not replace the Thumb compiler with stock 2.95.2 on this
evidence. The trial recovered no complete module, and the leaf-frame mechanism
provides a reason beyond source bias for retaining the July-era backend. Fixing
that mechanism in 2.95.2 would be a target compiler modification; emitting the
missing frame through assembly would change the source constraints. Neither is
the requested unmodified compiler plus readable-C consolidation.

This does not identify Camelot's exact executable, rule out every historical
snapshot, or undo 2.95.2's positive ARM result. It narrows the practical choice:
keep the demonstrated Thumb family, use the independently supported ARM compiler,
and avoid a wholesale Thumb rewrite founded only on the hope that current C is
the entire problem.

**Retained evidence.** The full experiment is local at
`/private/tmp/alchemy-compiler-study/thumb-rewrite`. Its protocol, exact case
paths, original/manual C, commands, compiler diagnostics, per-attempt TSVs,
probes, screening results and adapter sources are preserved. A source/evidence
bundle is retained at `out/compiler-study/thumb2952/`; compiler source and private
reference objects remain outside that bundle. The parent study records all GNU
archive pins. The reference ROM never feeds compilation.

Final validation: all twelve unchanged production ROMs pass
`make compare-editions`. No rewritten module reached exact English equality,
so there is no candidate adoption to validate in the other editions; the
independent leaf-frame anchor was checked in all six TBS editions.
