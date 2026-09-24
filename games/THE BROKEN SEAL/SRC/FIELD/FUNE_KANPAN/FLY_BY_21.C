#include "TYPES.H"
#include "FIELD_EVENT.H"

void Engine_ObjectSetPosition(struct FieldActor *object, s32 fixed_x, s32 fixed_y, s32 fixed_z);

struct FlyBy {
    u8 unknown_00[0x64];
    s16 step;
};

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* A deck object's update: wait a random while, fly to its mark, make actor 21 turn and react, then fly off and start over. */
s32 FuneKanpan_UpdateFlyByForActor21(struct FieldActor *obj)
{
    struct FlyBy *gull;

    gull = (struct FlyBy *)obj;
    switch (gull->step) {
    case 0:
        if ((((u32)Engine_RandomNext() * 40) >> 16) == 0) {
            gull->step++;
        }
        break;
    case 1:
        gull->step++;
        break;
    case 2:
        obj->velocity_y = 0x40000;
        obj->speed = 0x40000;
        obj->acceleration = 0x20000;
        Call4(Engine_ObjectSetPosition, (s32)obj, 0x1080000, 0, 0x2960000);
        gull->step++;
        break;
    case 3:
        gull->step++;
        break;
    case 4:
        if (obj->target_x == ACTOR_NO_TARGET && obj->target_y == obj->target_x && obj->target_z == obj->target_y) {
            gull->step++;
            Engine_AudioPlayCue(152);
            if (obj->rise_enabled != 0) {
                Call3(Engine_ActorFaceDirection, 21, 0xb000, 0);
            } else {
                Call3(Engine_ActorFaceDirection, 21, 0x5000, 0);
            }
            if (((u32)Engine_RandomNext() << 2) >> 16 != 0) {
                Engine_ActorGet(21)->velocity_y = 0x20000;
            } else {
                Call3(Engine_ActorShowEmote, 21, 0x103, 0);
                Engine_ActorGet(21)->velocity_y = 0x60000;
            }
        }
        break;
    case 5:
        gull->step++;
        break;
    case 6:
        gull->step++;
        obj->velocity_y = 0x40000;
        obj->speed = 0x20000;
        obj->acceleration = 0x10000;
        if (obj->rise_enabled != 0) {
            Call4(Engine_ObjectSetPosition, (s32)obj, 0xfc0000, 0, 0x2860000);
        } else {
            Call4(Engine_ObjectSetPosition, (s32)obj, 0x1000000, 0, 0x2ae0000);
        }
        break;
    case 7:
        gull->step++;
        break;
    case 8:
        if (obj->target_x == ACTOR_NO_TARGET && obj->target_y == obj->target_x && obj->target_z == obj->target_y) {
            gull->step++;
        }
        break;
    case 9:
        gull->step = 0;
        break;
    }
    return 1;
}
