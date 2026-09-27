/* NONMATCHING: 364 bytes, candidate 364, 60 differing halfwords, 39 aligned
 * halfword edits (2026-09-26). The halfword zero aggregate preserves the
 * complete extent; the Value mask keeps the full-width literal and result
 * in r3. Remaining: lamp and zero reload registers, the signed height
 * access index, and a second pool four bytes later than the reference.
 * An independent random-result local and scalar zero regress the pools.
 * 2026-09-27 H1: exact AERIE.C names the scene pointer Data_03001e70;
 * FIELD_EVENT.H owns gFrameCount. Replacing only the two literal-address
 * interfaces is byte-identical to baseline: 364/364, 60 halfwords/39 edits.
 * Full normalized diff unchanged, including signed height index, source
 * pointer reloads and late second pool. Retain the evidenced names, but
 * stop this named-global axis. Whole extent [020034bc,02003628) includes
 * both pools and terminal alignment; no exact bytes or gap credit.
 * H2: exact SceneEffect_UpdateArcPosition uses OverlayObject.angle_64,
 * linked_object and field_30, not actor semantics at those offsets. Give
 * creator and callback one union-backed record, replacing the parent store
 * through unknown_68 with the existing typed linked_object member. Calls,
 * stores and zero lifetime stay in place. Output again equals baseline:
 * 364/364 bytes, 60 halfwords/39 edits, all normalized hunks unchanged.
 * Retain the evidenced record views, stop both tested axes. No changes to
 * the exact callback or shared headers; scalar-zero/random-local stopped
 * axes remain closed. Further work must explain the null-test/copy use
 * of r1 and the zero's pre-angle-store r0 load, not permute declarations. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "OVERLAY_OBJECT.H"

extern u8 *Data_03001e70;

extern u8 Value_00000000;
extern u8 Value_0ffff000;

struct LampWork {
    s16 unused;
    s16 height;
};

struct Half {
    u16 value;
};

/* The creator uses actor fields; its exact arc callback uses this prefix. */
union ArcObject {
    struct FieldActor actor;
    struct OverlayObject arc;
};

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void Func_020034bc(void)
{
    struct FieldActor *lamp;
    struct FieldActor *actor;
    union ArcObject *spark;
    struct FieldSprite *sprite;
    struct LampWork *work;
    s32 height;
    s32 scale;
    s32 tick;
    struct Half zero;

    lamp = Engine_ActorGet(8);
    work = (struct LampWork *)(Data_03001e70 + 0xe8);
    height = ((u32)(Engine_RandomNext() * 48) >> 16) << 16;
    if (work->height <= 129) {
        if (gFrameCount & 1) {
            Engine_ActorSetPosition(8, 0x1300000, 0x900000);
            actor = Engine_ActorGet(8);
            /* FAKEMATCH: keep the scale load after the actor lookup. */
            do { scale = 0x10000; } while (0);
        } else {
            Engine_ActorSetPosition(8, 0x1300000, 0x970000);
            actor = Engine_ActorGet(8);
            scale = 0x14ccc;
        }
        actor->scale_x = scale;
        Engine_ActorGet(8)->scale_y = scale;
    } else {
        Call3(Engine_ActorSetPosition, 8, 0x80000, 0x80000);
    }
    if (lamp == NULL)
        return;
    tick = gFrameCount & 15;
    if (tick != 0)
        return;
    spark = (union ArcObject *)Engine_ObjectCreate(0x11c, lamp->x.fixed + 0x80000, lamp->y.fixed + height + 0x80000, lamp->z.fixed);
    height = Engine_MathDivide(height, 0x60000);
    height <<= 16;
    if (spark == NULL)
        return;
    sprite = spark->actor.sprite;
    Engine_ObjectSetScript(&spark->actor, (const s32 *)0x0200bc54);
    Engine_ObjectSetPalette(&spark->actor, 3);
    spark->actor.motion_flags = tick;
    spark->arc.angle_64 = (u32)&Value_0ffff000 & Engine_RandomNext();
    zero.value = (u16)(u32)&Value_00000000;
    spark->actor.unknown_66 = tick;
    spark->arc.linked_object = (struct OverlayObject *)lamp;
    spark->actor.update = (void (*)(union FieldObject *))0x0200b461;
    spark->arc.field_30 = (Engine_MathSin((height & 0xfffff) >> 4) * 24) >> 16;
    *((u8 *)sprite + 38) = zero.value;
    sprite->priority = lamp->sprite->priority;
}
