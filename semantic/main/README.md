# Main-image semantic reconstructions

These files are promoted from the clean, hand-reviewed candidate queue. Each
one already represented the corresponding ordinary compiler-output region but
was parked because its generated instructions were not byte-identical.

Admission here removes that byte-exact constraint while retaining the semantic
requirements in the parent README. The semantic build rejects files that no
longer correspond to live ordinary-assembly debt, duplicate an exact source,
overlap another semantic owner, use inline assembly, or fail the target
compiler.
