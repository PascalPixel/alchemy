#include "types.h"
#include "input.h"
#include "main_routine_work.h"
#include "io_write_queue.h"
#include "iwram_call.h"
#include "edition.h"
#include "ram_buffer.h"
#include "fixed_math.h"

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
extern s32 Data_0202a644;
extern s32 Data_0202a648;
extern s16 Data_0202a640;
extern s16 gData_0202a642;
extern s16 Data_0202a654[];
extern s16 Data_0202a656[];
// A halfword typed beside a word, for the FAKEMATCH that reads the name box's
// y; no such data exists.
struct WorldMapAliasedHalfword {
  s16 value;
  s32 unk04;
};
#define NAME_BOX_X Ram_WorldMapNameBox[0]
#define NAME_BOX_Y Ram_WorldMapNameBox[1]
#define NAME_BOX_WIDTH Ram_WorldMapNameBox[2]
#define NAME_BOX_HEIGHT Ram_WorldMapNameBox[3]
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
    dx = (x - work->x) / Q16_ONE;
    dz = (y - work->z) / Q16_ONE;
    distance = Iwram_Sqrt(dx * dx + dz * dz) << 16;
    dx = x - work->x;
    dz = y - work->z;
    if (distance < Q16_ONE * 64)
      distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
    step = distance / 4;
    if (step > Q16_ONE * 8)
      step = Q16_ONE * 8;
    if (distance < Q16_ONE / 2) {
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
      if (pos[0] < Q16_ONE * 16)
        pos[0] = Q16_ONE * 16;
      if (pos[0] > Q16_ONE * 511)
        pos[0] = Q16_ONE * 511;
      if (pos[2] < 0)
        pos[2] = 0;
      if (pos[2] > Q16_ONE * 365)
        pos[2] = Q16_ONE * 365;
      work->x = pos[0];
      work->z = pos[2];
      Ram_WorldMapCursorStart[0] = pos[0];
      Ram_WorldMapCursorStart[1] = pos[2];
      if (work->speed < Q16_ONE * 6)
        work->speed += Q16_ONE / 8;
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
        work->speed = Q16_ONE;
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
      work->speed = Q16_ONE;
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
             the name box; as plain stores, 20 of 1044 instructions differ in
             tla-en. */
          s32 value = width;
          NAME_BOX_WIDTH = value;
          value = height;
          NAME_BOX_HEIGHT = value;
        }
        NAME_BOX_X = cursorX;
        NAME_BOX_Y = cursorY;
      }
    }
  } else if (nearest != -1) {
    width = NAME_BOX_WIDTH;
    height = NAME_BOX_HEIGHT;
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
    width = cursorX - NAME_BOX_X;
    /* FAKEMATCH(temporary): the name box's y read through a pointer to a
       struct with an s32 member standing in for its own address, so its
       load may alias the width just stored and stays after it; as
       NAME_BOX_Y, 10 of 1044 instructions differ in tla-en. */
    height = cursorY - ((struct WorldMapAliasedHalfword *)&NAME_BOX_Y)->value;
    Func_080ec484(work->window, width, height);
    NAME_BOX_X = cursorX;
    NAME_BOX_Y = cursorY;
  }

  IO_WRITE_QUEUE_PUSH(0x04000050, 0x3f00, 0x20000);
  IO_WRITE_QUEUE_PUSH(0x04000052, ((16 - blend) << 8) | blend, 0x20000);
  IO_WRITE_QUEUE_PUSH(0x04000054, 0, 0x20000);
}
