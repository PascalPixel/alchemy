#ifndef ALCHEMY_MODULE_MENU_MENU_H
#define ALCHEMY_MODULE_MENU_MENU_H

/// @file
/// What games/tla/src/menu/ declares for the files that use it, written once.

#include "types.h"
#include "module/tla.h"

struct QueuedSprite;

typedef s32 (*PackedDecoder)(const u8 *source, void *destination, u32 fill);

/// One eight-byte OAM entry in the credits' sprite buffer.
struct DisplayScrollObject {
  u32 attr01;
  u32 attr2;
};

struct TitleScrollStep {
  s32 frames;
  s32 speed[4];
  u32 mask[4];
};

struct TitleScroll {
  s16 pos;
  s16 limit;
  u16 step;
  u8 *src;
  u8 *dst;
  u16 *cursor;
  u16 tile;
  s16 scroll;
  u16 rows;
  u32 frames;
};

struct TitleFadeTimers {
  u8 unk00[10];
  s16 white;
  s16 back;
};

struct OamEntry {
  u32 y : 8;
  u32 affine : 2;
  u32 mode : 2;
  u32 mosaic : 1;
  u32 colors256 : 1;
  u32 shape : 2;
  u32 x : 9;
  u32 matrix : 5;
  u32 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unused;
};

struct TitleLogoState {
  s16 phase;
  s16 frame; ///< index into gData_081a8378 (5 halfwords a frame)
  s16 timer;
  s16 step; ///< index into gData_081a84e6 (3 halfwords a step); the scale in
            ///< phase 1
  s16 delay;
  s16 white;
  s16 back;
};

/// The title screen's state, in heap slot 172.
struct TitleScreenWork {
  u8 unk00[16];
  struct TitleSprite sprites[8];
};

extern s16 Data_02007500;
extern struct DisplayScrollObject *gDisplayScroll_Objects;

extern const u32 gDisplayScroll_SlideResources[];
extern const void *gDisplayScroll_LineTable[];

/// The scrolling picture's state. Func_081a133c, a VBlank task, moves the
/// scroll toward its target while Func_081a17d0 waits for it to arrive.
///@{
extern s16 Data_02007514;           ///< which character block the picture is in
extern s16 Data_02007516;           ///< frames of the fade-in, to 64
extern u32 Data_02007518;           ///< the tilemap's scroll, 16.16 pixels
extern volatile s32 gData_0200751c; ///< the scroll, 16.16 pixels
extern s16 gData_02007520;          ///< the row the scroll is heading for
extern s16 Data_02007524;           ///< the most rows a frame moves
extern u8 *Data_02007528;           ///< the picture's map
/// The 24 sprites of the picture's border strips.
extern struct QueuedSprite *gData_0200752c;

void Func_081a0a70(void);
void Func_081a0e0c(s32 credits);
void Func_081a0fb8(void);
s32 DisplayScroll_DrawLine(const u8 *text, s32 slot, s32 align);

extern const u8 BattleEffect_ExpandCode[];

extern u8 BattleEffect_ExpandCodeSize[];

s32 Func_08038418(s32 message, u8 *canvas, s32 align);

extern const u8 gDisplayScroll_GlyphWidths[];

extern const u8 gDisplayScroll_Font[];

void Func_081a1830(void);

extern const u8 BattleEffect_Expand2Code[];

extern u8 BattleEffect_Expand2CodeSize[];

extern char ResourceId_Data018;
void PaletteFade_SetCurrent(s32 index, s32 transform);

extern u8 Data_02026800[];

extern char ResourceId_Data01B;
extern char ResourceId_Data01C;
extern char ResourceId_Data01D;
extern char ResourceId_Data01E;
extern char ResourceId_Data01F;

extern u8 Data_02019600[];
extern u8 Data_02017080[];

extern char ResourceId_IntroLighthousePicture;
extern char ResourceId_IntroGullSprites;
extern char ResourceId_TitleScreenPicture;

extern const u8 Data_081a873e[];

extern u16 Data_081a877a[];

extern u16 Data_081a87ba[];

extern u16 Data_081a87fa[];

s32 BattleEffect_ClampRgb555Channel(s32 value);
s32 BattleEffect_ClampRgb555Component(s32 value);

#endif
