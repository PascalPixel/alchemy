/* MATCHING H5 (2026-09-27): complete 600/600 bytes and pools, zero edits.
 * Exact bridge siblings initialize the column before the countdown. Transfer
 * that order after H4: sched2 emits r5=29 before the r9 countdown copy, with
 * every other byte unchanged. No register declarations or widths changed.
 * H4 at 09cccd3d5 is the admitted 2-halfword parent. Adoption still requires
 * 30 fresh repeats, production comparison, coverage and verification.
 *
 * NONMATCHING H4 (2026-09-27): 600/600 bytes, 2 differing halfwords,
 * 2 aligned edits; instruction selection/topology and every pool byte exact.
 * Transfer the exact RetractBridge/ExtendBridge call-owned dimensions:
 * remove the draft-only pre-loop height local and pass literal 3,4 through
 * Map_CopyCellsTo. Baseline height SI39 lived 182 instructions while width
 * SI132 lived 170, with five references/seven calls each; width won fp.
 * The call-owned height now wins fp and both rectangle sites are exact.
 * Retraction independently rechecked 532/532 exact, no sibling edits.
 * Only countdown r9 copy versus initial column r5=29 order remains.
 * Preserve call-owned dimensions and all 600 bytes/pools in any follow-up.
 * Full normalized diff read; no function or alignment credit yet.
 *
 * NONMATCHING: 600/600 bytes, 7 differing halfwords (2026-09-26).
 * Whole owner 02001af0..02001d48, including pool 02001d14..02001d48;
 * all 25 calls audited against our listing. Closest exact family members
 * are VinasuHeya_RetractBridge and VinasuHeya_ExtendBridge. Retraction
 * rechecks at 532/532 bytes, zero differences. No missing calls found.
 * H1: transfer their existing Map_CopyCellsTo helper to both loop sites:
 * 596/600 bytes, 196 differing halfwords / 48 aligned edits -> 600/600,
 * 7 halfwords. All calls, frame, remaining instructions and pool match.
 * Residual: fp holds width 3 instead of height 4; two call sites copy the
 * wrong dimension and the initial countdown/column moves exchange order.
 * H2: share height with both x +=/-= steps: identical 600/600, 7 halfwords.
 * H3: first branch u16 width = 3, second branch literal 3, retaining the
 * shared height: identical 600/600, 7 halfwords. H1 retained below.
 * Existing allocator decoder reports no unique source repair. Three
 * structural trials exhausted; no declaration or generic RA sweep. This
 * remains not-yet-c and earns no DONE bytes. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_EFFECT.H"

struct ScrollLayer {
    u8 unknown_00[8];
    s32 x;
};

struct MapWork {
    u8 unknown_00[0x164];
    struct ScrollLayer layer;
};

extern struct MapWork *Data_03001e70;

void VinasuHeya_ShiftBridge(void)
{
    struct EffectOptions options;
    struct ScrollLayer *layer;
    struct FieldActor *leader;
    s32 x;
    s32 z;
    s32 countdown;
    u32 i;
    s32 dust_x;

    layer = &Data_03001e70->layer;
    leader = Engine_ActorGet(0);
    x = leader->x.part.pixel;
    z = leader->z.part.pixel;
    leader->y.fixed = 0;
    if (x >= 308 && x <= 315 && z >= 532 && z < 540) {
        leader->y.fixed = -0x20000;
        if (!GameFlag_IsSet(0x300)) {
            Engine_EventBegin();
            Engine_AudioPlayCue(161);
            GameFlag_Set(0x300);
            Engine_MapCopyCellsTo(26, 33, 19, 33, 1, 1);
            Engine_EventWait(30);
            Engine_AudioPlayCue(239);
            Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
            Engine_EventWait(20);
            dust_x = 0x1200000;
            x = 29;
            countdown = 40;
            for (i = 0; i <= 479; i++, countdown--) {
                layer->x += 0x3333;
                dust_x += -0x3333;
                options.priority = 2;
                options.start_scale_x = ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
                options.start_scale_y = ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
                options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
                Effect_Spawn(dust_x, 0, 0x2100000, 0, -(((gFrameCount & 1) * 3) << 16), 0, 0x8a0000, &options);
                if (i == 240) {
                    dust_x += -0x300000;
                }
                if (countdown == 0) {
                    countdown = 40;
                    if (i <= 240) {
                        x -= 4;
                        Map_CopyCellsTo(x, 50, 15, 32, 3, 4);
                    } else {
                        x += 4;
                        Map_CopyCellsTo(x, 45, 9, 32, 3, 4);
                    }
                }
                Engine_TaskWait(1);
            }
            layer->x += 0x8000;
            layer->x = layer->x / 0x10000 << 16;
            Engine_MapCopyCellAttributes(15, 32, 3, 1, 9, 32);
            Engine_MapCopyCellAttributes(12, 32, 3, 1, 15, 32);
            Engine_AudioPlayCue(288);
            Engine_AudioPlayCue(188);
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            Engine_MapWaitWorkValuesBelow256();
            gEventWork->start_transition = 0x202;
            Engine_EventRequestExit(11);
            Engine_EventEnd();
        }
    }
}
