# Humanization throughput queue — 2026-07-28

The first 20-file edit passed the claimed-code build but failed the complete
source-ownership audit at alignment byte `0x080a3eee`. A non-destructive split
against clean `HEAD` isolated the result into two ten-file halves.

## Accepted half

These ten files preserve all 90,664 claimed bytes and pass the complete
8,388,608-byte source-only ownership build with zero unowned bytes:

- `src/080df90c.c`
- `src/080b8228.c`
- `src/080b83b4.c`
- `src/080b8000.c`
- `src/080ad318.c`
- `src/080ad5b4.c`
- `src/080ad5f4.c`
- `src/080ad608.c`
- `src/080ad658.c`
- `src/08012038.c`

## Parked half

These files were reverted rather than debugged in this pass. As a group they
reduce the claimed-byte total from 90,664 to 90,654 and make the full audit
reject an unproven assembly alignment at `0x080a3eee`:

- `src/08078414.c`
- `src/080a3e88.c`
- `src/080a5614.c`
- `src/080a56c8.c`
- `src/080a9aec.c`
- `src/080a9e48.c`
- `src/080aa448.c`
- `src/080a32b8.c`
- `src/08099738.c`
- `src/08099810.c`

The associated draft headers were `include/item.h`, `include/item_menu.h`, and
`include/cloak.h`; none are part of the accepted half.

## Next 20

All twenty are already exact C and belong to one call-connected owner/inventory
family.

### Half A — inventory helpers

- `src/0807842c.c`
- `src/080784b0.c`
- `src/080784d8.c`
- `src/08078500.c`
- `src/08078664.c`
- `src/08078698.c`
- `src/0807882c.c`
- `src/08078870.c`
- `src/08078948.c`
- `src/08078980.c`

### Half B — owner and party bookkeeping

- `src/08078bc0.c`
- `src/0807961c.c`
- `src/08079664.c`
- `src/080796c4.c`
- `src/0807a1b4.c`
- `src/0807a2bc.c`
- `src/0807a2e4.c`
- `src/0807a350.c`
- `src/0807a498.c`
- `src/0807a5bc.c`

Use neutral, evidence-backed owner and party layouts. Preserve the overlapping
views at owner offsets `+0xf8/+0x108`, and leave `0807a498` until last because
its byte-offset expression is codegen-sensitive.
