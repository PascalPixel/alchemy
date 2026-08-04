#include "types.h"

struct EffectChild_0809a484 {
    u8 reserved00[9];
    u8 flags;
    u8 reserved0a[0x14];
    u16 value1e;
    u8 reserved20[6];
    s8 mode;
};

struct EffectObject_0809a484 {
    u8 reserved00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 reserved14[4];
    s32 secondaryX;
    s32 secondaryY;
    u8 reserved20[3];
    u8 flags23;
    u8 reserved24[0x0c];
    s32 secondaryStepX;
    s32 secondaryStepY;
    u8 reserved38[0x0c];
    s32 stepX;
    s32 stepY;
    s32 stepZ;
    struct EffectChild_0809a484 *child;
    u8 reserved54;
    s8 mode55;
    u8 reserved56[0x0e];
    u16 childStep;
    u8 reserved66[6];
    void (*callback)(void *);
};

struct MotionTemplate_0809a484 {
    u8 reserved00[0x0c];
    s32 frames;
};

struct EffectOptions_0809a484 {
    u8 palette;
    u8 reserved01[3];
    s32 dispatchValue;
    s32 secondaryX;
    s32 secondaryY;
    s32 targetX;
    s32 targetY;
    s16 objectKind;
    u8 reserved1a[2];
    const void *animation;
    u16 childValue;
    u16 childStep;
    void (*callback)(void *);
};

struct EffectObject_0809a484 *Func_080090c8(s16 kind, s32 x, s32 y, s32 z);
void Func_08009080(struct EffectObject_0809a484 *object, s32 mode);
void Func_08009098(struct EffectObject_0809a484 *object, const void *animation);
struct EffectObject_0809a484 *Func_08092054(u32 objectId);
void Func_080929d8(struct EffectObject_0809a484 *object, s32 value);
s32 Func_080022ec(s32 value, s32 divisor);
void Func_0809a44c(void *object);

/*
 * Create a configurable moving effect.  The low flag nibble selects its
 * motion template; high flag bits selectively override its source, palette,
 * secondary trajectory, child fields, animation, and update callback.
 */
void Func_0809a484(
    s32 x,
    s32 y,
    s32 z,
    s32 stepX,
    s32 stepY,
    s32 stepZ,
    u32 flags,
    const struct EffectOptions_0809a484 *options)
{
    struct MotionTemplate_0809a484 *const *templates =
        (struct MotionTemplate_0809a484 *const *)0x080a012c;
    struct EffectObject_0809a484 *reference = Func_08092054(0);
    struct EffectObject_0809a484 *object;
    struct EffectChild_0809a484 *child;
    struct MotionTemplate_0809a484 *motion;
    s16 kind = 0xde;
    u32 templateIndex = flags & 0xf;

    if ((flags & 0x100000) != 0 && options != NULL)
        kind = options->objectKind;

    object = Func_080090c8(kind, x, y, z);
    if (object == NULL)
        return;

    child = object->child;
    motion = templates[templateIndex];
    Func_08009080(object, (flags + 1) & 0xf);
    Func_08009098(object, motion);
    object->mode55 = 0;
    child->mode = 0;
    object->callback = Func_0809a44c;
    object->stepX = stepX;
    object->stepY = stepY;
    object->stepZ = stepZ;
    child->flags =
        (child->flags & (u8)~0x0c) | (reference->child->flags & 0x0c);
    object->secondaryStepX = 0;
    object->secondaryStepY = 0;
    object->childStep = 0;

    if ((flags & 0xffff0000) == 0 || options == NULL)
        return;

    if ((flags & 0x10000) != 0)
        Func_080929d8(object, options->dispatchValue);

    if ((flags & 0x20000) != 0) {
        object->flags23 &= 0xfe;
        child->flags =
            (child->flags & (u8)~0x0c) | ((options->palette & 3) << 2);
    }

    if ((flags & 0x80000) != 0) {
        object->secondaryX = options->secondaryX;
        object->secondaryY = options->secondaryY;
    }

    if ((flags & 0x40000) != 0) {
        s32 targetY;

        if ((flags & 0x80000) != 0) {
            object->secondaryStepX = Func_080022ec(
                options->targetX - object->secondaryX, motion->frames);
            targetY = options->targetY - object->secondaryY;
        } else {
            object->secondaryStepX = Func_080022ec(
                options->targetX - 0x10000, motion->frames);
            targetY = options->targetY - 0x10000;
        }
        object->secondaryStepY = Func_080022ec(targetY, motion->frames);
    }

    if ((flags & 0x200000) != 0) {
        Func_08009080(object, 1);
        Func_08009098(object, options->animation);
    }
    if ((flags & 0x400000) != 0)
        child->value1e = options->childValue;
    if ((flags & 0x800000) != 0)
        object->childStep = options->childStep;
    if ((flags & 0x01000000) != 0)
        object->callback = options->callback;
}
