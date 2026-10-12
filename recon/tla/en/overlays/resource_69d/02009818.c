/* Near miss: differ 2 of 49 (one reordered row): the native loads the object's
 * x (ldr r6, [r0, #8]) before it copies the 64 mask to r3; the draft copies
 * the mask first. Tried: the masks as locals and as casts; three minutes of
 * the permuter. */
#include "types.h"
#include "party_state.h"
#include "field_event.h"

struct LightSpot {
    u8 unknown_00[6];
    s16 x;
    s16 z;
};

struct ObjectRuntime *Object_GetById(s32 id);
void Battle_WaitMode0(s32 frames);
void Func_02001790(void);
void Func_02001f08(s32 cue);
void Func_02001df0(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);

void Func_02001818(struct LightSpot *spot)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);
    s16 flag_x = spot->x & 64;
    s32 x = (object->x >> 20) | flag_x;
    s16 flag_z = spot->z & 64;
    s32 z = (object->z >> 20) | flag_z;

    Func_02001790();
    Func_02001f08(161);
    Func_02001df0(spot->x, spot->z + 1, 1, 1, x, z);
    Battle_WaitMode0(12);
}
