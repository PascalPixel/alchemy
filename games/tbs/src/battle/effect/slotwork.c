#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "types.h"
#include "motobj.h"
#include "game_state.h"
#include "fx_scene.h"
#include "system.h"
#include "dma.h"
#include "callback_scheduler.h"
#include "global_cells.h"
#include "effect_slot.h"
#include "io_reg.h"
#include "field_event.h"

#define Camera_WorldToScreen ((void (*)(struct Vec*))Camera_WorldToScreen)

struct MapActor {
    s16 id;
    s16 event;
};

void BattleFx_RunPageEffectForSlot(s32 actor, s32 element, s32 index);

extern struct BattleFxScene *gEffectWork;
void BattleFx_ClearOwnedSlot(struct EffectSlot *);

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

void Vector_AddPolarOffset(s32, s32, struct Vec *);

void Djinn_ResolvePendingEvent(s32 capture)
{
    struct GameState *st = &gGameState;
    s16 *pending = &st->pending_djinn_event;
    s32 kind = *pending & 0xf000;
    s32 flag = *pending & 0xfff;

    if (capture == 0) {
        if (kind == 0) {
            flag &= 0x7ff;
            if (flag < 300 || flag > 380)
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
        if (flag >= 300 && flag <= 380) {
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
    struct EffectSlot *slot;
    s32 remaining_slots;

    slot = gEffectWork->slots;
    remaining_slots = 0x17;
    do {
        remaining_slots -= 1;
        EffectSlot_Update(slot);
        slot++;
    } while (remaining_slots >= 0);
}

void BattleFx_InitializeSlots(void)
{
    void *work;
    volatile u32 zero;
    work = Runtime_AllocateHeapBlock(56, sizeof(struct BattleFxScene));
    zero = 0;
    Dma_Set(&zero, work, 0x85000000 | (sizeof(struct BattleFxScene) / 4), REG_DMA3);
    Scheduler_AddOrUpdateCallback(BattleFx_UpdateAllSlots, 3200);
}

void BattleFx_ClearActiveSlotsAndScheduleUpdates(void)
{
    struct BattleFxScene *scene = gEffectWork;
    struct EffectSlot *slot;
    s32 i;

    Scheduler_RemoveCallback(BattleFx_UpdateAllSlots);
    for (i = 0; i < 24; i++) {
        slot = &scene->slots[i];
        if (slot->active != 0)
            BattleFx_ClearOwnedSlot(slot);
    }
    Runtime_ReleaseHeapBlock(56);
    WaitFrames(1);
}

void BattleFx_AdvanceSpinAngle(struct MotionObject *object)
{
    object->angle += 0x2000;
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
