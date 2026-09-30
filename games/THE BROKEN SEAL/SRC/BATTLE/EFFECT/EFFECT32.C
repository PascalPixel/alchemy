#include "TYPES.H"
#include "SYSTEM.H"
#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"
#include "GLOBAL_CELLS.H"
#include "EFFECT_0809B11C.H"

/* Party-state word 0x234 holds a pending Djinn event: the top nibble is the
   kind and the low bits the game flag. Flags 300-380 are the eighty Djinn,
   twenty per element. */
struct DjinnEventState {
    u8 unknown_000[0x234];
    s16 pending;
    s16 result;
};

struct MapActor {
    s16 id;
    s16 event;
};

extern struct DjinnEventState Data_02000240;
s32 Math_Div(s32, s32);
s32 Math_Mod(s32, s32);
void GameFlag_SetBitFar(s32);
struct MapActor *BattleAction_FindDescriptor(s32 id);
void BattleFx_RunPageEffectForSlot(s32 actor, s32 element, s32 index);

extern u8 Data_03001f30[];
void *Runtime_AllocateHeapBlock(s32, s32);
void BattleFx_ClearOwnedSlot(struct EffectSlot *);

struct EffectScene {
    u8 unknown_00[0x58];
    struct EffectSlot slots[24];
};

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct Actor {
    u8 pad0[4];
    s32 screenX;
    s32 screenZ;
    u8 pad1[8];
    s32 x;
    s32 z;
    u8 pad2[28];
    s16 timer;
    u8 pad3[2];
    s16 yaw;
    s16 pitch;
    s8 phase;
};

struct Target {
    u8 pad0[8];
    struct Vec pos;
};

extern s32 gGameState[];
void Vector_AddPolarOffset(s32, s32, struct Vec *);
struct Target *Object_GetById(s32);
void Camera_WorldToScreen(struct Vec *);

void Djinn_ResolvePendingEvent(s32 capture)
{
    struct DjinnEventState *st = &Data_02000240;
    s16 *pending = &st->pending;
    s32 kind = *pending & 0xf000;
    s32 flag = (u16)*pending & 0xfff;

    if (capture == 0) {
        if (kind == 0) {
            flag &= 0x7ff;
            if ((u32)(flag - 300) > 80)
                return;
            if (st->result > 0 && st->result != 999)
                return;
            GameFlag_SetBitFar(flag - 172);
            *pending = kind;
        } else if (kind == 0x1000) {
            if (st->result == 1)
                GameFlag_SetBitFar(flag);
            *pending = capture;
        }
        return;
    }
    if (kind == 0) {
        flag &= 0x7ff;
        if ((u32)(flag - 300) <= 80) {
            flag &= 0x7ff;
            if (st->result > 0) {
                s32 djinn = flag - 300;
                s32 element = Math_Div(djinn, 20);
                s32 index = Math_Mod(djinn, 20);
                s32 actor;

                for (actor = 8; actor <= 65; actor++) {
                    struct MapActor *found = BattleAction_FindDescriptor(actor);
                    if (found != 0 && found->event - 48 == flag - 300) {
                        WaitFrames(40);
                        BattleFx_RunPageEffectForSlot(actor, element, index);
                        break;
                    }
                }
            }
        }
    }
    Data_02000240.pending = 0;
}

void BattleFx_UpdateAllSlots(void)
{
    s32 slot;
    s32 remaining_slots;

    slot = *(s32 *)((u32)&Data_03001f30) + 0x58;
    remaining_slots = 0x17;
    do {
        remaining_slots -= 1;
        EffectSlot_Update((struct EffectSlot *)slot);
        slot += 0x48;
    } while (remaining_slots >= 0);
}

void BattleFx_InitializeSlots(void)
{
    void *work;
    volatile u32 zero;
    work = Runtime_AllocateHeapBlock(56, 0x720);
    zero = 0;
    Dma_Set(&zero, work, 0x850001c8, (volatile u32 *)0x040000d4);
    Scheduler_AddOrUpdateCallback((s32)BattleFx_UpdateAllSlots, 3200);
}

void BattleFx_ClearActiveSlotsAndScheduleUpdates(void)
{
    struct EffectScene *scene = *(struct EffectScene **)((u32)&Data_03001f30);
    struct EffectSlot *slot;
    s32 i;

    Scheduler_RemoveCallback((u32)BattleFx_UpdateAllSlots);
    for (i = 0; i < 24; i++) {
        slot = &scene->slots[i];
        if (slot->active != 0)
            BattleFx_ClearOwnedSlot(slot);
    }
    Runtime_ReleaseHeapBlock(56);
    WaitFrames(1);
}

void BattleFx_AdvanceSpinAngle(void *object)
{
    *(u16 *)((u8 *)object + 6) += 0x2000;
}

void BattleFx_RunAngledApproachPhases(struct Actor *actor)
{
    struct Target *target;
    struct Vec pos;

    target = Object_GetById(gGameState[125]);
    if (actor->phase == 0) {
        actor->yaw += 1;
        actor->pitch += 1;
        if (actor->timer == 60) {
            actor->timer = 0;
            actor->phase += 1;
        }
    } else if (actor->phase == 1) {
        actor->pitch += 1;
        if (actor->timer == 40) {
            actor->timer = 0;
            actor->phase += 1;
        }
    } else if (actor->phase == 2) {
        actor->pitch += 1;
        pos.x = target->pos.x;
        pos.y = target->pos.y + 0x140000;
        pos.z = target->pos.z;
        Camera_WorldToScreen(&pos);
        actor->x += (pos.x - actor->x) / 8;
        actor->z += (pos.z - actor->z) / 8;
        if (actor->timer == 40) {
            actor->timer = 0;
            actor->phase += 1;
        }
    } else if (actor->phase == 3) {
        actor->yaw -= 1;
        actor->pitch += 1;
        if (actor->timer == 60) {
            actor->timer = 0;
            actor->phase += 1;
        }
    } else if (actor->phase == 4) {
        BattleFx_ClearOwnedSlot(actor);
    }
    pos.x = actor->x;
    pos.z = actor->z;
    Vector_AddPolarOffset(actor->yaw << 16, actor->pitch << 11, &pos);
    actor->screenX = pos.x;
    actor->screenZ = pos.z;
}
