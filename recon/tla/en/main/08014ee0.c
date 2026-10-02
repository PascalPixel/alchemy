/* Unlinked TLA near miss. Complete extent 28 bytes, literal pool included;
   6 differing byte positions in each of JA/EN/DE/ES/FR/IT.
   The unchanged existing CAMELOT_ASM Transform_SetIdentity macro shifts the identity-one value after zero-register loads; this native helper shifts it before them. The brief's macro-classification decision remains open. No modified macro or new steering is used. */
#include "TYPES.H"
#include "SYSTEM.H"
#include "TRANSFORM.H"

void SceneTransform_ResetMatrix(void)
{
    /* CAMELOT_ASM: the fixed-register identity store of TRANSFORM.H */
    Transform_SetIdentity(gTransform);
}
