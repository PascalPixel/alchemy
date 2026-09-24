#include "TYPES.H"
#include "FIELD_EVENT.H"

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows Data_02000240_t;

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Parks actors 8 and 9, poses actors 10 and 12, and moves them further along when flag 0x908 is set. */
void ArutinYama_PlaceFlaggedActors(void)
{
    struct FieldActor *actor;

    actor = Engine_ActorGet(10);
    Engine_ActorSetPosition(8, 0, 0);
    Engine_ActorSetPosition(9, 0, 0);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(9), 0);
    actor->motion_flags = 0;
    actor->scale_x = 0xe666;
    actor->scale_y = 0x9999;
    actor->sprite->rotation = 0x8000;
    Engine_ActorGet(12)->motion_flags = 0;
    Engine_ActorGet(12)->y.fixed = -0x1c0000;
    if (Value1(Engine_GameFlagIsSet, 0x908)) {
        actor->x.fixed += 0xe0000;
        actor->y.fixed += -0x80000;
        actor->sprite->rotation = 0xc000;
    }
    if (Value1(Engine_GameFlagIsSet, 0x908)) {
        Call6(Engine_MapCopyCellsTo, 25, 36, 43, 36, 11, 9);
        Call6(Engine_MapCopyCellAttributes, 25, 35, 10, 5, 43, 35);
        Engine_MapRedraw();
        Engine_TaskWait(1);
    }
    if (Data_02000240_t.halves[225][0] == 6 && !Engine_GameFlagIsSet(0x109)) {
        Engine_EventBegin();
        Engine_ActorGet(0)->y.fixed = -0x580000;
        Engine_CameraMoveTo(0x3180000, -0x580000, 0x2410000, 0);
        Engine_MapRedraw();
        Engine_TaskWait(1);
        Engine_EventEnd();
    }
}
