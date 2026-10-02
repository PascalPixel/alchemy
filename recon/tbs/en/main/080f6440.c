/* NONMATCHING: alchemy drafts scores 6025, 234 differing instructions of
 * 1731 (was 11572 and 528). The frame is the listing's 40 bytes now.
 * Found: the line check reads the symbol in each of its three branches (the
 * shared modulo call and cell read in the listing are those three tails
 * merged after register allocation, and the row offset is used twice per
 * branch, which keeps it an offset rather than a row pointer); its hit count,
 * mismatch flag and first symbol are declared in their blocks; the pad is
 * read twice before the pressed bits are stored, the second read masked into
 * a variable; n is cleared before blend is set. Earlier: the object buffer
 * words are volatile; a row stop count is a signed 8-bit bitfield; the fill
 * loops count with !=; the prizes live in a struct whose label is 12 bytes
 * into gCell, written here as gReelSave, which needs that label in
 * recon/tbs/sym_ewram.s on adoption.
 * Remaining: the listing spills the hit count and keeps the mismatch flag in
 * r9, the column in r10 and the bet address in r11 where this draft spills
 * the flag and keeps the count in r11, the column in r8, the bet address in
 * r9 (coins and the bet address are swapped the same way in state 0); the
 * build_objects join stubs follow from that. Messages other than MsgSlotsBet
 * still need names.
 * Measured since (global allocation order from the .greg dump): the draft
 * allocates column, line pointer, bet address, hit count in that order and
 * the flag last; the listing needs line pointer (r8), column (r10), flag
 * (r9), bet address (r11), with the hit count left over. The listing's
 * held-marker loop also copies each row's held byte into r10, the column's
 * register, before testing it, so that variable is live there while the
 * spins address sits in r8; holding the byte in col in the draft is folded
 * away and changes nothing. One flag for all-held and mismatch, the count
 * shared with the settled count, and function-scope declarations were each
 * tried and are worse or equal. */
#include "TYPES.H"
#include "DMA.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"
#include "IO_REG.H"
#include "UI.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFFECT_WORK.H"

/* Per-frame driver for the five-reel symbol minigame.
 *
 * One call advances the whole screen: it samples the pad, runs the small
 * state machine that owns betting, spinning, holding and paying out, then
 * rebuilds the complete 128-entry object buffer and hands it to DMA3 for the
 * next vblank.  Each reel is a 21-symbol ring scrolled sixteen units per
 * cell; the seven "lines" tested once every reel has settled are the five
 * columns plus the two diagonals, and how many of them count is gated by the
 * current bet. */

void UiWork_FinalizeFar(s32 window, s32 style);
void UiNumber_DrawAt(s32 value, s32 digits, s32 window, s32 x, s32 y);
s32 PartyInventory_RemoveFar(s32 item);
s32 PartyInventory_CountItemFar(s32 item);
void AudioCommand_PlayFar(s32 command);

struct ReelRow {
    s32 pos;      /* ring position, sixteen units per symbol, wraps at 336 */
    u8 cell[21];  /* symbol ring */
    u8 held;      /* row is held by the player */
    s32 stop : 8; /* frames left before the row settles, -1 while free */
};

struct ReelObject {
    volatile u32 attr01;
    volatile u32 attr2;
};

struct ReelWork {
    struct ReelRow row[5];
    s32 state;
    s32 cursor;       /* 0..4 pick a row, 5 picks the lever */
    s32 spins;
    s32 bet;
    u16 keys;         /* pad seen last frame */
    u16 dir;          /* direction bits, cleared while the repeat runs */
    u16 pressed;      /* newly pressed bits */
    u16 repeat;
    u8 unknown_0a4[4];
    s32 timer;
    s32 line[7];      /* per-line win flags */
    struct ReelObject obj[128];
    s32 window;
    s32 sub_window;
    u8 unknown_4d0[8];
    u16 scanline_offsets[160];
    s32 phase;        /* which prompt the window currently shows */
};

struct ReelSave {
    u8 unknown_000[0x120];
    s8 won_prizes[16];
};

extern u8 gBattleFxWork[];
extern volatile u32 gKeysHeld;
extern u8 gDebugPaused;
extern char MsgSlotsBet;
extern struct ReelSave gReelSave;
extern const u8 Data_080f870c[];
extern const u8 Data_080f8712[];
extern const u8 Data_080f871a[];
extern const u8 Data_080f8728[];

void ReelGame_RunFrame(void)
{
    struct ReelWork *work;
    struct BattleEffectWork *fx;
    volatile u16 *dma;
    s32 n;
    s32 blend;
    u16 pad;
    u32 held;
    s32 coins;
    s32 all;
    s32 cnt;
    s32 col;
    s32 v;
    s32 x;
    s32 y;
    s32 k;
    s32 i;

    work = ((struct ReelWork **)gBattleFxWork)[6];
    fx = ((struct BattleEffectWork **)gBattleFxWork)[0];
    n = 0;
    blend = 0x400;

    Random16();

    dma = REG_DMA0;
    dma[5] &= 0xc5ff;
    dma[5] &= 0x7fff;
    dma[5];
    Dma_Set(work->scanline_offsets, (void *)0x04000054, 0xa2600001,
            (volatile u32 *)dma);

    pad = gKeysHeld;
    held = gKeysHeld & 0xf0;
    work->pressed = pad & ~work->keys;
    work->dir = held;
    if ((work->keys & 0xf0) == work->dir) {
        if (work->repeat > 12)
            work->repeat = 12;
        if (work->repeat == 0) {
            work->repeat = 4;
        } else {
            work->repeat--;
            work->dir = n;
        }
    } else {
        work->repeat = 12;
    }
    work->keys = pad;

    if (gDebugPaused != 0)
        goto build_objects;

    if (work->state == 0) {
        coins = PartyInventory_CountItemFar(228);
        UiNumber_DrawAt(coins - work->bet, 2, work->sub_window, 64, 0);
        UiNumber_DrawAt(work->bet, 2, work->sub_window, 64, 8);
        if (work->pressed & 2) {
            work->state = 10;
            gReelSave.won_prizes[0] = 254;
            UiWork_FinalizeFar(work->window, 1);
            goto build_objects;
        }
        if (work->pressed & 0x40) {
            if (work->bet <= 3 && coins > work->bet) {
                work->bet++;
                AudioCommand_PlayFar(111);
            } else {
                AudioCommand_PlayFar(113);
            }
        }
        if (work->pressed & 0x80) {
            if (work->bet > 1) {
                work->bet--;
                AudioCommand_PlayFar(111);
            } else {
                AudioCommand_PlayFar(113);
            }
        }
        REG_BLDCNT = 0x3fd0;
        REG_BLDALPHA = 0x10;
        if (work->pressed & 1) {
            work->state = 1;
            fx->frame = 0;
            UiWork_FinalizeFar(work->window, 1);
            for (k = 0; k != work->bet; k++)
                PartyInventory_RemoveFar(228);
            UiWork_FinalizeFar(work->sub_window, 1);
            AudioCommand_PlayFar(0x130);
        }
    } else if (work->state == 5) {
        all = 0;
        work->timer++;
        for (k = 0; k != 5 && work->row[k].held != 0; k++)
            ;
        if (k == 5)
            all = 1;
        if (work->pressed & 1) {
            work->timer = 0;
            fx->frame = 0;
            if (work->spins == 4) {
                work->spins = 0;
                work->cursor = 0;
                work->state = 0;
                for (i = 0; i != 5; i++) {
                    work->row[i].held = 0;
                    work->row[i].stop = -1;
                }
            } else if (work->cursor <= 4) {
                AudioCommand_PlayFar(0x131);
                work->row[work->cursor].held ^= 1;
            } else if (all == 0) {
                AudioCommand_PlayFar(0x130);
                work->state = 1;
                work->cursor = 0;
                for (i = 0; i != 5; i++)
                    work->row[i].stop = -1;
                work->spins++;
            } else {
                AudioCommand_PlayFar(113);
            }
        } else {
            if (work->dir & 0x10) {
                work->cursor = (work->cursor + 1) % 6;
                AudioCommand_PlayFar(111);
            }
            if (work->dir & 0x20) {
                work->cursor = (work->cursor + 5) % 6;
                AudioCommand_PlayFar(111);
            }
        }

        if (work->state == 5) {
            if (work->cursor == 5) {
                if (all != 0) {
                    if (work->phase != 1 && work->phase != 2) {
                        UiWork_FinalizeFar(work->window, 1);
                        work->window = UiWindow_CreateFar(11, 0, 19, 4, 6);
                        UiText_DrawCharacterAtOffsetFar(0x912, work->window, 0, 0);
                        work->phase = 1;
                    } else if (work->phase == 1) {
                        UiText_DrawCharacterAtOffsetFar(0x913, work->window, 0, 8);
                        work->phase = 2;
                    }
                } else {
                    if (work->phase != 3) {
                        UiWork_FinalizeFar(work->window, 1);
                        work->window = UiWindow_CreateFar(16, 0, 14, 3, 6);
                        UiText_DrawCharacterAtOffsetFar(0x90f, work->window, 0, 0);
                    }
                    work->phase = 3;
                }
            } else if (work->row[work->cursor].held == 0) {
                if (work->phase != 4) {
                    UiWork_FinalizeFar(work->window, 1);
                    work->window = UiWindow_CreateFar(23, 0, 7, 3, 6);
                    UiText_DrawCharacterAtOffsetFar(0x90d, work->window, 0, 0);
                }
                work->phase = 4;
            } else {
                if (work->phase != 5) {
                    UiWork_FinalizeFar(work->window, 1);
                    work->window = UiWindow_CreateFar(23, 0, 7, 3, 6);
                    UiText_DrawCharacterAtOffsetFar(0x90e, work->window, 0, 0);
                }
                work->phase = 5;
            }
        } else {
            UiWork_FinalizeFar(work->window, 1);
        }
    } else if (work->state == 2) {
        work->timer++;
        blend = 0;
        if (work->timer == 60) {
            work->state = 3;
            AudioCommand_PlayFar(93);
            work->timer = 0;
            REG_BLDCNT = 0x3f44;
            REG_BLDALPHA = 0x1010;
            fx->transfer_mode = 2;
            fx->transfer_value = 75;
        }
    } else if (work->state == 3) {
        work->timer++;
        blend = 0;
        if (work->pressed & 1) {
            work->state = 10;
            AudioCommand_PlayFar(112);
        }
    } else if (work->state == 11) {
        if (work->phase == 0) {
            work->phase = 1;
            UiText_DrawCharacterAtOffsetFar(0x90c, work->window, 0, 8);
        }
        if (work->pressed & 1) {
            work->state = 5;
            work->phase = 0;
            AudioCommand_PlayFar(112);
            UiWork_FinalizeFar(work->window, 1);
        }
    } else if (work->state == 20) {
        work->timer++;
        if (work->timer == 45)
            work->state = 10;
    } else if (work->state != 10) {
        /* Reels are turning. */
        if (work->timer == 4) {
            work->window = UiWindow_CreateFar(18, 17, 12, 3, 6);
            UiText_DrawCharacterAtOffsetFar(0x90a, work->window, 0, 0);
        }
        if (work->timer == 16)
            AudioCommand_PlayFar(0x132);
        if (work->timer > 56) {
            if (fx->frame > 31 || (work->pressed & 0x100)) {
                fx->frame = 0;
                for (i = 0; i != 5; i++) {
                    if (work->row[i].held == 0 && work->row[i].stop == -1) {
                        work->row[i].stop = (Random16() & 3) + 4;
                        AudioCommand_PlayFar(0x133);
                        break;
                    }
                }
            }
        }

        for (i = 0; i != 5; i++) {
            if (work->row[i].stop > 0)
                work->row[i].stop--;
        }

        cnt = 0;
        for (i = 0; i != 5; i++) {
            if (work->row[i].held == 1 ||
                (work->row[i].stop == 0 && (work->row[i].pos & 15) == 8))
                cnt++;
        }

        if (cnt == 5) {
            s32 hits = 0;

            for (col = 0; col != 7; col++) {
                s32 mixed;
                s32 found;

                work->line[col] = 0;
                mixed = 0;
                found = -1;
                if (col > 3 - work->bet && col < work->bet + 3) {
                    for (i = 0; i != 5; i++) {
                        s32 sym;

                        if (col == 0)
                            sym = work->row[i].cell[(i - work->row[i].pos / 16 + 22) % 21];
                        else if (col == 6)
                            sym = work->row[i].cell[(-i - work->row[i].pos / 16 + 26) % 21];
                        else
                            sym = work->row[i].cell[(col - work->row[i].pos / 16 + 21) % 21];
                        if (sym != 5) {
                            if (found == -1)
                                found = sym;
                            else if (found != sym)
                                mixed = 1;
                        }
                    }
                    if (mixed == 0) {
                        work->line[col] = 1;
                        gReelSave.won_prizes[0 + hits] = Data_080f870c[found];
                        hits++;
                    }
                }
            }
            work->timer = 0;
            if (hits != 0) {
                gReelSave.won_prizes[0 + hits] = -1;
                work->state = 2;
                AudioCommand_PlayFar(171);
                fx->transfer_mode = 1;
                fx->transfer_value = 0;
                REG_BLDCNT = 0;
                UiWork_FinalizeFar(work->window, 1);
            } else {
                work->state = 11;
                work->phase = 0;
                UiWork_FinalizeFar(work->window, 1);
                work->window = UiWindow_CreateFar(3, 16, 24, 4, 6);
                UiText_DrawCharacterAtOffsetFar(0x90b, work->window, 0, 0);
                if (work->spins == 4) {
                    s32 left = PartyInventory_CountItemFar(228);
                    s32 window;
                    s32 message;

                    if (left > 0)
                        work->state = 20;
                    else
                        work->state = 20;
                    if (work->bet > left)
                        work->bet = left;
                    work->spins = 0;
                    work->cursor = 0;
                    for (i = 0; i != 5; i++) {
                        work->row[i].held = 0;
                        work->row[i].stop = -1;
                    }
                    fx->transfer_mode = 1;
                    fx->transfer_value = 0;
                    REG_BLDCNT = 0;
                    window = UiWindow_CreateFar(18, 0, 12, 4, 6);
                    work->sub_window = window;
                    message = (s32)&MsgSlotsBet;
                    UiText_DrawCharacterAtOffsetFar(message, window, 0, 8);
                    UiText_DrawCharacterAtOffsetFar(message - 1, work->sub_window, 0, 0);
                }
            }
        }

        if (work->state == 1) {
            for (i = 0; i != 5; i++) {
                if (work->row[i].held == 0) {
                    if (work->row[i].stop != 0 || (work->row[i].pos & 15) != 8)
                        work->row[i].pos += 8;
                    if (work->row[i].pos == 336)
                        work->row[i].pos = 0;
                }
            }
        }
        (fx->frame)++;
        work->timer++;
    }

build_objects:
    if (work->state == 5) {
        x = work->cursor * 36 + 36;
        y = 128;
        v = 0;
        if ((work->timer & 15) <= 7)
            v = 1;
        if (work->cursor == 5) {
            x = 208;
            y = 32;
        }
        work->obj[n].attr01 = ((x - 12) << 16) | blend | (y + 8) | 0x80006000;
        work->obj[n].attr2 = (v << 4) + 0x2b0;
        n++;
        work->obj[n].attr01 = ((x + 12) << 16) | blend | (y + 8) | 0x90006000;
        work->obj[n].attr2 = (v << 4) + 0x2b0;
        n++;
        work->obj[n].attr01 = (x << 16) | blend | y | 0x80002000;
        work->obj[n].attr2 = 0x1f0;
        n++;
    }

    if (work->state == 3) {
        for (i = 0; i != 8; i++) {
            struct EffectStep *p = &fx->particles[i];

            work->obj[n].attr01 = (*(s16 *)((u8 *)&p->x + 2) << 16) | blend |
                                  ((*(s16 *)((u8 *)&p->y + 2) + 256) & 255) | 0x80000000;
            work->obj[n].attr2 = ((Data_080f8712[i] << 4) + 0x370) | 0xf000;
            p->y += p->velocity_y;
            p->velocity_y += 0x4000;
            if (work->timer % 256 == i * 4 + 200) {
                p->velocity_y = 0x60000;
                p->variant = 0;
            }
            if (p->y > 0x400000) {
                p->y = 0x400000;
                if (p->variant <= 1)
                    p->velocity_y = -p->velocity_y / 2;
                p->variant++;
            }
            n++;
        }
    }

    for (k = 0; k != 14; k++) {
        work->obj[n].attr01 = (Data_080f871a[k] << 16) | blend | Data_080f8728[k] | 0x80006000;
        if (k <= 3)
            work->obj[n].attr2 = 0x4e0;
        else
            work->obj[n].attr2 = 0x4e8;
        n++;
    }

    for (i = 0; i != 5; i++) {
        if (work->row[i].held == 0) {
            work->obj[n].attr01 = blend | ((i * 36 + 32) << 16) | 0x8000207c;
            work->obj[n].attr2 = 0x460;
        } else {
            work->obj[n].attr01 = blend | ((i * 36 + 32) << 16) | 0x8000207c;
            work->obj[n].attr2 = 0x480;
        }
        n++;
    }

    for (i = 0; i != 5; i++) {
        work->obj[n].attr01 = blend | ((i * 16 + 32) << 16) | 0x80006003;
        if (i == work->spins)
            work->obj[n].attr2 = (i * 32 + 0x210) | 0x400;
        else
            work->obj[n].attr2 = (i * 32 + 0x220) | 0x400;
        n++;
    }

    for (col = 0; col != 7; col++) {
        work->obj[n].attr01 = (((0x204 - ((col & 1) << 3)) & 0x1ff) << 16) |
                              blend | (col * 16 + 5) | 0x80002000;
        if (col > 3 - work->bet && col < work->bet + 3)
            work->obj[n].attr2 = 0x5d0;
        else
            work->obj[n].attr2 = 0x510;
        n++;
    }

    for (i = 0; i != 5; i++) {
        for (col = 0; col != 7; col++) {
            work->obj[n].attr01 = (col * 16 + work->row[i].pos % 16 + 4) |
                                  ((i * 32 + 40) << 16) | 0x80006000;
            work->obj[n].attr2 =
                (work->row[i].cell[(col - work->row[i].pos / 16 + 21) % 21] << 4) | 0x800;
            n++;
        }
    }

    y = 40;
    if (work->state == 1) {
        if (work->timer <= 47)
            y = ((Trig_Sin(work->timer * 682) << 6) >> 16) + 40;
        else if (work->timer <= 55)
            y = ((Trig_Sin((work->timer << 12) - 0x30000) << 2) >> 16) + 40;
    }
    work->obj[n].attr01 = y | blend | 0x80d06000;
    work->obj[n].attr2 = 0x500;
    n++;

    for (k = 0; k != 8; k++) {
        work->obj[n].attr01 = blend | (k * 16 + 12) | 0x80ce6000;
        if (k == 0)
            work->obj[n].attr2 = 0x540;
        else if (k == 1)
            work->obj[n].attr2 = 0x550;
        else if (k == 6)
            work->obj[n].attr2 = 0x570;
        else if (k == 7)
            work->obj[n].attr2 = 0x580;
        else
            work->obj[n].attr2 = 0x560;
        n++;
    }

    while (n != 128) {
        work->obj[n].attr01 = 0x40f02000;
        work->obj[n].attr2 = 0;
        n++;
    }

    Dma_Set(work->obj, (void *)0x07000000, (n * 2) | 0x84000000, REG_DMA3);
}
