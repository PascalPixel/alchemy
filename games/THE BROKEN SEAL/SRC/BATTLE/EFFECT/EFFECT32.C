#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "GAME_STATE.H"
#include "FX_SCENE.H"
#include "SYSTEM.H"
#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"
#include "GLOBAL_CELLS.H"
#include "EFFECT_SLOT.H"

/* Party-state word 0x234 holds a pending Djinn event: the top nibble is the
   kind and the low bits the game flag. Flags 300-380 are the eighty Djinn,
   twenty per element. */

struct MapActor {
    s16 id;
    s16 event;
};

void GameFlag_SetBitFar(s32);
struct MapActor *BattleAction_FindDescriptor(s32 id);
void BattleFx_RunPageEffectForSlot(s32 actor, s32 element, s32 index);

extern struct BattleFxScene *gEffectWork;
void *Runtime_AllocateHeapBlock(s32, s32);
void BattleFx_ClearOwnedSlot(struct EffectSlot *);

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

void Vector_AddPolarOffset(s32, s32, struct Vec *);
struct MotionObject *Object_GetById(s32);
void Camera_WorldToScreen(struct Vec *);

void Djinn_ResolvePendingEvent(s32 capture)
{
    struct GameState *st = &gGameState;
    s16 *pending = &st->pending_djinn_event;
    s32 kind = *pending & 0xf000;
    s32 flag = (u16)*pending & 0xfff;

    if (capture == 0) {
        if (kind == 0) {
            flag &= 0x7ff;
            if ((u32)(flag - 300) > 80)
                return;
            if (st->scene_change_reason > 0 && st->scene_change_reason != 999)
                return;
            GameFlag_SetBitFar(flag - 172);
            *pending = kind;
        } else if (kind == 0x1000) {
            if (st->scene_change_reason == 1)
                GameFlag_SetBitFar(flag);
            *pending = capture;
        }
        return;
    }
    if (kind == 0) {
        flag &= 0x7ff;
        if ((u32)(flag - 300) <= 80) {
            flag &= 0x7ff;
            if (st->scene_change_reason > 0) {
                s32 djinn = flag - 300;
                s32 element = djinn / 20;
                s32 index = djinn % 20;
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
    gGameState.pending_djinn_event = 0;
}

void BattleFx_UpdateAllSlots(void)
{
    s32 slot;
    s32 remaining_slots;

    slot = (s32)gEffectWork + 0x58;
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
    struct BattleFxScene *scene = gEffectWork;
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

void BattleFx_RunAngledApproachPhases(struct EffectSlot *actor)
{
    struct MotionObject *target;
    struct Vec pos;

    target = Object_GetById(gGameState.selected_actor);
    if (actor->state == 0) {
        actor->orbit_radius += 1;
        actor->orbit_angle += 1;
        if ((s16)actor->age == 60) {
            actor->age = 0;
            actor->state += 1;
        }
    } else if (actor->state == 1) {
        actor->orbit_angle += 1;
        if ((s16)actor->age == 40) {
            actor->age = 0;
            actor->state += 1;
        }
    } else if (actor->state == 2) {
        actor->orbit_angle += 1;
        pos.x = target->x;
        pos.y = target->y + 0x140000;
        pos.z = target->z;
        Camera_WorldToScreen(&pos);
        actor->origin_x += (pos.x - actor->origin_x) / 8;
        actor->origin_z += (pos.z - actor->origin_z) / 8;
        if ((s16)actor->age == 40) {
            actor->age = 0;
            actor->state += 1;
        }
    } else if (actor->state == 3) {
        actor->orbit_radius -= 1;
        actor->orbit_angle += 1;
        if ((s16)actor->age == 60) {
            actor->age = 0;
            actor->state += 1;
        }
    } else if (actor->state == 4) {
        BattleFx_ClearOwnedSlot(actor);
    }
    pos.x = actor->origin_x;
    pos.z = actor->origin_z;
    Vector_AddPolarOffset(actor->orbit_radius << 16, actor->orbit_angle << 11, &pos);
    actor->x = pos.x;
    actor->z = pos.z;
}
