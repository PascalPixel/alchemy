/* The rock puzzle: turning one of the five rocks. */
#include "ARUTAMIRA.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SERVICE.H"

extern u8 MsgArutamiraRotatedRock[];

void SceneActor_SetPositionFromTransformedBase(s32 actor, s32 reach, s32 angle);
void ArutamiraDou_SpinActorWheel(void);
void ArutamiraDou_ReleaseWallBurst(void);

/*
 * The party turns a rock; the action names which. A round is won by turning
 * the first rock, then the last, then the one the wheel chose: the second
 * step raises the ring of five actors, and a wrong third rock lowers it
 * again. Winning a round counts it, chooses another rock and spins the wheel
 * to show it. The third win instead stops the wheel, draws the ring in to
 * its centre and releases the wall burst.
 */
void ArutamiraDou_TurnRock(s32 action)
{
    s32 won = 0;
    s32 i;
    struct FieldActor *actor;
    s32 size;
    s8 state;
    s32 chosen;
    u8 previous;
    s32 reach;
    u16 angle;
    u8 *rock;

    Engine_EventBegin();
    Battle_ResetEffectCounter();
    Engine_EventSetMessage((s32)MsgArutamiraRotatedRock);
    Engine_EventShowMessage(16, 0);
    Engine_EventEnd();
    for (i = 0; i <= 4; i++) {
        actor = Object_GetById(i + 11);
        actor->update = NULL;
        actor->scale_x = 0x10000;
        actor->scale_y = 0x10000;
    }
    {
        struct ActorWheel *wheel;

        state = ArutamiraDou_RockPuzzle->state;
        previous = ArutamiraDou_RockPuzzle->chosen;
        wheel = ArutamiraDou_Wheel;
        chosen = ArutamiraDou_RockPuzzle->chosen;
        wheel->angle = (chosen << 16) / 5 + 0x4000;
    }
    if (state == 0) {
        if (action == 16) {
            state = 1;
            Engine_AudioPlayCue(110);
        } else {
            Engine_AudioPlayCue(114);
        }
        gSceneState[0] = 0;
    } else if (state == 1) {
        if (action == 16) {
            Engine_AudioPlayCue(110);
        } else if (action == 20) {
            state = 2;
            Engine_AudioPlayCue(110);
            Engine_TaskWait(30);
            angle = ArutamiraDou_Wheel->angle;
            for (i = 0; i <= 4; i++) {
                SceneActor_SetPositionFromTransformedBase(i + 11, 0x180000, angle);
                Engine_AudioPlayCue(151);
                actor = Object_GetById(i + 11);
                actor->scale_x = 0;
                size = 0x6666;
                do {
                    actor->scale_y = size;
                    actor->scale_x = size;
                    Engine_TaskWait(1);
                    size += 0xc00;
                } while (actor->scale_x <= 0xffff);
                angle -= 0x3333;
            }
            Engine_TaskWait(30);
            won = 1;
        } else {
            Engine_AudioPlayCue(114);
            state = 0;
        }
    } else if (state == 2) {
        if (action != chosen + 16) {
            state = 0;
            Engine_AudioPlayCue(114);
            Engine_TaskWait(30);
            for (i = 0; i <= 4; i++) {
                actor = Object_GetById(i + 11);
                Engine_AudioPlayCue(151);
                for (size = actor->scale_x; actor->scale_x > 0x6666; size -= 0xc00) {
                    actor->scale_y = size;
                    actor->scale_x = size;
                    Engine_TaskWait(1);
                }
                Engine_ActorSetPosition(i + 11, 0, 0);
            }
        } else {
            Engine_AudioPlayCue(110);
            won = 1;
            Engine_TaskWait(30);
        }
    }
    rock = &gSceneState[1];
    rock[0] = state;
    if (won) {
        u8 rounds = ++rock[-1];

        rock[1] = ((s8)(((u32)(Engine_RandomNext() << 2) >> 16) + previous + 1) + 5) % 5;
        ArutamiraDou_Wheel->phase = 0;
        ArutamiraDou_Wheel->ticks = 0;
        ArutamiraDou_Wheel->speed = 0x200;
        ArutamiraDou_Wheel->travel = 0x3000;
        Engine_TaskAddCallback(ArutamiraDou_SpinActorWheel, TASK_PRIORITY_SCENE);
        if (rounds <= 2) {
            while (ArutamiraDou_Wheel->phase != 99) {
                Engine_TaskWait(1);
            }
            Engine_TaskWait(10);
            Engine_AudioPlayCue(110);
        } else {
            rock[0] = 99;
            while (ArutamiraDou_Wheel->phase != 2) {
                Engine_TaskWait(1);
            }
            ArutamiraDou_Wheel->phase = 2;
            ArutamiraDou_Wheel->ticks = 0;
            Engine_WorkSetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
            Battle_WaitMode0(20);
            Engine_WorkSetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
            ArutamiraDou_Wheel->phase = 99;
            Engine_AudioPlayCue(190);
            reach = 0x180000;
            angle = ArutamiraDou_Wheel->angle;
            do {
                for (i = 0; i <= 4; i++) {
                    actor = Object_GetById(i + 11);
                    actor->scale_x -= 16;
                    actor->scale_y -= 16;
                    SceneActor_SetPositionFromTransformedBase(i + 11, reach, angle);
                    angle -= 0x3333;
                }
                reach -= 0x3333;
                angle += 0xc00;
                Engine_TaskWait(1);
            } while (reach > 0);
            for (i = 0; i <= 4; i++) {
                Engine_ActorSetPosition(i + 11, 0, 0);
            }
            ArutamiraDou_ReleaseWallBurst();
            Engine_AudioPlayCue(80);
        }
        Engine_TaskRemoveCallback(ArutamiraDou_SpinActorWheel);
    }
}
