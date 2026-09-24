#include "TYPES.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_EFX.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"

/* Six drawn arguments: destination, source cell, x, y, width, height.
   The reference calls it through the r4 bx bank, so it is an indirect
   call through the cached kind-46 entry rather than a fixed callee. */

/* Heap-allocation cache: gWorkSlot[kind] holds kind's block address.
   This owner reads kinds 39 (its work block), 40 and 46. */
extern u8 gWorkSlot[];

/* Value_ symbols carry a literal the reference loads from its pool rather
   than materializing with a mov. */
extern u8 Value_000000b4;

void BattleFx_BeginCanvasLayer(s32);
s32 Trig_Sin(s32);
s32 Trig_Cos(s32);
void Audio_PlayCue(s32);
void BattleEventRuntime_BeginPhaseFar(s32);
void ObjectGroup_UpdateMembers(s32, s32, s32, s32, s32);
void ObjectGroup_TickMemberTimers(void);
void Runtime_ReleaseHeapBlock(s32);
s32 BattleFx_EndCanvasLayer(void);

/* Six animation cells, one entry each: width, height, vertical bias, and the
   byte offset of the cell inside the kind-39 work block.  The three byte
   tables sit six apart and every cell's width * height matches its source
   span, so the cells are 8bpp and the widths are also the source pitch. */
extern u8 Data_080ede9f[];
extern u8 Data_080edea5[];
extern u8 Data_080edeab[];
extern u16 Data_080edeb2[];

#define CELL_W Data_080ede9f
#define CELL_H Data_080edea5
#define CELL_DY Data_080edeab
#define CELL_SRC Data_080edeb2

/* One 28-byte record; the array starts at work + 0x7080. */
typedef struct Puff {
    s32 x;
    s32 y;
    s32 unk08;
    s32 unk0C;
    s32 unk10;
    s32 unk14;
    s32 tick;
} Puff;

#define WORK_EFX ((struct BattleEffectArgument *)work->effect)

/*
 * Effect sequence at 0x080d9fc8.
 *
 * Nine puffs are laid out along one sine/cosine arc across the top of the
 * screen, then animated for eighty frames.  Each puff plays a six-cell
 * sequence, four frames per cell, staggered four frames apart, and is drawn
 * once, twice or three times depending on the layer count the caller set.
 * The rectangle blitter is the kind-46 block BattleEffect_LoadWork loads, so every
 * draw goes through the entry cached in gWorkSlot[46].
 *
 * Aggregate names remain provisional; byte offsets into the kind-39 work
 * block are retained where no evidence-backed structure exists yet.
 */
void BattleFx_RunPuffArc(struct BattleEffectArgument *efx)
{
    u32 *cache;
    u32 *entry;
    struct BattleEffectWork *work;
    void *dst;
    s32 frame;
    DrawRectangle draw;
    s32 i;
    s32 cell;
    u32 wide;
    s32 sign;
    s32 org;
    s32 ang;
    s32 tick;
    Puff *puff;
    Puff *cur;

    cache = (u32 *)(gWorkSlot + 39 * 4);
    entry = cache;
    work = (struct BattleEffectWork *)*entry++;
    dst = (void *)*entry;
    work->effect = efx;
    BattleFx_BeginCanvasLayer(0);
    *(s16 *)0x04000050 = 0x3F46;
    *(s16 *)0x04000052 = 0x100E;
    Resource_LoadAndDecompress((s32)&Value_000000b4, work, 1, 1);
    BattleEffect_LoadWork(46, 7, 7, 3, 3);
    draw = (DrawRectangle)cache[46 - 39];
    if (WORK_EFX->actors[0] > 127) {
        org = 0;
        sign = 1;
    } else {
        org = 64;
        sign = -1;
    }
    ang = -0x4000;
    i = 0;
    tick = 0;
    puff = (Puff *)((u8 *)work + 0x7080);
    do {
        puff->x = ((sign *((Trig_Sin(ang) << 5) >> 16)) + org) + 20;
        puff->y = ((Trig_Cos(ang) << 4) >> 16) + 40;
        puff->tick = tick;
        ang += 0x1000;
        tick -= 4;
        i += 1;
        puff++;
    } while (i != 9);
    work->transfer_mode = 2;
    if (WORK_EFX->variant == 2) {
        work->transfer_value = 75;
    } else {
        work->transfer_value = 50;
    }
    Scheduler_AddOrUpdateCallback(0x080CD261, 0x480);
    Audio_PlayCue(0x88);
    frame = 0;
    do {
        if (frame == 24) {
            BattleEventRuntime_BeginPhaseFar(0x85);
        }
        i = 0;
        cur = (Puff *)((u8 *)work + 0x7080);
        do {
            /* Negative ticks stagger the puffs; the folded range test is
               what the reference's single unsigned compare came from. */
            if ((cur->tick >= 0) && (cur->tick <= 23)) {
                cell = cur->tick / 4;
                /* wide is loaded once and reused for the centring shift and
                   the width argument, then reloaded per layer because the
                   call clobbers it. */
                draw(dst, (u8 *)work + CELL_SRC[cell],
                    cur->x - ((wide = CELL_W[cell]) >> 1),
                    cur->y + CELL_DY[cell],
                    wide, CELL_H[cell]);
                if (WORK_EFX->variant != 0) {
                    draw(dst, (u8 *)work + CELL_SRC[cell],
                        cur->x - ((wide = CELL_W[cell]) >> 1),
                        (cur->y + CELL_DY[cell]) - 16,
                        wide, CELL_H[cell]);
                }
                if (WORK_EFX->variant == 2) {
                    draw(dst, (u8 *)work + CELL_SRC[cell],
                        cur->x - ((wide = CELL_W[cell]) >> 1),
                        (cur->y + CELL_DY[cell]) - 32,
                        wide, CELL_H[cell]);
                }
            }
            i += 1;
            cur->tick += 1;
            cur++;
        } while (i != 9);
        i = 0;
        while (i != WORK_EFX->count) {
            if (frame == (i * 8) + 16) {
                ObjectGroup_UpdateMembers(WORK_EFX->actors[i], 10, 5, i, 12);
            }
            i += 1;
        }
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
        frame += 1;
    } while (frame != 80);
    Runtime_ReleaseHeapBlock(46);
    Scheduler_RemoveCallback(0x080CD261);
    BattleFx_EndCanvasLayer();
}
