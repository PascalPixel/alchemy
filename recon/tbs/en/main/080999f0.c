/* DRAFT of RunBattleEffect05: an orb rises from the source object to a point
   one step ahead, sheds particles there (random-angle triplets, or drifting
   fall objects for the alternate form), and returns.
   Not exact: 828 of 808 bytes. In place: the sprite's mode as a two-bit
   field, the step count as a variable (the reference ends its glide loops
   cmp #11; blt), named callbacks. Remaining: the reference multiplies the
   step by the scale rate on every pass and rebuilds the rate there, where
   the loop pass here turns it into a running sum; it keeps the end pointer
   in r11 (here spilled) and reaches the particle position through r10 for
   the offset and r6 after it. */
#include "TYPES.H"

struct Vec3 { s32 x, y, z; };

struct EffectTarget {
    u8 reserved_00[8];
    struct Vec3 position;
};

struct EffectSprite {
    u32 unknown_00[2];
    u8 unknown_08;
    u32 unknown_09_0 : 2;
    u32 mode : 2;               /* 0x09, bits 2-3 */
    u32 unknown_09_4 : 4;
};

struct EffectObject {
    u8 reserved_00[8];
    s32 x, y, z;
    s32 altitude;
    s32 scale_x, scale_y;
    u8 reserved_20[0x30];
    struct EffectSprite *sprite;    /* 0x50 */
    u8 reserved_54;
    u8 flag;                        /* 0x55 */
    u8 reserved_56[0x16];
    void *callback;                 /* 0x6c */
};

struct Effect05State {
    s32 angle;
    s32 x, y, z;
    struct EffectTarget *target;    /* 0x10 */
    s32 initialized;                /* 0x14 */
    u8 reserved_18[8];
    u8 high_arc;                    /* 0x20 */
    u8 reserved_21[0x13];
    u8 variant;                     /* 0x34 */
    u8 reserved_35[0x10];
    u8 alternate;                   /* 0x45 */
};

extern struct Effect05State *gEffectWork;
s32 __divsi3(s32, s32);
void WaitFrames(s32);
s32 Random16(void);
void Vector_AddPolarOffset(s32, s32, struct Vec3 *);
void Object_SetMode(struct EffectObject *, s32);
void ObjectDispatch_ReleaseFar(struct EffectObject *);
s32 Map_GetTerrainHeightFar(s32, s32, s32);
void Animation_ApplyChildValuesFar(struct EffectObject *, s32);
struct EffectObject *Object_Spawn(s32, s32, s32, s32);
void BattleEffect_InitializeSharedScene(void);
void BattleFx_PrepareBufferInterpolation(void);
void AudioCommand_PlayFar(s32);
void BattleFx_SpawnRandomAngleTriplet(void);
void BattleFx_UpdateDriftingFallObject(void);

static __inline__ s32 Interpolate(s32 start, s32 end, s32 step)
{
    return start + __divsi3(step * (end - start), 10);
}

void RunBattleEffect05(void)
{
    struct Vec3 spawn;
    struct Vec3 start;
    struct Vec3 end;
    struct Effect05State *state = gEffectWork;
    struct EffectTarget *target = state->target;
    struct EffectObject *main;
    struct Vec3 *from;
    struct Vec3 *to;
    struct Vec3 *at;
    s32 i = 0;
    s32 steps = 11;
    s32 grow = 0xc000;
    s32 shrink = -0xc000;
    s32 count;

    main = Object_Spawn(0xef, 0, 0, 0);
    if (main == 0)
        return;
    BattleEffect_InitializeSharedScene();
    AudioCommand_PlayFar(0x8a);
    if (state->initialized == 0) {
        state->x = target->position.x;
        state->z = target->position.z;
        Vector_AddPolarOffset(0x100000, state->angle, (struct Vec3 *)&state->x);
        state->y = Map_GetTerrainHeightFar(0, state->x, state->z);
    }
    from = &start;
    from->x = target->position.x;
    from->y = target->position.y + 0x100000;
    from->z = target->position.z;
    to = &end;
    to->x = state->x;
    to->y = state->y + 0x200000;
    to->z = state->z;
    if ((s8)state->variant != 0)
        to->y = state->y + 0x500000;

    for (; i < steps; i++) {
        s32 scale;

        main->x = Interpolate(start.x, end.x, i);
        main->y = Interpolate(start.y, end.y, i);
        main->z = Interpolate(start.z, end.z, i);
        scale = __divsi3(i * grow, 10) + 0x4000;
        main->scale_x = scale;
        main->scale_y = scale;
        WaitFrames(1);
    }
    WaitFrames(10);

    if ((s8)state->alternate == 0) {
        count = 10;
        if ((s8)state->high_arc == 0)
            count = 24;
        at = &spawn;
        for (i = 0; i < count; i++) {
            struct EffectObject *particle;

            spawn.x = main->x;
            spawn.y = main->y;
            spawn.z = main->z;
            Vector_AddPolarOffset(Random16() * 5 + 0x30000, Random16(), &spawn);
            if (i == count - 1) {
                WaitFrames(25);
                at->x = main->x;
                at->y = main->y;
                at->z = main->z;
            }
            particle = Object_Spawn(0xf0, at->x, at->y, at->z);
            if (particle != 0) {
                particle->altitude = at->y - 0x200000;
                particle->callback = BattleFx_SpawnRandomAngleTriplet;
                particle->flag = 2;
            }
            AudioCommand_PlayFar(0x84);
            WaitFrames(6);
        }
        WaitFrames(10);
    } else {
        count = 10;
        if ((s8)state->high_arc == 0)
            count = 30;
        for (i = count; i != 0; i--) {
            struct EffectObject *particle;

            spawn.x = main->x;
            spawn.y = main->y;
            spawn.z = main->z;
            Vector_AddPolarOffset(Random16() * 5 + 0x30000, Random16(), &spawn);
            particle = Object_Spawn(0x11c, spawn.x, spawn.y, spawn.z);
            if (particle != 0) {
                particle->callback = BattleFx_UpdateDriftingFallObject;
                particle->flag = 0;
                particle->sprite->mode = 2;
                Object_SetMode(particle, 8);
                Animation_ApplyChildValuesFar(particle, 7);
            }
            WaitFrames(6);
        }
        WaitFrames(70);
    }

    for (i = 0; i < steps; i++) {
        s32 scale;

        main->x = Interpolate(end.x, start.x, i);
        main->y = Interpolate(end.y, start.y, i);
        main->z = Interpolate(end.z, start.z, i);
        scale = __divsi3(i * shrink, 10) + 0x10000;
        main->scale_x = scale;
        main->scale_y = scale;
        WaitFrames(1);
    }
    ObjectDispatch_ReleaseFar(main);
    BattleFx_PrepareBufferInterpolation();
}
