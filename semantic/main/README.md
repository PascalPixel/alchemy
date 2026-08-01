> **C/H hard blocker:** Never use `asm(...)`, `__asm(...)`, `__asm_(...)`, `__asm__(...)`, fixed-register bindings, or empty assembly barriers. Byte equality never overrides this rule.

# Main-image semantic reconstructions

These files are promoted from the clean, hand-reviewed candidate queue. Each
one already represented the corresponding ordinary compiler-output region but
was parked because its generated instructions were not byte-identical.

Admission here removes that byte-exact constraint while retaining the semantic
requirements in the parent README. The semantic build rejects files that no
longer correspond to live ordinary-assembly debt, duplicate an exact source,
overlap another semantic owner, use inline assembly, or fail the target
compiler.

The normal manifest boundary is only a seed. Follow live stack and
callee-saved-register state across direct continuation branches. If one owner
occupies noncontiguous executable ranges separated by pools, alignment, or
data, record all reviewed ranges in `semantic/main-regions.json` and exclude
the gaps; do not admit or count only the convenient first range.
