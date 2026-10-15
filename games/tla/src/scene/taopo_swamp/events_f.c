#include "local.h"

void Func_02000f9c(s32 a, s32 id)
{
    Object_SetModeById(id, 2);
    GameFlag_SetBit(0x338);
    GameFlag_ClearBit(0x339);
}

void Func_02000fbc(s32 a, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    if (object->z >> 20 == 21) {
        Object_SetModeById(id, 3);
        object->collision_flags = 0;
        /* FAKEMATCH: the native stores the collision flags before addressing the priority byte. */
        __asm__ volatile("");
        object->priority_flags |= 2;
        GameFlag_SetBit(0x339);
        GameFlag_SetBit(0x33a);
        Func_02002840(11, 19, 1, 1, 11, 21);
    }
}

void Func_02001018(s32 a, s32 id)
{
    Object_SetModeById(id, 2);
    GameFlag_SetBit(0x33b);
    GameFlag_ClearBit(0x33c);
}

void Func_02001038(s32 a, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    if (object->x >> 20 == 31) {
        Object_SetModeById(id, 3);
        object->collision_flags = 0;
        /* FAKEMATCH: the native stores the collision flags before addressing the priority byte. */
        __asm__ volatile("");
        object->priority_flags |= 2;
        GameFlag_SetBit(0x33c);
        GameFlag_SetBit(0x33d);
        ObjectMotion_SetSpeedParameters_(id, 0x8000, 0x4000);
        ObjectMotion_OffsetPositionAndResetMotion(id, 0, -32);
        ObjectMotion_CommitCurrentPositionAndActivate(id);
        Object_SetModeById(id, 3);
        Func_02002840(30, 13, 1, 1, 31, 11);
    }
}

#if !defined(TLA_EDITION_JA) /* 日本版にはない */
void Func_020010bc(void)
{
    Func_02002948();
}
#endif

void Func_020010c4(s32 a, s32 id)
{
    Object_SetModeById(id, 2);
    GameFlag_SetBit(0x340);
    GameFlag_ClearBit(0x341);
    Func_02002840_(7, 39, 1, 1, 7, 38);
    Func_02002084(1);
}

void Func_02001100(s32 a, s32 id)
{
    Object_SetModeById(id, 2);
    GameFlag_SetBit(0x342);
    GameFlag_ClearBit(0x343);
    Func_02002084(1);
}

void Func_02001128(s32 a)
{
    Func_02002958_(a, 64, 0x348);
}

void Func_02001138(void)
{
    Func_020029a0((s32)&MsgIThoughtI);
}

extern const u8 Data_0200302c[];
extern const u8 Data_02003038[];
extern const u8 Data_02003068[];
extern const u8 Data_02003128[];
extern const u8 Data_02003194[];
extern const u8 Data_020031e8[];
extern const u8 Data_02003314[];
extern const u8 Data_02003338[];
extern const u8 Data_020033e0[];
extern const u8 Data_02003464[];
extern const u8 Data_020034e8[];
extern const u8 Data_02003560[];

const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov678Room1)
        return (const struct SceneEvent *)Data_02003038;
    if (scene == (s32)&SceneId_Ov678Room2)
        return (const struct SceneEvent *)Data_02003068;
    if (scene == (s32)&SceneId_Ov678Room3)
        return (const struct SceneEvent *)Data_02003128;
    if (scene == (s32)&SceneId_Ov678Room4)
        return (const struct SceneEvent *)Data_02003194;
    if (scene == (s32)&SceneId_Ov678Room5)
        return (const struct SceneEvent *)Data_020031e8;
    if (scene == (s32)&SceneId_Ov678Room6)
        return (const struct SceneEvent *)Data_02003314;
    if (scene == (s32)&SceneId_Ov678Room7)
        return (const struct SceneEvent *)Data_02003338;
    if (scene == (s32)&SceneId_Ov678Room8)
        return (const struct SceneEvent *)Data_020033e0;
    if (scene == (s32)&SceneId_Ov678Room9)
        return (const struct SceneEvent *)Data_020034e8;
    if (scene == (s32)&SceneId_Ov678Room10)
        return (const struct SceneEvent *)Data_02003464;
    if (scene == (s32)&SceneId_Ov678Room11)
        return (const struct SceneEvent *)Data_02003560;
    return (const struct SceneEvent *)Data_0200302c;
}
