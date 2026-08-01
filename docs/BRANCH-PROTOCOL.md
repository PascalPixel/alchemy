# Branch protocol

`main` is Alchemy's authoritative integration branch. It contains the exact-C
tree, the reviewed semantic-C tree, tools, documentation, metrics, and dashboard
assets together. A worker branch is a temporary work queue, never a second
source of project truth.

## Branches

| branch | usual work | authority |
| --- | --- | --- |
| `main` | review, integration, metrics, docs, dashboard | authoritative |
| `mercury` | byte-exact C and compiler experiments | worker |
| `venus` | reviewed semantic C | worker |
| `jupiter` | reviewed semantic C | worker |
| `mars` | reviewed semantic C | worker |

The role names are scheduling defaults, not repository boundaries. Exact and
semantic work both end up on `main` after review.

## One-way integration

```text
main ──▶ worker branch ──▶ reviewed merge ──▶ main
```

1. Start or resume from the latest local `main`.
2. Work and commit only on the branch assigned to that worktree.
3. Report the branch tip and verification result.
4. Review the complete delta against `main`, then merge it in the primary
   checkout without rebasing or rewriting the worker's commits.
5. Resolve exact/semantic overlap in favor of exact C. Delete every semantic
   source superseded by an exact source; the semantic build enforces this.
6. Run the full verification gate on `main`.
7. Regenerate progress and coverage from the integrated worktree, update the
   handover and local dashboard state, commit, and push `main`.
8. Reconcile any reused worker branch from the new `main` before more work.

Never push a worker branch over `main`, and never treat a worker's metrics as the
published project total. If a worker is abandoned after its work has merged,
its branch and worktree may be recreated from `main` without losing history.

## Exact beats semantic

`src/<address>.c` and exact overlay C are byte claims. They are admitted only
after the linked bytes match the reference. `semantic/` sources are readable,
reviewed behavioral reconstructions and are deliberately not byte claims.

When exact C is adopted for an address already represented under `semantic/`,
the semantic file must be removed in the same integration. Run:

```sh
bun tools/semantic_superseded.ts --check
bun run verify
```

The first command catches duplicate ownership early; `verify` repeats the rule
and proves the complete ROM still builds byte-identically with zero fallback.

## Metrics and graphs

Only an integrated, verified `main` close-out updates the published views:

```sh
bun run progress:check
bun run coverage
bun run coverage:check
```

The coverage map records the exact and semantic refs used to draw it. On the
normal `main` close-out both are `worktree`, so the README SVGs, tracked JSON,
and local dashboard describe the same commit. Do not hand-edit generated SVGs
or their metric JSON.

## Communication

Durable state and reusable findings belong in the opening current-state section
or the relevant method section of [`../HANDOVER.md`](../HANDOVER.md). Local
coordination lives in `/tmp/ALCHEMY_GROUP_CHAT.csv` and
`/tmp/ALCHEMY_KANBAN.md`; those files power the dashboard but are not durable
project evidence. `MEETING.md` is retired.

The complete local roster, worktree bootstrap, chat format, and dashboard
service are documented in [`TEAM-OPS.md`](TEAM-OPS.md).
