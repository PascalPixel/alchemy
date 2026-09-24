/* Draft, not exact (2026-09-24): candidate=1056 reference=1056
   differing_halfwords=329 but halfword_edits=58, binary similarity 89.0%.
   The pillar loop now has the reference's inductions (read from the -dL
   loop dump): the start frame st = 8i + 8 as its own variable (spilled at
   sp+12, with st + 1 and st + 3 derived from it), a = frame - (8i + 8)
   computed before the test so that it is reduced itself (r8), the cue
   frame st + 4 as a separate induction (sp+20), and the X-table pointer
   (sp+16). A u8 kind makes the loop large enough (loop.c's threshold) that
   a * 3 is no longer strength-reduced. Residual: the reference copies kind
   before the < 2 test (adds r3, r2, #0) and so is 2 bytes longer there,
   compensated elsewhere; total - 64 and total - 16 swap stack slots; the
   work->transfer_value store is scheduled before the blit47 load. */
#include "TYPES.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_EFX.H"
/* As in mode 6, the blitters return a value the effect ignores. */
typedef s32 (*DrawRectangleResult)(void *, const void *, s32, s32, s32, s32);
#define DrawRectangle DrawRectangleResult
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"

void WaitFrames(s32);
u32 Random16(void);
void Audio_PlayCue(s32);
void BattleFx_BeginCanvasLayer(s32);
void BattleEventRuntime_BeginPhaseFar(s32);
void ObjectGroup_UpdateMembers(s32, s32, s32, s32, s32);
void ObjectGroup_TickMemberTimers(void);
void Camera_ApplyShake(s32, s32);
void Runtime_ReleaseHeapBlock(s32);
s32 BattleFx_EndCanvasLayer(void);

extern u8 gWorkSlot[];
extern u8 Value_0000007e;

extern u8 BattleFxPillar_Kinds[];
extern s8 BattleFxPillar_X[];
extern u8 BattleFxPillar_Counts[];
extern s8 BattleFxPillar_PuffWidths[];
extern u8 BattleFxPillar_PuffHeights[];
extern u16 BattleFxPillar_PuffCells[];

struct PillarWork {
    u8 sheet[0x7780];
    s32 transfer_mode;
    s32 transfer_value;
    u8 unknown_7788[0x20];
    s32 shake;
    u8 unknown_77ac[0x78];
    s32 transfer_pending;
    struct BattleEffectArgument *effect;
};

#define PARTICLES ((struct EffectStep *)0x02010000)

void FunctionHead_080dd9c0(struct BattleEffectArgument *efx)
{
    void *dst;
    s32 frame;
    DrawRectangle blit47;
    DrawRectangle blit46;
    s32 total;
    u32 *cache;
    u32 *cursor;
    struct PillarWork *work;
    s32 i;
    s32 j;
    s32 a;
    s32 st;

    cache = (u32 *)(gWorkSlot + 39 * 4);
    cursor = cache;
    work = (struct PillarWork *)*cursor++;
    dst = (void *)*cursor;
    work->effect = efx;
    BattleFx_BeginCanvasLayer(1);
    *(u16 *)0x04000020 = 0x100;
    *(u16 *)0x04000050 = 0;
    *(u16 *)0x04000052 = 0x1010;
    Resource_LoadAndDecompress((s32)&Value_0000007e, work, 1, 1);
    if (work->effect->side == 1) {
        *(u32 *)0x04000028 = 0xffff9000;
    }
    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    blit46 = (DrawRectangle)cache[46 - 39];
    BattleEffect_LoadWork(47, 7, 7, 7, 1);
    work->transfer_mode = 1;
    work->transfer_value = 0;
    blit47 = (DrawRectangle)cache[47 - 39];
    Scheduler_AddOrUpdateCallback(0x080CD261, 0x480);
    total = BattleFxPillar_Counts[work->effect->variant] * 8 + 56;

    for (i = 0; i != 1024; i++) {
        PARTICLES[i].variant = -1;
    }

    for (frame = 0; frame != total; frame++) {
        if (frame == total - 64) {
            BattleEventRuntime_BeginPhaseFar(132);
        }
        if (frame >= total - 16) {
            *(u16 *)0x04000050 = 0x3f44;
            *(u16 *)0x04000052 = (total - frame - 1) | 0x1000;
        }

        for (i = 0; i != BattleFxPillar_Counts[work->effect->variant]; i++) {
            a = frame - (i * 8 + 8);
            st = i * 8 + 8;
            if (frame > st) {
                u8 kind = BattleFxPillar_Kinds[i];
                s32 h;
                s32 w;

                if (kind < 2) {
                    h = a * 16;
                    w = a * 6;
                    if (h > 80) {
                        h = 80;
                    }
                    if (w > 30) {
                        w = 30;
                    }
                    if (kind & 1) {
                        blit47(dst, work->sheet, BattleFxPillar_X[i] - w, 108 - h, 48, h);
                    } else {
                        blit46(dst, work->sheet, BattleFxPillar_X[i] + w, 108 - h, 48, h);
                    }
                } else {
                    h = a * 8;
                    w = a;
                    if (h > 64) {
                        h = 64;
                    }
                    if (a > 8) {
                        w = 8;
                    }
                    if (kind & 1) {
                        blit47(dst, work->sheet + 0xf00, BattleFxPillar_X[i] - w, 108 - h, 32, h);
                    } else {
                        blit46(dst, work->sheet + 0xf00, BattleFxPillar_X[i] + w, 108 - h, 32, h);
                    }
                }
                if (frame == st + 1) {
                    work->shake = 3;
                }
                if (frame < st + 3) {
                    s32 y = (Random16() & 31) + 72;
                    struct EffectStep *p;
                    for (j = 0; j != 64; j++) {
                        p = &PARTICLES[j];
                        if (p->variant == -1) {
                            p->x = BattleFxPillar_X[i] + (Random16() & 31) + 32;
                            if (p->x > 96) {
                                p->x = 96;
                            }
                            p->y = y;
                            p->variant = 0;
                            break;
                        }
                    }
                }
            }
            {
            for (j = 0; j != work->effect->count; j++) {
                if (frame == st + 4) {
                    Audio_PlayCue(132);
                    ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 3);
                }
            }
            }
        }

        for (i = 0; i != 64; i++) {
            u8 *hts = BattleFxPillar_PuffHeights;
            if (PARTICLES[i].variant >= 0) {
                s32 n = PARTICLES[i].variant / 2;
                blit46(dst, work->sheet + BattleFxPillar_PuffCells[n], PARTICLES[i].x - BattleFxPillar_PuffWidths[n],
                    PARTICLES[i].y - (s8)hts[n] / 2, BattleFxPillar_PuffWidths[n], (s8)hts[n]);
                blit47(dst, work->sheet + BattleFxPillar_PuffCells[n], PARTICLES[i].x,
                    PARTICLES[i].y - (s8)hts[n] / 2, BattleFxPillar_PuffWidths[n], (s8)hts[n]);
                if (++PARTICLES[i].variant == 14) {
                    PARTICLES[i].variant = -1;
                }
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
