#include "TYPES.H"

/* A scene actor's descriptor: its scene number (offset by 48) and position. */
struct ActorDescriptor {
    s16 action;
    s16 scene;
    u8 pad4[4];
    s32 x;
    s32 y;
    s32 z;
};

struct ActorObject {
    u8 pad0[8];
    s32 x;
    s32 y;
    s32 z;
    s32 unknown14;
    u8 pad18[0x55 - 0x18];
    u8 state;
};

struct SceneActorState {
    u8 pad0[0x1f4];
    s32 anchor;
    u8 pad1f8[0x234 - 0x1f8];
    s16 destination;
    s16 count;
};

extern struct SceneActorState Data_02000240;
s32 GameFlag_TestFar(s32 flag);
struct ActorDescriptor *BattleAction_FindDescriptor(s32 action);
struct ActorObject *ObjectTable_Get(s32 object);
void Object_SetPositionAndResetMotionFar(struct ActorObject *obj, s32 x, s32 y, s32 z);
void Object_SetMode(struct ActorObject *obj, s32 mode);

/* Once flag 0x109 is set and the destination is one of scenes 300-380, put
   every actor (objects 8-65) whose descriptor names that scene at its own
   position, or, when bit 11 of the destination is set, just below the anchor
   object. */
void FieldObject_PlaceSceneActors(void)
{
    s32 flags = Data_02000240.destination & 0xf000;
    u32 scene = (u16)Data_02000240.destination & 0xfff;
    s32 id;

    if (GameFlag_TestFar(0x109) != 0 && flags == 0) {
        flags = scene & 0x800;
        scene &= 0x7ff;
        if (scene - 300 <= 80 && Data_02000240.count > 0) {
            for (id = 8; id <= 65; id++) {
                struct ActorDescriptor *desc = BattleAction_FindDescriptor(id);
                struct ActorObject *obj;

                if (desc == NULL || desc->scene - 48 != scene - 300)
                    continue;
                obj = ObjectTable_Get(id);
                if (obj == NULL)
                    continue;
                if (flags == 0) {
                    obj->unknown14 = flags;
                    obj->state = 3;
                    Object_SetPositionAndResetMotionFar(obj, desc->x, desc->y, desc->z);
                } else {
                    struct ActorObject *anchor = ObjectTable_Get(Data_02000240.anchor);

                    Object_SetPositionAndResetMotionFar(obj, anchor->x, anchor->y, anchor->z - 0x200000);
                }
                Object_SetMode(obj, 1);
            }
        }
    }
}
