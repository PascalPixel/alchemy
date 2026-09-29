/* Not-yet-C: complete 808-byte effect including its final pool.
 * Reused RunBattleEffect04's explicit default/override position stores;
 * this recovers the observed second Y store: 816 bytes / 161 aligned edits.
 * The ROM forms +0xc000 immediately but pool-loads -0xc000. Correcting those
 * two sources recovers the latter multiply but strength-reduces the former
 * loop into a second induction value: 824 bytes / 165 edits.
 * Initializing the first step before Object_Spawn and using it as the zero
 * X argument restores its early r8 lifetime and target in sl: 820 bytes,
 * 365 differing halfwords / 161 aligned edits, with the correct 44-byte
 * frame. Neither result is an exact match. Three bounded models stopped.
 * Remaining: start/end pointer spills, initial Y snapshot, first scale-loop
 * strength reduction, child-byte mask representation, and loop end tests.
 * The prior 808-byte baseline remains in Git. No new DONE credit. */
#include "TYPES.H"
extern u8 Value_0000c000;
extern u8 Value_ffff4000;
extern u8 Value_00004000;
extern u8 Value_0000011c;

struct Vec3 { s32 x, y, z; };
struct EffectTarget { u8 reserved_00[8]; struct Vec3 position; };
struct EffectObject {
    u8 reserved_00[8];
    s32 x, y, z;
    s32 altitude;
    s32 scale_x, scale_y;
    u8 reserved_20[0x30];
    u8 *child;
    u8 reserved_54;
    u8 mode;
    u8 reserved_56[0x16];
    void *callback;
};
struct Effect05State {
    s32 angle;
    s32 x, y, z;
    struct EffectTarget *target;
    s32 initialized;
    u8 reserved_18[8];
    s8 high_arc;
    u8 reserved_21[0x13];
    s8 variant;
    u8 reserved_35[0x10];
    s8 alternate;
};

extern struct Effect05State *Data_03001f30;
s32 Math_Div(s32, s32);
void WaitFrames(s32);
s32 Random16(void);
void Vector_AddPolarOffset(s32, s32, struct Vec3 *);
void Func_08009080(struct EffectObject *, s32);
void Func_080090d0(struct EffectObject *);
s32 Func_080091a8(s32, s32, s32);
void Animation_ApplyChildValuesFar(struct EffectObject *, s32);
struct EffectObject *Object_Spawn(s32, s32, s32, s32);
void BattleEffect_InitializeSharedScene(void);
void BattleFx_PrepareBufferInterpolation(void);
void Func_080f9010(s32);

static inline s32 Interpolate(s32 start, s32 end, s32 step)
{
    return start + Math_Div(step * (end - start), 10);
}

void Func_080999f0(void)
{
    struct Effect05State *state = Data_03001f30;
    struct EffectTarget *target = state->target;
    struct EffectObject *main;
    struct Vec3 spawn;
    struct Vec3 start;
    struct Vec3 end;
    s32 i;
    s32 count;

    /* FAKEMATCH: the first step also supplies the spawn's zero X. */
    i = 0;
    main = Object_Spawn(0xef, i, 0, 0);
    if (main == 0)
        return;
    BattleEffect_InitializeSharedScene();
    Func_080f9010(0x8a);
    if (state->initialized == 0) {
        state->x = target->position.x;
        state->z = target->position.z;
        Vector_AddPolarOffset(0x100000, state->angle, (struct Vec3 *)&state->x);
        state->y = Func_080091a8(0, state->x, state->z);
    }
    start.x = target->position.x;
    start.y = target->position.y + 0x100000;
    start.z = target->position.z;
    end.x = state->x;
    end.y = state->y + 0x200000;
    end.z = state->z;
    if (state->variant != 0)
        end.y = state->y + 0x500000;

    for (; i < 11; i++) {
        s32 scale;
        main->x = Interpolate(start.x, end.x, i);
        main->y = Interpolate(start.y, end.y, i);
        main->z = Interpolate(start.z, end.z, i);
        scale = Math_Div(i * 0xc000, 10) + 0x4000;
        main->scale_x = scale;
        main->scale_y = scale;
        WaitFrames(1);
    }
    WaitFrames(10);

    if (state->alternate == 0) {
        count = state->high_arc ? 10 : 24;
        for (i = 0; i < count; i++) {
            struct EffectObject *particle;
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
                particle->callback = (void *)0x08099921;
                particle->mode = 2;
            }
            Func_080f9010(0x84);
            WaitFrames(6);
        }
        WaitFrames(10);
    } else {
        count = state->high_arc ? 10 : 30;
        for (i = count; i != 0; i--) {
            struct EffectObject *particle;
            spawn.x = main->x;
            spawn.y = main->y;
            spawn.z = main->z;
            Vector_AddPolarOffset(Random16() * 5 + 0x30000, Random16(), &spawn);
            particle = Object_Spawn(0x11c, spawn.x, spawn.y, spawn.z);
            if (particle != 0) {
                particle->callback = (void *)0x080999a9;
                particle->mode = 0;
                particle->child[9] = (particle->child[9] & ~12) | 8;
                Func_08009080(particle, 8);
                Animation_ApplyChildValuesFar(particle, 7);
            }
            WaitFrames(6);
        }
        WaitFrames(70);
    }

    for (i = 0; i < 11; i++) {
        s32 scale;
        main->x = Interpolate(end.x, start.x, i);
        main->y = Interpolate(end.y, start.y, i);
        main->z = Interpolate(end.z, start.z, i);
        scale = Math_Div(i * (s32)&Value_ffff4000, 10) + 0x10000;
        main->scale_x = scale;
        main->scale_y = scale;
        WaitFrames(1);
    }
    Func_080090d0(main);
    BattleFx_PrepareBufferInterpolation();
}
