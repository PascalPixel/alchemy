# How the branches work

Read this once before you next push. It describes who owns what, what reaches
you and how, and the things that will break your verify if you ignore them.

## Local era (2026-07-31 onward): hub and spoke

The remote ring below is retired with the remote team. Work now runs
locally as a hub with PR-style gates:

| branch | agent | role |
| --- | --- | --- |
| `main` | **Vale** (local) | the trunk: docs, board, metrics, coverage, merges |
| `jupiter` | **Jupiter** | worker lane, own worktree (`../alchemy-jupiter`) |
| `mars` | **Mars** | worker lane, own worktree (`../alchemy-mars`) |

```
        pull main (anytime)              merge after review
main ────────────────▶ jupiter/mars ────────────────▶ main
```

- Spokes branch from `main`, pull `main` whenever it moves, and commit
  only to their own branch in their own worktree (separate build caches —
  never share a checkout between concurrent agents).
- Vale merges a spoke into `main` only after its work passes the verify
  chain, the hooks, and the counter check — a PR review without the
  ceremony.
- **The channel is `/tmp/ALCHEMY_GROUP_CHAT.md`** — a running group chat for all
  agents, append-only, read at every loop boundary. The tracked MEETING.md
  is retired; durable rulings live in HANDOVER/LAWS/commit messages. Vale
  is engineering lead and authoritative on `main` and on disputes.
- Assignments are disjoint by overlay so spoke merges never collide.

Everything below documents the retired remote ring, kept for reading the
2026-07-30/31 history.

---

## The three agents

| branch | agent | goal |
| --- | --- | --- |
| `main` | **Vale** | the published trunk, its docs, tooling and the README coverage map |
| `mercury` | **Mercury Lighthouse** | **exact C** — fully linked machine-code byte equality |
| `venus` | **Venus Lighthouse** | **semantic C** — readable, correct, not byte-bound |

Work moves in a ring, each hop about every 20 minutes:

```
main ──▶ mercury ──▶ venus ──▶ main
      (Mercury      (Venus      (Vale merges
       pulls all)    pulls all)  all of venus)
```

Nobody pushes to a branch they do not own. If your work belongs on another
branch, it gets there because that branch's agent pulls it.

## What Vale does on the third hop

**The circuit is closed.** Vale merges *all* of `venus` into `main` — `src/`,
`asm/`, `semantic/`, everything — so semantic C and exact C both travel the whole
ring and Mercury receives both. It was docs-and-tooling-only until 2026-07-30
22:40Z; that restriction is gone.

So: **anything you want on `main` reaches it by you banking it on your own
branch.** Vale picks it up within 20 minutes. Do not push to `main` to get it
there faster.

The standing consequence, for @mercury: `build_semantic.ts` throws `duplicates
exact source` when a semantic source and an exact source share an address, and
`verify` runs `build:semantic`. So each conversion of yours that supersedes a
Venus source breaks your verify until that file is deleted. Mechanical, and the
error names both paths.

## The coverage map is Vale's, and only Vale's

`assets/readme/gs1-en-coverage.svg` and `metrics/gs1-en-coverage-map.json` are
regenerated on `main` and nowhere else.

- **Do not run `bun run coverage` from `mercury` or `venus`.**
- **Do not hand-edit either file, or the README section that embeds them.**
- You would either collide with Vale or publish a figure your branch cannot
  substantiate.

**Publish numbers by banking them — the map follows within the hour.** Your
banked metrics are exactly what Vale draws from.

Two changes already on `main` make this safe, and you will see them arrive:

1. **`bun run verify` no longer ends with `bun run coverage:check`.** It used to.
   With the map no longer any lighthouse's to refresh, that check was a red
   verify neither of you could clear — it would have blocked banking on both
   branches. It is gone from `verify`. A map lagging your newest commit is never
   your problem and never blocks a bank.
2. **`tools/bank_cycle.sh` no longer redraws the map.** It briefly did. Those
   lines are removed; the script's other behaviour is unchanged.

If you find yourself resolving a merge conflict in the SVG or the map JSON:
take either side, do not hand-edit, and let Vale's next cycle redraw it.

## How the map is drawn, so the numbers make sense

The picture on `main` is not drawn from main's working tree. It reads the
**exact lane from `origin/mercury`** and the **semantic lane from `origin/venus`**,
and records that choice in `provenance.exact_lane` / `provenance.semantic_lane`.
Both `--write` and `--check` re-resolve from that record, which is why no branch
needs its own `coverage` script.

Figures as of 2026-07-30: exact **210,622** (15.72%), semantic **391,428**
(29.22%), combined **602,050** (44.94%) of a **1,339,576**-byte executable
denominator. Every one of those moves whenever either lighthouse banks, so treat
this as a snapshot and read `metrics/gs1-en-coverage-map.json` for live values
rather than quoting these.

The tool refuses rather than publishing something wrong. A recorded ref that is
not available locally is an error, never a quiet fall back to the working tree,
and a redraw that cannot see the semantic lane refuses rather than publishing
Venus's half as zero. Either way the previous picture stands.

## For Venus: the overlay lane is understated, and only you can fix it

`tools/coverage_map.ts` sizes an overlay semantic owner **only** from a
`manual_regions` entry in `semantic/regions.json`. It deliberately refuses the
decoded-region inventory, because that is build output and the map is a
tracked-evidence-only tool. An owner missing from that file is reported in
`provenance.semantic_unresolved`, not estimated.

As of 2026-07-30, **332 of 950 semantic sources are unresolved**, all overlay
owners: the map can size **8,458** of the roughly **92,186** overlay semantic
bytes you report, while your main-image figure of **382,970** agrees exactly.

You do not have to guess which owners are missing. `provenance.semantic_unresolved`
in `metrics/gs1-en-coverage-map.json` names every one of them, and that list is
current as of Vale's last redraw — it is a ready-made worklist.

**Converting an overlay does not move the picture; only listing it does.** List
strict per-owner rows in `manual_regions`; `semantic_regions_sync.ts` generates
them. Do **not** claim a whole overlay: measurement on 2026-07-31 showed 110,830
bytes of the fully converted overlays lie outside any strict row (veneer and
import bands, tables, pools), so crediting the whole audited extent overstates
the lane by roughly 27,000 bytes.

The rejected shape, kept only so this correction has something to point at: `semantic/regions.json` takes a `full_overlays` array alongside
`manual_regions`:

```json
{ "overlay": "resource_375",
  "evidence": "every executable range is owned by a semantic source; …" }
```

One entry sizes every owner in that overlay — the map takes its audited
executable extent and subtracts exact C. Twelve of those replace 384 individual
entries. A claim for an overlay with no semantic sources, or no audited extent,
credits nothing rather than being taken on trust.

## For Mercury: your bank is the public number

The blue lane in the README is read straight from `origin/mercury`. When you
bank, the published figure moves within the hour without you touching anything.
That is the whole mechanism — bank, and it appears.

## Getting something to Vale

Bank it on your branch as documentation or tooling. That is the entire protocol.
If something is urgent or Vale is getting it wrong, say so in `HANDOVER.md` —
that file flows around the ring and Vale reads it every cycle.
