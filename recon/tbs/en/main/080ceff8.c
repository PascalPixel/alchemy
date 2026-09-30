#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFX.H"

/*
 * Battle-presentation sub-effect in the same 0x03001eec "battle work"
 * family as games/THE BROKEN SEAL/SRC/BATTLE/EFFECT/MEMBER_ORBIT.C (owner
 * 0x080ce85c, family-matcher template).  Unlike that sibling, this owner
 * takes a second argument (`mode`, tested against 0/1/2 throughout) and
 * runs a fixed 48-frame loop rather than one sized from a party-member
 * count, so it is a related but distinct sub-effect, not the same body.
 *
 * `object` is republished at work + 0x7828, exactly as in the template.
 * Field 0x24 (36) of that object is read directly as a single s16
 * (a target id), not as the start of a member-id array.
 *
 * `gWorkSlot[46]` and `gWorkSlot[47]` are the same
 * "gWorkSlot[kind] holds kind's block address" heap-allocation cache
 * documented in recon/tbs/en/main/080e7404.c: this owner registers
 * two rectangle-blit routines through BattleEffect_LoadWork(46, ...) and
 * BattleEffect_LoadWork(47, ...) and then reads the resulting callbacks back out
 * of that cache by kind, exactly as 080e7404.c's own
 * `draw_rectangle = (DrawRectangle) gWorkSlot[46];` does.
 *
 * Value_0000007b, Value_0000008d, and Value_00000068 follow the
 * Value_<addr> convention already established for Resource_GetTableEntry's
 * resource-id argument (see games/THE BROKEN SEAL/src/map/locations/heidia/prologue and
 * games/THE BROKEN SEAL/src/unidentified/overlays/state_update): every retained call
 * site loads the id through the literal pool rather than a movs
 * immediate, which an 8-bit integer constant cannot force but the
 * address of a link-time absolute byte symbol does.  Value_000000cc is
 * the same idiom applied to a direct BG2PA hardware-register write: 0xCC
 * fits an 8-bit movs immediate, yet the reference still routes it
 * through the literal pool.
 *
 * Both palette copies and the WordCopy call at 0x03001388 route through
 * the r6 slot of the _call_via_rN trampoline (recon/tbs/raw/080072e4.s),
 * exactly as in the template.  The two per-frame rectangle draws route
 * through the r4 slot of the same trampoline, using function pointers
 * staged from gWorkSlot[46]/[47] before the loop and spilled across
 * it (r8/r9/sl/fp are already occupied by work, &pos, mode, and the
 * table_a base respectively, so canvas and the two draw callbacks live
 * on the stack for the whole function).
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*WordCopy)(void *dest, const void *src, s32 words);

extern void *gWorkSlot[];
extern u8 Value_000000cc;

void BattleFx_BeginCanvasLayer(s32 mode);
void *Resource_GetTableEntry(s32 id);
u32 Resource_DecodeType01(const void *source, void *destination);
s32 Math_Div(s32 numerator, s32 denominator);
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 interval);
void Audio_PlayCue(s32 value);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void BattleEventRuntime_BeginPhaseFar(s32 value);
void Camera_ApplyShake(s32 a, s32 b);
void ObjectGroup_TickMemberTimers(void);
void WaitFrames(s32 frames);
void Scheduler_RemoveCallback(void *callback);
void Runtime_ReleaseHeapBlock(s32 id);
s32 BattleFx_EndCanvasLayer(void);
void EffectPosition_ApplyAlternateStepAndYOffset(
    s32 arg0, struct EffectPosition *position);

void BattleFx_RunFortyEightFrameEffect(void *object, s32 mode)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *canvas;
    void *palette;
    struct EffectPosition pos;
    DrawRectangle draw_a;
    DrawRectangle draw_b;
    u8 *table_a;
    s32 frame;
    s32 idx;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    M2C_FIELD(work, void **, 0x7828) = object;
    BattleFx_BeginCanvasLayer(0);

    palette = Resource_GetTableEntry((s32)&ResourceId_EarthWallSheet);
    ((WordCopy)0x03001388)((void *)0x05000000, palette, 128);
    Resource_DecodeType01((u8 *)palette + 128, work);
    palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    ((WordCopy)0x03001388)((void *)0x05000000, palette, 128);
    if (mode == 2) {
        palette = Resource_GetTableEntry((s32)&ResourceId_FireballSheet);
        ((WordCopy)0x03001388)((void *)0x05000000, palette, 128);
    }

    EffectPosition_ApplyAlternateStepAndYOffset(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36), &pos);
    if (mode == 0) {
        M2C_FIELD((void *)0x04000020, s16 *, 0) = 0x100;
        M2C_FIELD((void *)0x04000028, s32 *, 0) = (64 - pos.x) << 8;
    } else {
        M2C_FIELD((void *)0x04000020, s16 *, 0) = (s16)(s32)&Value_000000cc;
        M2C_FIELD((void *)0x04000028, s32 *, 0) =
            (Math_Div(-pos.x * 4, 5) + 64) << 8;
    }

    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw_a = (DrawRectangle)gWorkSlot[46];
    BattleEffect_LoadWork(47, 7, 7, 7, 2);
    draw_b = (DrawRectangle)gWorkSlot[47];

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 50;
    Scheduler_AddOrUpdateCallback((void *)0x080CD261, 0x480);

    if (mode == 2) {
        M2C_FIELD(work, s32 *, 0x77A8) = 0;
        Audio_PlayCue(212);
    } else if (mode == 1) {
        M2C_FIELD(work, s32 *, 0x77A8) = 8;
        Audio_PlayCue(212);
    } else {
        M2C_FIELD(work, s32 *, 0x77A8) = 32;
    }

    table_a = (u8 *)0x080EE09F;
    for (frame = 0; frame != 48; frame++) {
        if (frame == 0) {
            if (mode == 2) {
                ObjectGroup_UpdateMembers(
                    M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36),
                    7, -1, 0, 32);
            } else {
                ObjectGroup_UpdateMembers(
                    M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36),
                    10, -1, 0, 32);
            }
        }
        if (frame == 24) {
            BattleEventRuntime_BeginPhaseFar(0);
        }
        if (frame == 8 && mode == 0) {
            Audio_PlayCue(126);
        }
        if (frame <= 31) {
            idx = frame / 4;
            if (idx > 2) {
                idx = (idx & 1) + 1;
            }
            if (frame <= 27) {
                draw_a(canvas,
                    (u8 *)work + *(u16 *)((u8 *)0x080EE096 + idx * 2),
                    64 - *((u8 *)0x080EE09C + idx),
                    (pos.y - table_a[idx]) + 8,
                    *((u8 *)0x080EE09C + idx),
                    table_a[idx]);
                draw_b(canvas,
                    (u8 *)work + *(u16 *)((u8 *)0x080EE096 + idx * 2),
                    64,
                    (pos.y - table_a[idx]) + 8,
                    *((u8 *)0x080EE09C + idx),
                    table_a[idx]);
            }
        }
        if (mode == 0) {
            Camera_ApplyShake(2, 2);
        } else {
            Camera_ApplyShake(16, 16);
        }
        ObjectGroup_TickMemberTimers();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((void *)0x080CD261);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
