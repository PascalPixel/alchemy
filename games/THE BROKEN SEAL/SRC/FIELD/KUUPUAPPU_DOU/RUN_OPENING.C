#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "TYPES.H"

u8 *Object_GetById(s32);

#include "TYPES.H"

void Vector_AddPolarOffset(s32, s32, s32 *);
void Object_SetMoveTarget(s32 *, s32, s32, s32);

#include "TYPES.H"

void KuupuappuDou_PushBlockAhead();

s32 SceneActor_LiftLowActorOnSubjectTile(s32 subject_actor);

#include "TYPES.H"

void Scheduler_AddOrUpdateCallback();

#include "TYPES.H"

extern s32 KuupuappuDou_ScheduleFrames[];
extern s32 KuupuappuDou_ScheduleTimer;
extern s32 KuupuappuDou_ScheduleIndex;
extern s32 KuupuappuDou_TickCounter;
extern s32 KuupuappuDou_TickValue;

void KuupuappuDou_SpawnPuffs();
s32 IwramUnsignedRemainder();

/* The scene step counter at 0x1d8 of the shared scene work record. */

#include "TYPES.H"

#include "TYPES.H"

#include "TYPES.H"

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
};

extern void KuupuappuDou_RaiseActorPriorities(void);

#include "TYPES.H"

/* Deliberate no-op callback. */

#include "TYPES.H"

#include "TYPES.H"

enum SelectByRuntimeSelectorMessage {
    MSG_DOOR_TIGHTLY_LOCKED = 0x953,
    MSG_ROBIN_FLIPPED_SWITCH = 0x1528
};

extern u8 *Object_GetById(s32);

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    s32 i;
    u8 *rec7;
    s32 flag;
    s32 index;

    flag = *(u8 *)(Object_GetById(10) + 91);
    if (flag == 0) {
        if (++KuupuappuDou_ScheduleTimer > 190) {
            KuupuappuDou_ScheduleTimer = 0;
        }
        index = KuupuappuDou_ScheduleIndex;
        if (KuupuappuDou_ScheduleFrames[index] == KuupuappuDou_ScheduleTimer) {
            rec7 = Actor_Get((index + 11));
            *(s32 *)(rec7 + 72) = 0xa3d;
            if (++KuupuappuDou_ScheduleIndex > 3) {
                KuupuappuDou_ScheduleIndex = 0;
            }
        }
        for (i = 0; i <= 3; i++) {
            rec7 = Actor_Get((i + 11));
            if (*(s32 *)(rec7 + 40) >= 0) {
                if (*(s32 *)(rec7 + 12) <= 0xffff) {
                    KuupuappuDou_SpawnPuffs();
                    *(s32 *)(rec7 + 12) = 0xff0000;
                    *(s32 *)(rec7 + 72) = 0;
                    *(s32 *)(rec7 + 40) = 0;
                    rec7[91] = 0;
                    Audio_PlayCue(106);
                }
            }
        }
        if (SceneActor_LiftLowActorOnSubjectTile(10) != 0) {
            Actor_SetAnimation(10, 1);
            if (GameFlag_IsSet(0x207) == 0) {
                GameFlag_Set(0x207);
                Audio_PlayCue(204);
            } else {
                Audio_PlayCue(106);
            }
        }
        if (SceneActor_LiftLowActorOnSubjectTile(9) != 0) {
            Audio_PlayCue(106);
        }
    }
}

void FieldScene_RunScene3a7SequenceD(void)
{

    s32 i;
    u8 *rec7;
    s32 record;
    s32 *selected;

    rec7 = (u8 *)Actor_Get(10);
    if (rec7[91] == 0) {
        if ((++KuupuappuDou_TickCounter & 63) == 0) {
            selected = &KuupuappuDou_TickValue;
            record = Random_Next();
            record = IwramUnsignedRemainder(record, 6);
            *selected = record;
            rec7 = Actor_Get((record + 10));
            *(s32 *)(rec7 + 72) = 0xa3d;
        }
        for (i = 0; i <= 5; i++) {
            rec7 = Actor_Get((i + 10));
            record = GameFlag_IsSet((i + 0x200));
            if (record != 0) {
                if (*(s32 *)(rec7 + 40) <= 0) {
                    if (*(s32 *)(rec7 + 12) > 0x20ffff) {
                        continue;
                    }
                }
                *(s32 *)(rec7 + 12) = 0xff0000;
                *(s32 *)(rec7 + 72) = 0;
                *(s32 *)(rec7 + 40) = 0;
                Audio_PlayCue(106);
            } else {
                if (*(s32 *)(rec7 + 40) <= 0) {
                    if (*(s32 *)(rec7 + 12) > 0xffff) {
                        continue;
                    }
                }
                *(s32 *)(rec7 + 72) = record;
                *(s32 *)(rec7 + 40) = record;
                *(s32 *)(rec7 + 12) = 0xff0000;
                Audio_PlayCue(106);
            }
        }
    }
}

void SceneActor_TransformAndApplyRecordPosition(s32 *rec, s32 v0, s32 v1)
{
    s32 pos[3];

    if (rec == 0) {
        return;
    }
    pos[0] = rec[2];
    pos[1] = rec[3];
    pos[2] = rec[4];
    Vector_AddPolarOffset(v0, v1, pos);
    Object_SetMoveTarget(rec, pos[0], pos[1], pos[2]);
}
