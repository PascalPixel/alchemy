# scratch

Throwaway probes live here. Nothing in this folder is gated: it is not
documented in AGENTS.md, not typechecked as a tool, not self-tested, and not
counted in the tool total.

This exists because the best result of the 2026-08-07 restructure came from a
one-off script that swept the fork's `toplev.c` against the board. It closed
`resource_39e:26d8` byte-exact with no compiler change, and it revealed that 31
of the fork's 74 `-fthumb-*` modes were missing from the sweep list, so every
official sweep had been blind to a third of the search space. Under a gate that
demands every tool be documented, that script would have been refused.

Graduate anything that survives a second use into a real tool with a section in
AGENTS.md. Delete the rest; git keeps it either way.
