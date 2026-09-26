#include "TYPES.H"
#include "FIELD_EFFECT.H"

struct FieldActor *OverlayObject_CreateConfigured(s32 x, s32 y, s32 z, s32 kind);

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* When the pillar (actor 9) stands in column 23, steps the leader aside and
 * lowers the pillar into the floor amid a spray of dust between two
 * markers, opens the cell it blocked and sets flag 0x200. */
void HaidiaDou_SinkPillarColumn23(void)
{
    struct EffectOptions options;
    struct EffectOptions *o;
    struct FieldActor *left;
    struct FieldActor *right;
    u32 i;

    Engine_EventBegin();
    if (Engine_ActorGet(9)->x.fixed >> 20 == 23) {
        Call3((void (*)())Engine_ObjectMotionSetPositionAndCommit, 0, 360, 664);
        Engine_ActorFaceDirection(0, 0xe000, 10);
        Engine_ActorGet(9)->x.fixed += 0x20000;
        left = OverlayObject_CreateConfigured(Engine_ActorGet(9)->x.fixed, 0,
                                              Engine_ActorGet(9)->z.fixed + 0x340000, 241);
        right = OverlayObject_CreateConfigured(Engine_ActorGet(9)->x.fixed + 0x100000, 0,
                                               Engine_ActorGet(9)->z.fixed + 0x340000, 241);
        Engine_ActorGet(9)->motion_flags = 0;
        o = &options;
        o->start_scale_x = 0x9999;
        o->start_scale_y = 0x9999;
        o->palette = 7;
        Engine_AudioPlayCue(216);
        for (i = 0; i < 68; i++) {
            s32 x = (((u32)(Engine_RandomNext() * 17) >> 16) << 16) + 0x1700000;
            s32 z = (((u32)(Engine_RandomNext() * 14) >> 16) << 16) + 0x2700000;

            Effect_Spawn(x, 0, z, 0, 0, 0, 0x90000, o);
            Engine_ActorGet(9)->y.fixed -= 0x8000;
            Engine_EventWait(1);
        }
        Call6((void (*)())Engine_MapCopyCellAttributes, 23, 41, 1, 1, 23, 39);
        Engine_ActorGet(9)->priority_flags |= 2;
        Engine_GameFlagSet(0x200);
        Engine_ActorGet(9)->y.fixed = -0x80000;
        Engine_ActorSetAnimation(9, 2);
        Engine_ObjectDispatchRelease(left);
        Engine_ObjectDispatchRelease(right);
        Engine_EventWait(30);
    }
    Engine_EventEnd();
}
