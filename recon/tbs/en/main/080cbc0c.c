/* 080cbc0c: differ 45 of 1051. Trials: copy wrapper differs 173; fill wrapper stays 45; consumed opaque destination/count operands differ 175; clobbering r5/r6 after copy differs 65. Remaining: CSE keeps 0x7800 and gMapCellBuffer in r5/r6 across the Iwram_CopyWords/Iwram_FillWords pair after the piece loop; the native rematerializes both (fill routine in r5), and the rest is that register shift plus literal-pool offsets. Variables for the size or buffer move the shift elsewhere (157-173). */
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
void BattleFx_ClearActorTimers(void);
void ObjectGroup_TickMemberTimers(void);
void ObjectGroup_UpdateMembers(s32 set, s32 object, s32 group, s32 slot, s32 value);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *ent, s32 arg1, s32 arg2);
void Audio_PlayCue(s32 id);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);

#define PLOT(x, y) (*(Ram_MapCellBuffer + ((x) & 7) + (x) / 8 * 64 + ((y) & 7) * 8 + (y) / 8 * 2048) = 2)

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
    s32 cx;
    s32 cy;

    work = Runtime_AllocateHeapBlock(39, 0x782c);
    size = 0x4000;
    canvas = Runtime_AllocateHeapBlock(40, size);
    Runtime_AllocateHeapBlock(41, 0x302);
    slots = (void **)&gBattleWork;
    battle = slots[0];
    view = slots[35];
    work->effect = effect;
    BattleFx_ClearActorTimers();
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
        s32 x, y, vx, vy;

        x = *start_x++ << 16;
        y = *start_y++ << 16;
        piece->x = x;
        piece->y = y;
        vx = (x - 0x200000) >> 2;
        vy = (y - 0x3c0000) >> 2;
        piece->velocity_x = vx;
        piece->velocity_y = vy;
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
            u16 *pal;

            pal = (u16 *)0x050000c0;
            for (i = 0; i != 128; i++) {
                s32 red = *pal & 0x1f;
                s32 green = (*pal >> 5) & 0x1f;
                s32 blue = (*pal >> 10) & 0x1f;
                s32 goal_red = battle->palette[i] & 0x1f;
                s32 goal_green = (battle->palette[i] >> 5) & 0x1f;
                s32 goal_blue = (battle->palette[i] >> 10) & 0x1f;

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
            cx = 96;
            cy = 60;
            for (; r != rmax; r++) {
                px = r;
                py = 0;
                d = r;
                while (px >= py) {
                    x2 = px + cx;
                    x1 = cx - px;
                    left = x1;
                    y1 = cy - py;
                    y2 = py + cy;
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

                    x2 = py + cx;
                    x1 = cx - py;
                    left2 = x1;
                    y2 = px + cy;
                    y1 = cy - px;
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
