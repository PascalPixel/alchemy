#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
/* Draft, not exact: score 27233, 1016 differing instructions of 2045 (was
 * 32534 and 1127). One whole function (Lucky Dice, reached from
 * Runtime_BlankDisplayAndRun); the two bl into its own body are the
 * compiler's far jumps for the main loop's break and continue.
 * Found this pass:
 *  - no link-time constant symbols are needed. The display register values
 *    are halfword stores, which load their constant from the pool by
 *    themselves; 0x0c, 0x42 and 0x43 are resource directory rows
 *    (ResourceId_EffectFarCalls and the two dice rows, now named in
 *    DIRECTORY.S); 0x910 is a message id and still needs its name
 *    (MsgLuckyDiceHelp here, undefined).
 *  - the fixed objects are written through the running count from 0, not
 *    through literal indices: that is what gives the listing's
 *    movs rN, #offset; str rM, [sprites, rN] instead of hoisted addresses.
 *  - the frame is now 12 bytes short of the listing's 0x300 (it was 28 over):
 *    three more values live in stack slots there.
 * Remaining, first divergence first:
 *  - the tilemap fill keeps a byte offset in r11 (the register of i) and
 *    steps it by 2, with the column in r1, the row in r5, row * 30 in r4
 *    and a per-row copy of that in r0. i as a byte offset with x as the
 *    column and base = row * 30 gives that shape (r11 stepped by 2) but the
 *    other registers differ, and it scores 28325.
 *  - the listing walks each die loop with a pointer in a different register
 *    (r5 start-up, r6 throw, r7 physics with the other die in r9, r10 for
 *    the shadows), so there is not one die pointer for the function; indexing
 *    work->die[i] everywhere is much worse (50827), and one block-local
 *    pointer in the start-up loop is no better.
 *  - the pair count addresses results and matches from the frame top
 *    (sp + 768 minus a pooled offset).
 *  - registers and spill slots throughout; with the frame 12 bytes off every
 *    stack reference differs, so the score moves by hundreds on any change
 *    and is a poor guide until the frame is right. */
#include "TYPES.H"
#include "DMA.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"
#include "UI.H"

/* Lucky Dice in Tolbi: two dice thrown onto a board of six rows. The bet is
   ten coins per party level; matching faces and rows pay out. */

struct LuckyDiceDie {
    s32 x;
    s32 y;
    s32 z;
    s32 vx;
    s32 vy;
    s32 vz;
    s32 angle;
};

struct LuckyDiceWork {
    u8 filler[0x7080];
    struct LuckyDiceDie die[2];
};

struct LuckyDiceObj {
    u32 attr01;
    u32 attr2;
};

struct LuckyDiceSprites {
    u8 filler[200];
    struct LuckyDiceObj obj[128];
    s32 coins_window;
    s32 winnings_window;
    s32 help_window;
};

struct LuckyDiceBlocks {
    void *render;
    struct LuckyDiceSprites *sprites;
    struct LuckyDiceWork *work;
    u16 *obj_palette;
};

struct LuckyDiceParty {
    u8 filler[16];
    u32 coins;
};

struct LuckyDiceScroll {
    u16 unk00;
    u16 unk02;
    u16 unk04;
    u16 unk06;
};

extern volatile u32 gKeysHeld;
extern volatile u32 gKeysRepeat;
extern struct LuckyDiceParty gGameState;
extern struct LuckyDiceScroll gBgScroll;
extern u8 gOamCopyEnabled;
extern u8 Data_080f53fc[];
extern u8 Data_080f5400[];
extern u8 Data_080f5408[];
extern s16 Data_080f541a[];
extern char ResourceId_EffectFarCalls;
extern char ResourceId_LuckyDiceBoard;
extern char ResourceId_LuckyDiceSprites;
extern char MsgLuckyDiceHelp;

void RuntimeDispatch_NoOpHook(s32 value);
void Camera_ResetSceneDefaults(void);
void Scheduler_ResetTaskTable(void);
s32 Graphics_ScaleRgb555Buffer(u16 *source, u16 *destination, s32 scale, s32 count);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(void *work, void *transfer);
s32 Render_ProjectPoint(void *point, s32 *out);
u32 __umodsi3(u32 numerator, u32 denominator);
void FarCall_WindowTable(void);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void UiWork_FinalizeFar(s32 window, s32 mode);
void Party_AdjustSixDigitCounterAFar(s32 amount);
s32 Party_GetAverageLevelFar(void);
void Audio_PlayCue(s32 cue);

#define REG_DISPCNT (*(u16 *)0x04000000)
#define REG_BG1CNT (*(u16 *)0x0400000a)
#define REG_BG1HOFS (*(u16 *)0x04000014)
#define REG_BG1VOFS (*(u16 *)0x04000016)
#define REG_BG2HOFS (*(u16 *)0x04000018)
#define REG_BG2VOFS (*(u16 *)0x0400001a)
#define REG_WIN0H (*(u16 *)0x04000040)
#define REG_WIN1H (*(u16 *)0x04000042)
#define REG_WIN0V (*(u16 *)0x04000044)
#define REG_WIN1V (*(u16 *)0x04000046)
#define REG_WININ (*(u16 *)0x04000048)
#define REG_WINOUT (*(u16 *)0x0400004a)
#define REG_BLDCNT (*(u16 *)0x04000050)
#define REG_BLDALPHA (*(u16 *)0x04000052)

#define LuckyDice_Sqrt ((s32 (*)(s32))0x030001d8)

void LuckyDice_Run(void)
{
    u16 palette[256];
    s32 depth[4];
    s32 results[4];
    s32 point[3];
    s32 proj[3];
    s32 unused[8];
    volatile u16 affine_a;
    volatile u16 affine_b;
    volatile u16 affine_c;
    volatile u16 affine_d;
    s32 spin[2];
    s32 flash[2];
    s32 faces[2];
    s32 order[2];
    s32 matches[2];
    u16 *obj_palette;
    struct LuckyDiceWork *work;
    struct LuckyDiceSprites *sprites;
    void *render;
    struct LuckyDiceDie *die;
    struct LuckyDiceDie *other;
    s32 state;
    s32 frame;
    s32 timer;
    s32 kind;
    s32 winnings;
    s32 counter;
    s32 bet;
    s32 digits;
    s32 count;
    s32 stopped;
    s32 reset;
    s32 window;
    s32 i;
    s32 j;
    s32 n;
    s32 x;
    s32 y;
    s32 z;
    s32 d;
    s32 e;
    s32 tile;
    s32 key;
    s32 row;
    s32 base;
    s32 pos;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 dist;
    s32 speed;
    u32 angle;
    s32 scale;
    s32 pairs;
    s16 value;

    Runtime_AllocateHeapBlock(41, 0x60e);
    obj_palette = Runtime_AllocateHeapBlock(40, 0x200);
    work = Runtime_AllocateBlock(39, 0x782c);
    sprites = Runtime_AllocateBlock(45, 0x618);
    render = Runtime_AllocateBlock(12, 76);
    RuntimeDispatch_NoOpHook((s32)&ResourceId_EffectFarCalls);
    Camera_ResetSceneDefaults();
    Scheduler_ResetTaskTable();
    gOamCopyEnabled = 0;

    i = 0;
    for (row = 0; row != 20; row++) {
        for (j = 0; j != 32; j++) {
            ((u16 *)0x06003000)[i] = j + row * 30;
            i++;
        }
    }

    base = (s32)Resource_GetTableEntry((s32)&ResourceId_LuckyDiceBoard);
    Dma_Set((void *)base, (void *)0x05000000, 0x84000070, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x05000000, palette, 0x84000080, (volatile u32 *)0x040000d4);
    Resource_DecodeType01((void *)(base + 0x1c0), (void *)0x02010000);
    Dma_Set((void *)0x02010000, (void *)0x06004000, 0x84002580, (volatile u32 *)0x040000d4);
    base = (s32)Resource_GetTableEntry((s32)&ResourceId_LuckyDiceSprites);
    Dma_Set((void *)base, obj_palette, 0x84000080, (volatile u32 *)0x040000d4);
    Dma_Set((void *)base, (void *)0x05000200, 0x84000080, (volatile u32 *)0x040000d4);
    Resource_DecodeType01((void *)(base + 0x200), (void *)0x02010000);
    Dma_Set((void *)0x02010000, (void *)0x06010000, 0x84001f00, (volatile u32 *)0x040000d4);
    Graphics_ScaleRgb555Buffer(obj_palette, (u16 *)0x05000200, 0, 256);
    Graphics_ScaleRgb555Buffer(palette, (u16 *)0x05000000, 0, 256);

    REG_BG1CNT = 0x0686;
    REG_WININ = 0x3737;
    REG_WINOUT = 0x2723;
    REG_DISPCNT = 0x3340;
    REG_BLDCNT = 0x3f44;
    REG_BLDALPHA = 0x0810;
    gBgScroll.unk04 = 0;
    gBgScroll.unk06 = 0;
    REG_BG1HOFS = 0;
    REG_BG1VOFS = 0xff60;
    REG_BG2HOFS = 0;
    REG_BG2VOFS = 0xff60;
    REG_WIN0H = 0xf0;
    REG_WIN0V = 0xa0;
    REG_WIN1H = 0xf0;
    REG_WIN1V = 0xa0;

    state = 0;
    timer = 0;
    kind = -1;
    winnings = 0;
    counter = 0;
    die = work->die;
    for (i = 0; i != 2; i++) {
        die->x = ((Random16() & 127) - 64) << 16;
        die->y = (Random16() & 31) << 16;
        die->z = ((Random16() & 127) - 64) << 16;
        die->vx = ((Random16() & 127) - 64) << 14;
        die->vy = ((Random16() & 31) + 32) << 12;
        die->vz = ((Random16() & 127) - 64) << 14;
        die->angle = 0;
        spin[i] = 300;
        die++;
    }
    results[0] = results[1] = results[2] = results[3] = 0;
    flash[0] = flash[1] = 0;
    faces[0] = __umodsi3(Random16(), 6);
    do {
        faces[1] = __umodsi3(Random16(), 6);
    } while (faces[0] == faces[1]);
    work->die[0].x = 0x280000;
    work->die[0].y = 0x600000;
    work->die[0].z = -0xa00000;
    work->die[1].x = -0x280000;
    work->die[1].y = 0x600000;
    work->die[1].z = -0xa00000;

    FarCall_WindowTable();
    window = UiWindow_CreateFar(18, 0, 12, 3, 6);
    sprites->coins_window = window;
    UiText_DrawCharacterAtOffsetFar(0x906, window, 48, 0);
    UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coins_window, 0, 0);
    window = UiWindow_CreateFar(22, 16, 8, 4, 6);
    sprites->help_window = window;
    UiText_DrawCharacterAtOffsetFar(0x910, window, 0, 0);
    UiText_DrawCharacterAtOffsetFar(0x911, sprites->help_window, 0, 8);

    frame = 0;
    for (;;) {
        reset = 0;
        if (frame <= 16) {
            Graphics_ScaleRgb555Buffer(obj_palette, (u16 *)0x05000200, frame << 12, 256);
            Graphics_ScaleRgb555Buffer(palette, (u16 *)0x05000000, frame << 12, 256);
        }
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(render, (u8 *)render + 12);

        count = 0;
        sprites->obj[count].attr01 = 0xc0006000;
        sprites->obj[count].attr2 = 0x358;
        count++;
        sprites->obj[count].attr01 = 0x40102020;
        sprites->obj[count].attr2 = 0x398;
        count++;
        sprites->obj[count].attr01 = 0x40102030;
        sprites->obj[count].attr2 = 0x3a0;
        count++;
        sprites->obj[count].attr01 = 0x40042020;
        sprites->obj[count].attr2 = 0x3a8;
        count++;
        sprites->obj[count].attr01 = 0x40042030;
        sprites->obj[count].attr2 = 0x3a8;
        count++;
        for (i = 0; i != 2; i++) {
            sprites->obj[count].attr01 = (32 + i * 16) | 0x40202000;
            sprites->obj[count].attr2 = faces[i] * 8 + 0x3b0;
            count++;
        }

        if (state == 2 || state == 3) {
            for (i = 0; i != 4; i++) {
                if (timer == i * 5) {
                    Audio_PlayCue(0x12f);
                }
                if (timer >= i * 5) {
                    sprites->obj[count].attr01 = 0x80002040 | ((56 + i * 32) << 16);
                    sprites->obj[count].attr2 = results[i] * 32 + 152;
                    count++;
                }
            }
            if (timer > 19 && kind >= 0) {
                n = Data_080f53fc[kind];
                if (n != 0) {
                    tile = Data_080f5400[kind] * 8 + 152;
                    x = 0x278 - n * 16;
                    for (j = 0; j != n; j++) {
                        sprites->obj[count].attr01 = (((x & 0x1ff) << 16) | 96) | 0x80002000;
                        sprites->obj[count].attr2 = tile;
                        count++;
                        tile += 32;
                        x += 32;
                    }
                }
            }
        }

        if (state == 2) {
            if (timer == 20 && winnings > 0) {
                Party_AdjustSixDigitCounterAFar(winnings);
                digits = 1;
                if (winnings > 9) {
                    digits = 2;
                    if (winnings > 99) {
                        digits = 3;
                        if (winnings > 999) {
                            digits = 4;
                            if (winnings > 9999) {
                                digits = 5;
                            }
                        }
                    }
                }
                window = UiWindow_CreateFar(9 - digits / 2, 16, digits + 12, 3, 6);
                sprites->winnings_window = window;
                UiText_DrawCharacterAtOffsetFar(0x907, window, 0, 0);
                UiText_DrawCharacterAtOffsetFar(0x906, sprites->winnings_window, digits * 8 + 48, 0);
                UiText_DrawNumberInWindowFar(winnings, digits, sprites->winnings_window, 48, 0);
            }
            if (counter > 0) {
                counter--;
            }
            if (timer > 19 && (gKeysRepeat & 1)) {
                Audio_PlayCue(112);
                if (winnings > 0) {
                    state = 3;
                    counter = 0;
                } else {
                    state = 0;
                    window = UiWindow_CreateFar(22, 16, 8, 4, 6);
                    sprites->help_window = window;
                    UiText_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceHelp, window, 0, 0);
                    UiText_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceHelp + 1, sprites->help_window, 0, 8);
                    reset = 1;
                }
            }
            timer++;
        } else if (state == 3) {
            if (counter == 0) {
                winnings = 0;
                UiText_DrawNumberInWindowFar(0, digits, sprites->winnings_window, 40, 0);
                UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coins_window, 0, 0);
            }
            if (counter == 15) {
                state = 0;
                UiWork_FinalizeFar(sprites->winnings_window, 1);
                window = UiWindow_CreateFar(22, 16, 8, 4, 6);
                sprites->help_window = window;
                UiText_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceHelp, window, 0, 0);
                UiText_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceHelp + 1, sprites->help_window, 0, 8);
                reset = 1;
            }
            counter++;
        } else if (state == 0 && frame > 17) {
            /* FAKEMATCH: two discarded reads of the repeat keys */
            if (gKeysRepeat & 0) {
            }
            if (gKeysRepeat & 0) {
            }
            if (gKeysHeld & 0x40) {
                work->die[0].y += 0x40000;
                work->die[1].y += 0x40000;
            }
            if (gKeysHeld & 0x80) {
                work->die[0].y += -0x40000;
                work->die[1].y += -0x40000;
            }
            if (work->die[0].y < 0x200000) {
                work->die[0].y = 0x200000;
            }
            if (work->die[0].y > 0xa00000) {
                work->die[0].y = 0xa00000;
            }
            if (work->die[1].y < 0x200000) {
                work->die[1].y = 0x200000;
            }
            if (work->die[1].y > 0xa00000) {
                work->die[1].y = 0xa00000;
            }
            if (gGameState.coins < (u32)(Party_GetAverageLevelFar() * 10)) {
                UiWork_FinalizeFar(sprites->help_window, 1);
                break;
            }
            if (gKeysRepeat & 2) {
                Audio_PlayCue(113);
                UiWork_FinalizeFar(sprites->help_window, 1);
                break;
            }
            if (gKeysRepeat & 1) {
                Audio_PlayCue(0x12e);
                UiWork_FinalizeFar(sprites->help_window, 1);
                state = 1;
                bet = Party_GetAverageLevelFar() * 10;
                if ((u32)bet > gGameState.coins) {
                    bet = gGameState.coins;
                }
                Party_AdjustSixDigitCounterAFar(-bet);
                UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coins_window, 0, 0);
                die = work->die;
                for (i = 0; i != 2; i++) {
                    if (i > 1) {
                        die->z = -0xb40000;
                        die->y = 0x600000;
                    }
                    die->vx = ((Random16() & 127) - 64) << 12;
                    die->vy = ((Random16() & 15) + 48) << 13;
                    n = Random16() & 63;
                    die->vz = ((n + 140) << 12) + __divsi3(die->y, 6);
                    die->angle = __umodsi3(Random16(), 0x4800);
                    spin[i] = 300;
                    die++;
                }
            }
        }

        if (reset == 1) {
            work->die[0].x = 0x280000;
            work->die[0].y = 0x600000;
            work->die[0].z = -0xa00000;
            work->die[1].x = -0x280000;
            work->die[1].y = 0x600000;
            work->die[1].z = -0xa00000;
            faces[0] = __umodsi3(Random16(), 6);
            do {
                faces[1] = __umodsi3(Random16(), 6);
            } while (faces[0] == faces[1]);
        }

        order[0] = 0;
        order[1] = 1;
        for (i = 1; i < 2; i++) {
            key = order[i];
            for (j = i - 1; j >= 0 && work->die[order[j]].z > work->die[key].z; j--) {
                order[j + 1] = order[j];
            }
            order[j + 1] = key;
        }

        for (i = 0; i != 2; i++) {
            die = &work->die[order[i]];
            Render_ProjectPoint(die, proj);
            if (proj[2] < 0x15e) {
                proj[2] = 0x15e;
            }
            if (proj[2] > 0x28a) {
                proj[2] = 0x28a;
            }
            depth[order[i]] = proj[2] - 0x15e;
            d = __divsi3(0x800, depth[order[i]] / 2 + 128);
            x = proj[0] - d;
            y = proj[1] - d;
            if (y + 96 > -16) {
                sprites->obj[count].attr01 = ((order[i] << 25) | (((x + 0x278) & 0x1ff) << 16) | ((y + 0x160) & 255)) | 0x40002300;
                sprites->obj[count].attr2 = (Data_080f5408[die->angle / 1024] << 3) | 0x400;
                count++;
            }
        }

        if (state == 0) {
            die = work->die;
            for (i = 0; i != 2; i++) {
                Render_ProjectPoint(die, proj);
                n = depth[order[i]] / 2 + 128;
                x = proj[0] - __divsi3(0x800, n);
                y = proj[1] + __divsi3(0x300, n);
                sprites->obj[count].attr01 = ((((x + 0x278) & 0x1ff) << 16) | ((y + 0x160) & 255)) | 0x80002000;
                if (i == 1) {
                    sprites->obj[count].attr01 |= 0x10000000;
                }
                sprites->obj[count].attr2 = 0x958;
                count++;
                die++;
            }
        }

        die = work->die;
        for (i = 0; i != 2; i++) {
            point[1] = 0;
            point[0] = die->x;
            point[2] = die->z;
            Render_ProjectPoint(point, proj);
            if (proj[2] < 0x15e) {
                proj[2] = 0x15e;
            }
            if (proj[2] > 0x28a) {
                proj[2] = 0x28a;
            }
            depth[2 + i] = proj[2] - 0x15e;
            depth[2 + i] += *((s16 *)&die->y + 1);
            if (proj[1] + 88 > -16) {
                sprites->obj[count].attr01 = ((((proj[0] + 0x270) & 0x1ff) << 16) | ((i + 2) << 25)) | ((proj[1] + 0x158) & 255) | 0x40002300;
                sprites->obj[count].attr2 = 0x890;
                count++;
            }
            die++;
        }

        if (state == 1) {
            stopped = 0;
            die = work->die;
            for (i = 0; i != 2; i++) {
                die->x += die->vx;
                die->y += die->vy;
                die->z += die->vz;
                other = work->die;
                for (j = 0; j != 2; j++) {
                    if (i != j) {
                        dx = (die->x - other->x) >> 16;
                        dy = (die->y - other->y) >> 16;
                        dz = (die->z - other->z) >> 16;
                        dist = dx * dx + dy * dy + dz * dz;
                        if (dist <= 0x9c3) {
                            dist = LuckyDice_Sqrt(dist);
                            die->vx += __divsi3(dx << 15, dist);
                            die->vy += __divsi3(dy << 15, dist);
                            die->vz += __divsi3(dz << 15, dist);
                        }
                    }
                    other++;
                }
                if (die->x < -0xc80000) {
                    die->x = -0xc80000;
                    die->vx = -die->vx * 48 / 64;
                }
                if (die->x > 0xc80000) {
                    die->x = 0xc80000;
                    die->vx = -die->vx * 48 / 64;
                }
                if (die->z < -0x960000) {
                    die->z = -0x960000;
                    die->vz = -die->vz * 48 / 64;
                }
                if (die->z > die->y + 0xb40000) {
                    die->z = die->y + 0xb40000;
                    if (die->vz > 0) {
                        die->vy += die->vz / 2;
                        die->vz = -die->vz * 48 / 64;
                    }
                }
                die->vy += -0x8000;
                if (die->y <= 0) {
                    die->y = 0;
                    die->vy = -die->vy * 48 / 64;
                    if ((Random16() & 3) == 0) {
                        speed = LuckyDice_Sqrt((die->vx >> 8) * (die->vx >> 8) + (die->vz >> 8) * (die->vz >> 8)) * 56 / 64;
                        angle = Random16() & 0xffff;
                        die->vx = speed * Trig_Sin(angle) >> 8;
                        die->vz = speed * Trig_Cos(angle) >> 8;
                    } else {
                        die->vx = die->vx * 56 / 64;
                        die->vz = die->vz * 56 / 64;
                    }
                    if (spin[i] > 19) {
                        spin[i] -= 20;
                    }
                    if (die->vy < 0x30000) {
                        die->vx = 0;
                        die->vy = 0;
                        die->vz = 0;
                        spin[i] = 0;
                        n = die->angle;
                        switch (__modsi3(n / 1024, 3)) {
                        case 1:
                            die->angle = n + 0x800;
                            break;
                        case 2:
                            die->angle = n + 0x400;
                            break;
                        }
                    }
                }
                die->vx = die->vx * 63 / 64;
                die->vy = die->vy * 63 / 64;
                die->vz = die->vz * 63 / 64;
                die->angle += spin[i];
                if (die->angle > 0x47ff) {
                    die->angle -= 0x4800;
                }
                if (spin[i] > 0) {
                    spin[i]--;
                }
                if (die->vx == 0 && die->vy == 0 && die->vz == 0 && die->y == 0) {
                    results[i] = __divsi3(die->angle, 0xc00);
                    for (j = 0; j != 6; j++) {
                        if (die->z > Data_080f541a[j] << 16) {
                            results[i + 2] = j;
                            break;
                        }
                    }
                    stopped++;
                }
                die++;
            }

            if (stopped == 2) {
                state = 2;
                timer = 0;
                kind = -1;
                winnings = 0;
                counter = 60;
                if (results[0] == results[1] && results[0] == results[2] && results[0] == results[3]) {
                    flash[0] = flash[1] = 120;
                    kind = 0;
                    if (results[0] == faces[0]) {
                        winnings = bet * 15;
                    } else if (results[0] == faces[1]) {
                        winnings = bet * 25;
                    } else {
                        winnings = bet * 5;
                    }
                    UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coins_window, 0, 0);
                    Audio_PlayCue(93);
                } else {
                    pairs = 0;
                    for (i = 0; i != 3; i++) {
                        for (j = i + 1; j != 4; j++) {
                            if (results[i] == results[j]) {
                                matches[pairs] = results[i];
                                pairs++;
                            }
                        }
                    }
                    if (pairs == 1) {
                        winnings = bet;
                        UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coins_window, 0, 0);
                        kind = 3;
                    }
                    if (pairs == 2) {
                        winnings = bet * 2;
                        UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coins_window, 0, 0);
                        flash[Random16() & 1] = 60;
                        kind = 2;
                        Audio_PlayCue(91);
                    }
                    if (pairs == 3) {
                        winnings = bet * 3;
                        UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coins_window, 0, 0);
                        flash[Random16() & 1] = 60;
                        kind = 1;
                        Audio_PlayCue(92);
                    }
                }
            }
        }

        for (; count != 128; count++) {
            sprites->obj[count].attr01 = 0x40f02000;
            sprites->obj[count].attr2 = 0;
        }

        for (i = 0; i != 4; i++) {
            value = depth[i] / 2 + 128;
            affine_a = value;
            affine_b = 0;
            affine_c = 0;
            affine_d = value;
            sprites->obj[i * 4 + 0].attr2 |= affine_a << 16;
            sprites->obj[i * 4 + 1].attr2 |= affine_b << 16;
            sprites->obj[i * 4 + 2].attr2 |= affine_c << 16;
            sprites->obj[i * 4 + 3].attr2 |= affine_d << 16;
        }

        Dma_Set(sprites->obj, (void *)0x07000000, 0x84000100, (volatile u32 *)0x040000d4);
        WaitFrames(1);
        frame++;
    }

    for (i = 0; i != 17; i++) {
        scale = 0x10000 - (i << 12);
        Graphics_ScaleRgb555Buffer(obj_palette, (u16 *)0x05000200, scale, 256);
        Graphics_ScaleRgb555Buffer(palette, (u16 *)0x05000000, scale, 256);
        WaitFrames(1);
    }
    Runtime_ReleaseHeapBlock(12);
    Runtime_ReleaseHeapBlock(45);
    Runtime_ReleaseHeapBlock(40);
    Runtime_ReleaseHeapBlock(39);
    Runtime_ReleaseHeapBlock(41);
}
