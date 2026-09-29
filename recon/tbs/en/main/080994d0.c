/* Draft, not exact (2026-09-26): 416 of 424 bytes, 207 halfword differences.
   The explicit spawn loop keeps its scale literal inside the non-null
   branch, as the ROM does. Remaining: the two object aliases coalesce,
   the target/counter exchange r6/r7, and the flash mask stays in a low
   register instead of r8. The original retained draft was 424 / 186.
   2026-09-29 slice 4: 540 against 1215 (31 register-only, 1 operand, 1
   reordered, 3 inserted). The callees and callbacks now use the build's
   names, the spawn and flash counters step after each wait, and the
   particle's null test is taken once into alive before the 12-frame wait,
   as alchemy permute found (820 before the names); a second 10-minute
   search from here found nothing lower. Left: target, object and the
   counters take r7/r6/r5 in a different order, and this draft tests the
   object for null once more than the reference before that wait. */
#include "TYPES.H"

struct BattleEffect03Object {
    u8 reserved_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[4];
    s32 scale_x;
    s32 scale_y;
    u8 reserved_20[16];
    s32 velocity_x;
    s32 velocity_y;
    u8 reserved_38[24];
    void *visual;
    u8 reserved_54;
    u8 mode;
    u8 reserved_56[4];
    u8 unknown_5a;
    u8 reserved_5b[9];
    s16 angle;
    s16 phase;
    u8 reserved_68[4];
    void *callback;
};

struct BattleEffect03Link {
    u8 reserved_00[28];
    u8 marker;
};

struct BattleEffect03State {
    s32 reserved_00;
    s32 x;
    s32 y;
    s32 z;
    struct BattleEffect03Object *target;
    u8 reserved_14[12];
    s8 long_delay;
    u8 reserved_21[3];
    void (*finish_callback)(void);
};

extern struct BattleEffect03State *Data_03001f30;
void BattleFx_UpdateShrinkingOrbitObject(void);
void BattleFx_RunSparkEmitter(void);

void BattleEffect_InitializeSharedScene(void);
struct BattleEffect03Object *Object_Spawn(s32, s32, s32, s32);
struct BattleEffect03Link *Object_ReplaceResourceEntry(void *, struct BattleEffect03Link *);
void Func_080030f8(s32);
void Func_080f9010(s32);
void Animation_ApplyChildValuesFar(struct BattleEffect03Object *, s32);
void Motion_SetTargetPositionFromMagnitudeAngle(struct BattleEffect03Object *, s32, s32);
void Object_CommitPosition(struct BattleEffect03Object *);
void Func_080090d0(struct BattleEffect03Object *);
void Resource_ResetEntry(u8);
void BattleFx_PrepareBufferInterpolation(void);

void RunBattleEffect03(void)
{
    struct BattleEffect03State *state = Data_03001f30;
    struct BattleEffect03Object *target = state->target;
    struct BattleEffect03Object *object;
    struct BattleEffect03Object *particle;
    struct BattleEffect03Link *last;
    u8 link_marker;
    s32 spawn_index;
    s32 flash_index;
    s32 alive;

    BattleEffect_InitializeSharedScene();
    last = 0;
    spawn_index = 0;
Spawn:
    {
        object = Object_Spawn(
            0xe9, target->x, target->y + 0x200000, target->z);
        if (object != 0) {
            object->scale_y = 0xb333;
            object->scale_x = 0xb333;
            object->callback = BattleFx_UpdateShrinkingOrbitObject;
            object->angle = 0x78;
            object->phase = spawn_index << 13;
            object->mode = 4;
            last = Object_ReplaceResourceEntry(object->visual, last);
        }
        Func_080030f8(1);
        spawn_index++;
    }
    if (spawn_index <= 7)
        goto Spawn;

    link_marker = last->marker;
    Func_080f9010(0x82);
    Func_080030f8(110);
    object = Object_Spawn(0xe9, 0, 0, 0);
    particle = object;
    if (object != 0) {
        object->scale_y = 0xb333;
        object->scale_x = 0xb333;
        object->x = state->x;
        object->y = state->y + 0x100000;
        object->z = state->z;
        object->mode = 4;
        Animation_ApplyChildValuesFar(object, 7);
    }

    Func_080f9010(0x83);
    alive = particle != 0;
    Func_080030f8(12);
    if (object != 0) {
        flash_index = 0;
        do {
            if (flash_index & 3)
                Animation_ApplyChildValuesFar(particle, 9);
            else
                Animation_ApplyChildValuesFar(particle, 10);
            Func_080030f8(2);
            flash_index++;
        } while (flash_index <= 29);
    }

    Animation_ApplyChildValuesFar(particle, 0);
    Func_080f9010(0x54);
    if (alive) {
        object->callback = BattleFx_RunSparkEmitter;
        object->angle = 0;
        if (state->long_delay != 0)
            Func_080030f8(128);
        else
            Func_080030f8(192);
    }
    if (object != 0) {
        object->angle = -1;
        object->velocity_x = 0x50000;
        object->velocity_y = 0x6666;
        object->unknown_5a = 0;
        Motion_SetTargetPositionFromMagnitudeAngle(object, 0xc00000, 0xe800);
        Object_CommitPosition(object);
        Func_080090d0(object);
    }
    if (link_marker != 0x60)
        Resource_ResetEntry(link_marker);
    if (state->finish_callback != 0)
        state->finish_callback();
    BattleFx_PrepareBufferInterpolation();
}
