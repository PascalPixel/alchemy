#include "types.h"
#include "ram_buffer.h"

// A flag entry as the world map reads it: its world position.
struct WorldMapPlace {
  u8 flag;
  u8 value;
  s16 unk02;
  s16 x;
  s16 z;
  u8 unk08[12];
};

// The world map screen's work, laid out as field/world_map.h has it.
struct WorldMapWork {
  u16 vramBlock;
  u16 windowSlot;
  s32 x; // the cursor's screen column, 16.16 pixels
  s32 z; // the cursor's screen row, 16.16 pixels
  u8 unk0c[0x638];
  s32 unk644;
  s32 unk648;
};

s32 Party_GetLeaderActor(void);
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
  s32 flag;
  u32 state;
  struct WorldMapWork *work = Ram_WorldMapWork;

  Party_GetLeaderActor();
  places = FlagEntries_GetTable();
  bestDistance = 0x4000000;
  best = -1;
  cursorX = work->x >> 16;
  cursorY = work->z >> 16;
  if (work->unk644 != 0) {
    cursorX = work->unk644 >> 16;
    cursorY = work->unk648 >> 16;
  }
  for (count = 0; count < 128; count++) {

    place = &places[count];
    if (place->flag == 0)
      break;
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
          flag = next->flag + 0x180;
          state = Func_080ec298(next);

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

// Moves a list of window sprites.

#include "graphics/render_input.h"

/// Moves every window sprite of a list by dx, dy: the output's position and its
/// queued sprite's OAM x and y.
void Func_080ec484(struct WindowSpriteList *list, s32 dx, s32 dy) {
  struct WindowSprite *output;
  struct QueuedSprite *sprite;

  if (list == NULL)
    return;
  output = list->head;
  while (output != NULL) {
    output->x += dx;
    output->y += dy;
    sprite = &output->sprite;
    output = output->next;
    sprite->oam.x += dx;
    sprite->oam.y += dy;
  }
}
