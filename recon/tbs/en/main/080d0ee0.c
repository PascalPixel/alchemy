/* Draft, not exact (2026-09-24): candidate=1172 reference=1136,
   differing_halfwords=429, binary similarity 52.4% (the previous draft
   scored 37.5%). Three projectiles leave the caster twelve frames apart
   and fly to the first target over 60 frames; each draws a ten-point
   spinning ring joined by dotted segments and knocks the target back on
   arrival. Residual: the reference frame is 116 bytes (this one 120); its
   slots run work 76, dst 72, frame 68, an unreferenced slot at 64 (the
   same gap mode 12 keeps between frame and blit46), blit46 60, k 56,
   aux 52, transfer work 48, peak 44. The ring and segment loops still
   differ in induction variables. */
#include "TYPES.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_EFX.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "FIXED_MATH.H"

void WaitFrames(s32);
void Audio_PlayCue(s32);
void BattleFx_BeginCanvasLayer(s32);
void *Resource_GetTableEntry(s32);
void Resource_DecodeType01(void *, void *);
void **GetBattleObjectSlotFar(s32);
void Object_SetMode(void *, s32);
void ObjectDispatch_ApplyValueToChildrenFar(void *, s32);
void Object_SetPosition(void *, s32, s32, s32);
void Object_ResetMotion(void *);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(void *, void *);
void Graphics_SaveTransferWorkOnce(void);
void Graphics_RestoreTransferWork(void);
void SceneTransform_ApplyPosition(void *);
void SceneTransform_ApplyRoll(s32);
void SceneTransform_ApplyYaw(s32);
void SceneTransform_ApplyScale(s32 *);
void BattleEventRuntime_BeginPhaseFar(s32);
void ObjectGroup_UpdateMembers(s32, s32, s32, s32, s32);
void ObjectGroup_TickMemberTimers(void);
void Camera_ApplyShake(s32, s32);
void Runtime_ReleaseHeapBlock(s32);
s32 BattleFx_EndCanvasLayer(void);

typedef s32 (*WordCopyFn)(void *dest, const void *src, s32 words);

extern u8 gWorkSlot[];
extern u8 Value_00000073;
extern u8 Value_00000079;
extern s32 BattleFxBolt_RingOffsets[][3];
extern u16 BattleFx12_DotCells[];

struct BoltObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0x14];
    s32 unknown_28;
    u8 unknown_2c[4];
    s32 unknown_30;
    s32 unknown_34;
    u8 unknown_38[0x10];
    s32 unknown_48;
    u8 unknown_4c[0xe];
    u8 unknown_5a;
};

struct BoltTransfer {
    u8 unknown_00[0x36];
    u16 scroll;
};

struct BoltWork {
    u8 sheet[0x7080];
    struct EffectStep bolts[64];
    s32 transfer_mode;
    s32 transfer_value;
    u8 unknown_7788[0x20];
    s32 shake;
    u8 unknown_77ac[0x78];
    s32 transfer_pending;
    struct BattleEffectArgument *effect;
};

#define TRAIL ((struct EffectStep *)0x02010000)

void Func_080d0ee0(struct BattleEffectArgument *efx)
{
    s32 out[3];
    s32 base[3];
    s32 sc[3];
    struct BoltWork *work;
    void *dst;
    s32 frame;
    DrawRectangle blit46;
    s32 k;
    u8 *aux;
    struct BoltTransfer *tw;
    struct BoltObject *caster;
    struct BoltObject *src;
    struct BoltObject *target;
    struct EffectStep *q;
    s32 m;
    s32 s;
    s32 size;

    work = *(struct BoltWork **)(gWorkSlot + 39 * 4);
    dst = *(void **)(gWorkSlot + 40 * 4);
    aux = *(u8 **)(gWorkSlot + 41 * 4);
    tw = *(struct BoltTransfer **)(gWorkSlot + 12 * 4);
    caster = *GetBattleObjectSlotFar(efx->actor);
    work->effect = efx;
    BattleFx_BeginCanvasLayer(1);
    ((WordCopyFn)0x03001388)((void *)0x05000000, Resource_GetTableEntry((s32)&Value_00000079), 128);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&Value_00000073), aux);
    Object_SetMode(caster, 2);
    ObjectDispatch_ApplyValueToChildrenFar(caster, 48);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    blit46 = *(DrawRectangle *)(gWorkSlot + 46 * 4);
    Scheduler_AddOrUpdateCallback(0x080CD261, 0x480);
    src = *GetBattleObjectSlotFar(work->effect->actor);
    target = *GetBattleObjectSlotFar(work->effect->actors[0]);

    for (k = 0; k != 3; k++) {
        q = &work->bolts[k];
        q->x = src->x;
        q->y = src->y + 0x280000;
        q->z = src->z;
        if (k == 0) {
            q->velocity_x = Math_Div(target->x - q->x, 12);
        } else {
            q->velocity_x = Math_Div(target->x * 2 - q->x, 12);
        }
        q->velocity_y = Math_Div(target->y - q->y + 0x280000, 12);
        q->velocity_z = Math_Div(target->z - q->z, 12);
        q->variant = 0;
    }

    for (frame = 0; frame != 60; frame++) {
        if (frame <= 47) {
            struct BoltTransfer *t = *(struct BoltTransfer **)(gWorkSlot + 12 * 4);
            s32 d = 128;
            if (frame > 39) {
                d = 0x300 - frame * 16;
            }
            if (work->effect->side == 0) {
                t->scroll -= d;
            } else {
                t->scroll += d;
            }
        }
        for (k = 0; k != 3; k++) {
            s32 age;
            s32 peak;
            s32 grow;
            struct EffectStep *trail;
            if (frame < k * 12) {
                continue;
            }
            q = &work->bolts[k];
            age = frame - k * 12;
            size = age / 4 + 2;
            if (size > 10) {
                size = 10;
            }
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork(tw, (u8 *)tw + 12);
            SceneTransform_ApplyPosition(q);
            peak = 0;
            grow = (age << 12) + 0x1000;
            trail = &TRAIL[k * 10];
            for (m = 0; m != 10; m++, trail++) {
                s32 r;
                Graphics_SaveTransferWorkOnce();
                SceneTransform_ApplyRoll(age << 10);
                SceneTransform_ApplyYaw(0x4000);
                sc[0] = grow;
                if (grow > 0x10000) {
                    sc[0] = 0x10000;
                }
                sc[1] = sc[0];
                sc[2] = sc[0];
                SceneTransform_ApplyScale(sc);
                SceneTransform_ApplyRoll(m * 0x199a);
                r = EffectPosition_ApplyBaseAndYOffset((s32)BattleFxBolt_RingOffsets[m & 1], (struct EffectPosition *)out);
                if (peak < r) {
                    peak = r;
                }
                out[0] >>= 1;
                trail->velocity_x = out[0] + base[0];
                trail->velocity_y = out[1] + base[1];
                trail->velocity_x = out[0];
                trail->velocity_y = out[1];
                Graphics_RestoreTransferWork();
            }
            if (peak <= 399999) {
                for (m = 0; m != 10;) {
                    struct EffectStep *a = &TRAIL[k * 10 + m];
                    struct EffectStep *b;
                    m++;
                    b = &TRAIL[k * 10 + Math_Mod(m, 10)];
                    for (s = 0; s != 16; s++) {
                        s32 x = a->velocity_x + (b->velocity_x - a->velocity_x) * s / 16;
                        s32 y = a->velocity_y + (b->velocity_y - a->velocity_y) * s / 16;
                        blit46(dst, aux + BattleFx12_DotCells[size - 1], x - size / 2, y - size, size, size * 2);
                    }
                }
            }
            q->x += q->velocity_x;
            q->y += q->velocity_y;
            q->z += q->velocity_z;
            if (frame == k * 12 + k + 10) {
                target->unknown_34 = 0x20000;
                target->unknown_30 = 0x80000;
                target->unknown_28 = 0x50000;
                target->unknown_48 = 0xab85;
                target->unknown_5a = 0;
                Object_ResetMotion(target);
                if (target->x < 0) {
                    Object_SetPosition(target, target->x - 0x280000, 0, target->z);
                } else {
                    Object_SetPosition(target, target->x + 0x280000, 0, target->z);
                }
                if (k == 2) {
                    BattleEventRuntime_BeginPhaseFar(134);
                } else {
                    Audio_PlayCue(134);
                    ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
                }
                work->shake = 4;
            }
        }
        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback(0x080CD261);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
