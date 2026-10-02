/* DRAFT of RunBattleEffect05: an orb rises from the source object to a point
   one step ahead, sheds particles there (random-angle triplets, or drifting
   fall objects for the alternate form), and returns.
   Not exact: 816 of 808 bytes. In place: the sprite's mode as a two-bit
   field, the step count as a variable (the reference ends its glide loops
   cmp #11; blt), the scale through the same interpolation as the position
   (so the step is multiplied on every pass, as in the reference), the end
   pointer in r11. Remaining: the reference's reload registers rotate over
   r0 to r3 from the first statement (its zero for the step goes through
   r1); here r1 never becomes a reload register, so most of the 213
   differing lines are r0/r1/r2 choices. */
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
    return start + step * (end - start) / 10;
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
    s32 i;
    s32 steps = 11;
    s32 count;
    s32 y;
    struct EffectObject *particle;

    main = Object_Spawn(0xef, i = 0, 0, 0);
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
    y = state->y;
    to->y = y + 0x200000;
    to->z = state->z;
    if ((s8)state->variant != 0)
        to->y = y + 0x500000;

    for (; i < steps; i++) {
        s32 scale;

        main->x = Interpolate(start.x, end.x, i);
        main->y = Interpolate(start.y, end.y, i);
        main->z = Interpolate(start.z, end.z, i);
        scale = Interpolate(0x4000, 0x10000, i);
        main->scale_x = scale;
        main->scale_y = scale;
        WaitFrames(1);
    }
    WaitFrames(10);

    if ((s8)state->alternate == 0) {
        count = 10;
        if ((s8)state->high_arc == 0)
            count = 24;
        for (i = 0; i < count; i++) {
            spawn.x = main->x;
            spawn.y = main->y;
            spawn.z = main->z;
            Vector_AddPolarOffset(Random16() * 5 + 0x30000, Random16(), &spawn);
            if (i == count - 1) {
                WaitFrames(25);
                spawn.x = main->x;
                spawn.y = main->y;
                spawn.z = main->z;
            }
            particle = Object_Spawn(0xf0, spawn.x, spawn.y, spawn.z);
            if (particle != 0) {
                particle->altitude = spawn.y - 0x200000;
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

    i = 0;
    do {
        s32 scale;

        main->x = Interpolate(end.x, start.x, i);
        main->y = Interpolate(end.y, start.y, i);
        main->z = Interpolate(end.z, start.z, i);
        scale = Interpolate(0x10000, 0x4000, i);
        main->scale_x = scale;
        main->scale_y = scale;
        WaitFrames(1);
        i++;
    } while (i < steps);
    ObjectDispatch_ReleaseFar(main);
    BattleFx_PrepareBufferInterpolation();
}
