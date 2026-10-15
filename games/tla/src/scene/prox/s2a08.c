#include "types.h"
#include "party_state.h"
#include "field_event.h"
#include "field_sprite.h"

struct FieldActor *Object_GetById(s32 id);
void               ObjectMotion_SetActionVariant(u32 actor, s32 variant);

void Func_02002a08(void)
{
    s32 variant = Object_GetById(gPartyState.currentOwner)->sprite->priority;

    ObjectMotion_SetActionVariant(8, variant);
    ObjectMotion_SetActionVariant(10, variant);
}
