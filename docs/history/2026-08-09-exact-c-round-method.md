# Exact-C production round, 2026-08-09

This is the measured origin of the repository's bounded parallel-round method.
It is historical evidence, not a live queue; current rules are in
[`AGENTS.md`](../../AGENTS.md) and [`CONTRIBUTING.md`](../../CONTRIBUTING.md).

## Result

The round began at commit `0b4cdedb1` with 321,934 Exact-C bytes and ended at
`fe064ffa2` with 350,806. It gained 28,872 byte-exact executable bytes without
weakening the clean-room, no-assembly, compiler-admission, or owner-adoption
boundaries.

| Checkpoint | Exact C | Gain |
|---|---:|---:|
| `0b4cdedb1` | 321,934 | baseline |
| `237ed1a25` | 335,680 | +13,746 |
| `9c00366f0` | 343,228 | +7,548 |
| `468b1adcd` | 347,254 | +4,026 |
| `5f326ece3` | 349,238 | +1,984 |
| `fe064ffa2` | 350,806 | +1,568 |

Every checkpoint passed the byte-identical ROM build and the authoritative
repository gate. The final round ran 112/112 native self-tests. Subsequent
commits continued from this baseline; use generated metrics for current counts.

## What produced the throughput

1. **Families before isolated size.** `overlay_twins --leads` identified one
   source-reading task that could unlock several owners. Exact siblings supplied
   witnessed C shapes, making transposition cheaper than fresh invention.
2. **Disjoint lane contracts.** Each worker received explicit owners, spans,
   potential bytes, permitted files, forbidden shared mutations, search bounds,
   and required measurements. Source workers did not adopt or commit.
3. **One compiler lane.** Only one worker could alter `alchemy-gcc/` and the
   staged compiler. Other workers held the compiler fixed, while the coordinator
   integrated root routing and approved digests. This avoided ambiguous exact
   witnesses and shared-runtime races.
4. **A coordinating integrator.** The coordinator preserved the baseline,
   avoided active lane files, reviewed every result, added path-specific routes,
   serialized adoption, regenerated metrics, and owned git history.
5. **Bounded searches and honest stops.** Workers tried witnessed forms and
   focused deterministic searches first. Permutation was capped. A large
   1,928-byte family whose source remained diffuse was restored rather than
   promoted or allowed to consume the round indefinitely.
6. **Focused iteration, one final gate.** Candidate comparison and cached native
   tools answered local questions quickly. The expensive corpus was rerun only
   when the compiler changed, and `make verify` ran after integration rather
   than after every edit.
7. **Serialized mutation.** Overlay adoption was deliberately sequential because
   every adoption rewrites shared assembly. Semantic retirement, inventory
   refresh, progress generation, verification, commit, and push formed one
   closing wave.

The compiler description above records authority in this historical round; it
does not authorize ordinary workers to edit compiler or routing state. Current
authority is defined in [`AGENTS.md`](../../AGENTS.md) and
[`CONTRIBUTING.md`](../../CONTRIBUTING.md).

## Stop rules learned

- Raw owner size is not expected value. Prefer exact bytes plausibly unlocked
  per unit of source understanding.
- Wrong size or diffuse mismatches mean reconstruction, not more flags.
- A near-match is not progress; only owner-specific adoption moves Exact C.
- A worker may retain truthful semantic improvement, but must remove its own
  speculative regressions and report the residual precisely.
- Do not start another round while the current round still has active workers or
  unintegrated shared changes.
- Assume agents share the checkout and generated outputs unless isolation is
  explicit. Disjoint write ownership is therefore a correctness rule, not just
  project management.
