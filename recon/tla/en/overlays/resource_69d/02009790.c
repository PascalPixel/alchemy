#include "types.h"
#include "party_state.h"
#include "field_event.h"

struct LightSpot {
    u8 unknown_00[6];
    s16 x;
    s16 z;
};

void ObjectMotion_SetSpeedParameters(u32 object_id, s32 speed_limit, s32 acceleration);
void ObjectMotion_SetPositionAndCommit(u32 object_id, s32 x, s32 z);

struct ObjectRuntime *Object_GetById(s32 id);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
struct FieldActor *Func_02001ea8(void);
void Func_02001f08(s32 cue);
void Func_02001df0(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);

void Func_02001790(void)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);
    s32 x = object->x;
    s32 z = object->z;

    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0x40000, 0x20000);
    x >>= 20;
    z >>= 20;
    ObjectMotion_SetPositionAndCommit(gPartyState.current_owner, (x << 4) + 8, (z << 4) + 8);
    WaitFrames(1);
    Object_SetPositionAndResetMotion(object, (x << 20) + 0x80000, object->y, (z << 20) + 0x80000);
    Battle_WaitMode0(4);
    Func_02001ea8()->y += 0xfff80000;
}

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
