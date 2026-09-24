#include "TYPES.H"
#include "FIELD_EVENT.H"

void SceneState_UpdateActor11WithFlag203(void);
void RunEventScript02(void);
void FieldScene_RunPrimaryScriptChoreography(void);
void FieldScene_RunThreeActorChoreography(void);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows Data_02000240_t;
extern u8 Data_00000032[];
extern u8 Data_00000033[];

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* The two flag clears go through Call1: the wrapper leaves fewer insns across
 * the leader actor's life, so it outranks the game-state base for r5. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Imil: entry setup for the two Imil areas, by entrance and story flags. */
s32 ImiruMura_Func02000b70(void)
{
    struct FieldActor *leader;
    s32 entrance;

    if (Data_02000240_t.halves[224][0] == (s32)Data_00000032) {
        leader = Engine_ActorGet(0);
        gEventWork->start_transition = 0x100;
        Engine_ActorSetAnimation(10, 9);
        if (Value1(Engine_GameFlagIsSet, 0x109)) {
            Call1(Engine_GameFlagClear, 0x200);
            Call1(Engine_GameFlagClear, 0x201);
        }
        leader->unknown_64 = 0;
        leader->unknown_66 = 0;
        Value2(Engine_TaskAddCallback, 0x2009795, 0xc80);
        Value2(Engine_TaskAddCallback, 0x20098c5, 0xc80);
        Engine_ActorSetSpritePriority(11, 1);
        if (Engine_GameFlagIsSet(0x203)) {
            SceneState_UpdateActor11WithFlag203();
        }
        if (!Value1(Engine_GameFlagIsSet, 0x109) && Data_02000240_t.halves[225][0] == 9) {
            RunEventScript02();
        }
    } else if (Data_02000240_t.halves[224][0] == (s32)Data_00000033) {
        gEventWork->start_transition = 0x209;
        entrance = Data_02000240_t.halves[225][0];
        if (entrance == 1) {
            Engine_ActorSetChildValue(21, 15);
            Engine_ActorGet(21)->collision_flags |= 8;
            Engine_ActorSetSpritePriority(21, 1);
            if (Engine_GameFlagIsSet(0x881)) {
                Call6(Engine_MapCopyCellAttributes, 10, 7, 1, 1, 10, 8);
                Call6(Engine_MapCopyCellsTo, 3, 125, 9, 69, 3, 3);
                Engine_MapRedraw();
                Engine_TaskWait(1);
                Engine_ActorEnableActionCallback(8, (const u8 *)2);
                Engine_ActorSetPosition(10, 0, 0);
            } else if (Engine_GameFlagIsSet(0x82c) && Engine_GameFlagIsSet(0x82a)) {
                Engine_ActorSetSpriteFlags(Engine_ActorGet(10), 0);
                Call3(Engine_ActorSetPosition, 9, 0xae0000, 0xa40000);
                Engine_ActorSetSpriteFlags(Engine_ActorGet(9), 0);
                Engine_ActorSetAnimation(9, 5);
                Call3(Engine_ActorSetPosition, 8, 0xa80000, 0x980000);
                Engine_ActorGet(8)->facing = 0x3000;
                if (!Engine_GameFlagIsSet(0x82b)) {
                    FieldScene_RunPrimaryScriptChoreography();
                }
            } else {
                Call6(Engine_MapCopyCellAttributes, 10, 7, 1, 1, 10, 8);
                Call6(Engine_MapCopyCellsTo, 3, 125, 9, 69, 3, 3);
                Engine_MapRedraw();
                Engine_TaskWait(1);
                if (Engine_GameFlagIsSet(0x82c)) {
                    Call3(Engine_ActorSetPosition, 8, 0x950000, 0x740000);
                    Engine_ActorGet(8)->facing = 0;
                    Engine_ActorGet(9)->unknown_66 = 0;
                    Engine_ActorEnableActionCallback(9, (const u8 *)0x200a4f4);
                } else {
                    Engine_ActorEnableActionCallback(8, (const u8 *)2);
                }
            }
        } else if (entrance == 2) {
            if (!Engine_GameFlagIsSet(0x881)) {
                Engine_ActorGet(11)->unknown_66 = 1;
                Engine_ActorEnableActionCallback(11, (const u8 *)0x200a4f4);
            }
        } else if (entrance == 4) {
            if (Engine_GameFlagIsSet(0x881)) {
                Call3(Engine_ActorSetPosition, 12, 0x16c0000, 0x2420000);
                Engine_ActorSetSpritePriority(12, 2);
                Engine_ActorGet(12)->collision_flags |= 4;
                Call6(Engine_MapCopyCellsTo, 6, 125, 22, 88, 3, 3);
                Call3(Engine_ActorSetPosition, 13, 0x1ec0000, 0x2420000);
                Engine_ActorSetSpritePriority(13, 2);
                Engine_ActorGet(13)->collision_flags |= 4;
                Call6(Engine_MapCopyCellsTo, 9, 125, 28, 88, 3, 3);
            } else {
                Engine_ActorGet(12)->scale_x = -0x10000;
                Engine_ActorSetSpriteFlags(Engine_ActorGet(12), 0);
                Engine_ActorSetAnimation(12, 5);
                Engine_ActorSetSpriteFlags(Engine_ActorGet(13), 0);
                Engine_ActorSetAnimation(13, 5);
            }
        } else if (entrance == 3) {
            if (Engine_GameFlagIsSet(0x881)) {
                Call3(Engine_ActorSetPosition, 15, 0x1cc0000, 0x1020000);
                Engine_ActorSetSpritePriority(15, 2);
                Engine_ActorGet(15)->collision_flags |= 4;
                Call3(Engine_ActorSetPosition, 14, 0x1980000, 0x1080000);
                Engine_ActorGet(14)->facing = 0x1000;
                Call6(Engine_MapCopyCellsTo, 12, 125, 26, 70, 3, 3);
            } else {
                Call3(Engine_ActorSetPosition, 14, 0x1cc0000, 0x1020000);
                Engine_ActorSetSpritePriority(14, 2);
                Engine_ActorGet(14)->collision_flags |= 4;
                Engine_ActorGet(15)->scale_x = -0x10000;
                Engine_ActorSetSpriteFlags(Engine_ActorGet(15), 0);
                Engine_ActorSetAnimation(15, 5);
            }
        } else if (entrance == 7) {
            if (Engine_GameFlagIsSet(0x881)) {
                Engine_ActorGet(20)->facing = 0x3000;
                if (!Engine_GameFlagIsSet(0x82e)) {
                    Call3(Engine_ActorSetPosition, 20, 0x28a0000, 0xa10000);
                    FieldScene_RunThreeActorChoreography();
                } else {
                    Call3(Engine_ActorSetPosition, 20, 0x2840000, 0xa60000);
                }
            }
        }
    }
    return 0;
}
