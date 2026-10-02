/* Unlinked TLA near miss. Complete extent 56 bytes, literal pool included;
   6 differing byte positions in each of JA/EN/DE/ES/FR/IT.
   The TLA allocator takes byte-offset8; the earlier TBS slot2 carryover was invalid. Three ordinary identity shapes emitted64/60/60 bytes versus native56, with48/37/35 differing byte positions in English. The existing TRANSFORM.H macro remains CAMELOT_ASM and its classification decision remains open. Its identity-one shift order differs in all six editions; no macro change is made. */
#include "TYPES.H"
#include "SYSTEM.H"
#include "TRANSFORM.H"

void Render_ResetTransformState(void)
{
    gTransformStackTop = Runtime_AllocateBlock(8, sizeof(gTransform));
    gTransformStackDepth = 0;
    /* CAMELOT_ASM: the fixed-register identity store of TRANSFORM.H */
    Transform_SetIdentity(gTransform);
}
