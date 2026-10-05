#include "RUNTIME_MEM.H"
#include "BATTLE_EFFECT_WORK.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFX.H"
#include "BATTLE_WORK.H"
#include "BATTLE_PRESENTATION.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"
#include "IWRAM_CALL.H"
#include "MAP_SCROLL.H"
#include "RESOURCE_IDS.H"
#include "RAM_BUFFER.H"

extern u32 gWorkSlot[];

extern u8 BattlePieceStartX[];
extern u8 BattlePieceStartY[];
extern u8 BattlePieceWidth[];
extern u8 BattlePieceHeight[];
extern u16 BattlePieceOffset[];

void WaitFrames(s32 arg0);
s32 Scheduler_AddOrUpdateCallback(s32 entry, s32 arg1);
void Scheduler_RemoveCallback(s32 entry);
s32 Random16(void);
void BattlePres_SetupTransitionSceneFar(s32 a, s32 b, s32 c, s32 d);
void BattlePresentation_ConfigurePaletteFadeFar(s32 a, s32 b, s32 c);
void BattleBackground_LoadFar(s32 a, s32 b, s32 c);
void Runtime_ApplyValueToWork7818(void);
void ObjectGroup_TickMemberTimers(void);
void ObjectGroup_UpdateMembers(s32 set, s32 object, s32 group, s32 slot, s32 value);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *ent, s32 arg1, s32 arg2);
void Audio_PlayCue(s32 id);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);

#define PLOT(x, y) (*(Ram_MapCellBuffer + ((x) & 7) + (x) / 8 * 64 + ((y) & 7) * 8 + (y) / 8 * 2048) = 2)

/*
 * Blocking battle effect scene at 0x080cbc0c, BattleEffect_RunTileAndPaletteAnimation.
 *
 * Allocates the effect work (slot 39), the canvas (slot 40) and a scratch
 * block (slot 41), programs BG2 and the windows, builds a 16-row by 8-column
 * tile map at 0x06003800, then runs 128 frames: the backdrop fades towards
 * the session palette, frames 0..3 draw a two-pixel ring of growing radius
 * into the 8bpp map cells at 0x02010000, 33 sheet pieces fly apart until
 * frame 50, and from frame 53 the flash burst cells scatter.
 *
 * Score: 173 (alchemy diff, listing 1051, draft 1052, common 965). The old
 * m2c draft scored 1312; the wave before last rewrote it as plain C (246).
 *
 * What moved it, in order (each measured against the previous):
 *   1312  old m2c draft
 *   1065  plain rewrite: IO_WRITE_QUEUE_PUSH, ResourceId_* literals,
 *         (void **)&gBattleWork with [0] and [35] (native adds #140 from the
 *         gBattleWork literal), draw blitters read through a cache pointer to
 *         gWorkSlot ([46], [47]; native adds #184/#188 from the gWorkSlot
 *         literal)
 *   1043  unused struct EffectPosition local: the native frame is 56 bytes,
 *         12 more than the spill slots need (sp+44..55 never accessed)
 *    910  ring pixels through the constant Ram_MapCellBuffer instead of the
 *         gMapCellBuffer label: the native reloads the base from the pool at
 *         every plot (a label is hoisted out of the loop and spilled)
 *    611  plot index (x & 7) + (x / 8 << 6) + (y & 7) * 8 + (y / 8 << 11);
 *         768 other orders/groupings/shift-or-multiply spellings all score
 *         worse (best 408)
 *    531  declaration order work, canvas, frame, draw1, draw0, battle, ...,
 *         rmax, r, d: the spill slots (40, 36, 32, 28, 24, 20, 16, 12, 8)
 *         follow pseudo order
 *    435  tile map: the byte offset i (+= 2) is the counter later loops use,
 *         rows/columns are k/j, and the store is *(volatile s16 *)(0x06003800
 *         + i) of an s16 entry (gives the native lsls/asrs pair and the
 *         unreduced base + offset address)
 *    433  i += 2 in the column loop's increment expression
 *    404  start tables read through pointers start_x/start_y (the counter
 *         stays a biv, as native)
 *    390  palette fade reads both colours through volatile u16 pointers
 *         (native ldrh + lsls #16/lsrs #21 for both; plain u16 gives ldrsh)
 *    325  pieces addressed as piece = &work->particles[i] inside the draw
 *         and scatter loops (native indexes the u16 offset table by i)
 *    303  the 0x33 palette ramp level is a block-local v, not the shared level
 *    281  same for the ring header and backdrop ramp levels; backdrop ramp
 *         written frame >= 8 && frame <= 50
 *    274  x1 = 96 - px; left = x1 (the unclamped copy is the second pseudo,
 *         native keeps it in lr); same for the swapped half
 *    246  plain-swap hill climb (declaration and independent statement order)
 *    236  the canvas size is a local set between the first two allocations
 *         and passed to the allocation and both clears (as EFFECT3.C does):
 *         GCSE propagates the constant into the clears, so each call builds
 *         0x4000 itself instead of sharing one register
 *    226  start_y assigned before start_x, and each piece written as
 *         piece->x = *start_x++ << 16 with the velocity read back through
 *         piece->x (the y pointer is the first pool entry, x values in r2)
 *    205  plain-swap hill climb again (x1/py/x2 declarations, the colour
 *         field order red after green, goal_red after goal_blue)
 *    171  plot through pointer arithmetic with multiplications:
 *         *(Ram_MapCellBuffer + (x & 7) + x / 8 * 64 + (y & 7) * 8
 *         + y / 8 * 2048). Expanded as an address sum, the products stay
 *         deferred until the address is forced, so x / 8 * 64 is formed after
 *         the y division and added as (x/8*64) + (x&7), as the native does;
 *         the same spelling as an array index lets fold factor the products
 *         (885)
 *    167  the second octant keeps its unclamped x1 in its own left2 (native:
 *         lr for the first octant, r10 for the second)
 *    173  IO_WRITE_QUEUE_PUSH closes a second one-pass block after the IME
 *         restore (IO_WRITE_QUEUE.H; PRESENT7.C and PRESENT9.C still match).
 *         All three pushes and the stores and fill call after them now match
 *         the native exactly (it restores IME before loading their operands);
 *         the loop depth it adds moves allocation elsewhere, so the total
 *         rose by 6. Wrapping only the second push scored 163.
 *
 * Tried and worse: gMapCellBuffer[...] for the ring (1043 vs 910),
 * cells/map locals for 0x7800 and the buffer (the copies rematerialise but
 * the fill routine pointer lands in r6 because draw1 prefers r5: 287),
 * every order of the particle loop's six statements with x/y temporaries
 * (best 226 with x loaded first; with temporaries the x chain wins r3),
 * goal = target[i] or battle->palette[i] (the giv increments at the loop end,
 * not after the load: 172-610), target pinned to r9 (599), every order of
 * the octant setup statements (167 at best), u16/u32/s16 colour and goal
 * fields in the fade (190+), hybrid temporary/piece-> forms of the particle
 * loop in every order (173 at best), a whole-macro do { } while (0) (blocks
 * the queue literal load from rising above the previous store: 175),
 * 96 - px + 1 (folds to 97 - px), shared left/left2 split (274 = same),
 * level shared across loops, piece++ pointer walks in the draw/scatter loops,
 * indexed start tables (eliminates the counter), indexed BG_PLTT for the fade,
 * x/y as u32 or s32 in the fade (687+).
 *
 * Remaining difference: register choice and scheduling only.
 *  - Copy/fill constants: the native rematerialises 0x7800 and the buffer
 *    address before each call; the draft keeps them in r5/r6.
 *  - Particle loop: the native loads both start bytes before the first
 *    store; the piece->x form stores x first (a u8 load cannot pass it).
 *  - Fade loop: reload takes r7 from the target pointer and r3 from the
 *    goal << 16 temporary in the same pass, and retry_global_alloc visits
 *    them in pseudo order: target gets r10 and the temporary r9; the
 *    native has them the other way round. The search's colour field order
 *    (red after green) scores better overall but differs here.
 *  - Ring: reload picks round-robin spill registers, so the y1 = 60 - py
 *    inputs (native r5/r7) and the base reloads follow from every reload
 *    before them; the shape of each plot now matches.
 *  - Fade colour fields: the native ties color << 16 to color (red's AND
 *    comes first), leaving r2 for green; here the shift gets its own
 *    register, so green and goal_green swap r2/r4.
 */
void BattleEffect_RunTileAndPaletteAnimation(struct BattleEffectArgument *effect)
{
    /* FAKEMATCH: never read; the native frame keeps 12 unused bytes at sp+44..55, exactly this struct. */
    struct EffectPosition position;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    DrawRectangle draw1;
    DrawRectangle draw0;
    struct BattleSession *battle;
    struct BattleBackgroundView *view;
    void **slots;
    void **cache;
    s32 rmax;
    struct EffectStep *piece;
    s32 r;
    s32 i;
    s32 px;
    s32 j;
    s32 k;
    s32 d;
    s32 py;
    s32 x1;
    s32 size;
    s32 left;
    s32 left2;
    s32 y1;
    s32 x2;
    s32 y2;
    u8 *start_y;
    u8 *start_x;

    work = Runtime_AllocateHeapBlock(39, 0x782c);
    size = 0x4000;
    canvas = Runtime_AllocateHeapBlock(40, size);
    Runtime_AllocateHeapBlock(41, 0x302);
    slots = (void **)&gBattleWork;
    battle = slots[0];
    view = slots[35];
    work->effect = effect;
    Runtime_ApplyValueToWork7818();
    view->second_mode = 1;
    gBgScroll[1].y = 32;
    BattlePresentation_ConfigurePaletteFadeFar(1, battle->background, 0);
    REG_BG2CNT = 0x784;
    BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
    view->second_mode = 0;
    REG_BG2X = 0;
    REG_BG2Y = 0xfffff000;
    REG_BG2PA = 0x80;
    REG_BG2PB = 0;
    REG_BG2PC = 0;
    REG_BG2PD = 0x100;
    REG_WIN0H = 0xf0;
    REG_WIN0V = 0x1088;
    REG_WIN1H = 0xf0;
    REG_WIN1V = 0x1088;
    REG_WININ = 0x3537;
    REG_WINOUT = 0x3f21;

    i = 0;
    for (k = 0; k != 16; k++) {
        for (j = 0; j != 8; j++, i += 2) {
            s16 entry;

            entry = ((k * 16 + j * 2 + 1) << 8) | ((k * 8 + j) * 2);
            *(volatile s16 *)(0x06003800 + i) = entry;
        }
    }

    Iwram_ClearWords(canvas, size);
    Iwram_ClearWords((void *)0x06004000, size);
    IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
    REG_BLDALPHA = 0x1010;
    REG_BLDCNT = 0;
    Resource_LoadAndDecompress((s32)&ResourceId_TileAnimationMask, work, 1, 1);
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    cache = (void **)gWorkSlot;
    draw0 = cache[46];
    BattleEffect_LoadWork(47, 7, 7, 3, 2);
    draw1 = cache[47];

    start_y = BattlePieceStartY;
    start_x = BattlePieceStartX;
    r = 0;
    piece = work->particles;
    for (i = 0; i != 33; i++) {
        piece->x = *start_x++ << 16;
        piece->y = *start_y++ << 16;
        piece->velocity_x = (piece->x - 0x200000) >> 2;
        piece->velocity_y = (piece->y - 0x3c0000) >> 2;
        piece++;
    }

    Iwram_CopyWords(Ram_MapCellBuffer, (void *)0x06008000, 0x7800);
    Iwram_FillWords(Ram_MapCellBuffer, 0x7800, 0x01010101);
    view->busy = 1;
    work->saved_bg1_x = gBgScroll[1].x;
    work->saved_bg1_y = gBgScroll[1].y;
    gBgScroll[1].x = 0;
    IO_WRITE_QUEUE_PUSH(0x0400000a, 0x1f81, 0x20000);
    Iwram_FillWords((void *)0x050000c0, 0x100, 0x7fff7fff);
    Audio_PlayCue(212);
    ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 3, 0, 30);

    for (frame = 0; frame != 128; frame++) {
        if (frame == 2)
            Audio_PlayCue(212);
        if (frame == 3)
            Audio_PlayCue(212);
        if (frame == 28)
            ObjectGroup_UpdateMembers(work->effect->actors[0], -1, 3, -1, 0);
        if (frame == 32)
            Audio_PlayCue(149);
        if (frame == 5) {
            Audio_PlayCue(145);
            gBgScroll[1].x = work->saved_bg1_x;
            BattleBackground_LoadFar(1, battle->background, -1);
        }

        if (frame > 7) {
            volatile u16 *pal;
            volatile u16 *target;

            pal = &BG_PLTT[0x60];
            target = battle->palette;
            for (i = 0; i != 128; i++) {
                u16 goal;
                u16 color;
                s32 red;
                s32 blue;
                s32 goal_green;
                s32 green;
                s32 goal_blue;
                s32 goal_red;

                color = *pal;
                green = (color >> 5) & 0x1f;
                red = color & 0x1f;
                blue = (color >> 10) & 0x1f;
                goal = *target++;
                goal_blue = (goal >> 10) & 0x1f;
                goal_red = goal & 0x1f;
                goal_green = (goal >> 5) & 0x1f;
                if (red < goal_red)
                    red++;
                else if (red > goal_red)
                    red--;
                if (green < goal_green)
                    green++;
                else if (green > goal_green)
                    green--;
                if (blue < goal_blue)
                    blue++;
                else if (blue > goal_blue)
                    blue--;
                *pal++ = (blue << 10) | (green << 5) | red;
            }
        }

        if (frame == 4)
            Iwram_FillWords((void *)0x06008000, 0x7800, 0x02020202);

        if (frame <= 3) {
            s32 v;

            v = frame * 4 + 8;
            rmax = frame * 32;
            BG_PLTT[2] = (v << 10) | (v << 5) | v;
            for (; r != rmax; r++) {
                px = r;
                py = 0;
                d = r;
                while (px >= py) {
                    y1 = 60 - py;
                    x1 = 96 - px;
                    x2 = px + 96;
                    left = x1;
                    y2 = py + 60;
                    if (y1 < 0)
                        y1 = 0;
                    if (y2 > 119)
                        y2 = 119;
                    if (x1 < 0)
                        x1 = 0;
                    if (x2 > 255)
                        x2 = 255;
                    PLOT(x2, y2);
                    PLOT(x2, y1);
                    PLOT(x1, y2);
                    PLOT(x1, y1);
                    x1 = left + 1;
                    x2 = px + 97;
                    if (x1 < 0)
                        x1 = 0;
                    if (x2 > 255)
                        x2 = 255;
                    PLOT(x2, y2);
                    PLOT(x2, y1);
                    PLOT(x1, y2);
                    PLOT(x1, y1);

                    y1 = 60 - px;
                    x1 = 96 - py;
                    x2 = py + 96;
                    left2 = x1;
                    y2 = px + 60;
                    if (x1 < 0)
                        x1 = 0;
                    if (x2 > 255)
                        x2 = 255;
                    if (y1 < 0)
                        y1 = 0;
                    if (y2 > 119)
                        y2 = 119;
                    PLOT(x2, y2);
                    PLOT(x2, y1);
                    PLOT(x1, y2);
                    PLOT(x1, y1);
                    x1 = left2 + 1;
                    x2 = py + 97;
                    if (x1 < 0)
                        x1 = 0;
                    if (x2 > 255)
                        x2 = 255;
                    PLOT(x2, y2);
                    PLOT(x2, y1);
                    PLOT(x1, y2);
                    PLOT(x1, y1);

                    d -= py * 2 + 1;
                    if (d < 0) {
                        d += px * 2 - 2;
                        px--;
                    }
                    py++;
                }
            }
            Iwram_CopyWords((void *)0x06008000, Ram_MapCellBuffer, 0x7800);
        }

        if (frame <= 50) {
            for (i = 0; i != 33; i++) {
                piece = &work->particles[i];
                draw0(canvas, (u8 *)work + BattlePieceOffset[i],
                      ((s16 *)&piece->x)[1], ((s16 *)&piece->y)[1],
                      BattlePieceWidth[i], BattlePieceHeight[i]);
                if (frame > 3)
                    EffectStep_AdvanceWithGravity2D(piece, 64, 0x4000);
            }
        }

        if (frame >= 8 && frame <= 50) {
            s32 v;

            v = frame - 8;
            if (v > 31)
                v = 31;
            BG_PLTT[1] = (v << 10) | (v << 5) | v;
        }

        if (frame == 51) {
            Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work, 1, 0);
            for (i = 1; i != 64; i++) {
                s32 v;

                v = i / 2;
                if (v < 0)
                    v = 0;
                BG_PLTT[i] = (v << 10) | ((v / 2) << 5) | v;
            }
            REG_BLDCNT = 0x3f44;
            for (i = 0; i != 32; i++) {
                piece = &work->particles[i];
                piece->x = ((Random16() & 31) + 32) << 16;
                piece->y = ((Random16() & 31) + 80) << 16;
                piece->velocity_x = ((Random16() & 0x1ff) - 0x100) << 12;
                piece->velocity_y = 0;
                piece->variant = 0;
            }
            work->transfer_mode = 2;
            work->transfer_value = 50;
        }

        if (frame > 52) {
            for (i = 0; i != 32; i++) {
                piece = &work->particles[i];
                if (frame >= i / 4 + 52 && piece->variant <= 39) {
                    s32 cell;

                    cell = piece->variant / 4;
                    if (cell > 5)
                        cell = 5;
                    draw1(canvas, (u8 *)work + (cell << 11),
                          ((s16 *)&piece->x)[1] - 16, ((s16 *)&piece->y)[1] - 32, 32, 64);
                    EffectStep_AdvanceWithGravity2D(piece, 60, -0x1000);
                    piece->variant++;
                }
            }
        }

        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    Scheduler_RemoveCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer);
    ObjectGroup_UpdateMembers(work->effect->actors[0], -1, 1, -1, 0);
    gBgScroll[1].x = work->saved_bg1_x;
    gBgScroll[1].y = 32;
    BattlePresentation_ConfigurePaletteFadeFar(2, battle->background, 0);
    WaitFrames(1);
    IO_WRITE_QUEUE_PUSH(0x04000000, 0x7541, 0x20000);
    Runtime_ReleaseHeapBlock(41);
    Runtime_ReleaseHeapBlock(40);
    Runtime_ReleaseHeapBlock(39);
}
