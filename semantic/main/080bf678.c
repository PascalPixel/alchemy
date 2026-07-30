#include "types.h"

typedef struct PendingActor_080bf678 {
    u8 column;
    u8 row;
    u8 actor;
    s8 delay;
} PendingActor_080bf678;

typedef struct PendingActorList_080bf678 {
    u8 unknown_000[8];
    PendingActor_080bf678 entries[64];
    s32 count;
} PendingActorList_080bf678;

typedef struct Scene_080bf678 {
    u8 unknown_000[0x44];
    u8 has_second_list;
    u8 unknown_045[11];
    u8 second_list_enabled;
    u8 unknown_051[0x5f7];
    u16 palette_fade;
} Scene_080bf678;

typedef struct ActorState_080bf678 {
    u8 unknown_000[0x34];
    s16 target_health;
    s16 target_energy;
    s16 health;
    s16 energy;
    u8 unknown_03c[8];
    u8 health_step;
    u8 energy_step;
    u8 unknown_046[0xfe];
    u8 action_delay;
} ActorState_080bf678;

typedef struct ActorSlot_080bf678 {
    void *object;
} ActorSlot_080bf678;

typedef s32 (*CounterUpdate_080bf678)(s32);

typedef struct CounterEffect_080bf678 {
    CounterUpdate_080bf678 update;
    u16 message;
    u8 refresh_pose;
} CounterEffect_080bf678;

PendingActorList_080bf678 *Func_08077000(s32);
ActorState_080bf678 *Func_08077008(s32);
void Func_08077010(s32);
void Func_08077118(s32, s32);
void Func_08077120(s32, s32);
void Func_080771b0(s32, s32, s32);
void Func_080771c0(s32, s32, s32);
ActorSlot_080bf678 *Func_080b7dd0(s32);
void Func_080b78e4(s32, ActorSlot_080bf678 *);
void Func_080b7aac(s32);
s32 Func_080b6c08(s32, u16 *);
void Func_080bb65c(void);
void Func_080bbabc(s32, u16);
void Func_080bd808(s32);
void Func_080bdfec(void);
void Func_080be02c(void);
void Func_080c0774(s32, s32, s32);
void Func_080c1798(s32, s32, s32, s32);
void Func_08009080(void *, s32);
void Func_08009088(void *, s32);
void Func_08015120(s32, s32);
void Func_080151c8(s32);
void Func_080f9010(s32);
s32 Func_080bf250(s32);
s32 Func_080bf2b4(s32);
s32 Func_080bf318(s32);
s32 Func_080bf37c(s32);
s32 Func_080bf3bc(s32);
s32 Func_080bf400(s32);
s32 Func_080bf440(s32);
s32 Func_080bf484(s32);
s32 Func_080bf4c4(s32);
s32 Func_080bf524(s32);
s32 Func_080bf54c(s32);
s32 Func_080bf574(s32);

static Scene_080bf678 *GetScene_080bf678(void)
{
    return *(Scene_080bf678 **)0x03001e74;
}

static void ShowCounterEffect_080bf678(
    s32 actor,
    const CounterEffect_080bf678 *effect)
{
    if (effect->update(actor) == 0)
        return;

    Func_080b78e4(actor, Func_080b7dd0(actor));
    Func_08015120(actor, 1);
    if (effect->refresh_pose != 0)
        Func_080b7aac(actor);
    Func_080151c8(effect->message);
    Func_080bb65c();
}

static void AdvanceActorMeters_080bf678(s32 actor)
{
    ActorState_080bf678 *state = Func_08077008(actor);
    s32 amount;

    if (state->health == 0)
        return;

    if (state->health_step != 0 &&
        state->health != state->target_health) {
        amount = state->health_step;
        if (state->health + amount > state->target_health)
            amount = state->target_health - state->health;

        Func_08077118(actor, amount);
        Func_08015120(actor, 1);
        Func_08015120(amount, 5);
        Func_080151c8(
            state->health == state->target_health ? 0x820 : 0x81d);
        Func_080f9010(175);
        Func_080bb65c();
    }

    if (state->energy_step != 0 &&
        state->energy != state->target_energy) {
        amount = state->energy_step;
        if (state->energy + amount > state->target_energy)
            amount = state->target_energy - state->energy;

        Func_08077120(actor, amount);
        Func_08015120(actor, 1);
        Func_08015120(amount, 5);
        Func_080151c8(
            state->energy == state->target_energy ? 0x821 : 0x81e);
        Func_080f9010(175);
        Func_080bb65c();
    }
}

static void RunNewActorAction_080bf678(PendingActor_080bf678 *pending)
{
    s32 actor = pending->actor;
    s32 column = pending->column;

    Func_080bdfec();
    Func_080bd808(30);
    Func_080bbabc(0, actor);
    Func_080bbabc(3, column * 20 + pending->row + 300);
    Func_080bbabc(14, 175);
    Func_080bbabc(10, 0);
    Func_080bbabc(4, 0x897);
    Func_080bbabc(11, actor);
    Func_080f9010(212);
    Func_08009080(Func_080b7dd0(actor)->object, 3);
    Func_08009088(Func_080b7dd0(actor)->object, 32);
    Func_080771b0(actor, column, pending->row);
    Func_080771c0(actor, pending->column, pending->row);
    Func_08077010(actor);
    Func_080c1798(actor, column, 3, 0);
    Func_080be02c();
}

void Func_080bf678(void)
{
    static const CounterEffect_080bf678 counter_effects[] = {
        {Func_080bf574, 0x889, 0},
        {Func_080bf250, 0x887, 0},
        {Func_080bf2b4, 0x888, 0},
        {Func_080bf318, 0x886, 0},
        {Func_080bf37c, 0x88b, 0},
        {Func_080bf3bc, 0x88a, 0},
        {Func_080bf400, 0x88e, 0},
        {Func_080bf440, 0x88d, 1},
        {Func_080bf484, 0x883, 1},
        {Func_080bf4c4, 0x88c, 0},
        {Func_080bf524, 0x891, 0},
        {Func_080bf54c, 0x892, 0},
    };
    static const s32 default_groups[] = {0x820, 0x81d};
    Scene_080bf678 *scene = GetScene_080bf678();
    s32 list_count = scene->has_second_list != 0 ? 2 : 1;
    s32 list_index;
    s32 group_index;
    const s32 *groups = default_groups;
    s32 special_groups[2];

    for (list_index = 0; list_index < list_count; list_index++) {
        PendingActorList_080bf678 *list = Func_08077000(list_index);
        s32 index;

        for (index = 0; index < list->count; index++) {
            PendingActor_080bf678 *pending = &list->entries[index];

            if (pending->delay > 0 &&
                Func_080b7dd0(pending->actor) != 0 &&
                Func_08077008(pending->actor)->health != 0) {
                pending->delay--;
            }
        }

        index = 0;
        while (index < list->count) {
            PendingActor_080bf678 *pending = &list->entries[index];

            if (pending->delay != 0) {
                index++;
            } else if (Func_080b7dd0(pending->actor) != 0) {
                RunNewActorAction_080bf678(pending);
            }
        }
    }

    Func_080c0774(2, scene->palette_fade, 0);

    if (scene->has_second_list != 0 && scene->second_list_enabled != 0) {
        special_groups[0] = 2;
        special_groups[1] = 1;
        groups = special_groups;
    }

    for (group_index = 0; group_index < 2; group_index++) {
        u16 actors[14];
        s32 actor_count = Func_080b6c08(groups[group_index], actors);
        s32 actor_index;

        for (actor_index = 0; actor_index < actor_count; actor_index++) {
            s32 actor = actors[actor_index];
            ActorState_080bf678 *state = Func_08077008(actor);
            u32 effect_index;

            if (state->action_delay != 0)
                state->action_delay--;

            AdvanceActorMeters_080bf678(actor);
            for (effect_index = 0;
                 effect_index < sizeof(counter_effects) /
                                    sizeof(counter_effects[0]);
                 effect_index++) {
                ShowCounterEffect_080bf678(
                    actor,
                    &counter_effects[effect_index]);
            }
        }
    }
}
