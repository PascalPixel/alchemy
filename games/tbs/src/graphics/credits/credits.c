#include "unit/graphics/credits/credits.h"
#ifdef UBFIX
// This object uses the credits' views of the borrowed flash work cells.
#define OWN_Flash_Handler0
#define OWN_gFlash
#define OWN_gFlashNumRemainingBytes
#endif
#include "unit/runtime/runtime.h"
#include "dma.h"
#include "io_reg.h"
#include "types.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "resource.h"
#include "ramcode.h"
#include "callback_scheduler.h"
#include "io_write_queue.h"
#include "iowrite.h"
#include "map_scroll.h"
#include "heap_state.h"
#include "resource_ids.h"
#include "edition.h"
#include "iwram_call.h"
#include "static_assert.h"
#include "battle_party.h"
#include "battle_system.h"
#include "frame.h"
#include "flags.h"

// Credits text uses six 32-pixel sprites across each 192-pixel line. Its
// 32 tile rows form a ring, with sixteen rows visible at a time.
#define CREDITS_LINE_PIXELS 192
#define CREDITS_VISIBLE_ROWS 16
#define CREDITS_COLUMNS 6
#define CREDITS_RING_ROWS 32
#define CREDITS_LINE_TILES 24 ///< A line's 4bpp tiles, 192 / 8.
#define CREDITS_RING_TILES (CREDITS_RING_ROWS * CREDITS_LINE_TILES)
#define CREDITS_RING_BYTES (CREDITS_RING_TILES * TILE_SIZE_4BPP)
/// A line's raster: nine rows of 256 one-byte pixels, the ninth for the
/// shadow below the eighth.
#define CREDITS_BUFFER_BYTES 0x900
#define CREDITS_RASTER_STRIDE 256
/// OAM entries 0 to 23: the opaque strips above and below the text.
#define CREDITS_BORDER_SPRITES 24
/// The whole OAM: the borders, 96 text sprites and 8 parked off screen.
#define CREDITS_OAM_ENTRIES 128

/// Fills 30 KB of 256-color tiles with color 1 and clears 128 palette colors.
/// Called by the credits slideshow before loading a backdrop into a half. The
/// 30 KB are a backdrop's 480 tiles, 256 by 120 pixels.
/// @param alternate 0 selects the first half (VRAM 0x06000000, palette 0-127).
///      1 selects the second half (VRAM 0x06008000, palette 128-255, fill 129).
void Graphics_ClearCharacterBlockAndPalette(s32 alternate) {
  u32 fillColor;
  void *tileDestination;
  void *paletteDestination;
  volatile u32 dmaFill;

  // Use a solid pixel in the palette being cleared, producing an opaque
  // black layer for the next crossfade.
  if (alternate == 0) {
    fillColor = 0x01010101;
    tileDestination = BG_CHAR_BLOCK(0);
    paletteDestination = (void *)BG_PLTT;
  } else {
    fillColor = 0x81818181;
    tileDestination = BG_CHAR_BLOCK(2);
    paletteDestination = (void *)&BG_PLTT[128];
  }
  Dma_Fill32Cell(dmaFill, fillColor, tileDestination, 0x7800);
  Dma_Fill32Cell(dmaFill, 0, paletteDestination, 0x100);
}

typedef s32 (*PackedDecoder)(const u8 *source, void *destination, u32 fill);

/// Battle backgrounds shown in the credits; 0 clears the display.
// clang-format off
const u32 gDisplayScroll_SlideResources[] = {
    (u32)&ResourceId_ValeStormBackdrop,
    (u32)&ResourceId_SolSanctumBackdrop,
    (u32)&ResourceId_GrasslandBackdrop,
    (u32)&ResourceId_InteriorBackdrop,
    (u32)&ResourceId_ForestBackdrop,
    (u32)&ResourceId_GomaCaveBackdrop,
    (u32)&ResourceId_KolimaForestBackdrop,
    (u32)&ResourceId_TretTreeInteriorsBackdrop,
    (u32)&ResourceId_TretTreeBranchBackdrop,
    (u32)&ResourceId_SnowfieldBackdrop,
    (u32)&ResourceId_MercuryLighthouseBackdrop,
    (u32)&ResourceId_MercuryLighthouseAerieBackdrop,
    (u32)&ResourceId_ImilFuchinFallsBackdrop,
    (u32)&ResourceId_MogallForestBackdrop,
    (u32)&ResourceId_AltinPeakBackdrop,
    (u32)&ResourceId_LamakanDesertBackdrop,
    (u32)&ResourceId_TolbiShipDeckBackdrop,
    (u32)&ResourceId_TolbiShipInteriorsBackdrop,
    (u32)&ResourceId_BeachBackdrop,
    (u32)&ResourceId_ColossoBackdrop,
    (u32)&ResourceId_AltmillerCaveBackdrop,
    (u32)&ResourceId_SuhallaDesertBackdrop,
    (u32)&ResourceId_CrossboneIsleCaveBackdrop,
    (u32)&ResourceId_PirateShipBackdrop,
    (u32)&ResourceId_SuhallaGateBackdrop,
    (u32)&ResourceId_TunnelRuinsBackdrop,
    (u32)&ResourceId_BabiLighthouseBackdrop,
    (u32)&ResourceId_VenusLighthouseBackdrop,
    (u32)&ResourceId_VenusLighthouseAerieBackdrop,
    (u32)&ResourceId_VenusLighthouseAerieStormBackdrop,
    (u32)&ResourceId_DuskCloudsBackdrop,
    (u32)&ResourceId_VioletSkyBackdrop,
    0               // Clear
};
// clang-format on

/// Loads a credits backdrop into one of the two tile/palette halves. A zero
/// resource clears that half; the second half offsets its palette indices.
/// Called by DisplayScroll_RunSlideshow for each picture. A backdrop resource
/// is its 128 colours (256 bytes) and then its packed tiles. The decoder
/// runs from a copy in HEAP_SLOT_TEMP_RENDERER, released before
/// returning; the colours reach palette RAM at the next VBlank, through the
/// I/O write queue.
void Graphics_LoadCharacterBlockAndPalette(u32 resourceId, s32 alternate) {
  const u8 *resourceData;
  u32 paletteDestination;
  u32 paletteOffset;
  void *tileDestination;

  if (resourceId == 0) {
    Graphics_ClearCharacterBlockAndPalette(alternate);
    return;
  }
  resourceData = Resource_GetTableEntry(resourceId);
  if (alternate == 0) {
    paletteOffset = 0;
    tileDestination = BG_CHAR_BLOCK(0);
    paletteDestination = (u32)BG_PLTT;
  } else {
    paletteOffset = 0x80808080;
    tileDestination = BG_CHAR_BLOCK(2);
    paletteDestination = (u32)&BG_PLTT[128];
  }
  // Offset every pixel into the selected palette half so both backdrops
  // can remain visible throughout a crossfade.
  RamCode_LoadSlot(HEAP_SLOT_TEMP_RENDERER, Tile_ExpandSlide,
                   Tile_ExpandSlideCodeSize);
  ((PackedDecoder)gWorkSlot[HEAP_SLOT_TEMP_RENDERER])(
      resourceData + 256, tileDestination, paletteOffset);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_TEMP_RENDERER);

  // Queue palette RAM writes for VBlank rather than transferring colours
  // during the active display.
  IO_WRITE_QUEUE_PUSH(paletteDestination, (u32)resourceData,
                      DMA_COPY_WORDS(0x40));
}

/// Builds the 512 tilemap words surrounding a sliding credits backdrop:
/// blank margins, 240 consecutive tile pairs, then a zero-filled tail.
/// Called by DisplayScroll_RunSlideshow. In 32-tile rows: two rows of tile
/// 511, the picture's tiles 0 to 479 in fifteen rows, three rows of tile 511
/// (160 pixels in all, the screen), then twelve rows of zeros below it. Each
/// map follows its picture's 30 KB of tiles in the same 32 KB, so tile 511 is
/// the map's own last 64 bytes, which the zeros keep transparent.
void DisplayScroll_BuildHblankWordTable(u32 *destination) {
  s32 remaining;
  u32 blankTiles = 0x01FF01FF;
  u32 tilePair = 0x10000;

  // Pad above and below the picture so vertical wrapping stays blank.
  remaining = 31;
  do {
    remaining--;
    *destination++ = blankTiles;
  } while (remaining >= 0);
  remaining = 239;
  do {
    remaining--;
    *destination++ = tilePair;
    tilePair += 0x20002;
  } while (remaining >= 0);
  remaining = 47;
  do {
    remaining--;
    *destination++ = blankTiles;
  } while (remaining >= 0);
  tilePair = 0;
  remaining = 191;
  do {
    remaining--;
    *destination++ = tilePair;
  } while (remaining >= 0);
}

/// A VBlank task of the credits slideshow: drifts both backdrops together,
/// keeping their horizontal offsets aligned throughout the crossfade. Each
/// fourth frame both scrolls fall by one (adding 0xffff to the 16-bit
/// value), moving the pictures a pixel right; a picture is as wide as its
/// map, 256 pixels, so it wraps without a seam.
void DisplayScroll_StepPositionEveryFourFrames(void) {
  if ((gFrameTick & 3) == 0) {
    u32 decrement = 0xffff;

    gBgScroll[2].x += decrement;
    gBgScroll[3].x += decrement;
  }
}

/// Crossfades the credits backdrop sequence through alternating tile banks
/// while the staff names scroll over it, then restores the display for the
/// following screen. Reached through the scroll far-call table at the end of
/// the game: Title_Run calls it as ScrollFar_Entry00 at the title scene's
/// entrance 9, MUS_ENDING playing, before the clear screen. Its waits come to
/// 11223 frames, about 3 minutes 8 seconds, besides the loads: a 300-frame
/// lead, then 33 slides of a 64-frame fade and a 267-frame hold, the last a
/// fade to black. Returns 0 with BG2, BG3 and blending off and the OAM copy
/// back on; the three tasks it schedules are still scheduled.
s32 DisplayScroll_RunSlideshow(void) {
  u32 slide;
  s32 alpha;

  gOamCopyEnabled = 0;
  gCpuLoadDisplayEnabled = 0;
  gDebugTextEnabled = 0;
  gIdleSleepEnabled = 0;
  Scheduler_ResetTaskTable();
  Scheduler_AddOrUpdateCallback(DisplayScroll_StepPositionEveryFourFrames,
                                SCHEDULER_KEY_VBLANK);
  // Each layer owns its map, tiles and palette half. Load the invisible
  // half while the other remains on screen.
  Io_Write16(DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
  DisplayScroll_BuildHblankWordTable((u32 *)BG_SCREEN_BLOCK(15));
  DisplayScroll_BuildHblankWordTable((u32 *)BG_SCREEN_BLOCK(31));
  Graphics_ClearCharacterBlockAndPalette(0);
  Graphics_ClearCharacterBlockAndPalette(1);
  Io_Write16(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(2) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(31),
             &REG_BG2CNT);
  Io_Write16(BGCNT_PRIORITY(3) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(15),
             &REG_BG3CNT);
  Io_Write16(DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
                 DISPCNT_OBJ_ON,
             &REG_DISPCNT);
  Io_Write16(BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG3 |
                 BLDCNT_TGT2_BD,
             &REG_BLDCNT);
  // Give the names a five-second lead before the first backdrop.
  DisplayScroll_InitObjectTable();
  WaitFrames(300);
  // Alternate the hidden layer after each 64-frame crossfade. Loading it
  // cannot overwrite the picture that is still showing.
  for (slide = 0; slide < sizeof(gDisplayScroll_SlideResources) /
                              sizeof(gDisplayScroll_SlideResources[0]);
       slide++) {
    Graphics_LoadCharacterBlockAndPalette(gDisplayScroll_SlideResources[slide],
                                          (slide & 1) ^ 1);
    // Fade the new half in: an even slide went to BG2, the first target
    // (BLDALPHA bits 4..0), an odd one to BG3, the second (bits 12..8).
    // The two weights are written out, bottom first: through BLEND_ALPHA
    // the operands swap and the crossfade's registers change.
    for (alpha = 1; alpha <= 16; alpha++) {
      if (slide & 1)
        Io_Write16((alpha << 8) | (16 - alpha), &REG_BLDALPHA);
      else
        Io_Write16(((16 - alpha) << 8) | alpha, &REG_BLDALPHA);
      WaitFrames(4);
    }
    WaitFrames(267);
  }
  Io_Write16(0, &REG_BLDCNT);
  Io_Write16(DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON, &REG_DISPCNT);
  Ui_LoadWindowGraphics();
  DebugText_Clear();
  gOamCopyEnabled = 1;
  return 0;
}

/// One eight-byte OAM entry in the credits' private sprite buffer.
struct DisplayScrollObject {
  u32 attr01; ///< Attribute 0 in the low half, attribute 1 in the high half.
  u32 attr2;  ///< Attribute 2 in the low half; the unused high half stays zero.
};

STATIC_ASSERT(sizeof(struct DisplayScrollObject) == 8);

/// Credits scroll position in pixels, in the low half of a flash work cell.
extern u16 gFlash_Handler3;

// ewram.s reserves four bytes for each cell. The credits replace the task table
// and borrow them while flash operations are inactive: a sprite-buffer pointer,
// the last rendered pixel position, and a zero/-1 end-of-text latch.
// Borrowing them is harmless because the flash library sets them again before
// it reads them: IdentifyFlash, which SaveState_InitializeWorkspace runs before
// each use of the save, writes gFlash, gFlash_Handler0 and gFlash_Handler3, and
// each program routine sets gFlashNumRemainingBytes.
// UB: the native casts alias the flash declarations and store a pointer through
// a declared u16. UBFIX gives these assembler-backed cells their credits views.
#ifdef UBFIX
extern struct DisplayScrollObject *gFlashNumRemainingBytes;
extern s16 gFlash;
extern s16 gFlash_Handler0;

#define SCROLL_OBJECTS gFlashNumRemainingBytes
#define CREDITS_LAST_LINE_POSITION gFlash
#define CREDITS_END_OF_TEXT gFlash_Handler0
#else
#define SCROLL_OBJECTS                                                         \
  (*(struct DisplayScrollObject **)&gFlashNumRemainingBytes)
#define CREDITS_LAST_LINE_POSITION (*(s16 *)&gFlash)
#define CREDITS_END_OF_TEXT (*(s16 *)&gFlash_Handler0)
#endif

/// Blank credits line
static const u8 sBlankCreditLine[] = "";

// These labels are translated credits content, not gameplay patch selectors.
#if LANGUAGE == LANGUAGE_JAPANESE
#define CREDITS_STAFF_TITLE "OUGON NO TAIYO STAFF"
#define CREDITS_FACES_SUBTITLE "& Faces"
#define CREDITS_SOUND_DIRECTION_LABEL "Sound Direction"
#define CREDITS_GRAPHICS_SUPPORT_LABEL "Graphic Support"
#define CREDITS_ARTWORK_LABEL "Artwork Design"
#else
#define CREDITS_STAFF_TITLE "GOLDEN SUN STAFF CREDITS"
#define CREDITS_FACES_SUBTITLE "  & Faces"
#define CREDITS_SOUND_DIRECTION_LABEL "Sound Director"
#define CREDITS_GRAPHICS_SUPPORT_LABEL "Graphic Supports"
#define CREDITS_ARTWORK_LABEL "Artwork"
#endif

/// Credits text, one row per line, terminated by 0. A line is 8 pixels of
/// scroll, 32 frames; the text is drawn with gDisplayScroll_Font, ASCII 0x20
/// to 0x7f.
// clang-format off
const void *gDisplayScroll_LineTable[] = {
    sBlankCreditLine,
    CREDITS_STAFF_TITLE,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Game Planning",
    sBlankCreditLine,
    "Hiroyuki Takahashi",
    "Shugo Takahashi",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Scenario",
    sBlankCreditLine,
    "Hiroyuki Takahashi",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Director",
    sBlankCreditLine,
    "Shugo Takahashi",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Assistant Director",
    sBlankCreditLine,
    "Yusuke Sugimoto",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Game Design",
    sBlankCreditLine,
    "Yusuke Sugimoto",
    "Shuji Shimizu",
    "Ayumu Shindo",
    "Kentaro Sakou",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Programming",
    sBlankCreditLine,
    "System",
    sBlankCreditLine,
    "Yasuhiro Taguchi",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Battle",
    sBlankCreditLine,
    "Haruki Kodera",
    "Toru Takamatsu",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Field",
    sBlankCreditLine,
    "Kaoru Shimada",
    "Hiroyuki Ichikawa",
    "Akiko Sato",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "User Interface",
    sBlankCreditLine,
    "Yutaka Yamamoto",
    "Yasushi Kazawa",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Support",
    sBlankCreditLine,
    "Kenji Numaya",
    "P. E. Jareth Hein",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Graphic Arts",
    sBlankCreditLine,
    "Character Design",
    sBlankCreditLine,
    "Shin Yamanouchi",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Monster Design",
    sBlankCreditLine,
    "Hiroshi Kajiyama",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Battle Characters",
    sBlankCreditLine,
    "Junko Nakamura",
    "Kaori Matsunaga",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Battle Effects",
    CREDITS_FACES_SUBTITLE,
    sBlankCreditLine,
    "Junichi Ochiai",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Battle Backgrounds",
    sBlankCreditLine,
    "Hiroto Nakashima",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Maps",
    sBlankCreditLine,
    "Masayuki Hashimoto",
    "Mitsumasa Muraishi",
    "Mitsuru Fukumoto",
    "Masashi Mikami",
    "Akihito Kimura",
    "Shinichiro Sugimoto",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Map Characters",
    sBlankCreditLine,
    "Mieko Koguchi",
    "Kanako Horiguchi",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Graphics Direction",
    sBlankCreditLine,
    "Fumihide Aoki",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Staff Coordination",
    sBlankCreditLine,
    "Akiko Sato",
    "Kenji Numaya",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Music",
    sBlankCreditLine,
    "Motoi Sakuraba",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Sound Effects",
    sBlankCreditLine,
    "Hiroyuki Hamada",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    CREDITS_SOUND_DIRECTION_LABEL,
    sBlankCreditLine,
    "Masaaki Uno",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Illustration",
    sBlankCreditLine,
    "Shin Yamanouchi",
    "Masanori Sato",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    CREDITS_GRAPHICS_SUPPORT_LABEL,
    sBlankCreditLine,
    "Yoichi Kotabe",
    "Masanori Sato",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    CREDITS_ARTWORK_LABEL,
    sBlankCreditLine,
    "Yoshitaka Nishikawa",
    "Mamoru Sakurai",
    "Nanako Kinoshita",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
#if (LANGUAGE != LANGUAGE_JAPANESE)
    "Coordination",
    sBlankCreditLine,
    "Masaaki Uno",
    "Toshiharu Izuno",
    "Atsushi Okada",
    "Kenichi Nishida",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
#endif
#if (LANGUAGE == LANGUAGE_JAPANESE)
    "Public Relations",
    sBlankCreditLine,
    "Mitsuaki Hagishima",
    "Asako Haruhana",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Coordination",
    sBlankCreditLine,
    "Masaaki Uno",
    "Toshiharu Izuno",
    "Kenichi Nishida",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Special Thanks to",
    "Super Mario Club Staff",
    sBlankCreditLine,
    "Tomohiko Komada",
    "Nobuaki Maeda",
    "Naoki Inoue",
    "Keisuke Moriyasu",
    "Kei Nadabe",
    "Syunsuke Nose",
    "Keiichiro Nishimoto",
    "Toshimasa Minegishi",
    "Hiroyuki Murata",
    "Hisashi Takizaki",
    "Satoru Uno",
    "Akira Nakatsumi",
    "Takeshi Morishima",
    "Yoshikazu Tanaka",
    "Haruka Kato",
    "Kazuyo Ikeda",
    "Yayoi Yagi",
    "Katsunori Masutani",
    "Takafumi Harano",
    "Shinya Tashiro",
    "Tomohiro Nishigaki",
    "Natsuko Shigeta",
    "Takahisa Ohfuji",
    "Masaru Ikeshita",
    "Masaharu Shima",
    "Kouichi Osaka",
    "Naoki Tsuta",
    "Yoshihiro Muramatsu",
    "Tsutomu Mukai",
    "Naotaka Matsumoto",
    "Naoki Takami",
    "Junya Hanai",
    "Kouzou Uchida",
    "Kouji Okamoto",
    "Tomoko Yuasa",
    "Youichiro Ushijima",
    "Koutaro Haneda",
    "Yukio Homma",
    "Kotoe Arihara",
    "Yuji Yamashita",
    "Syogo Tachibana",
    "Mitsuhiro Seno",
    "Takehiko Asaka",
    "Masato Mizuta",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Produced by",
    sBlankCreditLine,
    "Shinji Hatano",
    "Hiroyuki Takahashi",
    "Shugo Takahashi",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Executive Producer",
    sBlankCreditLine,
    "Hiroshi Yamauchi",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Copyrights of Game, Scenario",
    "and Program reserved by",
    sBlankCreditLine,
    "NINTENDO Co.,Ltd.",
    "and",
    "CAMELOT Co.,Ltd.",
    sBlankCreditLine,
    "Copyrights of All Characters",
    "and Music reserved by",
    sBlankCreditLine,
    "NINTENDO Co.,Ltd.",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    0
#elif (LANGUAGE == LANGUAGE_ENGLISH)
    sBlankCreditLine,
    "North American",
    " Localization",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Translation",
    " Coordination",
    sBlankCreditLine,
    "Bill Trinen",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Writing",
    " & Editing",
    sBlankCreditLine,
    "Richard Amtower",
    "Nate Bihldorf",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Additional",
    " Localization",
    sBlankCreditLine,
    "Tim O'Leary",
    "Scot Ritchey",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Localization",
    " Management",
    sBlankCreditLine,
    "Leslie Swan",
    "Jeff Miller",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Testing &",
    " Debugging",
    sBlankCreditLine,
    "Michael Kelbaugh",
    "Todd Buechele",
    "NOA Product Testing",
    " Department",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Special Thanks to",
    sBlankCreditLine,
    "Motoyasu Tojima",
    "Mike Fukuda",
    sBlankCreditLine,
    " and",
    sBlankCreditLine,
    sBlankCreditLine,
    "Minoru Arakawa",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Producer",
    sBlankCreditLine,
    "Shinji Hatano",
    "Hiroyuki Takahashi",
    "Shugo Takahashi",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Executive Producer",
    sBlankCreditLine,
    "Hiroshi Yamauchi",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Copyrights of Game, Scenario,",
    "and Program reserved by",
    sBlankCreditLine,
    "NINTENDO and CAMELOT.",
    sBlankCreditLine,
    "Copyrights of all characters",
    "and music reserved by",
    sBlankCreditLine,
    "NINTENDO.",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    0
#else
    // European releases share this order, with local staff and publisher rows.
    "North American",
    " Localization",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Translation",
    " Coordination",
    sBlankCreditLine,
    "Bill Trinen",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Writing",
    " & Editing",
    sBlankCreditLine,
    "Richard Amtower",
    "Nate Bihldorf",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Additional   ",
    " Localization",
    sBlankCreditLine,
    "Tim O'Leary",
    "Scot Ritchey",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Localization",
    " Management ",
    sBlankCreditLine,
    "Leslie Swan",
    "Jeff Miller",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Localization & ",
    " Testing Europe",
    sBlankCreditLine,
    "Andy Fey",
    "Kai Zeh",
    "Tanja Baar",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
#if (LANGUAGE == LANGUAGE_GERMAN)
    "German Translation",
    sBlankCreditLine,
    "Micky Auer",
    "Martina Deimel",
#elif (LANGUAGE == LANGUAGE_SPANISH)
    "Spanish Translation",
    sBlankCreditLine,
    "Carlos Montilla",
    "Miguel Lopez",
#elif (LANGUAGE == LANGUAGE_FRENCH)
    "French Translation",
    sBlankCreditLine,
    "Daniel Charbit",
    "Steven Arhan",
#elif (LANGUAGE == LANGUAGE_ITALIAN)
    "Italian Translation",
    sBlankCreditLine,
    "Andrea Salvi",
    "Calimero Tiiijun",
#endif
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
#if (LANGUAGE == LANGUAGE_GERMAN)
    "German Testing",
    sBlankCreditLine,
    "Maurice Tisdale",
    "Andreas Dietz",
    "Rudolf Schnitzer",
    "Wolfgang Weber",
    "Frank Heuchle",
    "Zoran Tasevski",
#elif (LANGUAGE == LANGUAGE_SPANISH)
    "Spanish Testing",
    sBlankCreditLine,
    "Maurice Tisdale",
    "Andreas Dietz",
    "Luis Martinez",
    "Jose Ramon Diaz",
    "Ana Paris",
    "Monica Gomez",
#elif (LANGUAGE == LANGUAGE_FRENCH)
    "French Testing",
    sBlankCreditLine,
    "Maurice Tisdale",
    "Peter Ehlert",
    "Emeline Condy",
    "Pascal Di Pasquale",
    "Kamel Chelirem",
    "Sebastien Aubre",
    "Michael Troger",
#elif (LANGUAGE == LANGUAGE_ITALIAN)
    "Italian Testing",
    sBlankCreditLine,
    "Maurice Tisdale",
    "Andreas Dietz",
    "Sonia Trepiccioni",
    "Michele Liuzzi",
    "Luca Riccioni",
    "Andrea De Benedetto",
#endif
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Special Thanks to",
    sBlankCreditLine,
    "Motoyasu Tojima",
    "Takako Morita",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Producer",
    sBlankCreditLine,
    "Shinji Hatano",
    "Hiroyuki Takahashi",
    "Shugo Takahashi",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Executive Producer",
    sBlankCreditLine,
    "Hiroshi Yamauchi",
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    "Copyrights of Game, Scenario,",
    "and Program reserved by",
    sBlankCreditLine,
    "NINTENDO and CAMELOT.",
    sBlankCreditLine,
    "Copyrights of all characters",
    "and music reserved by",
    sBlankCreditLine,
#if (LANGUAGE != LANGUAGE_FRENCH)
    "NINTENDO.",
#endif
    sBlankCreditLine,
    sBlankCreditLine,
    sBlankCreditLine,
    0
#endif
};
// clang-format on

#include "../display/glyph_widths.h"

/// A VBlank task scheduled by DisplayScroll_InitObjectTable: maps the visible
/// sixteen rows onto the tile ring and uploads their sprites. Advances one
/// pixel every fourth frame until the end-of-text latch stops the scroll.
void DisplayScroll_UpdateObjects(void) {
  u32 frame = gFlash_Handler3;
  u32 scrollWithinTile = frame & 7;
  s32 tileIndex =
      (((s16)frame / 8) & (CREDITS_RING_ROWS - 1)) * CREDITS_LINE_TILES;
  u32 *oamWords = (u32 *)(SCROLL_OBJECTS + CREDITS_BORDER_SPRITES);
  s32 row;

  // Sixteen rows of six 32x8 sprites from y 16 and x 24, raised by the
  // scroll within a line, showing the ring from the line at the top.
  for (row = 0; row < CREDITS_VISIBLE_ROWS; row++) {
    s32 column;

    for (column = 0; column < CREDITS_COLUMNS; column++) {
      *oamWords++ = (row * 8 + 16 - scrollWithinTile) |
                    ((column * 32 + 24) << 16) | 0x40004000;
      *oamWords++ = tileIndex;
      tileIndex += 4;
      if (tileIndex == CREDITS_RING_TILES)
        tileIndex = 0;
    }
  }
  Dma_Set(SCROLL_OBJECTS, OAM, DMA_COPY_WORDS(OAM_SIZE / 4), REG_DMA3);
  if (CREDITS_END_OF_TEXT == 0 && (gFrameTick & 3) == 0)
    gFlash_Handler3++;
}

/// A main-loop task scheduled by DisplayScroll_InitObjectTable: renders each
/// entering line sixteen rows ahead of the visible top, into the hidden half
/// of the tile ring. A null table entry latches the end-of-text state.
void DisplayScroll_RenderEnteringLine(void) {
  u32 scrollPosition;
  s32 lineIndex;

  // A line is 8 pixels, so a new one is drawn each time the scroll crosses
  // into the next line, every 32 frames.
  if (CREDITS_END_OF_TEXT == 0) {
    scrollPosition = gFlash_Handler3;
    if ((s16)gFlash_Handler3 / 8 != CREDITS_LAST_LINE_POSITION / 8) {
      lineIndex = (s16)gFlash_Handler3 / 8;
      CREDITS_LAST_LINE_POSITION = scrollPosition;
      CREDITS_END_OF_TEXT = DisplayScroll_DrawLine(
          gDisplayScroll_LineTable[lineIndex],
          ((lineIndex + CREDITS_VISIBLE_ROWS) & (CREDITS_RING_ROWS - 1)) *
              CREDITS_LINE_TILES,
          1);
    }
  }
}

/// Allocates the credits sprites, builds the blank border and text rows,
/// clears the tile ring and schedules scrolling and line rendering. Called by
/// DisplayScroll_RunSlideshow; the credits keep their state in borrowed
/// flash-library cells (see the graphics README). The 1 KB sprite buffer
/// comes from the alternate bump pool and is never freed; each line buffer
/// is taken from the pool after it and given back.
void DisplayScroll_InitObjectTable(void) {
  // Sharing the three border-strip loops through an ordinary inline helper
  // changes registers at the same 0x178-byte native English extent.
  volatile u32 dmaFill;
  u32 *oamWords;
  u32 row;
  u32 column;

  SCROLL_OBJECTS = Runtime_BumpAllocateAlternatePool(
      CREDITS_OAM_ENTRIES * sizeof(struct DisplayScrollObject));
  // Solid border sprites immediately after the ring mask the partial text
  // rows at the top and bottom of the window.
  Dma_Fill32Cell(dmaFill, 0, (void *)OBJ_VRAM0, CREDITS_RING_BYTES);
  Dma_Fill32Cell(dmaFill, 0x11111111, (void *)(OBJ_VRAM0 + CREDITS_RING_BYTES),
                 0x100);

  // OAM attributes 0 and 1 share the first word; attribute 2 is the second.
  // Here row counts the sprites across, 32 pixels apart (row << 21 is x).
  // The top strip: eight 32x16 sprites over y 0 to 15. Coming first in OAM,
  // the strips are drawn over the text.
  oamWords = (u32 *)SCROLL_OBJECTS;
  for (row = 0; row < 8; row++) {
    *oamWords++ = (row << 21) | 0x80004000;
    *oamWords++ = CREDITS_RING_TILES;
  }
  // The bottom strip: eight 32x16 sprites at y 136 and eight 32x8 at y 152,
  // over y 136 to 159.
  for (row = 0; row < 8; row++) {
    *oamWords++ = (row << 21) | 0x80004088;
    *oamWords++ = CREDITS_RING_TILES;
  }
  for (row = 0; row < 8; row++) {
    *oamWords++ = (row << 21) | 0x40004098;
    *oamWords++ = CREDITS_RING_TILES;
  }
  // The text sprites, as DisplayScroll_UpdateObjects rebuilds them each
  // VBlank.
  for (row = 0; row < CREDITS_VISIBLE_ROWS; row++) {
    for (column = 0; column < CREDITS_COLUMNS; column++) {
      u32 tileIndex = (row * 3) * 8 + column * 4;
      *oamWords++ = (row * 8 + 16) | ((column * 32 + 24) << 16) | 0x40004000;
      *oamWords++ = tileIndex;
    }
  }
  // The last eight entries are parked off screen at (192, 192).
  for (row = 0; row < 8; row++) {
    *oamWords++ = 0x00c000c0;
    *oamWords++ = CREDITS_RING_TILES;
  }
  gFlash_Handler3 = 0;
  CREDITS_LAST_LINE_POSITION = 0;
  CREDITS_END_OF_TEXT = 0;
  Scheduler_AddOrUpdateCallback(DisplayScroll_UpdateObjects,
                                SCHEDULER_KEY_VBLANK);
  Scheduler_AddOrUpdateCallback(DisplayScroll_RenderEnteringLine,
                                SCHEDULER_KEY_MAIN_LOOP);
  for (row = 0; row < CREDITS_RING_ROWS; row++)
    DisplayScroll_DrawLine(gDisplayScroll_LineTable[0],
                           row * CREDITS_LINE_TILES, 1);
}

/// Centers, right-aligns or left-aligns one credits line in a 192-pixel row.
/// Rasterizes its glyphs and shadow, packs 4-bit pixels and uploads 24 tiles
/// beginning at firstTile. Called by the line-rendering task and, for the blank
/// rows, by DisplayScroll_InitObjectTable. Returns -1 at the null end marker,
/// otherwise zero. alignment 1 centres, 2 right-aligns and any other value
/// left-aligns; every caller passes 1. The glyphs are colour 15 with a colour
/// 1 shadow a pixel down and right.
s32 DisplayScroll_DrawLine(const u8 *text, s32 firstTile, s32 alignment) {
  u8 *linePixels = Runtime_BumpAllocateAlternatePool(CREDITS_BUFFER_BYTES);
  s32 startX = 0;
  s32 lineWidth = CREDITS_LINE_PIXELS;
  const u8 *font = gDisplayScroll_Font;
  s32 tileCount;
  s32 textAdvance;
  const u8 *sourceBytes;
  u8 *destination;
  u32 sourceValue;
  s32 glyphIndex;
  s32 glyphWidth;
  s32 row;
  s32 columnIndex;
  u32 glyphBits;
  u32 glyphMask;

  /* FAKEMATCH(dead): an unused array for the frame's 32 bytes no instruction
   * touches; as no array, 2 of 277 instructions differ in tbs-en. */
  UNUSED u32 unused[8];

  if (text == NULL) {
    // BUG: the end marker is checked after allocation, leaking one line
    // buffer. MemFree rewinds this bump pool to the allocation's start.
#ifdef BUGFIX
    if (linePixels != NULL)
      MemFree(linePixels);
#endif
    return -1;
  }
  // The ninth raster row holds the shadow below this line. Carry it into
  // the next line before clearing the rest of the reused scratch buffer.
  // The buffer comes back at the same address each call (MemFree rewinds the
  // pool), so the last line's ninth row is still there. FLAG_SCENE_TEMP_00,
  // here a line has been drawn, is clear on entering the scene
  // (Scene_ResetFlagsOnEnter), so the first line starts from a clean buffer.
  if (!GameFlag_TestFar(FLAG_SCENE_TEMP_00)) {
    Iwram_FillWords(linePixels, CREDITS_BUFFER_BYTES, 0);
    GameFlag_SetBitFar(FLAG_SCENE_TEMP_00);
  } else {
    Iwram_CopyWords(linePixels, linePixels + 0x800, 0x100);
    Iwram_FillWords(linePixels + 0x100, 0x800, 0);
  }

  // Measure text with the actual credits font before choosing its start.
  // A previous character/pixel cursor split grew the frame 44 -> 48 bytes
  // and changed native registers; retain the shared byte cursors here.
  sourceBytes = text;
  textAdvance = 0;
  while ((sourceValue = *sourceBytes++) != 0) {
    if (sourceValue > 31)
      textAdvance += gDisplayScroll_GlyphWidths[sourceValue - 32];
  }
  if (alignment == 2)
    startX = lineWidth - textAdvance;
  else if (alignment == 1)
    startX = (lineWidth - textAdvance) / 2;

  // Rasterize each glyph and its one-pixel shadow at the aligned start.
  sourceBytes = text;
  textAdvance = 0;
  while ((sourceValue = *text++) != 0) {
    if (sourceValue > 31) {
      glyphIndex = sourceValue - 32;
      sourceBytes = font + glyphIndex * 8;
      destination = linePixels + startX + textAdvance;
      for (row = 0; row < 8; row++) {
        glyphBits = *sourceBytes++;
        glyphMask = 0x80;
        for (columnIndex = 7; columnIndex >= 0; columnIndex--) {
          if (glyphBits & glyphMask) {
            destination[0x101] = 1;
            destination[0] = 15;
          }
          destination++;
          glyphMask >>= 1;
        }
        destination += CREDITS_RASTER_STRIDE - 8;
      }
      glyphWidth = 1;
      if (sourceValue > 31)
        glyphWidth = gDisplayScroll_GlyphWidths[glyphIndex];
      textAdvance += glyphWidth;
    }
  }

  // Pack pairs of pixel indices into bytes, keeping the 256-pixel stride.
  tileCount = lineWidth / 8;
  sourceBytes = linePixels;
  destination = linePixels;
  for (row = 0; row < 8; row++) {
    for (columnIndex = 0; columnIndex < tileCount * 4; columnIndex++) {
      sourceValue = *sourceBytes++;
      sourceValue |= *sourceBytes++ << 4;
      *destination++ = sourceValue;
    }
    destination += CREDITS_RASTER_STRIDE - tileCount * 4;
    sourceBytes += CREDITS_RASTER_STRIDE - tileCount * 8;
  }

  // Transpose raster rows into the tile ring used by the scrolling sprites.
  for (row = 0; row < tileCount; row++) {
    s32 tileIndex = firstTile + row;
    u32 *words = (u32 *)linePixels + row;

    *(u32 *)(OBJ_VRAM0 + tileIndex * 32) = words[0x000];
    *(u32 *)(0x06010004 + tileIndex * 32) = words[0x040];
    *(u32 *)(0x06010008 + tileIndex * 32) = words[0x080];
    *(u32 *)(0x0601000c + tileIndex * 32) = words[0x0c0];
    *(u32 *)(0x06010010 + tileIndex * 32) = words[0x100];
    *(u32 *)(0x06010014 + tileIndex * 32) = words[0x140];
    *(u32 *)(0x06010018 + tileIndex * 32) = words[0x180];
    *(u32 *)(0x0601001c + tileIndex * 32) = words[0x1c0];
  }
  MemFree(linePixels);
  return 0;
}
