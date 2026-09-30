#include "TYPES.H"
#include "FIELD_EFFECT.H"
#include "CALL.H"

struct FieldActor *OverlayObject_CreateConfigured(s32 x, s32 y, s32 z, s32 kind);
void Map_CopyCellAttributeRect();

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
    if (Object_GetById(9)->x.fixed >> 20 == 23) {
        Call3((void (*)())Engine_ActorMoveToAndWait, 0, 360, 664);
        Engine_ActorFaceDirection(0, 0xe000, 10);
        Object_GetById(9)->x.fixed += 0x20000;
        left = OverlayObject_CreateConfigured(Object_GetById(9)->x.fixed, 0,
                                              Object_GetById(9)->z.fixed + 0x340000, 241);
        right = OverlayObject_CreateConfigured(Object_GetById(9)->x.fixed + 0x100000, 0,
                                               Object_GetById(9)->z.fixed + 0x340000, 241);
        Object_GetById(9)->motion_flags = 0;
        o = &options;
        o->start_scale_x = 0x9999;
        o->start_scale_y = 0x9999;
        o->palette = 7;
        Audio_PlayCue(216);
        for (i = 0; i < 68; i++) {
            s32 x = (((u32)(Engine_RandomNext() * 17) >> 16) << 16) + 0x1700000;
            s32 z = (((u32)(Engine_RandomNext() * 14) >> 16) << 16) + 0x2700000;

            Effect_Spawn(x, 0, z, 0, 0, 0, 0x90000, o);
            Object_GetById(9)->y.fixed -= 0x8000;
            Battle_WaitMode0(1);
        }
        Call6((void (*)())Map_CopyCellAttributeRect, 23, 41, 1, 1, 23, 39);
        Object_GetById(9)->priority_flags |= 2;
        Engine_GameFlagSet(0x200);
        Object_GetById(9)->y.fixed = -0x80000;
        Object_SetModeById(9, 2);
        Engine_ObjectDispatchRelease(left);
        Engine_ObjectDispatchRelease(right);
        Battle_WaitMode0(30);
    }
    Engine_EventEnd();
}
