/* Near miss: differ 2, one scheduling step. ☀️'s Battle_GetWorkEntryPair
   through ⚓️'s BattleSession. The listing sets the return value
   (movs r0, #0) before loading anchor_z (ldr r3, [r4, r2]); this build
   loads first. A slot pointer local (16), a result local (11) or storing
   y first (17) are further off. Goes after
   BattleMotion_DestroyAllSlotObjects in
   RESET_BATTLE_OBJECT_RECORD_GROUPS.C. */
#include "TYPES.H"
#include "BATTLE_WORK.H"
#include "FIXED_POINT_POSITION.H"

/* ☀️'s: a slot's anchor as a ground position. */
s32 Battle_GetWorkEntryPair(s32 no, struct FixedPointPosition *out)
{
    struct BattleSession *state = Ram_HeapSlots->battle_work;

    out->x = state->slots[no].anchor_x;
    out->y = 0;
    out->z = state->slots[no].anchor_z;
    return 0;
}
