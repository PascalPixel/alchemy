/* NONMATCHING: resource_3bd at 0x0200909c, the wheel game that follows
 * ArutamiraDou_SpinActorWheel in ARUTAMIRA_DOU/ACTOR_WHEEL.C, stays listing.
 *
 * Remaining difference (alchemy permute 4400; best permuted 2835): register
 * allocation and scheduling in the opening scene-state reads and in the two
 * scaling loops. It needs import labels __modsi3,
 * Engine_TaskAddCallback and Engine_TaskRemoveCallback on the matching
 * veneers in ARUTAMIRA_DOU/IMPORT.S.
 */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/ARUTAMIRA.H"

void ArutamiraDou_SpinActorWheel(void);

void SceneActor_SetPositionFromTransformedBase(s32 actor, s32 radius, s32 angle);
s32 __modsi3(s32 dividend, s32 divisor);

void ArutamiraDou_RunWheelGame(s32 actor)
{
    s32 spin = 0;
    s32 state;
    s32 chosen;
    s32 offset;
    s32 i;
    s32 scale;
    s16 angle;
    s32 radius;
    s32 turns;
    u16 *wheel;
    u8 *object;
    u8 *p;
    u8 *q;

    Engine_EventBegin();
    Battle_ResetEffectCounter();
    Engine_EventSetMessage(0x21db);
    Engine_EventShowMessage(16, 0);
    Engine_EventEnd();
    for (i = 0; i <= 4; i++) {
        object = (u8 *)Object_GetById(i + 11);
        *(s32 *)(object + 108) = 0;
        *(s32 *)(object + 24) = 0x10000;
        *(s32 *)(object + 28) = 0x10000;
    }
    p = gSceneState + 1;
    state = p[0];
    chosen = ((s8 *)p)[1];
    wheel = ArutamiraDou_ClearTarget;
    offset = p[1];
    wheel[3] = __divsi3(chosen << 16, 5) + 0x4000;
    if ((s8)state == 0) {
        if (actor == 16) {
            state = 1;
            Audio_PlayCue(110);
        } else {
            Audio_PlayCue(114);
        }
        gSceneState[0] = 0;
    } else if ((s8)state == 1) {
        if (actor == 16) {
            Audio_PlayCue(110);
        } else if (actor == 20) {
            state = 2;
            Audio_PlayCue(110);
            Engine_TaskWait(30);
            angle = (s16)ArutamiraDou_ClearTarget[3];
            for (i = 0; i <= 4; i++) {
                SceneActor_SetPositionFromTransformedBase(i + 11, 0x180000, (u16)angle);
                Audio_PlayCue(151);
                object = (u8 *)Object_GetById(i + 11);
                *(s32 *)(object + 24) = 0;
                scale = 0x6666;
                do {
                    *(s32 *)(object + 28) = scale;
                    *(s32 *)(object + 24) = scale;
                    Engine_TaskWait(1);
                    scale += 0xc00;
                } while (*(s32 *)(object + 24) <= 0xffff);
                angle = (u16)angle - 0x3333;
            }
            Engine_TaskWait(30);
            spin = 1;
        } else {
            Audio_PlayCue(114);
            state = 0;
        }
    } else if ((s8)state == 2) {
        if (actor != chosen + 16) {
            state = 0;
            Audio_PlayCue(114);
            Engine_TaskWait(30);
            for (i = 0; i <= 4; i++) {
                object = (u8 *)Object_GetById(i + 11);
                Audio_PlayCue(151);
                scale = *(s32 *)(object + 24);
                while (*(s32 *)(object + 24) > 0x6666) {
                    *(s32 *)(object + 28) = scale;
                    *(s32 *)(object + 24) = scale;
                    Engine_TaskWait(1);
                    scale -= 0xc00;
                }
                Engine_ActorSetPosition(i + 11, 0, 0);
            }
        } else {
            Audio_PlayCue(110);
            spin = 1;
            Engine_TaskWait(30);
        }
    }
    q = gSceneState + 1;
    q[0] = state;
    if (spin == 0)
        return;
    turns = ++q[-1];
    q[1] = __modsi3((s8)((((u32)Engine_RandomNext() << 2) >> 16) + offset + 1) + 5, 5);
    wheel = ArutamiraDou_ClearTarget;
    {
        s32 zero = 0;

        wheel[0] = zero;
        wheel[1] = zero;
    }
    {
        s32 speed = 0x200;

        wheel[4] = speed;
    }
    {
        s32 travel = 0x3000;

        wheel[5] = travel;
    }
    Engine_TaskAddCallback(ArutamiraDou_SpinActorWheel, 0xc80);
    if ((u8)turns <= 2) {
        while ((s16)ArutamiraDou_ClearTarget[0] != 99)
            Engine_TaskWait(1);
        Engine_TaskWait(10);
        Audio_PlayCue(110);
    } else {
        q[0] = 99;
        while ((s16)ArutamiraDou_ClearTarget[0] != 2)
            Engine_TaskWait(1);
        {
            s32 settle = 2;
            s32 zero = 0;

            ArutamiraDou_ClearTarget[0] = settle;
            ArutamiraDou_ClearTarget[1] = zero;
        }
        Engine_WorkSetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Battle_WaitMode0(20);
        Engine_WorkSetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
        {
            s32 done = 99;

            ArutamiraDou_ClearTarget[0] = done;
        }
        Audio_PlayCue(190);
        radius = 0x180000;
        angle = (s16)ArutamiraDou_ClearTarget[3];
        do {
            for (i = 0; i <= 4; i++) {
                object = (u8 *)Object_GetById(i + 11);
                *(s32 *)(object + 24) -= 16;
                *(s32 *)(object + 28) -= 16;
                SceneActor_SetPositionFromTransformedBase(i + 11, radius, (u16)angle);
                angle = (u16)angle - 0x3333;
            }
            angle = (u16)angle + 0xc00;
            radius -= 0x3333;
            Engine_TaskWait(1);
        } while (radius > 0);
        for (i = 0; i <= 4; i++)
            Engine_ActorSetPosition(i + 11, 0, 0);
        ArutamiraDou_ReleaseWallBurst();
        Audio_PlayCue(80);
    }
    Engine_TaskRemoveCallback(ArutamiraDou_SpinActorWheel);
}
