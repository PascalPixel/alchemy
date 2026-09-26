/* Draft, not exact (2026-09-26): 444 bytes, 122 differing halfwords.
   Typed positions and the canonical EffectSlot replace unrelated arrays
   and padded records. Value snapshots keep each interpolation origin over
   its divide call; the typed model alone gave 448 bytes / 174 halfwords.
   Remaining: step in r8 rather than r7, destination in r7 rather than r8,
   origin-load order, and strength-reduced scale instead of a multiply.
 */
#include "TYPES.H"
#include "EFFECT_0809B11C.H"
extern u8 Value_0000011d;

struct MotionPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct BattleEffectMotionObject {
    u8 reserved_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[4];
    s32 scale_x;
    s32 scale_y;
};

struct BattleEffectMotionState {
    s32 reserved_00;
    s32 x;
    s32 y;
    s32 z;
    struct BattleEffectMotionObject *target;
    u8 reserved_14[0x44];
    struct EffectSlot records[16];
};

extern struct BattleEffectMotionState *Data_03001f30;

struct BattleEffectMotionObject *Func_08096c80(s32, s32, s32, s32);
void Func_08009080(struct BattleEffectMotionObject *, s32);
void Func_08097384(void);
s32 Func_080022ec(s32, s32);
void Func_080030f8(s32);
void Func_080f9010(s32);
void Func_080974d8(s32 *);
u32 Func_08004458(void);
void Func_0800447c(s32, u32, s32 *);
void EffectSlot_Initialize(struct EffectSlot *, s32, s32, s32);
void Func_08009248(struct BattleEffectMotionObject *, s32);
void Func_080090d0(struct BattleEffectMotionObject *);
void Func_0809aa98(struct EffectSlot *);
void Func_0809748c(void);

static __inline__ s32 InterpolateCoordinate(s32 from, s32 to, s32 step)
{
    s32 delta;

    delta = to - from;
    return from + Func_080022ec(step * delta, 10);
}

void BattleEffect_RunFallbackObjectTransition(void)
{
    struct BattleEffectMotionState *state = Data_03001f30;
    struct BattleEffectMotionObject *target = state->target;
    struct BattleEffectMotionObject *object;
    struct EffectSlot *record;
    struct MotionPosition position;
    struct MotionPosition origin;
    struct MotionPosition destination;
    s32 step;
    s32 index;

    state->y = target->y;
    object = Func_08096c80(0xfa, 0, 0, 0);
    step = 0;
    Func_08009080(object, 0);
    if (object == 0)
        return;

    Func_08097384();
    origin.x = target->x;
    origin.y = target->y + 0x100000;
    origin.z = target->z;
    destination.x = state->x;
    destination.y = state->y + 0x80000;
    destination.z = state->z;
Interpolate:
    {
        s32 scale;

        object->x = InterpolateCoordinate(origin.x, destination.x, step);
        object->y = InterpolateCoordinate(origin.y, destination.y, step);
        object->z = InterpolateCoordinate(origin.z, destination.z, step);
        scale = Func_080022ec(step * 0xc000, 10) + 0x4000;
        object->scale_x = scale;
        object->scale_y = scale;
        step++;
        Func_080030f8(1);
    }
    if (step < 11)
        goto Interpolate;

    Func_080030f8(5);
    Func_08009080(object, 1);
    Func_080f9010(0x6c);
    Func_080030f8(10);
    Func_080f9010(0x6c);
    Func_080030f8(10);
    Func_080f9010(0x6c);
    Func_080030f8(10);
    Func_080f9010(0x6d);

    record = &state->records[0];
    index = 15;
    do {
        position.x = object->x;
        position.y = object->y + 0x80000;
        position.z = object->z;
        Func_080974d8(&position.x);
        Func_0800447c(0x40000, Func_08004458(), &position.x);
        EffectSlot_Initialize(record, (s32)&Value_0000011d, position.x, position.z);
        EffectSlot_SetCallback(record, Func_0809aa98);
        Func_08009248(record->object, 7);
        record++;
        index--;
    } while (index >= 0);

    position.x = object->x;
    position.y = object->y + 0x80000;
    position.z = object->z;
    Func_080030f8(8);
    Func_080090d0(object);
    Func_080030f8(4);
    Func_080030f8(30);
    Func_0809748c();
}
