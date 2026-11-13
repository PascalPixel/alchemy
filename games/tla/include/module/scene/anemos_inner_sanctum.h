#ifndef ALCHEMY_MODULE_SCENE_ANEMOS_INNER_SANCTUM_H
#define ALCHEMY_MODULE_SCENE_ANEMOS_INNER_SANCTUM_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"

struct Actor;
struct CellTarget;
struct CellTargetEntry;
struct Work;

struct AnemosInnerSanctumEventWork {
  u8 unk000[0x1ac];
  s32 value1ac;
};

struct Mover {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  s32 fall;
  u8 unk18[24];
  s32 step;
  s32 height;
  u8 unk38[12];
  s32 originX;
  s32 originZ;
  s32 radius;
  u8 unk50[20];
  s16 angle;
  s16 spin;
};

struct SpawnParams {
  s32 kind;
  s32 part;
  u8 unk08[32];
};

struct Globals {
  u8 pad[108];
  struct Work *work;
};

struct MainRoutineWork2c2 {
  u8 unk000[0x2c2];
  u8 value;
};

struct AnemosInnerSanctumPicture {
  const void *data;
  u16 a;
  u16 b;
};

extern u16 *gPaletteSlots;
extern u16 *gPaletteColors;
extern u16 *gPaletteCycleColors;
extern u16 *gPaletteCycleSlots;

void Func_02000328(void);

extern u16 gPaletteCycleBright;
extern u16 gPaletteCycleTimer;
extern u16 gPaletteCycleDelay;
extern u16 gPaletteCycleLength;
extern u16 gPaletteCycleIndex;

s32 List_CountEntries(u16 *list);

void PaletteCycle_Fade(void);

void Teleport_PlayWhirl(void);
void MapGrid_SetCellHeight(s32 x, s32 z, s32 a, s32 b);
s32 Actor_LiftTargetBlock();

extern char SceneId_Ov6B0Room2;
extern char SceneId_Ov6B0Room3;
extern char SceneId_Ov6B0Room4;

void Actor_PlaceMovedBlock(void);

void Func_02000158(void);

void Actor_Release(struct Actor *object);

extern char SceneId_Ov6B0Room5;

void Func_020000f4(void);

void Teleport_PlaySparks(void);

void Leader_CenterInCell(void);

extern struct CellTarget gCrackedTiles;

void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

void Func_02000ac8(void);
void Func_02001090(void);
void Func_0200109c(void);
void Func_02001160(void);
void Func_02001398(void);
void Func_02001518(void);
void Func_02001614(void);
void Leader_SetLayerByte(void);
void Func_02001660(void);
void Func_020016fc(void);
void Func_0200178c(void);
void Func_020017a0(void);

void Func_02001938(void);
void Func_020019a8(void);
void Func_02001a94(void);
void Func_02001b88(void);
void Func_02001c60(void);

extern u32 gData_020034c0[];
extern u32 gData_020034fc[];
extern u32 gData_02003538[];
extern u32 gData_02003804[];
extern s32 gData_020038fc[];
extern s16 gData_020043fa[];
extern s32 gData_02003584[];

#endif
