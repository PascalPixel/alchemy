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

enum SelectByRuntimeSelectorMessage {
    MSG_DOOR_TIGHTLY_LOCKED = 0x953,
    MSG_ROBIN_FLIPPED_SWITCH = 0x1528
};

extern u8 *Object_GetById(s32);

s32 *SceneData_FindActiveSlotAtCell(s32 cx, s32 cz)
{
    s32 **slots = (s32 **)((u8 *)gEventWork + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if (cx == (p[2] >> 20) && cz == (p[4] >> 20) && *((u8 *)p + 0x59) != 0) {
            return p;
        }
    }
    return 0;
}
