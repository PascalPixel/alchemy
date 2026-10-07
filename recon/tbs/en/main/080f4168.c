/* 080f4168 LuckyDice_Run: uncredited draft; listing 2046, draft 2039, differ 1291.
 * Frame 768 with every array and spill slot at its native offset; the
 * remaining difference is mostly register choice (reload's round-robin
 * spill registers follow from everything before) and a few open shapes.
 *
 * Source shapes the bytes prove (session 2026-10-07, sun-080f4168):
 * - The OAM shadow entries are volatile u32. loop.c then forms no DEST_ADDR
 *   givs for them, so every obj loop keeps native's offset givs
 *   (count * 8 + 200, + 204) and [sprites, off] stores with sprites
 *   reloaded from its slot (1846 -> 1614).
 * - The BG map store is *(volatile u16 *)(0x06003000 + i), i += 2, with its
 *   own x/y counters (SETUP_CANVAS_TILE_MAP.C writes VRAM the same way);
 *   combine cannot fold the address into the volatile strh (1918 -> 1846).
 * - Declarations in native slot order: obj_palette, work, sprites, render,
 *   frame, state, bet, timer, kind, winnings, counter, digits, count; the
 *   hoisted &help_window and &depth temps come next, then stopped, which is
 *   declared in the state 1 block (1614 -> 1520).
 * - Messages relative to one symbol: Coins + 0/1/10/11 (CSE derives
 *   Winnings - 1, Throw + 1 and Coins + 10/11 as native does).
 * - digits as an if/else-if chain on <= 9/99/999/9999: the dead labels
 *   between the arms end reload inheritance, so winnings is reloaded for
 *   each compare as in native.
 * - Two discarded gKeysRepeat reads before the KEY_UP test.
 * - The kind tiles loop counts with i to Data_080f53fc[kind] itself (the
 *   loop bound is hoisted and copied as native's r5); x = 0x278 - n * 16 and
 *   y = 96 are set inside the loop, so loop.c hoists them after the entry
 *   test, keeps the MINUS as the giv's base (subs) and ORs 96 and
 *   0x80002000 separately.
 * - order[] is filled by a two-pass loop (completely unrolled) and sorted by
 *   a one-pass i loop (removed in loop pass 2), so key is 1 and the j loop
 *   starts at 0 as in native.
 * - Divisions and modulos are C operators (libcalls): the die loop then
 *   keeps order[i] across __divsi3 for << 25 as native does.
 * - The bet loop and the physics loop address the dice through
 *   die = &work->particles[i] (and other = &work->particles[j]); loop.c
 *   reduces each to its own pointer giv with base offset 0 (native r6, r7,
 *   r9), not one shared biv.
 * - The die loop has its own px/py: native's die-loop x/y are r2/r0 while
 *   the marker loop's are r5/r2, so they are not the same pseudos (shared
 *   x/y took a callee-saved register and spilled &depth: 1601 -> 1291).
 * - The shadow loop adds work->particles[i].y >> 16 to depth[2 + i]; combine
 *   narrows it to native's ldrsh and the int access keeps the first store.
 *
 * Open:
 * - The 0x02010000 decode buffer: native rematerialises it from the pool at
 *   all four uses (its pseudo got no register); the draft keeps it in r5.
 * - Sort: native stores order[0] from i (r11 = 0) and has a separate zero
 *   for j; the draft folds both to one zero.
 * - Shadow loop: native reduces (i + 2) << 25 to a giv (r9 += 1 << 25); the
 *   draft's is "not worth while" (34 vs 75).
 * - The affine loop's depth pointer: native's global allocation gave it a
 *   register that reload then spilled to sp+4 (after the caller-save
 *   slots); the draft leaves it to the initial spill.
 * - Affine halfwords: native stores four halfwords to sp+166..160 and
 *   reloads each with ldrh; the zeros come from two literal-pool loads of 0.
 *   The draft's volatile u16 locals are tagged FAKEMATCH; whether that is
 *   an admissible device or real source is not settled.
 *
 * Earlier trials (kept where still informative):
 * - Native locals, top down: palette 256, depth 240, results 224, point 212,
 *   proj 200, 32 unused bytes 168..200, affine halfwords at 166..160, spin
 *   152, flash 144, faces 136, order 128, matches 120; spills from 116.
 * - Tile loop as row * 30 + j, < bounds, through pos, as x/y: 1930..1966.
 * - sprites as a volatile pointer, as an extern-escaped local, or render,
 *   sprites, work and obj_palette as one local struct: all worse; the cause
 *   of native's offset givs was the volatile OAM stores.
 * - u16 affine[4] instead of four halfword locals: worse.
 */
#include "RESOURCE.H"
#include "MEM.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "DMA.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"
#include "UI.H"
#include "GAME_STATE.H"
#include "MAP_SCROLL.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BTLDISP.H"
#include "IWRAM_CALL.H"
#include "TRANSFORM.H"
#include "RESOURCE_IDS.H"
#include "text/MSG_IDS.H"

struct LuckyDiceObj {
    volatile u32 attr01;
    volatile u32 attr2;
};

struct LuckyDiceSprites {
    u8 filler[200];
    struct LuckyDiceObj obj[128];
    s32 coins_window;
    s32 winnings_window;
    s32 help_window;
};

extern volatile u32 gKeysHeld;
extern volatile u32 gKeysRepeat;
extern u8 gOamCopyEnabled;
extern u8 Data_080f53fc[];
extern u8 Data_080f5400[];
extern u8 Data_080f5408[];
extern s16 Data_080f541a[];
extern char ResourceId_EffectFarCalls;
extern char ResourceId_LuckyDiceBoard;
extern char ResourceId_LuckyDiceSprites;

void Camera_ResetSceneDefaults(void);
void Scheduler_ResetTaskTable(void);
s32 Graphics_ScaleRgb555Buffer(u16 *source, u16 *destination, s32 scale, s32 count);
void Render_ResetTransformState(void);
s32 Render_ProjectPoint(void *point, s32 *out);
u32 __umodsi3(u32 numerator, u32 denominator);
void FarCall_WindowTable(void);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void UiWork_FinalizeFar(s32 window, s32 mode);
void Party_AdjustCoinsFar(s32 amount);
s32 Party_GetAverageLevelFar(void);
void Audio_PlayCue(s32 cue);

void LuckyDice_Run(void)
{
    u16 palette[256];
    s32 depth[4];
    s32 results[4];
    s32 point[3];
    s32 proj[3];
    /* FAKEMATCH: native frame keeps 32 unused bytes at sp+168..200, between proj and the affine halfwords */
    s32 unused[8];
    /* FAKEMATCH: native stores the four halfwords to sp+166..160 and reloads each with ldrh */
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
    struct BattleEffectWork *work;
    struct LuckyDiceSprites *sprites;
    struct BattleCamera *render;
    s32 frame;
    s32 state;
    s32 bet;
    s32 timer;
    s32 kind;
    s32 winnings;
    s32 counter;
    s32 digits;
    s32 count;
    struct EffectStep *die;
    struct EffectStep *other;
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
    s32 tx;
    s32 ty;
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
    ((void (*)(s32))RuntimeDispatch_NoOpHook)((s32)&ResourceId_EffectFarCalls);
    Camera_ResetSceneDefaults();
    Scheduler_ResetTaskTable();
    gOamCopyEnabled = 0;

    i = 0;
    for (ty = 0; ty != 20; ty++) {
        for (tx = 0; tx != 32; tx++) {
            *(volatile u16 *)(0x06003000 + i) = tx + ty * 30;
            i += 2;
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
    gBgScroll[1].x = 0;
    gBgScroll[1].y = 0;
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
    for (i = 0; i != 2; i++) {
        work->particles[i].x = ((Random16() & 127) - 64) << 16;
        work->particles[i].y = (Random16() & 31) << 16;
        work->particles[i].z = ((Random16() & 127) - 64) << 16;
        work->particles[i].velocity_x = ((Random16() & 127) - 64) << 14;
        work->particles[i].velocity_y = ((Random16() & 31) + 32) << 12;
        work->particles[i].velocity_z = ((Random16() & 127) - 64) << 14;
        work->particles[i].variant = 0;
        spin[i] = 300;
    }
    results[0] = results[1] = results[2] = results[3] = 0;
    flash[0] = flash[1] = 0;
    faces[0] = Random16() % 6;
    do {
        faces[1] = Random16() % 6;
    } while (faces[0] == faces[1]);
    work->particles[0].x = 0x280000;
    work->particles[0].y = 0x600000;
    work->particles[0].z = -0xa00000;
    work->particles[1].x = -0x280000;
    work->particles[1].y = 0x600000;
    work->particles[1].z = -0xa00000;

    FarCall_WindowTable();
    window = UiWindow_CreateFar(18, 0, 12, 3, 6);
    sprites->coins_window = window;
    UiText_DrawCharacterAtOffsetFar((s32)MsgLuckyDiceCoins, window, 48, 0);
    UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coins_window, 0, 0);
    window = UiWindow_CreateFar(22, 16, 8, 4, 6);
    sprites->help_window = window;
    UiText_DrawCharacterAtOffsetFar((s32)MsgLuckyDiceCoins + 10, window, 0, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgLuckyDiceCoins + 11, sprites->help_window, 0, 8);

    frame = 0;
    for (;;) {
        reset = 0;
        if (frame <= 16) {
            Graphics_ScaleRgb555Buffer(obj_palette, (u16 *)0x05000200, frame << 12, 256);
            Graphics_ScaleRgb555Buffer(palette, (u16 *)0x05000000, frame << 12, 256);
        }
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(render->eye, render->pos);

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
                for (i = 0; i != Data_080f53fc[kind]; i++) {
                    x = 0x278 - Data_080f53fc[kind] * 16;
                    y = 96;
                    sprites->obj[count].attr01 = ((((x + i * 32) & 0x1ff) << 16) | y) | 0x80002000;
                    sprites->obj[count].attr2 = Data_080f5400[kind] * 8 + 152 + i * 32;
                    count++;
                }
            }
        }

        if (state == 2) {
            if (timer == 20 && winnings > 0) {
                Party_AdjustCoinsFar(winnings);
                if (winnings <= 9) {
                    digits = 1;
                } else if (winnings <= 99) {
                    digits = 2;
                } else if (winnings <= 999) {
                    digits = 3;
                } else if (winnings <= 9999) {
                    digits = 4;
                } else {
                    digits = 5;
                }
                window = UiWindow_CreateFar(9 - digits / 2, 16, digits + 12, 3, 6);
                sprites->winnings_window = window;
                UiText_DrawCharacterAtOffsetFar((s32)MsgLuckyDiceCoins + 1, window, 0, 0);
                UiText_DrawCharacterAtOffsetFar((s32)MsgLuckyDiceCoins, sprites->winnings_window, digits * 8 + 48, 0);
                UiText_DrawNumberInWindowFar(winnings, digits, sprites->winnings_window, 48, 0);
            }
            if (counter > 0) {
                counter--;
            }
            if (timer > 19 && (gKeysRepeat & KEY_A)) {
                Audio_PlayCue(112);
                if (winnings > 0) {
                    state = 3;
                    counter = 0;
                } else {
                    state = 0;
                    window = UiWindow_CreateFar(22, 16, 8, 4, 6);
                    sprites->help_window = window;
                    UiText_DrawCharacterAtOffsetFar((s32)MsgLuckyDiceCoins + 10, window, 0, 0);
                    UiText_DrawCharacterAtOffsetFar((s32)MsgLuckyDiceCoins + 11, sprites->help_window, 0, 8);
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
                UiText_DrawCharacterAtOffsetFar((s32)MsgLuckyDiceCoins + 10, window, 0, 0);
                UiText_DrawCharacterAtOffsetFar((s32)MsgLuckyDiceCoins + 11, sprites->help_window, 0, 8);
                reset = 1;
            }
            counter++;
        } else if (state == 0 && frame > 17) {
            (void)gKeysRepeat;
            (void)gKeysRepeat;
            if (gKeysHeld & KEY_UP) {
                work->particles[0].y += 0x40000;
                work->particles[1].y += 0x40000;
            }
            if (gKeysHeld & KEY_DOWN) {
                work->particles[0].y += -0x40000;
                work->particles[1].y += -0x40000;
            }
            if (work->particles[0].y < 0x200000) {
                work->particles[0].y = 0x200000;
            }
            if (work->particles[0].y > 0xa00000) {
                work->particles[0].y = 0xa00000;
            }
            if (work->particles[1].y < 0x200000) {
                work->particles[1].y = 0x200000;
            }
            if (work->particles[1].y > 0xa00000) {
                work->particles[1].y = 0xa00000;
            }
            if (gGameState.coins < (u32)(Party_GetAverageLevelFar() * 10)) {
                UiWork_FinalizeFar(sprites->help_window, 1);
                break;
            }
            if (gKeysRepeat & KEY_B) {
                Audio_PlayCue(113);
                UiWork_FinalizeFar(sprites->help_window, 1);
                break;
            }
            if (gKeysRepeat & KEY_A) {
                Audio_PlayCue(0x12e);
                UiWork_FinalizeFar(sprites->help_window, 1);
                state = 1;
                bet = Party_GetAverageLevelFar() * 10;
                if ((u32)bet > gGameState.coins) {
                    bet = gGameState.coins;
                }
                Party_AdjustCoinsFar(-bet);
                UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coins_window, 0, 0);
                for (i = 0; i != 2; i++) {
                    die = &work->particles[i];
                    if (i > 1) {
                        die->z = -0xb40000;
                        die->y = 0x600000;
                    }
                    die->velocity_x = ((Random16() & 127) - 64) << 12;
                    die->velocity_y = ((Random16() & 15) + 48) << 13;
                    die->velocity_z = (((Random16() & 63) + 140) << 12) + die->y / 6;
                    die->variant = Random16() % 0x4800;
                    spin[i] = 300;
                }
            }
        }

        if (reset == 1) {
            work->particles[0].x = 0x280000;
            work->particles[0].y = 0x600000;
            work->particles[0].z = -0xa00000;
            work->particles[1].x = -0x280000;
            work->particles[1].y = 0x600000;
            work->particles[1].z = -0xa00000;
            faces[0] = Random16() % 6;
            do {
                faces[1] = Random16() % 6;
            } while (faces[0] == faces[1]);
        }

        for (i = 0; i != 2; i++) {
            order[i] = i;
        }
        for (i = 1; i < 2; i++) {
            key = order[i];
            for (j = i - 1; j >= 0 && work->particles[order[j]].z > work->particles[key].z; j--) {
                order[j + 1] = order[j];
            }
            order[j + 1] = key;
        }

        for (i = 0; i != 2; i++) {
            s32 px;
            s32 py;

            die = &work->particles[order[i]];
            Render_ProjectPoint(die, proj);
            if (proj[2] < 0x15e) {
                proj[2] = 0x15e;
            }
            if (proj[2] > 0x28a) {
                proj[2] = 0x28a;
            }
            depth[order[i]] = proj[2] - 0x15e;
            d = 0x800 / (depth[order[i]] / 2 + 128);
            px = proj[0] - d;
            py = proj[1] - d;
            if (py + 96 > -16) {
                sprites->obj[count].attr01 = ((order[i] << 25) | (((px + 0x278) & 0x1ff) << 16) | ((py + 0x160) & 255)) | 0x40002300;
                sprites->obj[count].attr2 = (Data_080f5408[die->variant / 1024] << 3) | 0x400;
                count++;
            }
        }

        if (state == 0) {
            for (i = 0; i != 2; i++) {
                Render_ProjectPoint(&work->particles[i], proj);
                x = proj[0] - 0x800 / (depth[order[i]] / 2 + 128);
                y = proj[1] + 0x300 / (depth[order[i]] / 2 + 128);
                sprites->obj[count].attr01 = ((((x + 0x278) & 0x1ff) << 16) | ((y + 0x160) & 255)) | 0x80002000;
                if (i == 1) {
                    sprites->obj[count].attr01 |= 0x10000000;
                }
                sprites->obj[count].attr2 = 0x958;
                count++;
            }
        }

        for (i = 0; i != 2; i++) {
            point[1] = 0;
            point[0] = work->particles[i].x;
            point[2] = work->particles[i].z;
            Render_ProjectPoint(point, proj);
            if (proj[2] < 0x15e) {
                proj[2] = 0x15e;
            }
            if (proj[2] > 0x28a) {
                proj[2] = 0x28a;
            }
            depth[2 + i] = proj[2] - 0x15e;
            depth[2 + i] += work->particles[i].y >> 16;
            if (proj[1] + 88 > -16) {
                sprites->obj[count].attr01 = ((((proj[0] + 0x270) & 0x1ff) << 16) | ((i + 2) << 25)) | ((proj[1] + 0x158) & 255) | 0x40002300;
                sprites->obj[count].attr2 = 0x890;
                count++;
            }
        }

        if (state == 1) {
            s32 stopped;

            stopped = 0;
            for (i = 0; i != 2; i++) {
                die = &work->particles[i];
                die->x += die->velocity_x;
                die->y += die->velocity_y;
                die->z += die->velocity_z;
                for (j = 0; j != 2; j++) {
                    other = &work->particles[j];
                    if (i != j) {
                        dx = (die->x - other->x) >> 16;
                        dy = (die->y - other->y) >> 16;
                        dz = (die->z - other->z) >> 16;
                        dist = dx * dx + dy * dy + dz * dz;
                        if (dist <= 0x9c3) {
                            dist = Iwram_Sqrt(dist);
                            die->velocity_x += (dx << 15) / dist;
                            die->velocity_y += (dy << 15) / dist;
                            die->velocity_z += (dz << 15) / dist;
                        }
                    }
                }
                if (die->x < -0xc80000) {
                    die->x = -0xc80000;
                    die->velocity_x = -die->velocity_x * 48 / 64;
                }
                if (die->x > 0xc80000) {
                    die->x = 0xc80000;
                    die->velocity_x = -die->velocity_x * 48 / 64;
                }
                if (die->z < -0x960000) {
                    die->z = -0x960000;
                    die->velocity_z = -die->velocity_z * 48 / 64;
                }
                if (die->z > die->y + 0xb40000) {
                    die->z = die->y + 0xb40000;
                    if (die->velocity_z > 0) {
                        die->velocity_y += die->velocity_z / 2;
                        die->velocity_z = -die->velocity_z * 48 / 64;
                    }
                }
                die->velocity_y += -0x8000;
                if (die->y <= 0) {
                    die->y = 0;
                    die->velocity_y = -die->velocity_y * 48 / 64;
                    if ((Random16() & 3) == 0) {
                        speed = Iwram_Sqrt((die->velocity_x >> 8) * (die->velocity_x >> 8) + (die->velocity_z >> 8) * (die->velocity_z >> 8)) * 56 / 64;
                        angle = Random16() & 0xffff;
                        die->velocity_x = speed * Trig_Sin(angle) >> 8;
                        die->velocity_z = speed * Trig_Cos(angle) >> 8;
                    } else {
                        die->velocity_x = die->velocity_x * 56 / 64;
                        die->velocity_z = die->velocity_z * 56 / 64;
                    }
                    if (spin[i] > 19) {
                        spin[i] -= 20;
                    }
                    if (die->velocity_y < 0x30000) {
                        die->velocity_x = 0;
                        die->velocity_y = 0;
                        die->velocity_z = 0;
                        spin[i] = 0;
                        n = die->variant;
                        switch (n / 1024 % 3) {
                        case 1:
                            die->variant = n + 0x800;
                            break;
                        case 2:
                            die->variant = n + 0x400;
                            break;
                        }
                    }
                }
                die->velocity_x = die->velocity_x * 63 / 64;
                die->velocity_y = die->velocity_y * 63 / 64;
                die->velocity_z = die->velocity_z * 63 / 64;
                die->variant += spin[i];
                if (die->variant > 0x47ff) {
                    die->variant -= 0x4800;
                }
                if (spin[i] > 0) {
                    spin[i]--;
                }
                if (die->velocity_x == 0 && die->velocity_y == 0 && die->velocity_z == 0 && die->y == 0) {
                    results[i] = die->variant / 0xc00;
                    for (j = 0; j != 6; j++) {
                        if (die->z > Data_080f541a[j] << 16) {
                            results[i + 2] = j;
                            break;
                        }
                    }
                    stopped++;
                }
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
