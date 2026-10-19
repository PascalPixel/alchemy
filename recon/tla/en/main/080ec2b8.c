// First draft of Func_080ec2b8, written from the listing: picks the world map
// place for the cursor. The nearest shown place (its flag-entry state not 0 or
// 15, its flag 0x180 + n set) to the cursor wins; when the cursor is already
// on it (distance squared at most 4) it steps by step through the table to
// the next shown place, wrapping at both ends, giving up after two wraps.
// With no place shown, Func_080ec1d0 gives the leader's place.
//
// Differ 48 of 216 (permuter, 5 minutes, found the work pointer). Left: in
// the first loop the native walks the table with a pointer in r5 and copies
// it to r7 for the body, keeping the flag number in r6; this indexes it,
// giving r6 and r7 and the flag in r5 (a place++ walk is differ 196). The
// work's address loads after the calls in the native, before them here.
#include "types.h"

// A flag entry as the world map reads it: its world position.
struct WorldMapPlace {
  u8 flag;
  u8 value;
  s16 unk02;
  s16 x;
  s16 z;
  u8 unk08[12];
};

struct WorldMapWork {
  u16 vramBlock;
  u16 windowSlot;
  s16 xFraction;
  s16 x;
  s16 zFraction;
  s16 z;
  u8 unk0c[0x638];
  s32 unk644;
};

extern struct WorldMapWork gWorldMapWork;
extern s16 Data_0202a648[2];

s32 EventRuntime_GetControlledOwner(void);
struct WorldMapPlace *FlagEntries_GetTable(void);
u32 Func_080ec298(struct WorldMapPlace *place);
s32 GameFlag_Test(s32 flag);
void Func_080ec1d0(s32 *screenX, s32 *screenY);

// The place's position on the world map screen.
#define PLACE_SCREEN_X(place) ((place)->x * 853 / 16384 - 269)
#define PLACE_SCREEN_Y(place) ((place)->z * 640 / 16384 - 112)

void Func_080ec2b8(s32 *screenX, s32 *screenY, s32 step) {
  struct WorldMapPlace *places;
  struct WorldMapPlace *place;
  struct WorldMapPlace *next;
  s32 best;
  s32 bestDistance;
  s32 cursorX;
  s32 cursorY;
  s32 count;
  s32 wraps;
  struct WorldMapWork *work = &gWorldMapWork;

  EventRuntime_GetControlledOwner();
  places = FlagEntries_GetTable();
  bestDistance = 0x4000000;
  best = -1;
  cursorX = work->x;
  cursorY = work->z;
  if (work->unk644 != 0) {
    cursorX = work->unk644 >> 16;
    cursorY = Data_0202a648[1];
  }
  for (count = 0; count < 128 && places[count].flag != 0; count++) {
    s32 flag;
    u32 state;

    place = &places[count];
    flag = place->flag + 0x180;
    state = Func_080ec298(place);

    if (state != 0 && state != 15 && GameFlag_Test(flag)) {
      s32 sx = PLACE_SCREEN_X(place);
      s32 sy = PLACE_SCREEN_Y(place);
      s32 dx = sx - cursorX;
      s32 dy = sy - cursorY;
      s32 distance = dx * dx + dy * dy;

      if (bestDistance > distance) {
        bestDistance = distance;
        best = count;
      }
    }
  }
  if (best == -1) {
    Func_080ec1d0(screenX, screenY);
    return;
  }
  place = &places[best];
  if (bestDistance <= 4) {
    wraps = 0;
    do {
      best += step;
      if (best < 0) {
        best = count - 1;
        wraps++;
      } else {
        next = &places[best];
        if (next->flag == 0) {
          best = 0;
          wraps++;
        } else {
          s32 flag = next->flag + 0x180;
          u32 state = Func_080ec298(next);

          if (state != 0 && state != 15 && GameFlag_Test(flag)) {
            place = next;
            break;
          }
        }
      }
    } while (wraps <= 1);
  }
  *screenX = PLACE_SCREEN_X(place);
  *screenY = PLACE_SCREEN_Y(place);
}
