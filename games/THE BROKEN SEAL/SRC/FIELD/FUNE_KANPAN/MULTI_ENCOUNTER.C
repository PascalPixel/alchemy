#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KANPAN.H"
extern u8 MsgFuneDidntDoAnything[];
/* FAKEMATCH: calls that cast Object_GetById to another return type keep their original register order. */
s32 Object_GetById();

union Slot {
    s32 w;
    s16 h[2];
};

s32 BuildMotionCountdown(s32, s16);

void Battle_ResetEffectCounter();

void FieldScene_RunScene3af_02000bf0(void);

void FieldScene_RunScene3af_020010a0(void)
{
    u8 bits;

    if (GameFlag_IsSet(0x911) != 0) {
        if (GameFlag_IsSet(0x922) == 0) {
            Event_Begin();
            Battle_ResetEffectCounter();
            FieldScene_RunScene3af_020012f0();
            Actor_SetSpeed(20, 0x6666, 0x3333);
            *(u8 *)(Object_GetById(20) + 90) &= 254;
            Actor_WalkToAndWait(20, 232, 0x330);
            Event_Wait(1);
            bits = 1;
            {
                u8 *record = Object_GetById(20);
                u8 value = record[90];

                record[90] = value | bits;
            }
            Event_Wait(20);
            Actor_StartRepeatedMotion(20, 2);
            FieldScene_RunStepThen10(20);
            Actor_SetSpeed(20, 0x13333, 0x9999);
            *(u8 *)(Object_GetById(20) + 90) &= 254;
            Actor_WalkToAndWait(20, 244, 0x324);
            Event_Wait(1);
            {
                u8 *record = ((u8 *(*)())Object_GetById)(20);

                bits |= record[90];
                record[90] = bits;
            }
            Event_Wait(20);
            Actor_SetSpeed(20, 0x33333, 0x19999);
            Actor_WalkToAndWait(20, 248, 0x30a);
            Actor_WalkToAndWait(20, 248, 0x2bc);
            Actor_SetPosition(20, 0xf60000, 0x2000000);
            Actor_FaceDirection(20, 0, 0);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
            Event_End();
        }
    }
}

void FieldScene_RunScene3af_020011c8(void)
{
    u8 bits;

    if (GameFlag_IsSet(0x911) != 0) {
        if (GameFlag_IsSet(0x922) == 0) {
            Event_Begin();
            Battle_ResetEffectCounter();
            FieldScene_RunScene3af_020012f0();
            Actor_SetSpeed(20, 0x6666, 0x3333);
            *(u8 *)(Object_GetById(20) + 90) &= 254;
            Actor_WalkToAndWait(20, 202, 0x330);
            Event_Wait(1);
            bits = 1;
            {
                u8 *record = Object_GetById(20);
                u8 value = record[90];

                record[90] = value | bits;
            }
            Event_Wait(20);
            Actor_StartRepeatedMotion(20, 2);
            FieldScene_RunStepThen10(20);
            Actor_SetSpeed(20, 0x13333, 0x9999);
            *(u8 *)(Object_GetById(20) + 90) &= 254;
            Actor_WalkToAndWait(20, 192, 0x324);
            Event_Wait(1);
            {
                u8 *record = ((u8 *(*)())Object_GetById)(20);

                bits |= record[90];
                record[90] = bits;
            }
            Event_Wait(20);
            Actor_SetSpeed(20, 0x33333, 0x19999);
            Actor_WalkToAndWait(20, 180, 0x30a);
            Actor_WalkToAndWait(20, 180, 0x2bc);
            Actor_SetPosition(20, 0xf60000, 0x2000000);
            Actor_FaceDirection(20, 0, 0);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
            Event_End();
        }
    }
}

void FieldScene_RunScene3af_020012f0(void)
{
    u32 i;
    s32 record;

    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0xd80000, -1, 0x3380000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    FieldScene_RunScene3af_02000bf0();
    Map_CopyCellsTo(30, 108, 13, 108, 1, 2);
    Event_Wait(10);
    Actor_SetPosition(20, 0xd80000, 0x3200000);
    Actor_SetSpeed(20, 0x13333, 0x9999);
    Actor_WalkToAndWait(20, 216, 0x32e);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 20, 10);
    Actor_SetAnimationAndWait(20, 4);
    Actor_StartRepeatedMotion(20, 2);
    Actor_ShowEmote(20, 0x100, 20);
    Actor_FaceActor(20, ACTOR_PARTY_LEADER, 20);
    Actor_StartRepeatedMotion(20, 2);
    Event_SetMessage((s32)MsgFuneDidntDoAnything);
    Event_ShowMessageAndWait(20, 0, 20);
    Engine_ActorShowEmote(20, 0x102, 0);
    GameFlag_Set(0x923);
}
void FieldScene_RunStepThen10(s32 a);
