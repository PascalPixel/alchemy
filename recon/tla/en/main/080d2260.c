/* 2026-10-03 finite trial record (extent/differing bytes, relocations unlinked):
 * Eight ordinary forms: 72/11,11,11,18; byte-pointer64/31; reversed or chained zero stores72/10; mode-local72/11. Four scheduling forms: pointer72/4; r2/r1 scratch boundaries72/0; retained memory boundary72/0.
 * No whole-edition linked proof is claimed by these object measurements. */
/* Current-object setup trial: ordinary C through maintained state records.
 * Full linked edition proof remains pending. */
#include "OBJECT_RUNTIME.H"
#include "PARTY_STATE.H"

s32 Func_080cdf5c(void);

void Func_080d2260(void)
{
    struct ObjectRuntime *object = ObjectTable_Get(Func_080cdf5c());
    object->speed_limit = 0x10000;
    object->acceleration = 0x8000;
    object->target_x = 0x80000000;
    object->target_z = 0x80000000;
    object->velocity_x = 0;
    object->velocity_z = 0;
    /* FAKEMATCH: eight ordinary forms left the zero stores and party-mode address construction reordered; the memory boundary keeps the native 72-byte instruction order. */
    __asm__ volatile("" : : : "memory");
    if (gPartyState.render_mode == 2)
        Object_SetMode(object, 12);
    else
        Object_SetMode(object, 1);
}
