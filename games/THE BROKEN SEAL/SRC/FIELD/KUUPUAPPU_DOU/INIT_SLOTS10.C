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
void FieldScene_RunOpeningAuxiliarySequence(void);
void FieldScene_RunScene3a7SequenceD(void);

enum SelectByRuntimeSelectorMessage {
    MSG_DOOR_TIGHTLY_LOCKED = 0x953,
    MSG_ROBIN_FLIPPED_SWITCH = 0x1528
};

extern u8 *Object_GetById(s32);

void SceneActor_InitSlots10To15AndStartTask(void)
{
    s32 selector = 10;
    s32 remaining = 5;

    do {
        s32 *record;

        Actor_SetSpriteFlags(Actor_Get(selector), 0);
        record = Actor_Get(selector);
        record[17] = 0x1999;
        record[18] = 0;
        remaining--;
        record[3] = 0x00ff0000;
        selector++;
    } while (remaining >= 0);

    {
        s32 rank = 0xc80;

        Scheduler_AddOrUpdateCallback((s32)FieldScene_RunScene3a7SequenceD, rank);
    }
}

void SceneActor_SetupActors11To14AndInstallTask(void)
{
    s32 no = 11;
    s32 i = 0;

    do {
        s32 *rec;

        Actor_SetSpriteFlags(Actor_Get(no), 0);
        rec = Actor_Get(no);
        rec[17] = 0x1999;
        rec[18] = 0;
        rec[3] = 0x00ff0000;
        Actor_SetSpritePriority(i + 11, 1);
        i++;
        no++;
    } while (i <= 3);

    {
        s32 rate = 0xc80;

        Scheduler_AddOrUpdateCallback((s32)FieldScene_RunOpeningAuxiliarySequence, rate);
    }
}
