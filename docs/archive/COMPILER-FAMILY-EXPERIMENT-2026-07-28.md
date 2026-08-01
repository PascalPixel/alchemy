> **C/H hard blocker:** Never use `asm(...)`, `__asm(...)`, `__asm_(...)`, `__asm__(...)`, fixed-register bindings, or empty assembly barriers. Byte equality never overrides this rule.

# Historical compiler-family experiment — 2026-07-28

## Clean-source boundary

This experiment used only public compiler sources. No SDK or provenance-
problematic project code, binaries, headers, libraries, samples, names, or
documentation were accessed. Source provenance and complete incorporated-file
hash manifests live in the sibling `alchemy-gcc` repository:

- `pret-early-thumb-PROVENANCE.md`
- `pret-early-thumb-SHA256SUMS`
- `gcc-2.95.1-PROVENANCE.md`
- `gcc-2.95.1-SHA256SUMS`

## Families

- `old-agbcc`: Alchemy's existing older Cygnus family.
- `pret-early-thumb`: experimental family assembled from the earliest mutually
  compatible snapshots found in public pret/agbcc. No vendor revision identity
  is asserted.
- `gcc2951`: official stock GCC 2.95.1 configured for its separate Thumb/COFF
  target. Its assembly is accepted by the GNU ARM ELF assembler; stock 2.95.1
  has no Thumb/ELF configuration.
- `gcc296`: Alchemy's stock GCC 2.96 family without source-specific routing.
- `routed`: the production source-aware combination of GCC 2.96, the existing
  older compiler, and evidenced flags.

## Exact-C corpus

All rows compiled the same 1,366 claimed GS1 sources.

| Family | Exact | Regressions |
|---|---:|---:|
| routed | 1,366 | 0 |
| gcc296 | 1,242 | 124 |
| old-agbcc | 238 | 1,128 |
| pret-early-thumb | 206 | 1,160 |
| gcc2951 | 152 | 1,214 |

The experimental intermediate family does not improve the broad corpus over
the existing older family. GCC 2.96 remains overwhelmingly closer, while the
production routed configuration is required to retain every exact claim.

## Near-match cohort

The same three parked candidates were compiled with each family and no
source-specific modes. Values are `candidate/reference bytes; differing
halfwords`.

| Family | 0808fecc | 08097540 | 080a6a98 |
|---|---:|---:|---:|
| old-agbcc | 44/48; 21 | 196/200; 89 | 200/204; 102 |
| pret-early-thumb | 44/48; 21 | 196/200; 89 | 200/204; 102 |
| gcc2951 | 44/48; 21 | 196/200; 89 | 200/204; 102 |
| gcc296 | 44/48; 16 | 200/200; 10 | 204/204; 12 |

The three pre-2.96 families produce identical machine-code scores throughout
this cohort. No shared exact configuration or multi-region improvement
emerged.

## Decision

Keep both new families available for reproducible historical experiments, but
do not route production sources to them and do not infer new source-specific
backend modes from this result. Continue prioritizing compiler-family changes
only when they improve multiple unrelated regions.
