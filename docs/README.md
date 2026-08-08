> **C/H hard blocker:** Never use `asm(...)`, `__asm(...)`, `__asm_(...)`, `__asm__(...)`, fixed-register bindings, or empty assembly barriers. Byte equality never overrides this rule.

# Where things are

One page so nobody has to guess. `main` is authoritative; update the single
handover when an operational fact changes instead of creating another status
document.

## Start here

| you want | read |
| --- | --- |
| current state and what to do next | [`../HANDOVER.md`](../HANDOVER.md) |
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
| `LAWS.md` and `HANDOVER.md` | resolved twin-template evidence and transposition method |
| [compiler-evidence/](compiler-evidence/) | collateral for each admitted compiler lever |
| [full-c-history.json](full-c-history.json) / [.csv](full-c-history.csv) | first-parent measurement history |

## Archive

Finished humanization records and compiler-family experiments remain in Git
history and in the dated documents above. They are kept for their measurements,
not because they are current. If you are looking for what to do next, use
`HANDOVER.md`.

## The rule that keeps this list short

A dated document is a **record**, not a queue. Actionable findings belong in
`HANDOVER.md`; completed historical records belong in `archive/`.
Git holds the durable project history.
