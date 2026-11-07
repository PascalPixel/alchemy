// Matches: differ 0 of 1044 (tla-en, 2026-11-07).
// 2026-11-07: from 2; the saved y is read through a type holding an s32, so
// sched2 keeps the width's store before its load (the native's alias order),
// replacing the empty asm; de, es, fr and it match with the German name 3
// pixels left; ja differs (older layout).
// Draft, differ 44 of 1044, skeleton gap 26 (tla-en, 2026-11-01): the frame,
// every register and spill slot are the native's; what is left is order.
// Left: five store-against-constant orders in sched2 (the native's str before
// the lsls of a split constant, at the scroll words, the flag number, the
// saved leader position, and ldrh/movs at the start; statement orders tried),
// the Func_080ec484 block's str, and the four stores after Text_DrawMessageFar.
// There local-alloc gives the width value r3 and its constant address r2, where
// the native has them the other way round, so reload_cse_move2add cannot reach
// 0x0202a638 by subs #6: the native's value is born before its address, which
// expand does for a symbol (then no adds) but not for a constant address.
// Forms this match rests on: the place loop is a do-while, not a goto loop, so
// loop.c hoists 640 and reload rebuilds it at each use; targetX and targetZ are
// the variables x and y, declared before dx and dz (y ties dz's rank), which
// is what spills the marker; the flag number has its own variable; tile and
// always are set before placeX; the cursor's y and x take their name offset
// first, then the scroll; a marker's mode and the cursor's affine index are
// OAM bit-fields (their masks are QImode -13 and -63); 0x0202a654 and
// 0x0202a656 are arrays because a plain halfword scalar stores a constant from
// the pool; the three message numbers are symbols (pool loads, not split
// constants), and tla's en.po has yet to name 03625 "Present location" and
// 03627 "Boat". Kept forms: the saved leader position as savedX >> 16 (combine
// narrows it to the native's ldrsh at +2 off the field's address), the cursor's
// start at its fixed address (the z word as x + 4), the 9-bit x masked in C on
// the scrolled branch (the native's split 0x1ff), and a u16 angle (the native's
// 0xffff as the scroll mask >> 16). The world map screen's frame callback, this
// game's form of ☀️'s WorldPicture_UpdateMarkers (games/tbs/src/field/
// event_commands/world_picture.c). The cursor glides to a target set at
// Data_0202a644 (its step a quarter of the distance, at most 8 pixels) or
// moves with the d-pad, faster the longer it is held, kept on the map; each
// of up to 128 places of the flag entries whose state is not 0 and whose flag
// 0x180 + n is set, plus the leader's place (or the ship's, for state 15
// once flag 0x8de is set), gets a marker, scrolled with the screen or, when
// Data_0202a640 is set, on the whole map scaled by 300/640; places scrolled
// off the screen are skipped. With the d-pad released the cursor moves onto
// the nearest place within 8 pixels; the nearest blinks in a frame, the
// cursor is drawn, and the nearest place's name (MsgPresentLocation for the
// leader, the ship's name, or its area name) appears six frames after the
// choice changes and follows the scroll. The markers' blend pulses with
// Data_080f1100.
#include "types.h"
#include "input.h"
#include "main_routine_work.h"
#include "io_write_queue.h"
#include "iwram_call.h"
#include "edition.h"
#include "ram_buffer.h"

// The scrolled name's left edge, in pixels left of its place.
#if LANGUAGE == LANGUAGE_GERMAN
#define WORLD_MAP_NAME_LEFT 3
#else
#define WORLD_MAP_NAME_LEFT 1
#endif

struct WorldMapMarker {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 objectMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
};

struct WorldMapWork {
  u16 vramBlock;
  u16 windowSlot;
  s32 x;
  s32 z;
  u8 unk0c[6];
  s16 shown;
  u8 unk14[4];
  s32 speed;
  void *window;
  struct WorldMapMarker markers[129];
  u8 unk62c[4];
  s32 scrollX;
  s32 scrollZ;
};

struct WorldMapPlace {
  u8 flag;
  u8 value;
  s16 unk02;
  s16 x;
  s16 z;
  u8 unk08[4];
  s16 name;
  s16 nameEntrance;
  u8 unk10[4];
};

struct ResourceTableEntry {
  u16 unk00;
  u16 offset;
};

#define WORK Ram_WorldMapWork
extern struct ResourceTableEntry gResourceTableEntries[];
extern u32 gFrameCount;
extern const u8 Data_080f1100[];
extern const u16 Data_080f1120[];
extern u8 MsgPresentLocation[];
extern u8 MsgBoat[];
extern u8 MsgDebugAreaName[];
#define Data_0202a644 (*(s32 *)0x0202a644)
#define Data_0202a648 (*(s32 *)0x0202a648)
#define Data_0202a640 (*(s16 *)0x0202a640)
#define gData_0202a642 (*(s16 *)0x0202a642)
extern s16 Data_0202a654[];
extern s16 Data_0202a656[];
#define Data_0202a638 (*(s16 *)0x0202a638)
#define Data_0202a63a (*(s16 *)0x0202a63a)
// The saved y's type in the FAKEMATCH below.
struct Unk_0202a63a {
  s16 unk00;
  s32 unk04;
};
#define Data_0202a63c (*(s16 *)0x0202a63c)
#define Data_0202a63e (*(s16 *)0x0202a63e)
extern struct WorldMapMarker Data_0202a620;

s32 Party_GetLeaderActor(void);
struct WorldMapPlace *FlagEntries_GetTable(void);
s32 FixedSqrt(s32 value);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
void Func_080ec1d0(s32 *screenX, s32 *screenY);
u32 Func_080ec298(struct WorldMapPlace *place);
s32 GameFlag_Test(s32 flag);
void Func_080140d8(void *entry, s32 slot);
s32 ArcTan2(s32 y, s32 x);
void WindowSprite_PrepareForRedrawFar(void *window);
s32 Scene_GetAreaNameIndex(s32 scene, s32 entrance);
void Text_MeasureMessageFar(s32 message, s32 *width, s32 *height);
void Text_DrawMessageFar(s32 message, void *window, s32 x, s32 y);
void Func_080ec484(void *window, s32 dx, s32 dy);

#define WORLD_MAP_HIGH(value) (((s16 *)&(value))[1])
#define WORLD_MAP_SCALE(value) ((value) * 300 / 640)

void Func_080ec4d4(void) {
  struct WorldMapWork *work;
  struct WorldMapMarker *marker;
  s32 tileBase;
  s32 blend;
  s32 nearest;
  s32 nearestDistance;
  s32 nearestX;
  s32 nearestY;
  s32 name;
  s32 nearestName;
  s32 nearestEntrance;
  s32 entrance;
  struct WorldMapPlace *places;
  s32 scrollX;
  s32 scrollY;
  struct WorldMapPlace *place;
  s32 width;
  s32 height;
  s32 placeX;
  s32 placeY;
  s32 pos[3];
  s32 x;
  s32 y;
  s32 dx;
  s32 dz;
  s32 distance;
  s32 step;
  s32 cursorX;
  s32 cursorY;
  u16 angle;
  s32 tile;
  s32 always;
  s32 flag;
  s32 i;

  Party_GetLeaderActor();
  work = WORK;
  marker = work->markers;
  tileBase = gResourceTableEntries[work->vramBlock].offset >> 5;
  nearest = -1;
  nearestDistance = 64;
  nearestX = 0;
  nearestY = 0;
  places = FlagEntries_GetTable();
  scrollX = work->scrollX & 0xffff0000;
  scrollY = work->scrollZ & 0xffff0000;
  blend = Data_080f1100[(gFrameCount >> 1) & 31];

  if (Data_0202a644 != 0) {
    x = Data_0202a644;
    y = Data_0202a648;
    dx = (x - work->x) / 0x10000;
    dz = (y - work->z) / 0x10000;
    distance = Iwram_Sqrt(dx * dx + dz * dz) << 16;
    dx = x - work->x;
    dz = y - work->z;
    if (distance < 0x400000)
      distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
    step = distance / 4;
    if (step > 0x80000)
      step = 0x80000;
    if (distance < 0x8000) {
      work->x = x;
      work->z = y;
      Data_0202a644 = 0;
      Data_0202a648 = 0;
    } else {
      if (distance > step) {
        dx = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dx), step);
        dz = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dz), step);
      }
      work->x += dx;
      work->z += dz;
    }
  } else {
    angle = Data_080f1120[(gInput.held >> 4) & 15];
    if (angle != 0xffff) {
      pos[0] = work->x;
      pos[1] = 0;
      pos[2] = work->z;
      Vector_AddPolarOffset(work->speed, angle, pos);
      if (pos[0] < 0x100000)
        pos[0] = 0x100000;
      if (pos[0] > 0x1ff0000)
        pos[0] = 0x1ff0000;
      if (pos[2] < 0)
        pos[2] = 0;
      if (pos[2] > 0x16d0000)
        pos[2] = 0x16d0000;
      work->x = pos[0];
      work->z = pos[2];
      Ram_WorldMapCursorStart[0] = pos[0];
      Ram_WorldMapCursorStart[1] = pos[2];
      if (work->speed < 0x60000)
        work->speed += 0x2000;
    }
  }

  cursorX = WORLD_MAP_HIGH(work->x);
  cursorY = WORLD_MAP_HIGH(work->z);
  place = places;
  i = 0;
  do {
    if (place->flag == 0) {
      if (gData_0202a642 != 0)
        break;
      place = &places[128];
      i = 128;
      name = -1;
      tile = 0;
      always = 0;
      Func_080ec1d0(&placeX, &placeY);
    } else {
      name = place->name;
      entrance = place->nameEntrance;
      flag = place->flag + 0x180;
      tile = Func_080ec298(place);
      if (tile == 0)
        continue;
      if (tile == 15) {
        if (!GameFlag_Test(0x8de))
          continue;
        if (gMainRoutineWork.ownerOverride != 0)
          continue;
        name = -2;
        tile = 2;
        always = 0;
        placeX = (gMainRoutineWork.savedX >> 16) * 853 / 16384 - 269;
        placeY = (gMainRoutineWork.savedZ >> 16) * 640 / 16384 - 112;
      } else {
        if (!GameFlag_Test(flag))
          continue;
        always = 1;
        placeX = place->x * 853 / 16384 - 269;
        placeY = place->z * 640 / 16384 - 112;
      }
    }
    marker->objectMode = always;
    marker->tile = tileBase + tile;
    if (Data_0202a640 == 0) {
      x = placeX - (scrollX >> 16) - 1;
      y = placeY - (scrollY >> 16) - 1;
      if ((u32)(x + 4) > 247)
        continue;
      if (y < -4)
        continue;
      if (y > 163)
        continue;
      marker->x = x & 0x1ff;
      marker->y = y;
    } else {
      marker->x = WORLD_MAP_SCALE(placeX) - 1;
      marker->y = WORLD_MAP_SCALE(placeY) - 1;
    }
    width = placeX - cursorX;
    height = placeY - cursorY;
    if (width * width + height * height < nearestDistance) {
      nearest = i;
      nearestName = name;
      nearestEntrance = entrance;
      nearestDistance = width * width + height * height;
      nearestX = placeX;
      nearestY = placeY;
    }
    if (always != 0 || (gFrameCount & 15) <= 7)
      Func_080140d8(marker++, 246);
  } while (++i, place++, i < 128);

  if (gInput.held == 0) {
    if (nearest != -1) {
      if (nearestDistance <= 2) {
        work->x = nearestX << 16;
        work->z = nearestY << 16;
        work->speed = 0x10000;
      } else {
        angle = ArcTan2(nearestY - cursorY, nearestX - cursorX);
        pos[0] = work->x;
        pos[1] = 0;
        pos[2] = work->z;
        Vector_AddPolarOffset(work->speed, (u16)angle, pos);
        work->x = pos[0];
        work->z = pos[2];
      }
      cursorX = WORLD_MAP_HIGH(work->x);
      cursorY = WORLD_MAP_HIGH(work->z);
    } else {
      work->speed = 0x10000;
    }
  }

  if (nearest != -1 && (gFrameCount & 15) <= 7) {
    marker->objectMode = 0;
    marker->tile = tileBase + 3;
    if (Data_0202a640 == 0) {
      marker->x = nearestX - (scrollX >> 16) - 2;
      marker->y = nearestY - (scrollY >> 16) - 2;
    } else {
      marker->x = WORLD_MAP_SCALE(nearestX) - 2;
      marker->y = WORLD_MAP_SCALE(nearestY) - 2;
    }
    Func_080140d8(marker, 246);
  }

  marker = &Data_0202a620;
  if (Data_0202a640 == 0) {
    marker->affineIndex = 0;
    marker->x = cursorX - (scrollX >> 16) - 17;
    marker->y = cursorY - (scrollY >> 16) + 1;
  } else {
    marker->x = WORLD_MAP_SCALE(cursorX) - 17;
    marker->y = WORLD_MAP_SCALE(cursorY) + 1;
  }
  Func_080140d8(marker, 246);

  if (work->shown != nearest) {
    if (Data_0202a654[0] == 0) {
      WindowSprite_PrepareForRedrawFar(work->window);
      Data_0202a654[0] = 1;
      Data_0202a656[0] = 6;
    } else if (--Data_0202a656[0] <= 0) {
      Data_0202a654[0] = 0;
      work->shown = nearest;
      if (nearest != -1) {
        if (nearestName == -1)
          nearestName = (s32)MsgPresentLocation;
        else if (nearestName == -2)
          nearestName = (s32)MsgBoat;
        else
          nearestName = Scene_GetAreaNameIndex(nearestName, nearestEntrance) +
                        (s32)MsgDebugAreaName;
        Text_MeasureMessageFar(nearestName, &width, &height);
        if (Data_0202a640 == 0) {
          cursorX = nearestX - WORLD_MAP_NAME_LEFT;
          cursorY = nearestY - 11;
          cursorX -= scrollX >> 16;
          cursorY -= scrollY >> 16;
        } else {
          cursorX = WORLD_MAP_SCALE(nearestX) - 1;
          cursorY = WORLD_MAP_SCALE(nearestY) - 11;
        }
        if (cursorX + width > 239) {
          cursorX = 232 - width;
          cursorY += -10;
        }
        if (cursorY < 0)
          cursorY = 0;
        Text_DrawMessageFar(nearestName, work->window, cursorX, cursorY);
        {
          /* FAKEMATCH(temporary): one local carries width, then height, into
             the saved size; as plain stores, 20 of 1044 instructions differ in
             tla-en.
          */
          s32 value = width;
          Data_0202a63c = value;
          value = height;
          Data_0202a63e = value;
        }
        Data_0202a638 = cursorX;
        Data_0202a63a = cursorY;
      }
    }
  } else if (nearest != -1) {
    width = Data_0202a63c;
    height = Data_0202a63e;
    if (Data_0202a640 == 0) {
      cursorX = nearestX - WORLD_MAP_NAME_LEFT;
      cursorY = nearestY - 11;
      cursorX -= scrollX >> 16;
      cursorY -= scrollY >> 16;
    } else {
      cursorX = WORLD_MAP_SCALE(nearestX) - 1;
      cursorY = WORLD_MAP_SCALE(nearestY) - 11;
    }
    if (cursorX + width > 239) {
      cursorX = 232 - width;
      cursorY += -10;
    }
    if (cursorY < 0)
      cursorY = 0;
    width = cursorX - Data_0202a638;
    /* FAKEMATCH(barrier): the saved y read through a struct with an s32
       member, so its load may alias the width just stored and stays after
       it; as Data_0202a63a, 10 of 1044 instructions differ in tla-en. */
    height = cursorY - ((struct Unk_0202a63a *)&Data_0202a63a)->unk00;
    Func_080ec484(work->window, width, height);
    Data_0202a638 = cursorX;
    Data_0202a63a = cursorY;
  }

  IO_WRITE_QUEUE_PUSH(0x04000050, 0x3f00, 0x20000);
  IO_WRITE_QUEUE_PUSH(0x04000052, ((16 - blend) << 8) | blend, 0x20000);
  IO_WRITE_QUEUE_PUSH(0x04000054, 0, 0x20000);
}
