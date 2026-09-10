#include "types.h"

#define NULL ((void *)0)
#define StagedActorStepTable Data_020096c0
#define GetStagedActor Func_020018ec
#define FindNextStagedActor Func_0200073e
#define FindBlockingStagedActor Func_0200076a
#define FindElevatedBlockingStagedActor Func_02000796
#define CanStartStagedActorMove Func_02001958
#define SetStagedActorMode Func_02001940
#define SelectStagedActorSlot Func_02001930
#define StartStagedActorEffect Func_02001a7e
#define StartNextStagedActorMove Func_02001976
#define StartLeadStagedActorMove Func_02001986
#define FinishStagedActorMove Func_02001994
#define FinishStagedActorEffect Func_02001aa0
#define SetStagedActorTransition Func_020019ac
#define StagedActorPairScene_SpawnPrimaryEffect Func_02000048
#define StagedActorPairScene_SpawnSecondaryEffect Func_020000a0
#define StagedActorPairScene_SpawnConfiguredEffect Func_0200013c
#define StagedActorPairScene_AdvancePair Func_020003a8
#define StagedActorPairScene_StopBlockedMotion Func_0200058c
#define StagedActorPairScene_FindClearPosition Func_02000758
#define StagedActorPairScene_GetVariantData Func_02000cc0
#define StagedActorPairScene_GetInitialValue Func_02000d14
#define StagedActorPairScene_RunStep Func_020010f0
#define StagedActorPairScene_RunUpdate Func_02001148
#define StagedActorPairScene_RotateActorPart Func_020011c8
#define StagedActorPairScene_WaitForHeight Func_020011d8
#define StagedActorPairScene_RunActorTwelveCommand Func_02001384

#include "staged_actor_pair_scene.h"
#include "staged_actor.h"

struct EffectSprite {
    u8 pad00[9];
    u8 flags;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state;
};

struct SceneEffect {
    u8 pad00[24];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flags;
    u8 pad24[12];
    s32 rate_x;
    s32 rate_y;
    u8 pad38[12];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    struct EffectSprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct EffectOptions {
    u8 mode_bits;
    u8 pad01[3];
    s32 mode;
    s32 accum18;
    s32 accum1c;
    s32 target30;
    s32 target34;
    s16 kind;
    u16 pad1a;
    s32 callback_arg;
    u16 angle;
    u16 step;
    u32 callback;
};

struct EffectDescriptor {
    s32 pad00[3];
    s32 duration;
};

typedef struct { s32 x; s32 y; s32 z; } ScenePosition;

typedef struct { u8 filler0[0x28]; s16 *kind; } ActorSpriteRef;

typedef struct {
    u8 filler0[6];
    u16 direction_and_kind;
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[0x10];
    s32 move_delta_x;
    u8 filler28[4];
    s32 move_delta_z;
    u8 filler30[8];
    s32 move_target_x;
    u8 filler3C[4];
    s32 move_target_z;
    u8 filler44[0xC];
    ActorSpriteRef *sprite;
} MovingActor;

/*
 * resource_3be owner at 0x020011d8, 32 bytes.
 *
 * Runs a step at most 40 times, stopping early once the caller's +12 field has
 * come down to the limit. Both the counter and the field test guard the loop.
 */
struct HeightTrackedObject {
    u8 pad00[12];
    s32 height;                 /* +12 */
};

extern struct EffectDescriptor *Data_02009778[];
extern s32 Data_02009700[];
extern s32 Data_02009718[];
extern s16 Data_02000240[];
extern u8 Value_00000098;
extern u8 Value_0000009d;
extern u8 Value_0000009e;
extern u8 Data_020097b4[];
extern u8 Data_020097fc[];
extern u8 Data_02009874[];
extern u8 Data_02009784[];

void *Func_0200150e(s32, s32, s32, s32);
void Func_0200155c(void *, s32);
void Func_02001574(void *, s32);
void Func_0200160c(void *, s32);
void *Func_02001566(s32, s32, s32, s32);
void Func_020015b8(void *, s32);
void Func_02001668(void *, s32);
struct SceneEffect *Func_02001692();
struct SceneEffect *Func_02001638();
void Func_02001642();
void Func_0200165c();
void Func_0200179e();
s32 Func_02001700();
s32 Func_02001718();
s32 Func_02001726();
void Func_0200175c();
void Func_0200176c();
struct StagedActor *Func_020018ec(s32 arg0);
struct StagedActor *Func_0200073e(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_0200076a(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_02000796(s32 *arg0, struct StagedActor *arg1);
s32 Func_02001958(struct StagedActor *arg0, s32 *arg1);
void Func_02001940(struct StagedActor *arg0, s32 arg1);
void Func_02001930(s32 arg0);
void Func_02001a7e(s32 arg0);
void Func_02001976(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02001986(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02001994(struct StagedActor *arg0);
void Func_02001aa0(void);
void Func_020019ac(struct StagedActor *arg0, s32 arg1);
MovingActor *Func_0200090a(ScenePosition *, MovingActor *);
s32 Func_02001ada(MovingActor *, ScenePosition *);
s32 Func_02000da6(s32 *, s32 *, s32 *);
s32 Func_02001d14(u8 *, s32 *);
void Func_020026a8(void);
void Func_020014f4(void);
void Func_020022a8(void);
void Func_02002674();
void Func_020028f2(int, int, int);

void *StagedActorPairScene_SpawnPrimaryEffect(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *effect = Func_0200150e(kind, x, y, z);

    if (effect != NULL) {
        u8 *sprite = *(u8 **)(effect + 0x50);
        s32 sprite_flags;
        s32 flag_mask = 13;

        sprite_flags = sprite[9];
        flag_mask = -flag_mask;
        flag_mask &= sprite_flags;
        sprite[9] = flag_mask;
        effect[0x55] = 0;
        effect[0x59] = 8;
        Func_0200155c(effect, 0);
        Func_0200160c(effect, 14);
        Func_02001574(effect, 1);
        return effect;
    }
    return NULL;
}

void *StagedActorPairScene_SpawnSecondaryEffect(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *effect = Func_02001566(kind, x, y, z);

    if (effect != NULL) {
        u8 *sprite = *(u8 **)(effect + 0x50);
        s32 sprite_flags;
        s32 flag_mask = 13;

        sprite_flags = sprite[9];
        flag_mask = -flag_mask;
        flag_mask &= sprite_flags;
        flag_mask |= 4;
        sprite[9] = flag_mask;
        effect[0x55] = 0;
        effect[0x59] = 8;
        Func_020015b8(effect, 0);
        Func_02001668(effect, 15);
        effect[0x23] = (effect[0x23] & 0xfe) | 2;
        return effect;
    }
    return NULL;
}

void StagedActorPairScene_SpawnConfiguredEffect(
    s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz, u32 flags,
    const struct EffectOptions *options)
{
    u32 table_offset;
    struct SceneEffect *source_effect;
    u32 copied_bits;
    s32 flag_mask;
    u32 block_bits;
    struct SceneEffect *effect;
    struct EffectSprite *sprite;
    struct EffectSprite *mode_sprite;
    u32 option_bits;
    u16 *step_ptr;
    s32 duration;
    s32 first_delta;
    s32 accumulated;
    source_effect = Func_02001692(0);

    if ((flags & 0x100000) != 0 && options != 0) {
        effect = Func_02001638(options->kind, x, y, z);
    } else {
        effect = Func_02001638(222, x, y, z);
    }
    if (effect == 0) return;

    sprite = effect->sprite;
    mode_sprite = sprite;

    Func_02001642(effect, (flags + 1) & 15);
    table_offset = (flags & 15) << 2;
    Func_0200165c(effect, Data_02009778[table_offset >> 2]);

    effect->mode55 = 0;
    sprite->state = 0;
    effect->callback = 0x02008105;

    effect->velocity_x = vx;
    x = 3;
    effect->velocity_y = vy;
    effect->velocity_z = vz;

    copied_bits = source_effect->sprite->flags & 12;
    block_bits = *(volatile u8 *)&sprite->flags;
    flag_mask = ~12;
    sprite->flags = (u8)((block_bits & flag_mask) | copied_bits);

    effect->rate_x = 0;
    effect->rate_y = 0;
    effect->step64 = 0;
    step_ptr = &effect->step64;

    if ((flags & 0xffff0000) == 0 || options == 0) return;

    if ((flags & 0x10000) != 0) {
        Func_0200179e(effect, options->mode);
    }

    if ((flags & 0x20000) != 0) {
        effect->flags &= 0xfe;
        option_bits = *(const u8 *)options & x;
        sprite->flags = (u8)((*((const u8 *)mode_sprite + 9) & flag_mask)
                              | (option_bits << 2));
    }

    if ((flags & 0x80000) != 0) {
        effect->accum18 = options->accum18;
        effect->accum1c = options->accum1c;
    }

    if ((flags & 0x40000) != 0) {
        const struct EffectDescriptor *descriptor =
            Data_02009778[table_offset >> 2];
        s32 delta;

        if ((flags & 0x80000) != 0) {
            first_delta = *(volatile const s32 *)&options->target30;
            accumulated = *(volatile const s32 *)&effect->accum18;
            first_delta -= accumulated;
            effect->rate_x = Func_02001700(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta -= effect->accum1c;
        } else {
            first_delta = options->target30;
            first_delta += (s32)0xffff0000;
            effect->rate_x = Func_02001718(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta += (s32)0xffff0000;
        }

        effect->rate_y = Func_02001726(delta, duration);
    }

    if ((flags & 0x200000) != 0) {
        Func_0200175c(effect, 1);
        Func_0200176c(effect, options->callback_arg);
    }

    if ((flags & 0x400000) != 0) {
        sprite->angle = options->angle;
    }

    if ((flags & 0x800000) != 0) {
        *step_ptr = options->step;
    }

    if ((flags & 0x1000000) != 0) {
        effect->callback = options->callback;
    }
}

void StagedActorPairScene_AdvancePair(void)
{
    extern u32 Data_020096c0[];

    s32 destination[3];
    struct StagedActor *lead_actor;
    struct StagedActor *next_actor;
    struct StagedActor *blocking_actor;
    s32 facing_index;
    u32 step;
    s32 move_rate;
    s32 clear_value;

    lead_actor = GetStagedActor(0);
    facing_index = lead_actor->direction_and_kind >> 12;
    step = StagedActorStepTable[facing_index];
    destination[0] = lead_actor->x.value + (step & 0xffff0000);
    destination[1] = lead_actor->y;
    step <<= 16;
    destination[2] = lead_actor->z.value + step;
    next_actor = FindNextStagedActor(destination, lead_actor);
    if (next_actor == 0) return;

    step = StagedActorStepTable[facing_index];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    blocking_actor = FindBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    destination[0] = next_actor->x.value;
    destination[1] = next_actor->y + 0x100000;
    destination[2] = next_actor->z.value;
    blocking_actor = FindElevatedBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    next_actor->transition_mode = 2;
    step = StagedActorStepTable[facing_index];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    if (CanStartStagedActorMove(next_actor, destination) > 0) return;

    clear_value = next_actor->transition_busy;
    if (clear_value != 0) return;

    SetStagedActorMode(lead_actor, 8);
    move_rate = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next_actor->move_rate_x = move_rate;
    next_actor->move_rate_z = move_rate;
    StartNextStagedActorMove(next_actor, destination[0], destination[1], destination[2]);
    lead_actor->move_rate_x = move_rate;
    lead_actor->move_rate_z = move_rate;
    StartLeadStagedActorMove(lead_actor, destination[0], destination[1], destination[2]);
    FinishStagedActorMove(next_actor);
    FinishStagedActorEffect();
    next_actor->x.value = destination[0];
    next_actor->z.value = destination[2];
    next_actor->unknown_24 = clear_value;
    next_actor->unknown_2c = clear_value;
    lead_actor->unknown_38 = 0x80000000;
    lead_actor->unknown_40 = 0x80000000;
    lead_actor->unknown_24 = clear_value;
    lead_actor->unknown_2c = clear_value;
    lead_actor->x.value = lead_actor->x.parts.cell << 16;
    lead_actor->z.value = lead_actor->z.parts.cell << 16;
    SetStagedActorTransition(lead_actor, 1);
}

s32 StagedActorPairScene_StopBlockedMotion(MovingActor *actor)
{
    extern s32 Data_020096c0[];

    ScenePosition destination;
    u32 direction;
    s32 step;
    MovingActor *blocker;

    direction = actor->direction_and_kind >> 12;
    step = Data_020096c0[direction];
    destination.x = actor->x + (step & 0xffff0000);
    destination.y = actor->y;
    step = step << 16;
    destination.z = actor->z + step;
    blocker = Func_0200090a(&destination, actor);
    if (blocker != 0) {
        u32 allowed_index = 0;
        s32 blocker_kind = *blocker->sprite->kind;
        s32 *allowed_kinds = Data_02009700;

        do {
            if (blocker_kind == *allowed_kinds++) goto done;
            allowed_index++;
        } while (allowed_index <= 5);
        actor->move_delta_x = 0;
        actor->move_delta_z = 0;
        actor->move_target_x = 0x80000000;
        actor->move_target_z = 0x80000000;
    }
    step = Data_020096c0[direction];
    destination.x = actor->x + (step & 0xffff0000);
    destination.y = actor->y;
    step = step << 16;
    destination.z = actor->z + step;
    if (Func_02001ada(actor, &destination) > 0) {
        actor->move_delta_x = 0;
        actor->move_delta_z = 0;
        actor->move_target_x = 0x80000000;
        actor->move_target_z = 0x80000000;
    }
done:
    return 0;
}

s32 StagedActorPairScene_FindClearPosition(s32 *search)
{
    extern s32 Data_020096c0[];

    s32 direction;
    s32 origin[3];
    u8 *actor;
    s32 advance_count;
    s32 rows, columns;
    u8 *collision_mode;
    s32 bounds_offset;
    s32 extent_a, extent_b;
    u8 *bounds;
    s32 *candidate;
    s32 moved;
    search[5] = 0;
    actor = (u8 *)Func_02000da6(&direction, search + 1, search);
    if (actor == 0)
        return 0;
    collision_mode = actor + 0x22;
    *collision_mode = 2;
    advance_count = 0;
    bounds = (u8 *)Data_02009718;
    bounds_offset = search[0] << 4;
    {
        s32 entry_offset = bounds_offset + 4;
        extent_a = *(s32 *)(bounds + entry_offset);
        if (extent_a < 0)
            extent_a = -extent_a;
        entry_offset = bounds_offset;
        entry_offset += 12;
        extent_b = *(s32 *)(bounds + entry_offset);
        if (extent_b < 0)
            extent_b = -extent_b;
        rows = (extent_a + extent_b) >> 4;
        extent_a = *(s32 *)(bounds + bounds_offset);
        if (extent_a < 0)
            extent_a = -extent_a;
        entry_offset = bounds_offset;
        entry_offset += 8;
        extent_b = *(s32 *)(bounds + entry_offset);
        if (extent_b < 0)
            extent_b = -extent_b;
        columns = (extent_a + extent_b) >> 4;
    }
    {
        u8 *actor_bytes;
        s32 height;
        s32 step_x;
        candidate = origin;
        step_x = Data_020096c0[direction] & 0xffff0000;
        actor_bytes = actor;
        candidate[0] = *(s32 *)(actor_bytes + 8) + step_x;
        height = *(s32 *)(actor_bytes + 12);
        candidate[1] = height;
        candidate[2] = *(s32 *)(actor_bytes + 16) +
                       (Data_020096c0[direction] << 16);
        search[3] = height;
    }
    for (;;) {
        s32 row, column;
        {
            u8 *bounds_row = (u8 *)Data_02009718;
            s32 row_offset = search[0] << 4;
            row_offset += 4;
            search[4] = candidate[2] +
                        (*(s32 *)(bounds_row + row_offset) << 16);
        }
        row = 0;
        while (row < rows) {
            {
                u8 *bounds_column = (u8 *)Data_02009718;
                search[2] = candidate[0] +
                            (*(s32 *)(bounds_column + (search[0] << 4)) << 16);
            }
            column = 0;
            while (column < columns) {
                s32 *position = search + 2;
                if (Func_02001d14(actor, position) == 2)
                    goto found;
                position[0] = position[0] + 0x100000;
                column++;
            }
            search[4] = search[4] + 0x100000;
            row++;
        }
        advance_count++;
        origin[0] = origin[0] +
                    (Data_020096c0[direction] & 0xffff0000);
        origin[2] = origin[2] + (Data_020096c0[direction] << 16);
    }
found:
    *collision_mode = 0;
    moved = 0;
    if (advance_count != 0) {
        s32 step = Data_020096c0[direction];
        s32 step_x = step & 0xffff0000;
        s32 offset_x = step_x *advance_count;
        s32 offset_z = (step << 16) * advance_count;
        u8 *actor_bytes = actor;
        search[2] = *(s32 *)(actor_bytes + 8) + offset_x;
        search[3] = *(s32 *)(actor_bytes + 12);
        search[4] = *(s32 *)(actor_bytes + 16) + offset_z;
        moved = 1;
    }
    return moved;
}

s32 StagedActorPairScene_GetVariantData(void)
{
    s16 scene_id = Data_02000240[224];

    if (scene_id == (s32)&Value_00000098) {
        return (s32)Data_020097b4;
    }
    if (scene_id == (s32)&Value_0000009d) {
        return (s32)Data_020097fc;
    }
    if (scene_id == (s32)&Value_0000009e) {
        return (s32)Data_02009874;
    }
    return (s32)Data_02009784;
}

s32 StagedActorPairScene_GetInitialValue(void) { return 0; }

void StagedActorPairScene_RunStep(void)
{
    Func_020026a8();
}

void StagedActorPairScene_RunUpdate(void)
{
    Func_020014f4();
    Func_020022a8();
}

void StagedActorPairScene_RotateActorPart(u8 *actor)
{
    u8 *sprite_part = *(u8 **)(actor + 80);

    *(u16 *)(sprite_part + 30) -= 0x400;
}

void StagedActorPairScene_WaitForHeight(struct HeightTrackedObject *object,
                                       s32 limit)
{
    s32 frames = 40;

    while (frames != 0) {
        Func_02002674(1);
        frames--;
        if (object->height <= limit) {
            break;
        }
    }
}

void StagedActorPairScene_RunActorTwelveCommand(void)
{
    Func_020028f2(12, 0, 0);
}
