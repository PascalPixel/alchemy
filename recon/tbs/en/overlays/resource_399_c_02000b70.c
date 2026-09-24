/* NONMATCHING: 1056 of 1056 bytes, 2 differing halfwords (2026-09-24). Written
 * as a single-overlay unit source (Engine_* names bind at the runtime import
 * veneers, listing offset + 0x8000). Remaining: in the room-1 branch the
 * reference loads r1 (125) before r3 (69) for the first Engine_MapCopyCellsTo;
 * wrappers, argument temporaries and empty loops did not move it. */
#include "TYPES.H"

u8 *Engine_ActorGet();
void Engine_ActorSetAnimation();
s32 Engine_GameFlagIsSet();
void Engine_GameFlagClear();
s32 Engine_TaskAddCallback();
s32 Engine_ActorSetSpritePriority();
void SceneState_UpdateActor11WithFlag203();
void RunEventScript02();
void Engine_ActorSetChildValue();
void Engine_MapCopyCellAttributes();
u8 * Engine_MapCopyCellsTo();
void Engine_MapRedraw();
void Engine_TaskWait();
void Engine_ActorEnableActionCallback();
s32 Engine_ActorSetPosition();
void Engine_ActorSetSpriteFlags();
void FieldScene_RunPrimaryScriptChoreography();
void FieldScene_RunThreeActorChoreography();


extern u8 Data_00000032[];
extern u8 Data_00000033[];
extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

s32 Local_02000b70(s32 a0)
{
    s32 room;
    u8 *record;

    if (Data_02000240_t[224][0] == (s32)Data_00000032) {
        room = Engine_ActorGet(0);
        *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x100;
        Engine_ActorSetAnimation(10, 9);
        if (Value1(Engine_GameFlagIsSet, 0x109) != 0) {
            Call1(Engine_GameFlagClear, 0x200);
            Call1(Engine_GameFlagClear, 0x201);
        }
        {
            s32 shown = 0;

            *(u16 *)(room + 100) = shown;
        }
        {
            s32 shown = 0;

            *(u16 *)((room + 100) + 2) = shown;
        }
        Value2(Engine_TaskAddCallback, 0x2009795, 0xc80);
        Value2(Engine_TaskAddCallback, 0x20098c5, 0xc80);
        Engine_ActorSetSpritePriority(11, 1);
        if (Value1(Engine_GameFlagIsSet, 0x203) != 0) {
            SceneState_UpdateActor11WithFlag203();
        }
        if (Value1(Engine_GameFlagIsSet, 0x109) == 0 && Data_02000240_t[225][0] == 9) {
            RunEventScript02();
        }
    } else if (Data_02000240_t[224][0] == (s32)Data_00000033) {
        *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x209;
        room = Data_02000240_t[225][0];
        if (room == 1) {
            Engine_ActorSetChildValue(21, 15);
            {
                u8 *actor = Engine_ActorGet(21);
                u8 value = *(volatile u8 *)&actor[89];

                actor[89] = (u8)(8 | value);
            }
            Engine_ActorSetSpritePriority(21, 1);
            if (Value1(Engine_GameFlagIsSet, 0x881) != 0) {
                Call6(Engine_MapCopyCellAttributes, 10, 7, 1, 1, 10, 8);
                Engine_MapCopyCellsTo(3, 125, 9, 69, 3, 3);
                Engine_MapRedraw();
                Engine_TaskWait(1);
                Engine_ActorEnableActionCallback(8, 2);
                Engine_ActorSetPosition(10, 0, 0);
                return 0;
            }
            if (Value1(Engine_GameFlagIsSet, 0x82c) != 0 && Value1(Engine_GameFlagIsSet, 0x82a) != 0) {
                Engine_ActorSetSpriteFlags(Engine_ActorGet(10), 0);
                Call3((void (*)())Engine_ActorSetPosition, 9, 0xae0000, 0xa40000);
                Engine_ActorSetSpriteFlags(Engine_ActorGet(9), 0);
                Engine_ActorSetAnimation(9, 5);
                Call3((void (*)())Engine_ActorSetPosition, 8, 0xa80000, 0x980000);
                record = Engine_ActorGet(8);
                { s32 shown = 0x3000; *(u16 *)(record + 6) = shown; }
                if (Value1(Engine_GameFlagIsSet, 0x82b) == 0) {
                    FieldScene_RunPrimaryScriptChoreography();
                }
                return 0;
            }
            Call6(Engine_MapCopyCellAttributes, 10, 7, 1, 1, 10, 8);
            Call6((void (*)())Engine_MapCopyCellsTo, 3, 125, 9, 69, 3, 3);
            Engine_MapRedraw();
            Engine_TaskWait(1);
            if (Value1(Engine_GameFlagIsSet, 0x82c) != 0) {
                Call3((void (*)())Engine_ActorSetPosition, 8, 0x950000, 0x740000);
                record = Engine_ActorGet(8);
                {
                    s32 shown = 0;

                    *(u16 *)(record + 6) = shown;
                    *(u16 *)(Engine_ActorGet(9) + 102) = shown;
                }
                Call2(Engine_ActorEnableActionCallback, 9, 0x200a4f4);
                return 0;
            }
            Engine_ActorEnableActionCallback(8, 2);
        } else if (room == 2) {
            if (Value1(Engine_GameFlagIsSet, 0x881) == 0) {
                {
                    u8 *actor = Engine_ActorGet(11);
                    s32 shown = 1;

                    *(u16 *)(actor + 102) = shown;
                }
                Call2(Engine_ActorEnableActionCallback, 11, 0x200a4f4);
            }
        } else if (room == 4) {
            if (Value1(Engine_GameFlagIsSet, 0x881) != 0) {
                Call3((void (*)())Engine_ActorSetPosition, 12, 0x16c0000, 0x2420000);
                Engine_ActorSetSpritePriority(12, 2);
                Engine_ActorGet(12)[89] |= 4;
                Call6((void (*)())Engine_MapCopyCellsTo, 6, 125, 22, 88, 3, 3);
                Call3((void (*)())Engine_ActorSetPosition, 13, 0x1ec0000, 0x2420000);
                Engine_ActorSetSpritePriority(13, 2);
                {
                    u8 *actor = Engine_ActorGet(13);
                    u8 value = *(volatile u8 *)&actor[89];

                    actor[89] = (u8)(4 | value);
                }
                Engine_MapCopyCellsTo(9, 125, 28, 88, 3, 3);
            } else {
                *(s32 *)(Engine_ActorGet(12) + 24) = -0x10000;
                Engine_ActorSetSpriteFlags(Engine_ActorGet(12), 0);
                Engine_ActorSetAnimation(12, 5);
                Engine_ActorSetSpriteFlags(Engine_ActorGet(13), 0);
                Engine_ActorSetAnimation(13, 5);
            }
        } else if (room == 3) {
            if (Value1(Engine_GameFlagIsSet, 0x881) != 0) {
                Call3((void (*)())Engine_ActorSetPosition, 15, 0x1cc0000, 0x1020000);
                Engine_ActorSetSpritePriority(15, 2);
                {
                u8 *actor = Engine_ActorGet(15);
                u8 value = *(volatile u8 *)&actor[89];

                actor[89] = (u8)(4 | value);
            }
                Value3(Engine_ActorSetPosition, 14, 0x1980000, 0x1080000);
                record = Engine_ActorGet(14);
                { s32 shown = 0x1000; *(u16 *)(record + 6) = shown; }
                Engine_MapCopyCellsTo(12, 125, 26, 70, 3, 3);
            } else {
                Call3((void (*)())Engine_ActorSetPosition, 14, 0x1cc0000, 0x1020000);
                Engine_ActorSetSpritePriority(14, 2);
                {
                u8 *actor = Engine_ActorGet(14);
                u8 value = *(volatile u8 *)&actor[89];

                actor[89] = (u8)(4 | value);
            }
                *(s32 *)(Engine_ActorGet(15) + 24) = -0x10000;
                Engine_ActorSetSpriteFlags(Engine_ActorGet(15), 0);
                Engine_ActorSetAnimation(15, 5);
            }
        } else if (room == 7) {
            if (Value1(Engine_GameFlagIsSet, 0x881) != 0) {
                record = Engine_ActorGet(20);
                { s32 shown = 0x3000; *(u16 *)(record + 6) = shown; }
                if (Value1(Engine_GameFlagIsSet, 0x82e) == 0) {
                    Call3((void (*)())Engine_ActorSetPosition, 20, 0x28a0000, 0xa10000);
                    FieldScene_RunThreeActorChoreography();
                } else {
                    Call3((void (*)())Engine_ActorSetPosition, 20, 0x2840000, 0xa60000);
                }
            }
        }
    }
    return 0;
}
