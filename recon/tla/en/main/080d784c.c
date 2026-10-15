/* Draft, differ 34 of 107 (ported from The Broken Seal's
   FieldObject_PlaceSceneActors; ⚓️ places ids 8 to 79 and calls
   Object_SetMode): the native copies the 0xfff mask into r10 before it ands
   the scene (regmove's subreg form), and loop.c hoists the selected owner's
   address, derived from the 0x256 offset (subs r2, #66), into r9 just before
   the loop; here the owner's address is hoisted by hand at the top. With
   the owner read plainly in the loop it differs 70; a u16 field for the
   pending event read as (s16) for the flags changes nothing. */
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

struct ActorDescriptor {
    s16 action;
    s16 scene;
    u8 unk04[4];
    s32 x;
    s32 y;
    s32 z;
};

s32 GameFlag_Test(s32 flag);
struct ActorDescriptor *BattleAction_FindDescriptor(s32 id);
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *object, s32 x, s32 y, s32 z);


void Func_080d784c(void)
{
    struct PartyState *party = &gPartyState;
    s32 flags = *(s16 *)&party->unk24c[8] & 0xf000;
    u32 scene = *(s16 *)&party->unk24c[8] & 0xfff;
    s32 id;
    s32 *owner = &party->currentOwner;

    if (GameFlag_Test(0x109) != 0 && flags == 0) {
        flags = scene & 0x800;
        scene &= 0x7ff;
        if (scene - 300 <= 80 && party->unk256 > 0) {
            for (id = 8; id <= 79; id++) {
                struct ActorDescriptor *desc = BattleAction_FindDescriptor(id);
                struct ObjectRuntime *obj;

                if (desc == NULL || desc->scene - 48 != scene - 300)
                    continue;
                obj = ObjectTable_Get(id);
                if (obj == NULL)
                    continue;
                if (flags == 0) {
                    obj->terrainHeight = flags;
                    obj->motionFlags = 3;
                    Object_SetPositionAndResetMotionFar(obj, desc->x, desc->y, desc->z);
                } else {
                    struct ObjectRuntime *selection = ObjectTable_Get(*owner);

                    Object_SetPositionAndResetMotionFar(obj, selection->x, selection->y, selection->z - (32 << 16));
                }
                Object_SetMode(obj, 1);
            }
        }
    }
}
