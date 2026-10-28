// Draft, differ 1209 of 1044: the world map screen's frame callback, this
// game's form of ☀️'s RegionMap_UpdateMarkers (games/tbs/src/overworld/
// field_services/world_map.c). The cursor glides to a target set at
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
// The same calls, offsets and control flow; left: stack and register
// allocation throughout (the native stack frame is 84 bytes against 88, the scroll
// at sp 4 and 0, the cursor in r10 and r8), which the alignment counts in
// nearly every row.
#include "types.h"
#include "input.h"
#include "adventure.h"
#include "io_write_queue.h"
#include "iwram_call.h"

struct WorldMapMarker {
  u8 unk00[4];
  u8 y;
  u8 attr0;
  u16 x : 9;
  u16 attr1 : 7;
  u16 tile : 10;
  u16 attr2 : 6;
  u16 unk0a;
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

extern struct WorldMapWork gWorldMapWork;
extern struct ResourceTableEntry gResourceTableEntries[];
extern u32 gFrameCount;
extern const u8 Data_080f1100[];
extern const u16 Data_080f1120[];
extern s32 Data_0202a644;
extern s32 Data_0202a648;
extern s32 Data_0202a64c[2];
extern s16 Data_0202a640;
extern s16 gData_0202a642;
extern s16 Data_0202a654;
extern s16 Data_0202a656;
extern s16 Data_0202a638;
extern s16 Data_0202a63a;
extern s16 Data_0202a63c;
extern s16 Data_0202a63e;
extern struct WorldMapMarker Data_0202a620;

s32 EventRuntime_GetControlledOwner(void);
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
  struct WorldMapMarker *marker;
  s32 tileBase;
  s32 nearest;
  s32 nearestDistance;
  s32 nearestX;
  s32 nearestY;
  s32 nearestName;
  s32 nearestEntrance;
  struct WorldMapPlace *places;
  s32 scrollX;
  s32 scrollY;
  s32 blend;
  struct WorldMapPlace *place;
  s32 name;
  s32 entrance;
  s32 width;
  s32 height;
  s32 placeX;
  s32 placeY;
  s32 pos[3];
  s32 targetX;
  s32 targetZ;
  s32 dx;
  s32 dz;
  s32 distance;
  s32 step;
  s32 cursorX;
  s32 cursorY;
  s32 angle;
  s32 tile;
  s32 always;
  s32 i;
  s32 x;
  s32 y;

  EventRuntime_GetControlledOwner();
  marker = gWorldMapWork.markers;
  tileBase = gResourceTableEntries[gWorldMapWork.vramBlock].offset >> 5;
  nearest = -1;
  nearestDistance = 64;
  nearestX = 0;
  nearestY = 0;
  places = FlagEntries_GetTable();
  scrollX = gWorldMapWork.scrollX & 0xffff0000;
  scrollY = gWorldMapWork.scrollZ & 0xffff0000;
  blend = Data_080f1100[(gFrameCount >> 1) & 31];

  if (Data_0202a644 != 0) {
    targetX = Data_0202a644;
    targetZ = Data_0202a648;
    dx = (targetX - gWorldMapWork.x) / 0x10000;
    dz = (targetZ - gWorldMapWork.z) / 0x10000;
    distance = Iwram_Sqrt(dx * dx + dz * dz) << 16;
    dx = targetX - gWorldMapWork.x;
    dz = targetZ - gWorldMapWork.z;
    if (distance < 0x400000)
      distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
    step = distance / 4;
    if (step > 0x80000)
      step = 0x80000;
    if (distance < 0x8000) {
      gWorldMapWork.x = targetX;
      gWorldMapWork.z = targetZ;
      Data_0202a644 = nearestY;
      Data_0202a648 = nearestY;
    } else {
      if (distance > step) {
        dx = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dx), step);
        dz = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dz), step);
      }
      gWorldMapWork.x += dx;
      gWorldMapWork.z += dz;
    }
  } else {
    angle = Data_080f1120[(gInput.held >> 4) & 15];
    if (angle != 0xffff) {
      pos[0] = gWorldMapWork.x;
      pos[1] = nearestY;
      pos[2] = gWorldMapWork.z;
      Vector_AddPolarOffset(gWorldMapWork.speed, angle, pos);
      if (pos[0] < 0x100000)
        pos[0] = 0x100000;
      if (pos[0] > 0x1ff0000)
        pos[0] = 0x1ff0000;
      if (pos[2] < 0)
        pos[2] = nearestY;
      if (pos[2] > 0x16d0000)
        pos[2] = 0x16d0000;
      gWorldMapWork.x = pos[0];
      gWorldMapWork.z = pos[2];
      Data_0202a64c[0] = pos[0];
      Data_0202a64c[1] = pos[2];
      if (gWorldMapWork.speed < 0x60000)
        gWorldMapWork.speed += 0x2000;
    }
  }

  cursorX = WORLD_MAP_HIGH(gWorldMapWork.x);
  cursorY = WORLD_MAP_HIGH(gWorldMapWork.z);
  place = places;
  for (i = 0; i < 128; i++, place++) {
    if (place->flag == 0) {
      if (gData_0202a642 != 0)
        break;
      name = -1;
      i = 128;
      place = &places[128];
      tile = 0;
      always = 0;
      Func_080ec1d0(&placeX, &placeY);
    } else {
      name = place->name;
      entrance = place->nameEntrance;
      x = place->flag + 0x180;
      tile = Func_080ec298(place);
      if (tile == 0)
        continue;
      if (tile == 15) {
        if (!GameFlag_Test(0x8de))
          continue;
        if (gAdventure.ownerOverride != 0)
          continue;
        name = -2;
        placeX = ((s16 *)&gAdventure.savedX)[1] * 853 / 16384 - 269;
        tile = 2;
        always = 0;
        placeY = ((s16 *)&gAdventure.savedZ)[1] * 640 / 16384 - 112;
      } else {
        if (!GameFlag_Test(x))
          continue;
        placeX = place->x * 853 / 16384 - 269;
        always = 1;
        placeY = place->z * 640 / 16384 - 112;
      }
    }
    marker->attr0 = (marker->attr0 & ~12) | (always << 2);
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
      marker->x = x;
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
  }

  if (gInput.held == 0) {
    if (nearest != -1) {
      if (nearestDistance <= 2) {
        gWorldMapWork.x = nearestX << 16;
        gWorldMapWork.z = nearestY << 16;
        gWorldMapWork.speed = 0x10000;
      } else {
        angle = ArcTan2(nearestY - cursorY, nearestX - cursorX);
        pos[0] = gWorldMapWork.x;
        pos[1] = gInput.held;
        pos[2] = gWorldMapWork.z;
        Vector_AddPolarOffset(gWorldMapWork.speed, (u16)angle, pos);
        gWorldMapWork.x = pos[0];
        gWorldMapWork.z = pos[2];
      }
      cursorX = WORLD_MAP_HIGH(gWorldMapWork.x);
      cursorY = WORLD_MAP_HIGH(gWorldMapWork.z);
    } else {
      gWorldMapWork.speed = 0x10000;
    }
  }

  if (nearest != -1 && (gFrameCount & 15) <= 7) {
    marker->attr0 &= ~12;
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
    ((u8 *)marker)[7] &= ~62;
    marker->x = cursorX - (scrollX >> 16) - 17;
    marker->y = cursorY - (scrollY >> 16) + 1;
  } else {
    marker->x = WORLD_MAP_SCALE(cursorX) - 17;
    marker->y = WORLD_MAP_SCALE(cursorY) + 1;
  }
  Func_080140d8(marker, 246);

  if (gWorldMapWork.shown != nearest) {
    if (Data_0202a654 == 0) {
      WindowSprite_PrepareForRedrawFar(gWorldMapWork.window);
      Data_0202a654 = 1;
      Data_0202a656 = 6;
    } else if (--Data_0202a656 <= 0) {
      Data_0202a654 = 0;
      gWorldMapWork.shown = nearest;
      if (nearest != -1) {
        if (nearestName == -1)
          nearestName = 0xe29;
        else if (nearestName == -2)
          nearestName = 0xe2b;
        else
          nearestName =
              Scene_GetAreaNameIndex(nearestName, nearestEntrance) + 0xe58;
        Text_MeasureMessageFar(nearestName, &width, &height);
        if (Data_0202a640 == 0) {
          cursorX = nearestX - 1 - (scrollX >> 16);
          cursorY = nearestY - 11 - (scrollY >> 16);
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
        Text_DrawMessageFar(nearestName, gWorldMapWork.window, cursorX,
                               cursorY);
        Data_0202a63c = width;
        Data_0202a63e = height;
        Data_0202a638 = cursorX;
        Data_0202a63a = cursorY;
      }
    }
  } else if (nearest != -1) {
    width = Data_0202a63c;
    height = Data_0202a63e;
    if (Data_0202a640 == 0) {
      cursorX = nearestX - 1 - (scrollX >> 16);
      cursorY = nearestY - 11 - (scrollY >> 16);
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
    height = cursorY - Data_0202a63a;
    Func_080ec484(gWorldMapWork.window, width, height);
    Data_0202a638 = cursorX;
    Data_0202a63a = cursorY;
  }

  IO_WRITE_QUEUE_PUSH(0x04000050, 0x3f00, 0x20000);
  IO_WRITE_QUEUE_PUSH(0x04000052, ((16 - blend) << 8) | blend, 0x20000);
  IO_WRITE_QUEUE_PUSH(0x04000054, 0, 0x20000);
}
