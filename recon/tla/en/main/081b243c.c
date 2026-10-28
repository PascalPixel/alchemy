// Draft, differ 169 of 1707 (tla-en): the slot machine's VBlank task, The
// Broken Seal's LuckyWheels_Update (games/tbs/src/overworld/lucky_wheels.c)
// in its patched form (the hint windows and phases), on this game's work: the
// sprite count lives in the work (0xa8) for Func_081b3294, which restarts the
// scanline DMA and copies the sprites; the sprite words are volatile, as The
// Broken Seal's OAM shadow; the held keys are read twice through a volatile
// word, as the native's two loads show (514 against 551 plainly). Measured
// in order from 494 under K2's -mthumb-slow-loads: the messages by their
// linker names (MsgSlotsStopReels to MsgSlotsCannotPull), which the native
// loads from the pool where a number is built in three instructions: 417; the
// wheel symbols' first word computed before its store's index, through v:
// 233 (the frame then matches, 40 bytes); its x, y and attribute bits ORed in
// at the store, v holding only the column's x: 189; the state test after the
// debug block through k (k = work->state == 5): 169. That k is a forced
// temporary: it needs a FAKEMATCH(temporary) tag if this is adopted (as a
// plain if, 189). Left: register rotations in the wheel symbols' loop, two
// reload blocks the native keeps apart where this shares one (after the
// spinning state's timer and the choosing state's A test), which moves the
// literal pools, and scheduling rows. permute (30 minutes from 169) found
// only devices (a volatile local, 164).
#include "types.h"
#include "io_reg.h"
#include "input.h"
#include "frame.h"
#include "effect_step.h"
#include "ram_buffer.h"

struct Reel {
  s32 position;
  u8 symbols[21];
  u8 held;
  s8 stop;
};

struct ReelSprite {
  volatile u32 attr01;
  volatile u32 attr2;
};

struct ReelWork {
  struct Reel reels[5];
  s32 state;
  s32 cursor;
  s32 spins;
  s32 bet;
  u16 keys;
  u16 direction;
  u16 pressed;
  u16 repeat;
  s32 timer;
  s32 spriteCount;
  s32 winningLines[7];
  struct ReelSprite obj[128];
  s32 window;
  s32 subWindow;
  u8 unk4d0[8];
  u16 scanlineOffsets[160];
  s32 phase;
};

struct ReelEffectWork {
  struct EffectStep particles[8];
  u8 unk00e0[0x7780 - 0xe0];
  s32 transferMode;
  s32 transferValue;
  u8 unk7788[4];
  s32 frame;
};

struct ReelSave {
  u8 unk000[0x140];
  s8 wonPrizes[16];
};

extern u8 Data_0200024c[];
extern const u8 gReelGame_SymbolPrizes[];
extern const u8 gReelGame_TitleLetterTiles[];
extern const u8 gReelGame_MachineSpriteX[];
extern const u8 gReelGame_MachineSpriteY[];

u32 Random16(void);
s32 Trig_Sin(s32 angle);
void Sound_PlayFar(s32 cue);
s32 PartyInventory_CountItemFar(s32 item);
void PartyInventory_RemoveFar(s32 item);
void UiText_DrawNumberInWindowFar(s32 value, s32 width, s32 window, s32 x,
                                  s32 y);
void UiWork_FinalizeFar(s32 window, s32 mode);
s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 flags);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);

extern char MsgSlotsBet[];
extern char MsgSlotsStopReels[];
extern char MsgSlotsChooseHeld[];
extern char MsgSlotsPullLever[];
extern char MsgSlotsHold[];
extern char MsgSlotsRelease[];
extern char MsgSlotsStart[];
extern char MsgSlotsAllHeld[];
extern char MsgSlotsCannotPull[];

void Func_081b243c(void) {
  struct ReelWork *work;
  struct ReelEffectWork *fx;
  s32 objMode;
  u16 keys;
  u32 dpad;
  s32 tickets;
  s32 allHeld;
  s32 stoppedReels;
  s32 col;
  s32 v;
  s32 x;
  s32 y;
  s32 lever;
  s32 k;
  s32 i;

  work = (struct ReelWork *)Ram_HeapSlots->canvasMap;
  fx = (struct ReelEffectWork *)Ram_HeapSlots->battleFxWork[0];
  objMode = 0x400;
  work->spriteCount = 0;

  Random16();

  keys = *(volatile u32 *)&gInput.held;
  dpad = *(volatile u32 *)&gInput.held & 0xf0;
  work->pressed = keys & ~work->keys;
  work->direction = dpad;
  if ((work->keys & 0xf0) == work->direction) {
    if (work->repeat > 12)
      work->repeat = 12;
    if (work->repeat == 0) {
      work->repeat = 4;
    } else {
      work->repeat--;
      work->direction = 0;
    }
  } else {
    work->repeat = 12;
  }
  work->keys = keys;

  if (gDebugPaused == 0) {
    if (work->state == 0) {
      tickets = PartyInventory_CountItemFar(228);
      UiText_DrawNumberInWindowFar(tickets - work->bet, 2, work->subWindow, 64,
                                   0);
      UiText_DrawNumberInWindowFar(work->bet, 2, work->subWindow, 64, 8);
      if (work->pressed & KEY_B) {
        struct ReelSave *save = (struct ReelSave *)Data_0200024c;

        work->state = 10;
        save->wonPrizes[0] = -2;
        UiWork_FinalizeFar(work->window, 1);
      } else {
        if (work->pressed & KEY_UP) {
          if (work->bet <= 3 && tickets > work->bet) {
            work->bet++;
            Sound_PlayFar(111);
          } else {
            Sound_PlayFar(113);
          }
        }
        if (work->pressed & KEY_DOWN) {
          if (work->bet > 1) {
            work->bet--;
            Sound_PlayFar(111);
          } else {
            Sound_PlayFar(113);
          }
        }
        REG_BLDCNT = 0x3fd0;
        REG_BLDALPHA = 0x10;
        if (work->pressed & KEY_A) {
          work->state = 1;
          fx->frame = 0;
          UiWork_FinalizeFar(work->window, 1);
          for (k = 0; k != work->bet; k++)
            PartyInventory_RemoveFar(228);
          UiWork_FinalizeFar(work->subWindow, 1);
          Sound_PlayFar(304);
        }
      }
    } else if (work->state == 5) {
      allHeld = 0;
      work->timer++;
      for (k = 0; k != 5 && work->reels[k].held != 0; k++)
        ;
      if (k == 5)
        allHeld = 1;
      if (work->pressed & KEY_A) {
        work->timer = 0;
        fx->frame = 0;
        if (work->spins == 4) {
          work->spins = 0;
          work->cursor = 0;
          work->state = 0;
          for (i = 0; i != 5; i++) {
            work->reels[i].held = 0;
            work->reels[i].stop = -1;
          }
        } else if (work->cursor <= 4) {
          Sound_PlayFar(305);
          work->reels[work->cursor].held ^= 1;
        } else if (allHeld == 0) {
          Sound_PlayFar(304);
          work->state = 1;
          work->cursor = 0;
          for (i = 0; i != 5; i++)
            work->reels[i].stop = -1;
          work->spins++;
        } else {
          Sound_PlayFar(113);
        }
      } else {
        if (work->direction & KEY_RIGHT) {
          work->cursor = (work->cursor + 1) % 6;
          Sound_PlayFar(111);
        }
        if (work->direction & KEY_LEFT) {
          work->cursor = (work->cursor + 5) % 6;
          Sound_PlayFar(111);
        }
      }

      if (work->state == 5) {
        if (work->cursor == 5) {
          if (allHeld != 0) {
            if (work->phase != 1 && work->phase != 2) {
              UiWork_FinalizeFar(work->window, 1);
              work->window = UiWindow_CreateFar(11, 0, 19, 4, 6);
              UiText_DrawCharacterAtOffsetFar((s32)MsgSlotsAllHeld, work->window, 0, 0);
              work->phase = 1;
            } else if (work->phase == 1) {
              UiText_DrawCharacterAtOffsetFar((s32)MsgSlotsCannotPull, work->window, 0, 8);
              work->phase = 2;
            }
          } else {
            if (work->phase != 3) {
              UiWork_FinalizeFar(work->window, 1);
              work->window = UiWindow_CreateFar(16, 0, 14, 3, 6);
              UiText_DrawCharacterAtOffsetFar((s32)MsgSlotsStart, work->window, 0, 0);
            }
            work->phase = 3;
          }
        } else if (work->reels[work->cursor].held == 0) {
          if (work->phase != 4) {
            UiWork_FinalizeFar(work->window, 1);
            work->window = UiWindow_CreateFar(23, 0, 7, 3, 6);
            UiText_DrawCharacterAtOffsetFar((s32)MsgSlotsHold, work->window, 0, 0);
          }
          work->phase = 4;
        } else {
          if (work->phase != 5) {
            UiWork_FinalizeFar(work->window, 1);
            work->window = UiWindow_CreateFar(23, 0, 7, 3, 6);
            UiText_DrawCharacterAtOffsetFar((s32)MsgSlotsRelease, work->window, 0, 0);
          }
          work->phase = 5;
        }
      } else {
        UiWork_FinalizeFar(work->window, 1);
      }
    } else if (work->state == 2) {
      work->timer++;
      objMode = 0;
      if (work->timer == 60) {
        work->state = 3;
        Sound_PlayFar(93);
        work->timer = 0;
        REG_BLDCNT = 0x3f44;
        REG_BLDALPHA = 0x1010;
        fx->transferMode = 2;
        fx->transferValue = 75;
      }
    } else if (work->state == 3) {
      work->timer++;
      objMode = 0;
      if (work->pressed & KEY_A) {
        work->state = 10;
        Sound_PlayFar(112);
      }
    } else if (work->state == 11) {
      if (work->phase == 0) {
        work->phase = 1;
        UiText_DrawCharacterAtOffsetFar((s32)MsgSlotsPullLever, work->window, 0, 8);
      }
      if (work->pressed & KEY_A) {
        work->state = 5;
        work->phase = 0;
        Sound_PlayFar(112);
        UiWork_FinalizeFar(work->window, 1);
      }
    } else if (work->state == 20) {
      work->timer++;
      if (work->timer == 45)
        work->state = 10;
    } else if (work->state != 10) {
      if (work->timer == 4) {
        work->window = UiWindow_CreateFar(18, 17, 12, 3, 6);
        UiText_DrawCharacterAtOffsetFar((s32)MsgSlotsStopReels, work->window, 0, 0);
      }
      if (work->timer == 16)
        Sound_PlayFar(306);
      if (work->timer > 56) {
        if (fx->frame > 31 || (work->pressed & KEY_R)) {
          fx->frame = 0;
          for (i = 0; i != 5; i++) {
            if (work->reels[i].held == 0 && work->reels[i].stop == -1) {
              work->reels[i].stop = (Random16() & 3) + 4;
              Sound_PlayFar(307);
              break;
            }
          }
        }
      }

      for (i = 0; i != 5; i++) {
        if (work->reels[i].stop > 0)
          work->reels[i].stop--;
      }

      stoppedReels = 0;
      for (i = 0; i != 5; i++) {
        if (work->reels[i].held == 1)
          stoppedReels++;
        else if (work->reels[i].stop == 0 &&
                 (work->reels[i].position & 15) == 8)
          stoppedReels++;
      }

      if (stoppedReels == 5) {
        struct ReelSave *save = (struct ReelSave *)Data_0200024c;
        s32 hits = 0;

        for (col = 0; col != 7; col++) {
          s32 mixed;
          s32 found;

          found = -1;
          mixed = 0;
          work->winningLines[col] = 0;
          if (col > 3 - work->bet && col < work->bet + 3) {
            for (i = 0; i != 5; i++) {
              s32 sym;

              if (col == 0)
                sym =
                    work->reels[i]
                        .symbols[(i - work->reels[i].position / 16 + 22) % 21];
              else if (col == 6)
                sym =
                    work->reels[i]
                        .symbols[(-i - work->reels[i].position / 16 + 26) % 21];
              else
                sym = work->reels[i]
                          .symbols[(col - work->reels[i].position / 16 + 21) %
                                   21];
              if (sym != 5) {
                if (found == -1)
                  found = sym;
                else if (found != sym)
                  mixed = 1;
              }
            }
            if (mixed == 0) {
              work->winningLines[col] = 1;
              save->wonPrizes[hits] = gReelGame_SymbolPrizes[found];
              hits++;
            }
          }
        }
        work->timer = 0;
        if (hits != 0) {
          save->wonPrizes[hits] = -1;
          work->state = 2;
          Sound_PlayFar(171);
          fx->transferMode = 1;
          fx->transferValue = 0;
          REG_BLDCNT = 0;
          UiWork_FinalizeFar(work->window, 1);
        } else {
          work->state = 11;
          work->phase = 0;
          UiWork_FinalizeFar(work->window, 1);
          if (work->spins <= 3) {
            s32 message;

            work->window = UiWindow_CreateFar(3, 16, 24, 4, 6);
            message = (s32)MsgSlotsChooseHeld;
            UiText_DrawCharacterAtOffsetFar(message, work->window, 0, 0);
            UiText_DrawCharacterAtOffsetFar(message + 1, work->window, 0, 8);
          }
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
              work->reels[i].held = 0;
              work->reels[i].stop = -1;
            }
            fx->transferMode = 1;
            fx->transferValue = 0;
            REG_BLDCNT = 0;
            window = UiWindow_CreateFar(18, 0, 12, 4, 6);
            work->subWindow = window;
            message = (s32)MsgSlotsBet;
            UiText_DrawCharacterAtOffsetFar(message, window, 0, 8);
            UiText_DrawCharacterAtOffsetFar(message - 1, work->subWindow, 0, 0);
          }
        }
      }

      if (work->state == 1) {
        for (i = 0; i != 5; i++) {
          struct Reel *reel = &work->reels[i];

          if (reel->held == 0) {
            if (reel->stop != 0)
              reel->position += 8;
            else if ((reel->position & 15) != 8)
              reel->position += 8;
            if (reel->position == 336)
              reel->position = 0;
          }
        }
      }
      (fx->frame)++;
      work->timer++;
    }
  }

  k = work->state == 5;
  if (k) {
    x = work->cursor * 36 + 36;
    y = 128;
    v = 0;
    if ((work->timer & 15) <= 7)
      v = 1;
    if (work->cursor == 5) {
      x = 208;
      y = 32;
    }
    work->obj[work->spriteCount].attr01 =
        ((x - 12) << 16) | objMode | (y + 8) | 0x80006000;
    work->obj[work->spriteCount].attr2 = (v << 4) + 0x2b0;
    work->spriteCount++;
    work->obj[work->spriteCount].attr01 =
        ((x + 12) << 16) | objMode | (y + 8) | 0x90006000;
    work->obj[work->spriteCount].attr2 = (v << 4) + 0x2b0;
    work->spriteCount++;
    work->obj[work->spriteCount].attr01 = (x << 16) | objMode | y | 0x80002000;
    work->obj[work->spriteCount].attr2 = 0x1f0;
    work->spriteCount++;
  }

  if (work->state == 3) {
    for (i = 0; i != 8; i++) {
      struct EffectStep *p = &fx->particles[i];

      work->obj[work->spriteCount].attr01 =
          (*(s16 *)((u8 *)&p->x + 2) << 16) | objMode |
          ((*(s16 *)((u8 *)&p->y + 2) + 256) & 255) | 0x80000000;
      work->obj[work->spriteCount].attr2 =
          ((gReelGame_TitleLetterTiles[i] << 4) + 0x370) | 0xf000;
      p->y += p->velocityY;
      p->velocityY += 0x4000;
      if (work->timer % 256 == i * 4 + 200) {
        p->velocityY = 0x60000;
        p->variant = 0;
      }
      if (p->y > 0x400000) {
        p->y = 0x400000;
        if (p->variant <= 1)
          p->velocityY = -p->velocityY / 2;
        p->variant++;
      }
      work->spriteCount++;
    }
  }

  for (k = 0; k != 14; k++) {
    work->obj[work->spriteCount].attr01 =
        (gReelGame_MachineSpriteX[k] << 16) | objMode |
        gReelGame_MachineSpriteY[k] | 0x80006000;
    if (k <= 3)
      work->obj[work->spriteCount].attr2 = 0x4e0;
    else
      work->obj[work->spriteCount].attr2 = 0x4e8;
    work->spriteCount++;
  }

  for (i = 0; i != 5; i++) {
    if (work->reels[i].held == 0) {
      work->obj[work->spriteCount].attr01 =
          objMode | ((i * 36 + 32) << 16) | 0x8000207c;
      work->obj[work->spriteCount].attr2 = 0x460;
    } else {
      work->obj[work->spriteCount].attr01 =
          objMode | ((i * 36 + 32) << 16) | 0x8000207c;
      work->obj[work->spriteCount].attr2 = 0x480;
    }
    work->spriteCount++;
  }

  for (i = 0; i != 5; i++) {
    work->obj[work->spriteCount].attr01 =
        objMode | ((i * 16 + 32) << 16) | 0x80006003;
    if (i == work->spins)
      work->obj[work->spriteCount].attr2 = (i * 32 + 0x210) | 0x400;
    else
      work->obj[work->spriteCount].attr2 = (i * 32 + 0x220) | 0x400;
    work->spriteCount++;
  }

  for (col = 0; col != 7; col++) {
    work->obj[work->spriteCount].attr01 =
        (((0x204 - ((col & 1) << 3)) & 0x1ff) << 16) | objMode |
        (col * 16 + 5) | 0x80002000;
    if (col > 3 - work->bet && col < work->bet + 3)
      work->obj[work->spriteCount].attr2 = 0x5d0;
    else
      work->obj[work->spriteCount].attr2 = 0x510;
    work->spriteCount++;
  }

  for (i = 0; i != 5; i++) {
    struct Reel *reel = &work->reels[i];

    for (col = 0; col != 7; col++) {
      v = col * 16 + reel->position % 16 + 4;
      work->obj[work->spriteCount].attr01 = v | ((i * 32 + 40) << 16) | 0x80006000;
      work->obj[work->spriteCount].attr2 =
          (reel->symbols[(col - reel->position / 16 + 21) % 21] << 4) | 0x800;
      work->spriteCount++;
    }
  }

  lever = 40;
  if (work->state == 1) {
    if (work->timer <= 47)
      lever = ((Trig_Sin(work->timer * 682) << 6) >> 16) + 40;
    else if (work->timer <= 55)
      lever = ((Trig_Sin((work->timer << 12) - 0x30000) << 2) >> 16) + 40;
  }
  work->obj[work->spriteCount].attr01 = lever | objMode | 0x80d06000;
  work->obj[work->spriteCount].attr2 = 0x500;
  work->spriteCount++;

  for (k = 0; k != 8; k++) {
    work->obj[work->spriteCount].attr01 =
        objMode | (k * 16 + 12) | 0x80ce6000;
    if (k == 0)
      work->obj[work->spriteCount].attr2 = 0x540;
    else if (k == 1)
      work->obj[work->spriteCount].attr2 = 0x550;
    else if (k == 6)
      work->obj[work->spriteCount].attr2 = 0x570;
    else if (k == 7)
      work->obj[work->spriteCount].attr2 = 0x580;
    else
      work->obj[work->spriteCount].attr2 = 0x560;
    work->spriteCount++;
  }

  while (work->spriteCount != 128) {
    work->obj[work->spriteCount].attr01 = 0x40f02000;
    work->obj[work->spriteCount].attr2 = 0;
    work->spriteCount++;
  }
}
