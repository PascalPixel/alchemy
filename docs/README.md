# Where things are

One page so nobody has to guess. `main` is authoritative; update the single
handover when an operational fact changes instead of creating another status
document.

## Start here

| you want | read |
| --- | --- |
| current state and what to do next | [`../HANDOVER.md`](../HANDOVER.md) |
| how worker branches return to `main` | [BRANCH-PROTOCOL.md](BRANCH-PROTOCOL.md) |
| local dashboard, chat, and kanban | [TEAM-OPS.md](TEAM-OPS.md) |
| how to do the work — levers, park classes, compiler modes | [`../HANDOVER.md`](../HANDOVER.md) |
| the rules that are not negotiable | [`../PROVENANCE.md`](../PROVENANCE.md), [`../LAWS.md`](../LAWS.md) |

## Reference

| document | what it is |
| --- | --- |
| [FULL-C-BYTE-SHARE.md](FULL-C-BYTE-SHARE.md) | the headline metric and how the coverage map derives it |
| [PATH-TO-COMPLETION.md](PATH-TO-COMPLETION.md) | measured status and the remaining-work breakdown |
| [DISCOVERY-QUEUE.md](DISCOVERY-QUEUE.md) | what `asm_c_debt_bytes` is and is not |
| [COMPILER-QUEUE.md](COMPILER-QUEUE.md) | compiler-mode candidates |
| [REMAINING.md](REMAINING.md) | the residual categories |
| [SOURCE-STYLE.md](SOURCE-STYLE.md) | how canonical C is written here |
| [TWIN-384-DRAFT.md](TWIN-384-DRAFT.md) | Mercury's live working draft on the 384-byte twin |
| [compiler-evidence/](compiler-evidence/) | collateral for each admitted compiler lever |
| [full-c-history.json](full-c-history.json) / [.csv](full-c-history.csv) | first-parent measurement history |

## Archive

[`archive/`](archive/) holds dated session records that are finished: the
2026-07-28 humanization batches and their queue, the compiler-family experiment,
and the 2026-07-30 re-probe with its two TSVs. They are kept because they carry
measurements nobody should have to re-derive, not because they are current. If
you are looking for what to do next, they are the wrong place.

## The rule that keeps this list short

A dated document is a **record**, not a queue. When a session ends, actionable
findings belong in `HANDOVER.md`; finished session records belong in `archive/`.
The live chat and kanban are intentionally local dashboard state, while git
holds durable history.
