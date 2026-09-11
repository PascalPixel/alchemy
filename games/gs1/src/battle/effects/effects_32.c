#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "effect_0809b11c.h"

/* battle/effects/run/run_page_effect_for_slot.c */
struct MenuState {
    u8 padding_000[20];
    s32 slots[4];
    u8 padding_024[414 - 36];
    s16 mode;
};

s32 GameFlag_IsSet(s32);
void GameFlag_Set(void *);

void Battle_Reset(void);

void BattleFx_FinishHeavyImpact(s32);

void BattleFx_RunPageEffectForSlot(s32 slot, s32 page, void *entries)
{
    struct MenuState *state;
    s32 selection;

    state = *(struct MenuState **)ADDR_03001EBC;
    if (GameFlag_IsSet(366) != 0) {
        selection = 0;
        GameFlag_Set((u8 *)entries + page * 20 + 48);
    } else {
        selection = Battle_Apply(page, entries);
    }

    if (selection >= 0) {
        Battle_Reset();
        Battle_Run();
        if (slot != -1) {
            if (state->mode == 3)
                Battle_unk2_4();

            if (page == 0)
                Battle_Do(slot);
            else if (page == 1)
                Battle_unk2_2(slot);
            else if (page == 2)
                Battle_unk3_2(slot);
            else if (page == 3)
                BattleFx_FinishHeavyImpact(slot);

            state->slots[slot] = 0;
            if (state->mode == 3)
                Battle_unk3_4();
        }

        Battle_Place(selection, page, entries);
        Battle_unk4_4();
        Battle_unk5_2();
    }
}

/* battle/effects/runtime/upd/update_all_slots.c */
void BattleFx_UpdateAllSlots(void)
{
    s32 slot;
    s32 remaining_slots;

    slot = *(s32 *)ADDR_03001F30 + 0x58;
    remaining_slots = 0x17;
    do {
        remaining_slots -= 1;
        EffectSlot_Update((struct EffectSlot *)slot);
        slot += 0x48;
    } while (remaining_slots >= 0);
}

/* battle/effects/runtime/misc/clear_active_slots_and_schedule_updates.c */
void ScheduleCallback(void *);

void WaitFrames(s32);
extern u8 gRom;

void BattleFx_ClearActiveSlotsAndScheduleUpdates(void)
{
    u8 *slot = *(u8 **)ADDR_03001F30;
    u8 *active_flag;
    s32 slot_index;

    ScheduleCallback(&gRom);
    active_flag = slot + 157;
    slot += 88;
    for (slot_index = 23; slot_index >= 0; slot_index--) {
        u32 is_active = *(volatile u8 *)active_flag;
        active_flag += 72;
        if ((is_active << 24) != 0) {
            Battle_Do(slot);
        }
        slot += 72;
    }
    Battle_unk2_2(56);
    WaitFrames(1);
}

void BattleFx_AdvanceSpinAngle(void *object)
{
    *(u16 *)((u8 *)object + 6) += 0x2000;
}

/* battle/effects/objects/run_angled_approach_phases.c */
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

extern s32 gCell[];

struct Target *Battle_Run(s32);

void Battle_unk2_4(struct Actor *actor)
{
    struct Target *target;
    struct Vec pos;

    target = Battle_Run(gCell[125]);
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
        Battle_Do(&pos);
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
        Battle_unk2_2(actor);
    }
    pos.x = actor->x;
    pos.z = actor->z;
    Battle_Place(actor->yaw << 16, actor->pitch << 11, &pos);
    actor->screenX = pos.x;
    actor->screenZ = pos.z;
}
