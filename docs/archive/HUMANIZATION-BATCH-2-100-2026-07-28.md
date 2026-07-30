# Second 100-file humanization batch — 2026-07-28

This run used the same wide-batch, narrow-isolation method as the first
100-file pass. Ten cohorts of ten were attempted independently; failures were
parked immediately while exact cohorts continued through review.

## Result

- 100 queued sources attempted.
- 83 queued sources retained byte-exact.
- 17 queued sources parked without extended debugging.
- 26 adjacent callers received a byte-exact `Func_0808ba1c` ABI cleanup.
- 109 source files and 6 shared headers are in the final reviewed patch.
- Claimed code: 1,345 linked, zero failures, 90,664 bytes.
- Source-only ownership: all 8,388,608 bytes, zero unowned bytes.
- Full ROM: byte-identical, zero fallback bytes.
- Aggregate self-tests: green.

Review corrected volatile hardware access, callback typing, script-state
signedness, object field semantics, and the project-wide object-lookup return
type. One rewrite with a null dereference before its guard was parked.

## Parked sources

### Transfer runtime

- `src/080037d4.c`
- `src/0800383c.c`
- `src/0800387c.c`
- `src/080038bc.c`
- `src/080038fc.c`
- `src/0800393c.c`
- `src/0800397c.c`
- `src/080039bc.c`
- `src/08003a3c.c`

### Event runtime

- `src/08091c1c.c`
- `src/08091e6c.c`
- `src/08091fa8.c`

### Object runtime

- `src/080920a0.c`
- `src/08092548.c`
- `src/080925cc.c`
- `src/080925e0.c`
- `src/08092708.c`

## Next 100

The third pass is already partitioned into ten independent cohorts.

### A — resource/archive

`080051d8`, `08005258`, `08005904`, `080051e8`, `08005868`, `08005208`,
`08005340`, `08005394`, `080053e8`, `08005810`

### B — save/state

`080064f4`, `08006384`, `08006b84`, `08006c24`, `08006358`, `08006458`,
`08006488`, `080064b8`, `08006a00`, `08006ba8`

### C — dispatch

`0800c454`, `0800c3ec`, `0800c430`, `0800c0cc`, `0800c2d8`, `0800c4bc`,
`0800c388`, `0800c300`, `0800c344`, `0800c0f4`

### D — map loader

`08011ae0`, `08011af0`, `08011bc8`, `08011984`, `080119a8`, `080118a8`,
`08011164`, `0801173c`, `080110e0`, `08011b00`

### E — battle state

`0801964c`, `080197b4`, `08019ebc`, `08019d0c`, `080198dc`, `08019a54`,
`08019908`, `0801999c`, `08019da8`, `08019944`

### F — battle controller

`0801c17c`, `0801c2e4`, `0801c2d0`, `0801c2f0`, `0801c21c`, `0801c0c8`,
`0801c3e8`, `0801c304`, `0801c0dc`, `0801c34c`

### G — position math

`08028d74`, `0802851c`, `0802875c`, `080288a8`, `0802899c`, `080284dc`,
`080287a8`, `08028b80`, `08028920`, `080289e8`

### H — menu/UI

`0808b05c`, `0808b02c`, `0808b048`, `0808b248`, `0808b3d0`, `0808b074`,
`0808b25c`, `0808b1d8`, `0808b398`, `0808b2b0`

### I — action

`080b2110`, `080b20e8`, `080b2764`, `080b2778`, `080b27b0`, `080b2720`,
`080b280c`, `080b2884`, `080b26cc`, `080b28d4`

### J — sequencer

`080fb690`, `080fb670`, `080fb6ec`, `080fb700`, `080fb714`, `080fb728`,
`080fb73c`, `080fb750`, `080fb6a4`, `080fb4a4`
