# Discovery queue (measured 2026-07-30)

**`asm_c_debt_bytes=395816` and the overlay inventory's ~16 KB adoptable queue
do not describe the same bytes, and neither one is missing the other's work.**
The debt is 100% main-image assembly under `asm/`; the overlay inventory reads
only `assets/code/*_overlay.s` and contributes exactly **0 bytes** to
`asm_c_debt_bytes`. The apparent 380,000-byte hole between them is a category
error, not a discovery deficit.

The corollary is the useful part: the main-image debt is **already enumerated at
function granularity**. 727 regions, one function body per file, every boundary
proven by ROM byte-equality. Nothing needs to be discovered before it can be
drafted. This file is that queue.

Measured from `out/full/asm/manifest.json` (written by `tools/build_asm.ts`,
`verification=rom`), the classifier in `tools/remaining_survey.ts`, and
`tools/overlay_inventory.ts`. Region counts are scheduling diagnostics, not
project progress; the headline metric remains Full-C Byte Share.

## 1. Where the 395,816 bytes live

`asm_c_debt_bytes` is not a separate measurement. `tools/build_full.ts`
partitions every assembly region by its `retention` field:
`assemblySourceAccounting()` sums the five debt retentions (`c_candidate`,
`split_first`, `merge_with_owner`, `merge_with_function_owner`,
`merge_with_continuations`) into `cDebtBytes`, and the three keep retentions
(`keep_asm`, `keep_structured_asm`, `adjacent_section_alignment`) into
`retainedStructuralBytes`. `tools/audit_residuals.ts` asserts the two sum to
`asm_bytes`.

So the per-kind census printed by `bun tools/build_asm.ts` reconciles exactly,
with every kind falling on one side or the other:

| kind | retention | regions | bytes | debt? |
| --- | --- | ---: | ---: | --- |
| `compiler_output` | `c_candidate` | 631 | 357,360 | **yes** |
| `mixed_region` | `split_first` | 30 | 27,352 | **yes** |
| `multi_region_function_head` | `merge_with_continuations` | 9 | 3,820 | **yes** |
| `multi_region_function_continuation` | `merge_with_function_owner` | 7 | 2,928 | **yes** |
| `executable_gap_continuation` | `merge_with_owner` | 41 | 2,710 | **yes** |
| `parent_function_fragment` | `merge_with_owner` | 9 | 1,646 | **yes** |
| | | **727** | **395,816** | |
| `nonstandard_thumb_call_module` | `keep_structured_asm` | 64 | 26,278 | no |
| `hidden_register_context_module` | `keep_structured_asm` | 17 | 6,598 | no |
| `relocated_arm_runtime_module` | `keep_structured_asm` | 13 | 6,208 | no |
| `relocated_iwram_payload` | `keep_asm` | 1 | 5,120 | no |
| `linker_veneer` | `keep_asm` | 608 | 4,864 | no |
| `armv4t_helper_bank` | `keep_structured_asm` | 7 | 2,652 | no |
| `alignment_padding` | `adjacent_section_alignment` | 330 | 660 | no |
| 20 further keep classes | | 40 | 4,404 | no |
| | | **1,080** | **56,784** | |
| **all assembly** | | **1,807** | **452,600** | |

Split by target, the debt is unambiguous:

| target | debt regions | debt bytes |
| --- | ---: | ---: |
| main image (`asm/*.s`, run address `0x08xxxxxx`) | 727 | 395,816 |
| overlays (`assets/code/*_overlay.s`) | 0 | 0 |

Every one of the 1,807 assembly regions has its `source` under `asm/`, and the
only run-address banks present are `0x08` (ROM) and `0x03` (the single relocated
IWRAM payload, which is a keep class). Overlay code is not in this manifest at
all — it lives inside compressed resources, is owned by the asset manifest, and
is tracked separately by `metrics/gs1-en-executable.json`.

## 2. Why those bytes are not queued anywhere

Four distinct reasons, separated because they need different responses.

### 2a. Never-C by classification — 56,784 bytes, already excluded

This is not part of the 395,816 and never was. The keep classes are in the
assembly denominator but explicitly outside the debt. The classification work
is live and has moved material *out* of debt: 49 regions containing
`mov ip, pc` (25,930 bytes) now sit in `nonstandard_thumb_call_module`, which
is why the 43 "not emittable" regions in the 2026-07-26 survey have shrunk to
**one row of 62 bytes** in the debt today. No action; this bucket is correct.

### 2b. The overlay inventory does not walk the main image — the whole 380 KB

`discoverOverlay()` in `tools/overlay_inventory.ts` constructs
`new Discovery(data, OVERLAY_BASE)` where `data` comes from
`assembleOverlay(overlay.source, OVERLAY_BASE)` and `OVERLAY_BASE` is
`0x02000000`. Its input set is the 96 files matching `assets/code/*_overlay.s`.
The main image is not in that set. **This accounts for the entire gap**: the
~16 KB queue and the 395,816 bytes are disjoint worlds, and the overlay
inventory was never the main image's queue.

For the record, the overlay-side filtering the question describes is real but
is a separate problem: 540 rows, of which 220 are `contained_by`, 43 are
`structural_veneer`, and 51 more non-contained rows are `data_walk` — and those
51 carry **89,328 code bytes**, far more than the 16,194 that survive. The
`data_walk` filter, not `contained_by`, is where the overlay bytes go.

### 2c. Regions walked but not emitted as function rows — 0 bytes, main image

There is no unwalked main-image region. `tools/audit_residuals.ts` proves
`unowned_bytes=0`: the claimed, assembly, and asset manifests together span all
8 MiB with no gap, and `tools/build_asm.ts` rejects any overlap. Each debt
region's span is verified against the ROM (`verification=rom`), and each abuts
either a claimed C region or another assembly region.

Boundary quality is measured, not assumed:

- **671 of 727** debt regions contain exactly one `.thumb_func`.
- **Zero of 727** contain a second `push {…, lr}` prologue. Every debt region
  is one function body or one fragment of one — not a multi-function blob.
- **632 rows / 357,388 bytes** carry a `Func_XXXXXXXX` global at the region
  start: a known function entry with a ROM-proven span.

`bun tools/build_asm.ts --source asm/080bbb0c.s` returns
`regions=1 bytes=6332` against the ROM, which is a stronger boundary proof than
the `overlay_adopt` dry-run oracle used on the overlay side — it is byte
equality over the whole span, not just non-straddling ends.

### 2d. Rows that exist but need a structural step first — 38,428 bytes

The 95 rows without a `Func_` entry are queued but not directly draftable. They
are not undiscovered; they are known to be shaped wrong for one-file-one-function
adoption:

<!--NONFUNC-->
| rows | bytes | kind | entry symbol | what it needs first |
| ---: | ---: | --- | --- | --- |
| 30 | 27,352 | `mixed_region` | `Region_` | split the interleaved data out into its own region |
| 9 | 3,820 | `multi_region_function_head` | `FunctionHead_` | relink head + continuation into one function |
| 7 | 2,928 | `multi_region_function_continuation` | `Continuation_` | merge into its `FunctionHead_` owner |
| 39 | 2,350 | `executable_gap_continuation` | `Fragment_` | merge into the owning function |
| 9 | 1,646 | `parent_function_fragment` | `Fragment_` | merge into the parent function that precedes it |
| 1 | 332 | `compiler_output` | `Region_` | confirm the single-function boundary |
| **95** | **38,428** | | | |
<!--/NONFUNC-->

The nine `FunctionHead_` and seven `Continuation_` rows are about nine functions
between them, so merging is a 16-region, ~9-function job worth 6,748 bytes.

One further gap worth naming: `tools/remaining_survey.ts` filters
`retention === "c_candidate"`, so it reports 631 regions and is blind to the
other 96 debt rows (38,456 bytes). Widening that filter to the five debt
retentions is a one-line change and makes the survey agree with
`asm_c_debt_bytes`.

## 3. The prioritized queue

All 727 rows, ranked by size. `boundary=function` means a `Func_` entry with a
ROM-proven span — draftable as-is. `boundary=region` means section 2d applies
first. A region row is an accounting fragment, not permission to admit a
head-only semantic function: follow direct branches carrying a live frame and
high registers, then register the complete owner in
`semantic/main-regions.json`, excluding pools and data gaps. `08026080`
demonstrates the rule: its 2,138-byte ranked row is one 3,442-byte function
across the `08026080`, `0802691c`, and `08026b44` executable ranges. Blocker
classes are `tools/remaining_survey.ts`'s, extended to every debt retention.

<!--TOP40-->
| # | address | size | insns | kind | boundary | blocker |
| ---: | --- | ---: | ---: | --- | --- | --- |
| 1 | `0x080bbb0c` | 6,332 | 2,483 | `compiler_output` | function | `0xffff` AND mask |
| 2 | `0x080ea0d8` | 5,756 | 2,461 | `compiler_output` | function | DMA descriptor, no poll |
| 3 | `0x080ab5e4` | 4,888 | 2,110 | `compiler_output` | function | plain |
| 4 | `0x08027114` | 4,224 | 1,743 | `compiler_output` | function | plain |
| 5 | `0x080f6440` | 3,804 | 1,731 | `compiler_output` | function | DMA descriptor, no poll |
| 6 | `0x080dea70` | 3,656 | 1,609 | `compiler_output` | function | plain |
| 7 | `0x080e7404` | 3,640 | 1,537 | `compiler_output` | function | DMA descriptor, no poll |
| 8 | `0x080d1714` | 3,384 | 1,484 | `compiler_output` | function | `0xffff` AND mask |
| 9 | `0x08023178` | 3,320 | 1,397 | `compiler_output` | function | plain |
| 10 | `0x080d6970` | 3,308 | 1,437 | `compiler_output` | function | plain |
| 11 | `0x080a2680` | 3,128 | 1,271 | `compiler_output` | function | plain |
| 12 | `0x080ca60c` | 2,968 | 1,262 | `compiler_output` | function | `0xffff` AND mask |
| 13 | `0x08023e70` | 2,756 | 1,203 | `compiler_output` | function | plain |
| 14 | `0x080cbc0c` | 2,508 | 1,054 | `compiler_output` | function | plain |
| 15 | `0x080eb754` | 2,444 | 1,031 | `compiler_output` | function | `0xffff` AND mask |
| 16 | `0x0808c4f8` | 2,428 | 971 | `compiler_output` | function | DMA descriptor, no poll |
| 17 | `0x080f7460` | 2,388 | 950 | `compiler_output` | function | DMA descriptor, no poll |
| 18 | `0x080dc968` | 2,372 | 962 | `compiler_output` | function | plain |
| 19 | `0x080d2464` | 2,356 | 988 | `compiler_output` | function | plain |
| 20 | `0x0808f52c` | 2,316 | 890 | `compiler_output` | function | plain |
| 21 | `0x08021e6c` | 2,300 | 1,044 | `compiler_output` | function | plain |
| 22 | `0x080d91dc` | 2,268 | 965 | `compiler_output` | function | plain |
| 23 | `0x080e302c` | 2,188 | 940 | `compiler_output` | function | plain |
| 24 | `0x08026080` | 2,138 | 978 | `mixed_region` | region | plain |
| 25 | `0x08024934` | 2,124 | 948 | `compiler_output` | function | plain |
| 26 | `0x08077428` | 2,024 | 867 | `compiler_output` | function | plain |
| 27 | `0x080d89ac` | 2,024 | 882 | `compiler_output` | function | `0xffff` AND mask |
| 28 | `0x080acab8` | 1,980 | 869 | `compiler_output` | function | plain |
| 29 | `0x080e823c` | 1,968 | 821 | `compiler_output` | function | `0xffff` AND mask |
| 30 | `0x080f7f78` | 1,920 | 879 | `compiler_output` | function | plain |
| 31 | `0x0802592c` | 1,876 | 797 | `compiler_output` | function | plain |
| 32 | `0x080bd898` | 1,876 | 797 | `compiler_output` | function | plain |
| 33 | `0x080bae40` | 1,864 | 740 | `compiler_output` | function | plain |
| 34 | `0x08025200` | 1,836 | 777 | `compiler_output` | function | plain |
| 35 | `0x080e99c0` | 1,816 | 781 | `compiler_output` | function | `0xffff` AND mask |
| 36 | `0x080dab74` | 1,776 | 784 | `compiler_output` | function | plain |
| 37 | `0x080d52c8` | 1,768 | 758 | `compiler_output` | function | `0xffff` AND mask |
| 38 | `0x080d4604` | 1,764 | 777 | `compiler_output` | function | `0xffff` AND mask |
| 39 | `0x080e89ec` | 1,724 | 745 | `compiler_output` | function | plain |
| 40 | `0x080e2974` | 1,720 | 744 | `compiler_output` | function | plain |
<!--/TOP40-->

The full 727-row table is not transcribed here; regenerate it from the manifest
with the retention filter above. The top 20 rows are 67,976 bytes (17.2% of the
debt) and the top 100 are 185,288 bytes (46.8%) — this queue is
size-concentrated, so ranking by size matters.

### Size distribution

<!--LADDER-->
| size | rows | bytes | share |
| --- | ---: | ---: | ---: |
| under 64 B | 38 | 1,836 | 0.5% |
| 64-127 B | 91 | 8,654 | 2.2% |
| 128-255 B | 167 | 30,922 | 7.8% |
| 256-511 B | 195 | 69,258 | 17.5% |
| 512-1,023 B | 131 | 94,670 | 23.9% |
| 1,024-2,047 B | 80 | 111,482 | 28.2% |
| 2,048 B and up | 25 | 78,994 | 20.0% |
| **total** | **727** | **395,816** | **100%** |
<!--/LADDER-->

### Blocker distribution

<!--BLOCKER-->
| rows | bytes | blocker class |
| ---: | ---: | --- |
| 548 | 284,856 | plain |
| 135 | 63,000 | DMA descriptor, no poll |
| 36 | 47,458 | `0xffff` AND mask |
| 7 | 440 | twelve-store record |
| 1 | 62 | return address in `ip` |
<!--/BLOCKER-->

`plain` means no identified construct blocker — the cost is drafting time and
the usual allocation and scheduling residuals. **72% of the debt has no
diagnosed construct blocker at all.**

### Cheapest draftable tier

Function-boundary rows with no construct blocker, by instruction count. 105
such rows exist at 80 instructions or fewer, worth 13,768 bytes; 177
function-boundary rows of any blocker class fall under that threshold, worth
22,188 bytes.

| address | size | insns | pool words |
| --- | ---: | ---: | ---: |
| `0x080fb2a4` | 40 | 18 | 1 |
| `0x0800651c` | 64 | 20 | 6 |
| `0x08077394` | 68 | 28 | 3 |
| `0x080063bc` | 76 | 28 | 5 |
| `0x08006408` | 80 | 28 | 6 |
| `0x080f7f30` | 72 | 30 | 3 |
| `0x080f9ef8` | 68 | 31 | 1 |
| `0x0801fd34` | 80 | 33 | 3 |
| `0x080c00d8` | 88 | 33 | 4 |
| `0x0800430c` | 76 | 34 | 2 |
| `0x08004278` | 80 | 34 | 3 |
| `0x080a3ddc` | 76 | 36 | 1 |
| `0x080ae99c` | 84 | 37 | 2 |
| `0x080c1fa8` | 84 | 39 | 1 |
| `0x08004144` | 84 | 40 | 1 |
| `0x080a1f74` | 96 | 40 | 4 |
| `0x080f9438` | 142 | 40 | 15 |
| `0x080ae9f0` | 92 | 41 | 2 |
| `0x080f0614` | 100 | 41 | 4 |
| `0x08011fd8` | 96 | 42 | 3 |

`0x08006408`, `0x0800430c` and `0x080f7f30` are documented near-misses in
[REMAINING.md](REMAINING.md) — they are cheap to re-score but each parks on a
different residual mechanism.

## 4. The cheapest way to widen discovery

Discovery is **not** the constraint on the main image: the queue above is
complete and needs no walking. Discovery *is* the constraint on overlays, and
one measured change dominates.

**File `tools/overlay_inventory.ts`, function `discoverOverlay()` — the
after-return adjacency recovery block.** Today that block walks forward from a
verified return and gives up at the first halfword that is neither `0x0000` nor
`0x46c0` (`nop`), within 8 bytes:

```text
if (![0, 0x46c0].includes(half)) break;
```

A GCC-Thumb function's return is followed by **its own literal pool**. Those
words are arbitrary data, so this loop breaks immediately on essentially every
real function boundary. The walker refuses to step over a literal pool, and
therefore almost never finds the next function by adjacency.

The data needed to fix it already exists: `Discovery` in `tools/discover.ts`
records every pc-relative load target in `literal_slots`. Advancing `candidate`
over the contiguous 4-aligned run of `literal_slots` entries before testing for
a `0xb5xx` prologue is a few lines inside that same block.

Measured on a read-only copy of `discoverOverlay()` across all 96 overlays,
scored against the 835 overlay functions already known to be functions because
they have exact C:

| variant | converted functions rediscovered | functions walked | strict queue rows | strict queue code bytes |
| --- | ---: | ---: | ---: | ---: |
| stock | 119 / 835 (14.3%) | 685 | 20 | 6,110 |
| pool-skip | **327 / 835 (39.2%)** | 4,449 | **460** | **108,446** |

Recall on known-good functions nearly triples and the strict queue widens 18x.
"Strict queue" here counts unconverted entries that start with a prologue, reach
a return, are not nested inside another walked function, and do not decode
through data — a tighter filter than the inventory's own, used so the two
variants are compared on identical terms.

This also explains the `undiscovered_converted_functions=716` the inventory
already prints and nobody acts on. It is not a prologue-shape problem:
**743 of the 835 converted overlay functions (89%) start with `0xb5xx`**, the
exact shape the seeder recognizes. They are missed because nothing reaches
them, and adjacency is the reach that is broken.

Two smaller levers behind it:

- The `first-prologue` seed loop `break`s after **one** seed. Seeding every
  `0xb5xx` in the header window costs nothing and is independent of the above.
- `tools/remaining_survey.ts` filters `retention === "c_candidate"`. Widening it
  to the five debt retentions makes it report 727 rows / 395,816 bytes and
  agree with `asm_c_debt_bytes`.

Precision is the honest caveat: functions walked rises from 685 to 4,449, so
some new seeds are pool words that happen to look like prologues. The existing
`contained_by`, `structural_veneer` and `data_walk` filters plus a
`overlay_adopt` dry-run per candidate are the triage, and they are cheap.

## 5. How much of the 395,816 bytes is really convertible

| bytes | rows | assessment |
| ---: | ---: | --- |
| 357,388 | 632 | **Convertible.** Single function bodies, ROM-proven spans, `Func_` entries. Obstacles are compiler fidelity and drafting time, not structure. |
| 11,014 | 64 | **Convertible after a merge.** Heads, continuations and fragments of otherwise ordinary functions; needs the section 2d relink, then joins the tier above. |
| 27,352 | 30 | **Partly convertible, not measured.** `mixed_region` interleaves code and data. The code fraction cannot be stated without performing the split, and I did not perform it. |
| 62 | 1 | **Permanently structural.** `Fragment_0800f1bc`, the single remaining `mov ip, pc` row; neither approved compiler emits it. |
| **395,816** | **727** | |

**So roughly 368,000 of the 395,816 bytes (93%) are function-granular ordinary
compiler output that should become C, and only 62 bytes are measurably never-C.**
That is expected rather than surprising: the classification pass has already
moved the genuinely structural material into the 56,784-byte keep bucket, so
`asm_c_debt_bytes` is a well-curated figure and not an upper bound to be
discounted.

The unmeasured quantity is the code/data ratio inside 27,352 bytes of
`mixed_region`. Splitting one of the 30 — `0x08026080` at 2,138 bytes is the
largest — would calibrate the rest.

## What this changes

The bottleneck is not discovery. **The main image has a complete, boundary-proven,
727-row queue that nothing has been drafting against**: the `work/hand/` tree
referenced by [REMAINING.md](REMAINING.md) no longer exists, and all 575 notes
under `work/claude/notes/` are `resource_*` overlays. The overlay lane
absorbed the effort while a 395,816-byte queue with 72% of its bytes carrying no
construct blocker sat unattended. Section 3 is that queue; section 4 is what to
fix if the overlay lane is resumed instead.
