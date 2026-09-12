#include "types.h"
#include "scene.h"
#include "staged_actor_pair_scene.h"
#include "staged_actor.h"

/* overlays/scene/actor/staged_pair/staged_pair.c */
/* overlays/scene/actor/staged_pair/actor_presentation.c */

void *Actor_Run();

s32 *Actor_unk2_4();

s32 *Actor_unk3_4();

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Actor_unk6_3(actor, x, y);
}

void ActorDraw_RunActorElevenRecoveryScene(void)
{
    Actor_unk4_4();
    Actor_unk5_4(11, 0, 0);
    Actor_unk6_4(0, 11, 0);
    Actor_unk7_4(0, 1);
    Actor_unk8_4(10);
    Actor_unk9_4(0, 11, 0);
    Actor_unk10_4(0x23d9);
    Actor_unk11_4(11, 0);
    Actor_unk12_4(11, 2);
    {
        s16 *position = Actor_Run(0);

        if (position != 0)
            Actor_unk13_4(11, position[5], position[9]);
    }
    Actor_unk14_4(11);
    Actor_unk15_4(11, 0, 0);
    Actor_unk16_4(20);
    Actor_unk17_4(2464);
    Actor_unk18_4();
}

void ActorDraw_SelectActorNineScript(void)
{
    if (Actor_unk17(2384) != 0 && Actor_unk18(2415) == 0)
        Actor_unk19_4(9173);
    else
        Actor_unk20_4(9170);
    Actor_unk21_4(9, 0);
}

void ActorDraw_RunActorEightThresholdScene(void)
{
    Actor_unk22_4(8);
    Actor_unk23_4();
    {
        s32 *actor = Actor_unk2_4(8);

        if ((actor[2] >> 20) <= 30) {
            Actor_unk24_4(8);
            {
                s32 x = 27;
                s32 y = 19;

                Actor_unk25_4(29, 19, 1, 1, x, y);
            }
            Actor_unk26_4(2466);
        }
    }
    Actor_unk27_4();
}

void ActorDraw_RunActorNineThresholdScene(void)
{
    Actor_unk28_4();
    if ((Actor_unk3_4(9)[2] >> 20) > 42) {
        s32 x = 107;
        s32 y = 17;

        Actor_unk29_4(108, 17, 1, 1, x, y);
        Actor_unk30_4(8);
        Actor_unk31_4(9, 0, 0);
        PlaceActor(10, 45613056, 18874368);
        Actor_unk32_4(10, 3);
        Actor_unk33_4(154);
        Actor_unk34_4(2469);
    }
    Actor_unk35_4();
}

/* overlays/scene/actor/staged_pair/scene_primary_script.c */
extern u8 gVal[];
extern u8 gVal2[];
extern u8 gVal3[];
extern u8 gVal4[];
extern s16 gCell[];
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];
extern u8 gOv9[];
extern u8 gOv10[];
extern u8 gWork[];
extern s16 gCell2[][1];
extern u8 gVal5[];

s32 Scene_RunOpeningAuxiliarySequence(void)
{
    if (gCell2[224][0] == (s32)gVal2) {
        if (Actor_Check(0x96f) != 0) {
            return (s32)gOv3;
        }
        return (s32)gOv2;
    } else {
        if (gCell2[224][0] == (s32)gVal3) {
            return (s32)gOv4;
        } else {
            if (gCell2[224][0] == (s32)gVal4) {
                return (s32)gOv5;
            } else {
            }
        }
    }
    return (s32)gOv;
}

s32 Scene_RunScene3beSequenceA(void)
{
    if (gCell2[224][0] == (s32)gVal2) {
        if (Actor_unk2(0x96f) != 0) {
            return (s32)gOv8;
        }
        return (s32)gOv7;
    } else {
        if (gCell2[224][0] == (s32)gVal3) {
            return (s32)gOv9;
        } else {
            if (gCell2[224][0] == (s32)gVal4) {
                return (s32)gOv10;
            } else {
            }
        }
    }
    return (s32)gOv6;
}

void Scene_RunScene3beSequenceB(void)
{
    s32 record;

    if (Actor_unk3(0x98a) != 0) {
    } else {
        if (Actor_unk4(0x9a0) == 0) {
        } else {
            Actor_unk37_4();
            Actor_Place(11, 0x10000, 0x8000);
            record = Actor_unk5(0);
            if (record != 0) {
                Actor_unk38_4(11, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
            }
            Actor_unk2_3(11, -8, 16);
            Actor_unk39_4(11);
            Actor_unk3_3(11, 0xd000, 0);
            Actor_unk40_4(10);
            Actor_unk41_4(0, 11, 0);
            Actor_Do(0x23da);
            Actor_unk6(11, 0);
            if (Actor_unk7(0, 0) == 0) {
                Actor_unk42_4(11, 0);
                Actor_unk43_4(11, 152, 232);
                Actor_unk2_2(0x9a0);
                Actor_unk44_4(11);
                Actor_unk45_4(11, 1);
                gCell2[226][0] = (s32)gVal;
                gCell2[227][0] = 30;
            } else {
                bump_step(1);
                Actor_unk46_4(11, 0);
                Actor_unk47_4(11, 2);
                record = Actor_unk8(0);
                if (record != 0) {
                    Actor_unk48_4(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
                }
                Actor_unk49_4(11);
                Actor_unk50_4(11, 0, 0);
                Actor_unk51_4(30);
                Actor_unk52_4(0, 2);
                Actor_unk53_4(0, 0, 16);
                Actor_unk54_4(0);
                Actor_unk55_4(0, 1);
            }
            Actor_unk56_4();
        }
    }
}

void Scene_RunScene3be(void)
{
    u32 i;
    s32 record;
    s32 base5_23cc;

    base5_23cc = (s32)gVal5;
    Actor_unk57_4(base5_23cc);
    Actor_unk9(8, 0);
    if (Actor_unk10(0, 0) == 0) {
        if (Actor_unk11(0x950) != 0) {
            if (Actor_unk12(0x96f) == 0) {
                Actor_unk58_4((base5_23cc + 8));
            }
        }
        Actor_unk59_4(8, 0);
    } else {
        bump_step(1);
        Actor_unk60_4(8, 0);
    }
}

void Scene_RunScene3be(void)
{
    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(volatile s32 *)gWork;
    Actor_unk61_4();
    if (Actor_unk13(0x204) != 0) {
        Actor_unk3_2(0x9a3);
        Actor_unk4_2(0x9a5);
        Actor_unk5_2(0x9a4);
        Actor_unk6_2(0x9a6);
        Actor_unk7_2(0x9a5);
        Actor_unk8_2(0x9a4);
    }
    Actor_unk62_4(*(s16 *)(((s32)p5 + 0x16c)));
    Actor_unk63_4();
    Actor_unk64_4();
    Actor_unk65_4();
}

void Scene_RunScene3be(void)
{
    u32 i;
    s32 record;

    if (gCell2[224][0] == (s32)gVal2) {
        Actor_unk9_2(0x144);
        if (Actor_unk14(0x9a0) != 0) {
            Actor_unk66_4(11, 0, 0);
        }
    }
    if (gCell2[224][0] == (s32)gVal4) {
        if (gCell2[225][0] == 1) {
            Actor_SetRect(108, 17, 1, 1, 107, 17);
        }
        if (Actor_unk15(0x9a2) != 0) {
            Actor_unk4_3(8, 0x1b80000, 0x1340000);
            Actor_unk67_4(8, 2);
            Actor_unk2_5(29, 19, 1, 1, 27, 19);
        }
        if (Actor_unk16(0x9a5) != 0) {
            Actor_unk68_4(9, 0, 0);
            Actor_unk5_3(10, 0x2b80000, 0x1200000);
            Actor_unk69_4(10, 2);
        }
        record = Actor_unk19(12);
        Actor_unk70_4(record, 0);
    }
    return 0;
}

/* overlays/scene/actor/staged_pair/staged_actor_pair_scene.c */

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

extern struct EffectDescriptor *gOv11[];
extern s32 gOv12[];
extern s32 gOv13[];
extern u8 Value_00000098;
extern u8 Value_0000009d;
extern u8 Value_0000009e;
extern u8 gOv14[];
extern u8 gOv15[];
extern u8 gOv16[];
extern u8 gOv17[];

void *Actor_unk71_4(s32, s32, s32, s32);

void *Actor_unk72_4(s32, s32, s32, s32);

struct SceneEffect *Actor_unk73_4();
struct SceneEffect *Actor_unk74_4();

struct StagedActor *GetStagedActor(s32 arg0);
struct StagedActor *FindNextStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindElevatedBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);

MovingActor *Actor_unk75_4(ScenePosition *, MovingActor *);

void *StagedActorPairScene_SpawnPrimaryEffect(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *effect = Actor_unk71_4(kind, x, y, z);

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
        Actor_Apply(effect, 0);
        Actor_Apply2(effect, 14);
        Actor_Apply3(effect, 1);
        return effect;
    }
    return NULL;
}

void *StagedActorPairScene_SpawnSecondaryEffect(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *effect = Actor_unk72_4(kind, x, y, z);

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
        Actor_Apply4(effect, 0);
        Actor_Apply5(effect, 15);
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
    source_effect = Actor_unk73_4(0);

    if ((flags & 0x100000) != 0 && options != 0) {
        effect = Actor_unk74_4(options->kind, x, y, z);
    } else {
        effect = Actor_unk74_4(222, x, y, z);
    }
    if (effect == 0) return;

    sprite = effect->sprite;
    mode_sprite = sprite;

    Actor_unk76_4(effect, (flags + 1) & 15);
    table_offset = (flags & 15) << 2;
    Actor_unk77_4(effect, gOv11[table_offset >> 2]);

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
        Actor_unk78_4(effect, options->mode);
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
            gOv11[table_offset >> 2];
        s32 delta;

        if ((flags & 0x80000) != 0) {
            first_delta = *(volatile const s32 *)&options->target30;
            accumulated = *(volatile const s32 *)&effect->accum18;
            first_delta -= accumulated;
            effect->rate_x = Actor_unk20(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta -= effect->accum1c;
        } else {
            first_delta = options->target30;
            first_delta += (s32)0xffff0000;
            effect->rate_x = Actor_unk21(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta += (s32)0xffff0000;
        }

        effect->rate_y = Actor_unk22(delta, duration);
    }

    if ((flags & 0x200000) != 0) {
        Actor_unk79_4(effect, 1);
        Actor_unk80_4(effect, options->callback_arg);
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
    extern u32 StagedActorStepTable[];

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
    extern s32 StagedActorStepTable[];

    ScenePosition destination;
    u32 direction;
    s32 step;
    MovingActor *blocker;

    direction = actor->direction_and_kind >> 12;
    step = StagedActorStepTable[direction];
    destination.x = actor->x + (step & 0xffff0000);
    destination.y = actor->y;
    step = step << 16;
    destination.z = actor->z + step;
    blocker = Actor_unk75_4(&destination, actor);
    if (blocker != 0) {
        u32 allowed_index = 0;
        s32 blocker_kind = *blocker->sprite->kind;
        s32 *allowed_kinds = gOv12;

        do {
            if (blocker_kind == *allowed_kinds++) goto done;
            allowed_index++;
        } while (allowed_index <= 5);
        actor->move_delta_x = 0;
        actor->move_delta_z = 0;
        actor->move_target_x = 0x80000000;
        actor->move_target_z = 0x80000000;
    }
    step = StagedActorStepTable[direction];
    destination.x = actor->x + (step & 0xffff0000);
    destination.y = actor->y;
    step = step << 16;
    destination.z = actor->z + step;
    if (Actor_Apply6(actor, &destination) > 0) {
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
    extern s32 StagedActorStepTable[];

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
    actor = (u8 *)Actor_unk7_3(&direction, search + 1, search);
    if (actor == 0)
        return 0;
    collision_mode = actor + 0x22;
    *collision_mode = 2;
    advance_count = 0;
    bounds = (u8 *)gOv13;
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
        step_x = StagedActorStepTable[direction] & 0xffff0000;
        actor_bytes = actor;
        candidate[0] = *(s32 *)(actor_bytes + 8) + step_x;
        height = *(s32 *)(actor_bytes + 12);
        candidate[1] = height;
        candidate[2] = *(s32 *)(actor_bytes + 16) +
                       (StagedActorStepTable[direction] << 16);
        search[3] = height;
    }
    for (;;) {
        s32 row, column;
        {
            u8 *bounds_row = (u8 *)gOv13;
            s32 row_offset = search[0] << 4;
            row_offset += 4;
            search[4] = candidate[2] +
                        (*(s32 *)(bounds_row + row_offset) << 16);
        }
        row = 0;
        while (row < rows) {
            {
                u8 *bounds_column = (u8 *)gOv13;
                search[2] = candidate[0] +
                            (*(s32 *)(bounds_column + (search[0] << 4)) << 16);
            }
            column = 0;
            while (column < columns) {
                s32 *position = search + 2;
                if (Actor_Apply7(actor, position) == 2)
                    goto found;
                position[0] = position[0] + 0x100000;
                column++;
            }
            search[4] = search[4] + 0x100000;
            row++;
        }
        advance_count++;
        origin[0] = origin[0] +
                    (StagedActorStepTable[direction] & 0xffff0000);
        origin[2] = origin[2] + (StagedActorStepTable[direction] << 16);
    }
found:
    *collision_mode = 0;
    moved = 0;
    if (advance_count != 0) {
        s32 step = StagedActorStepTable[direction];
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
    s16 scene_id = gCell[224];

    if (scene_id == (s32)&Value_00000098) {
        return (s32)gOv14;
    }
    if (scene_id == (s32)&Value_0000009d) {
        return (s32)gOv15;
    }
    if (scene_id == (s32)&Value_0000009e) {
        return (s32)gOv16;
    }
    return (s32)gOv17;
}

s32 StagedActorPairScene_GetInitialValue(void) { return 0; }

void StagedActorPairScene_RunStep(void)
{
    Actor_unk81_4();
}

void StagedActorPairScene_RunUpdate(void)
{
    Actor_unk82_4();
    Actor_unk83_4();
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
        Actor_unk84_4(1);
        frames--;
        if (object->height <= limit) {
            break;
        }
    }
}

void StagedActorPairScene_RunActorTwelveCommand(void)
{
    Actor_unk8_3(12, 0, 0);
}

/* overlays/scene/actor/staged_pair/move_and_redraw.c */
#define Value Value2

#include "staged_actor_movement.h"

void Actor_RunMoveAndRedraw(StagedActorMovementRequest request)
{
#include "run_staged_actor_movement_and_redraw_body.inc"
}

/* overlays/scene/actor/staged_pair/get_message_data.c */
u8 *StagedActorPairScene_GetMessageData(void)
{
    return (u8 *)0x0200991c;
}

/* overlays/scene/actor/staged_pair/noop_actor_callback.c */
void StagedActorPairScene_NoopActorCallback(void){}

/* overlays/scene/actor/staged_pair/noop_scene_callback.c */
void StagedActorPairScene_NoopSceneCallback(void){}
