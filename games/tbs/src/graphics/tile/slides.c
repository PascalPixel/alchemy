//--------------------------------------------------------------------------
// clear_character_block.c
// Clear half of the background tile graphics and palette.
//--------------------------------------------------------------------------

#include "unit/graphics/tile/slides.h"
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
#include "layout_guard.h"
#include "battle_party.h"
#include "battle_system.h"
#include "frame.h"

//--------------------------------------------------------------------------
// Fill 30 KB of 256-color tiles with color 1 and clear 128 palette colors.
// alt: 0 selects the first half (VRAM 0x06000000, palette 0-127).
//      1 selects the second half (VRAM 0x06008000, palette 128-255, fill 129).
//--------------------------------------------------------------------------
void Graphics_ClearCharacterBlockAndPalette(s32 alt) {
  u32 v;
  u32 vram;
  u32 pal;
  volatile u32 fill;

  if (alt == 0) {
    v = 0x01010101;
    vram = 0x06000000;
    pal = 0x05000000;
  } else {
    v = 0x81818181;
    vram = 0x06008000;
    pal = 0x05000100;
  }
  Dma_Fill32Cell(fill, v, (void *)vram, 0x7800);
  Dma_Fill32Cell(fill, 0, (void *)pal, 0x100);
}

typedef s32 (*PackedDecoder)(u8 *source, u32 destination, u32 fill);

// Battle backgrounds shown in the credits; 0 clears the display.
// clang-format off
const u32 gDisplayScroll_SlideResources[] = {
    (u32)&ResourceId_ValeStormBackdrop,
    (u32)&ResourceId_SolSanctumBackdrop,
    (u32)&ResourceId_GrasslandBackdrop,
    (u32)&ResourceId_InteriorBackdrop,
    (u32)&ResourceId_ForestBackdrop,
    (u32)&ResourceId_GomaCaveBackdrop,
    (u32)&ResourceId_KolimaForestBackdrop,
    (u32)&ResourceId_TretInteriorsBackdrop,
    (u32)&ResourceId_TretBranchBackdrop,
    (u32)&ResourceId_SnowfieldBackdrop,
    (u32)&ResourceId_MercuryLighthouseBackdrop,
    (u32)&ResourceId_MercuryAerieBackdrop,
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
    (u32)&ResourceId_VenusAerieBackdrop,
    (u32)&ResourceId_VenusAerieStormBackdrop,
    (u32)&ResourceId_DuskCloudsBackdrop,
    (u32)&ResourceId_VioletSkyBackdrop,
    0               // Clear
};
// clang-format on

void Graphics_LoadCharacterBlockAndPalette(u32 resource, s32 alternate) {
  u8 *data;
  u32 palette;
  u32 fill;
  u32 vram;

  if (resource == 0) {
    Graphics_ClearCharacterBlockAndPalette(alternate);
    return;
  }
  data = Resource_GetTableEntry(resource);
  if (alternate == 0) {
    fill = 0;
    vram = 0x06000000;
    palette = 0x05000000;
  } else {
    fill = 0x80808080;
    vram = 0x06008000;
    palette = 0x05000100;
  }
  RamCode_LoadSlot(49, BattleEffect_Expand, BattleEffect_ExpandCodeSize);
  ((PackedDecoder)((union HeapState *)gWorkSlot)->slots[49])(data + 256, vram,
                                                             fill);
  Runtime_ReleaseHeapBlock(49);

  IO_WRITE_QUEUE_PUSH(palette, (u32)data, 0x84000040);
}

void DisplayScroll_BuildHblankWordTable(u32 *arg0) {
  s32 count;
  u32 value = 0x01FF01FF;
  u32 step = 0x10000;

  count = 31;
  do {
    count--;
    *arg0++ = value;
  } while (count >= 0);
  count = 239;
  do {
    count--;
    *arg0++ = step;
    step += 0x20002;
  } while (count >= 0);
  count = 47;
  do {
    count--;
    *arg0++ = value;
  } while (count >= 0);
  step = 0;
  count = 191;
  do {
    count--;
    *arg0++ = step;
  } while (count >= 0);
}

void DisplayScroll_StepPositionEveryFourFrames(void) {
  if ((gFrameTick & 3) == 0) {
    u32 decrement = 0xffff;

    gBgScroll[2].x += decrement;
    gBgScroll[3].x += decrement;
  }
}

s32 DisplayScroll_RunSlideshow(void) {
  u32 slide;
  s32 alpha;

  gOamCopyEnabled = 0;
  gCpuLoadDisplayEnabled = 0;
  gDebugTextEnabled = 0;
  gIdleSleepEnabled = 0;
  Scheduler_ResetTaskTable();
  Scheduler_AddOrUpdateCallback(DisplayScroll_StepPositionEveryFourFrames,
                                0x480);
  Io_Write16(0x40, &REG_DISPCNT);
  DisplayScroll_BuildHblankWordTable((u32 *)0x06007800);
  DisplayScroll_BuildHblankWordTable((u32 *)0x0600f800);
  Graphics_ClearCharacterBlockAndPalette(0);
  Graphics_ClearCharacterBlockAndPalette(1);
  Io_Write16(0x1f8a, &REG_BG2CNT);
  Io_Write16(0x0f83, &REG_BG3CNT);
  Io_Write16(0x1c40, &REG_DISPCNT);
  Io_Write16(0x2844, &REG_BLDCNT);
  DisplayScroll_InitObjectTable();
  WaitFrames(300);
  for (slide = 0; slide <= 32; slide++) {
    Graphics_LoadCharacterBlockAndPalette(gDisplayScroll_SlideResources[slide],
                                          (slide & 1) ^ 1);
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
  Io_Write16(0x1040, &REG_DISPCNT);
  Ui_LoadWindowGraphics();
  DebugText_Clear();
  gOamCopyEnabled = 1;
  return 0;
}

struct DisplayScrollObject {
  u32 attr01;
  u32 attr2;
};

extern u16 gFlash_Handler3; // The credits reuse a flash-library work word.

// 128 entries sent to OAM, also stored in flash-library work space.
#define SCROLL_OBJECTS                                                         \
  (*(struct DisplayScrollObject **)&gFlashNumRemainingBytes)

// Blank credits line
static const u8 sBlank[] = "";

// Credits text, one row per line, terminated by 0.
// clang-format off
const void *gDisplayScroll_LineTable[] = {
    sBlank,
#if EDITION_INTERNATIONAL
    "GOLDEN SUN STAFF CREDITS",
#else
    "OUGON NO TAIYO STAFF",
#endif
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Game Planning",
    sBlank,
    "Hiroyuki Takahashi",
    "Shugo Takahashi",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Scenario",
    sBlank,
    "Hiroyuki Takahashi",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Director",
    sBlank,
    "Shugo Takahashi",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Assistant Director",
    sBlank,
    "Yusuke Sugimoto",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Game Design",
    sBlank,
    "Yusuke Sugimoto",
    "Shuji Shimizu",
    "Ayumu Shindo",
    "Kentaro Sakou",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Programming",
    sBlank,
    "System",
    sBlank,
    "Yasuhiro Taguchi",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Battle",
    sBlank,
    "Haruki Kodera",
    "Toru Takamatsu",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Field",
    sBlank,
    "Kaoru Shimada",
    "Hiroyuki Ichikawa",
    "Akiko Sato",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "User Interface",
    sBlank,
    "Yutaka Yamamoto",
    "Yasushi Kazawa",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Support",
    sBlank,
    "Kenji Numaya",
    "P. E. Jareth Hein",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Graphic Arts",
    sBlank,
    "Character Design",
    sBlank,
    "Shin Yamanouchi",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Monster Design",
    sBlank,
    "Hiroshi Kajiyama",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Battle Characters",
    sBlank,
    "Junko Nakamura",
    "Kaori Matsunaga",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Battle Effects",
#if EDITION_INTERNATIONAL
    "  & Faces",
#else
    "& Faces",
#endif
    sBlank,
    "Junichi Ochiai",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Battle Backgrounds",
    sBlank,
    "Hiroto Nakashima",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Maps",
    sBlank,
    "Masayuki Hashimoto",
    "Mitsumasa Muraishi",
    "Mitsuru Fukumoto",
    "Masashi Mikami",
    "Akihito Kimura",
    "Shinichiro Sugimoto",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Map Characters",
    sBlank,
    "Mieko Koguchi",
    "Kanako Horiguchi",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Graphics Direction",
    sBlank,
    "Fumihide Aoki",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Staff Coordination",
    sBlank,
    "Akiko Sato",
    "Kenji Numaya",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Music",
    sBlank,
    "Motoi Sakuraba",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Sound Effects",
    sBlank,
    "Hiroyuki Hamada",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
#if EDITION_INTERNATIONAL
    "Sound Director",
#else
    "Sound Direction",
#endif
    sBlank,
    "Masaaki Uno",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Illustration",
    sBlank,
    "Shin Yamanouchi",
    "Masanori Sato",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
#if EDITION_INTERNATIONAL
    "Graphic Supports",
#else
    "Graphic Support",
#endif
    sBlank,
    "Yoichi Kotabe",
    "Masanori Sato",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
#if EDITION_INTERNATIONAL
    "Artwork",
#else
    "Artwork Design",
#endif
    sBlank,
    "Yoshitaka Nishikawa",
    "Mamoru Sakurai",
    "Nanako Kinoshita",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
#if EDITION_INTERNATIONAL
    "Coordination",
    sBlank,
    "Masaaki Uno",
    "Toshiharu Izuno",
    "Atsushi Okada",
    "Kenichi Nishida",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
#endif
#if !EDITION_INTERNATIONAL
    "Public Relations",
    sBlank,
    "Mitsuaki Hagishima",
    "Asako Haruhana",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Coordination",
    sBlank,
    "Masaaki Uno",
    "Toshiharu Izuno",
    "Kenichi Nishida",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Special Thanks to",
    "Super Mario Club Staff",
    sBlank,
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
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Produced by",
    sBlank,
    "Shinji Hatano",
    "Hiroyuki Takahashi",
    "Shugo Takahashi",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Executive Producer",
    sBlank,
    "Hiroshi Yamauchi",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Copyrights of Game, Scenario",
    "and Program reserved by",
    sBlank,
    "NINTENDO Co.,Ltd.",
    "and",
    "CAMELOT Co.,Ltd.",
    sBlank,
    "Copyrights of All Characters",
    "and Music reserved by",
    sBlank,
    "NINTENDO Co.,Ltd.",
    sBlank,
    sBlank,
    sBlank,
    0
#elif defined(TBS_EDITION_EN)
    sBlank,
    "North American",
    " Localization",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Translation",
    " Coordination",
    sBlank,
    "Bill Trinen",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Writing",
    " & Editing",
    sBlank,
    "Richard Amtower",
    "Nate Bihldorf",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Additional",
    " Localization",
    sBlank,
    "Tim O'Leary",
    "Scot Ritchey",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Localization",
    " Management",
    sBlank,
    "Leslie Swan",
    "Jeff Miller",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Testing &",
    " Debugging",
    sBlank,
    "Michael Kelbaugh",
    "Todd Buechele",
    "NOA Product Testing",
    " Department",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Special Thanks to",
    sBlank,
    "Motoyasu Tojima",
    "Mike Fukuda",
    sBlank,
    " and",
    sBlank,
    sBlank,
    "Minoru Arakawa",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Producer",
    sBlank,
    "Shinji Hatano",
    "Hiroyuki Takahashi",
    "Shugo Takahashi",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Executive Producer",
    sBlank,
    "Hiroshi Yamauchi",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Copyrights of Game, Scenario,",
    "and Program reserved by",
    sBlank,
    "NINTENDO and CAMELOT.",
    sBlank,
    "Copyrights of all characters",
    "and music reserved by",
    sBlank,
    "NINTENDO.",
    sBlank,
    sBlank,
    sBlank,
    0
#elif defined(TBS_EDITION_DE)
    "North American",
    " Localization",
    sBlank,
    sBlank,
    sBlank,
    "Translation",
    " Coordination",
    sBlank,
    "Bill Trinen",
    sBlank,
    sBlank,
    sBlank,
    "Writing",
    " & Editing",
    sBlank,
    "Richard Amtower",
    "Nate Bihldorf",
    sBlank,
    sBlank,
    sBlank,
    "Additional   ",
    " Localization",
    sBlank,
    "Tim O'Leary",
    "Scot Ritchey",
    sBlank,
    sBlank,
    sBlank,
    "Localization",
    " Management ",
    sBlank,
    "Leslie Swan",
    "Jeff Miller",
    sBlank,
    sBlank,
    sBlank,
    "Localization & ",
    " Testing Europe",
    sBlank,
    "Andy Fey",
    "Kai Zeh",
    "Tanja Baar",
    sBlank,
    sBlank,
    sBlank,
    "German Translation",
    sBlank,
    "Micky Auer",
    "Martina Deimel",
    sBlank,
    sBlank,
    sBlank,
    "German Testing",
    sBlank,
    "Maurice Tisdale",
    "Andreas Dietz",
    "Rudolf Schnitzer",
    "Wolfgang Weber",
    "Frank Heuchle",
    "Zoran Tasevski",
    sBlank,
    sBlank,
    sBlank,
    "Special Thanks to",
    sBlank,
    "Motoyasu Tojima",
    "Takako Morita",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Producer",
    sBlank,
    "Shinji Hatano",
    "Hiroyuki Takahashi",
    "Shugo Takahashi",
    sBlank,
    sBlank,
    sBlank,
    "Executive Producer",
    sBlank,
    "Hiroshi Yamauchi",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Copyrights of Game, Scenario,",
    "and Program reserved by",
    sBlank,
    "NINTENDO and CAMELOT.",
    sBlank,
    "Copyrights of all characters",
    "and music reserved by",
    sBlank,
    "NINTENDO.",
    sBlank,
    sBlank,
    sBlank,
    0
#elif defined(TBS_EDITION_ES)
    "North American",
    " Localization",
    sBlank,
    sBlank,
    sBlank,
    "Translation",
    " Coordination",
    sBlank,
    "Bill Trinen",
    sBlank,
    sBlank,
    sBlank,
    "Writing",
    " & Editing",
    sBlank,
    "Richard Amtower",
    "Nate Bihldorf",
    sBlank,
    sBlank,
    sBlank,
    "Additional   ",
    " Localization",
    sBlank,
    "Tim O'Leary",
    "Scot Ritchey",
    sBlank,
    sBlank,
    sBlank,
    "Localization",
    " Management ",
    sBlank,
    "Leslie Swan",
    "Jeff Miller",
    sBlank,
    sBlank,
    sBlank,
    "Localization & ",
    " Testing Europe",
    sBlank,
    "Andy Fey",
    "Kai Zeh",
    "Tanja Baar",
    sBlank,
    sBlank,
    sBlank,
    "Spanish Translation",
    sBlank,
    "Carlos Montilla",
    "Miguel Lopez",
    sBlank,
    sBlank,
    sBlank,
    "Spanish Testing",
    sBlank,
    "Maurice Tisdale",
    "Andreas Dietz",
    "Luis Martinez",
    "Jose Ramon Diaz",
    "Ana Paris",
    "Monica Gomez",
    sBlank,
    sBlank,
    sBlank,
    "Special Thanks to",
    sBlank,
    "Motoyasu Tojima",
    "Takako Morita",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Producer",
    sBlank,
    "Shinji Hatano",
    "Hiroyuki Takahashi",
    "Shugo Takahashi",
    sBlank,
    sBlank,
    sBlank,
    "Executive Producer",
    sBlank,
    "Hiroshi Yamauchi",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Copyrights of Game, Scenario,",
    "and Program reserved by",
    sBlank,
    "NINTENDO and CAMELOT.",
    sBlank,
    "Copyrights of all characters",
    "and music reserved by",
    sBlank,
    "NINTENDO.",
    sBlank,
    sBlank,
    sBlank,
    0
#elif defined(TBS_EDITION_FR)
    "North American",
    " Localization",
    sBlank,
    sBlank,
    sBlank,
    "Translation",
    " Coordination",
    sBlank,
    "Bill Trinen",
    sBlank,
    sBlank,
    sBlank,
    "Writing",
    " & Editing",
    sBlank,
    "Richard Amtower",
    "Nate Bihldorf",
    sBlank,
    sBlank,
    sBlank,
    "Additional   ",
    " Localization",
    sBlank,
    "Tim O'Leary",
    "Scot Ritchey",
    sBlank,
    sBlank,
    sBlank,
    "Localization",
    " Management ",
    sBlank,
    "Leslie Swan",
    "Jeff Miller",
    sBlank,
    sBlank,
    sBlank,
    "Localization & ",
    " Testing Europe",
    sBlank,
    "Andy Fey",
    "Kai Zeh",
    "Tanja Baar",
    sBlank,
    sBlank,
    sBlank,
    "French Translation",
    sBlank,
    "Daniel Charbit",
    "Steven Arhan",
    sBlank,
    sBlank,
    sBlank,
    "French Testing",
    sBlank,
    "Maurice Tisdale",
    "Peter Ehlert",
    "Emeline Condy",
    "Pascal Di Pasquale",
    "Kamel Chelirem",
    "Sebastien Aubre",
    "Michael Troger",
    sBlank,
    sBlank,
    sBlank,
    "Special Thanks to",
    sBlank,
    "Motoyasu Tojima",
    "Takako Morita",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Producer",
    sBlank,
    "Shinji Hatano",
    "Hiroyuki Takahashi",
    "Shugo Takahashi",
    sBlank,
    sBlank,
    sBlank,
    "Executive Producer",
    sBlank,
    "Hiroshi Yamauchi",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Copyrights of Game, Scenario,",
    "and Program reserved by",
    sBlank,
    "NINTENDO and CAMELOT.",
    sBlank,
    "Copyrights of all characters",
    "and music reserved by",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    0
#else
    "North American",
    " Localization",
    sBlank,
    sBlank,
    sBlank,
    "Translation",
    " Coordination",
    sBlank,
    "Bill Trinen",
    sBlank,
    sBlank,
    sBlank,
    "Writing",
    " & Editing",
    sBlank,
    "Richard Amtower",
    "Nate Bihldorf",
    sBlank,
    sBlank,
    sBlank,
    "Additional   ",
    " Localization",
    sBlank,
    "Tim O'Leary",
    "Scot Ritchey",
    sBlank,
    sBlank,
    sBlank,
    "Localization",
    " Management ",
    sBlank,
    "Leslie Swan",
    "Jeff Miller",
    sBlank,
    sBlank,
    sBlank,
    "Localization & ",
    " Testing Europe",
    sBlank,
    "Andy Fey",
    "Kai Zeh",
    "Tanja Baar",
    sBlank,
    sBlank,
    sBlank,
    "Italian Translation",
    sBlank,
    "Andrea Salvi",
    "Calimero Tiiijun",
    sBlank,
    sBlank,
    sBlank,
    "Italian Testing",
    sBlank,
    "Maurice Tisdale",
    "Andreas Dietz",
    "Sonia Trepiccioni",
    "Michele Liuzzi",
    "Luca Riccioni",
    "Andrea De Benedetto",
    sBlank,
    sBlank,
    sBlank,
    "Special Thanks to",
    sBlank,
    "Motoyasu Tojima",
    "Takako Morita",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Producer",
    sBlank,
    "Shinji Hatano",
    "Hiroyuki Takahashi",
    "Shugo Takahashi",
    sBlank,
    sBlank,
    sBlank,
    "Executive Producer",
    sBlank,
    "Hiroshi Yamauchi",
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    sBlank,
    "Copyrights of Game, Scenario,",
    "and Program reserved by",
    sBlank,
    "NINTENDO and CAMELOT.",
    sBlank,
    "Copyrights of all characters",
    "and music reserved by",
    sBlank,
    "NINTENDO.",
    sBlank,
    sBlank,
    sBlank,
    0
#endif
};
// clang-format on

#include "../../../../common/src/graphics/display/glyph_widths.h"

void DisplayScroll_UpdateObjects(void) {
  u32 frame = gFlash_Handler3;
  u32 fine = frame & 7;
  s32 tile = (((s16)frame / 8) & 0x1f) * 3 * 8;
  u32 *entry = (u32 *)(SCROLL_OBJECTS + 24);
  s32 row;

  for (row = 0; row <= 15; row++) {
    s32 col;

    for (col = 0; col < 6; col++) {
      *entry++ = (row * 8 + 16 - fine) | ((col * 32 + 24) << 16) | 0x40004000;
      *entry++ = tile;
      tile += 4;
      if (tile == 0x300)
        tile = 0;
    }
  }
  Dma_Set(SCROLL_OBJECTS, OAM, 0x84000100, REG_DMA3);
  if ((*(s16 *)&gFlash_Handler0) == 0 && (gFrameTick & 3) == 0)
    gFlash_Handler3++;
}

void DisplayScroll_RenderEnteringLine(void) {
  u32 current;
  s32 line;

  if ((*(s16 *)&gFlash_Handler0) == 0) {
    current = gFlash_Handler3;
    if ((s16)gFlash_Handler3 / 8 != *(s16 *)&gFlash / 8) {
      line = (s16)gFlash_Handler3 / 8;
      *(s16 *)&gFlash = current;
      (*(s16 *)&gFlash_Handler0) = DisplayScroll_DrawLine(
          gDisplayScroll_LineTable[line], ((line + 16) & 31) * 24, 1);
    }
  }
}

void DisplayScroll_InitObjectTable(void) {
  volatile u32 fill;
  u32 *entry;
  u32 i;
  u32 j;

  SCROLL_OBJECTS = Runtime_BumpAllocateAlternatePool(
      128 * sizeof(struct DisplayScrollObject));
  Dma_Fill32Cell(fill, 0, (void *)0x06010000, 0x6000);
  Dma_Fill32Cell(fill, 0x11111111, (void *)0x06016000, 0x100);

  entry = (u32 *)SCROLL_OBJECTS;
  for (i = 0; i < 8; i++) {
    *entry++ = (i << 21) | 0x80004000;
    *entry++ = 0x300;
  }
  for (i = 0; i < 8; i++) {
    *entry++ = (i << 21) | 0x80004088;
    *entry++ = 0x300;
  }
  for (i = 0; i < 8; i++) {
    *entry++ = (i << 21) | 0x40004098;
    *entry++ = 0x300;
  }
  for (i = 0; i < 16; i++) {
    for (j = 0; j < 6; j++) {
      u32 tile = (i * 3) * 8 + j * 4;
      *entry++ = (i * 8 + 16) | ((j * 32 + 24) << 16) | 0x40004000;
      *entry++ = tile;
    }
  }
  for (i = 0; i < 8; i++) {
    *entry++ = 0x00c000c0;
    *entry++ = 0x300;
  }
  gFlash_Handler3 = 0;
  *(s16 *)&gFlash = 0;
  (*(s16 *)&gFlash_Handler0) = 0;
  Scheduler_AddOrUpdateCallback(DisplayScroll_UpdateObjects, 0x480);
  Scheduler_AddOrUpdateCallback(DisplayScroll_RenderEnteringLine, 0xc80);
  for (i = 0; i < 32; i++)
    DisplayScroll_DrawLine(gDisplayScroll_LineTable[0], i * 24, 1);
}

s32 DisplayScroll_DrawLine(const u8 *text, s32 slot, s32 align) {
  u8 *buf = Runtime_BumpAllocateAlternatePool(0x900);
  s32 x = 0;
  s32 width = 192;
  const u8 *font = gDisplayScroll_Font;
  s32 tiles;
  s32 pos;
  const u8 *p;
  u8 *dst;
  u32 c;
  s32 glyph;
  s32 w;
  s32 row;
  s32 bit;
  u32 bits;
  u32 mask;

  /* FAKEMATCH: unused local; its 32 bytes are the native frame space no
   * instruction touches. Without it differ 4 on tbs-ja, tbs-en and tbs-de: sub
   * sp, #12 for #44 and the matching add. */
  u32 unused[8];

  if (text == NULL)
    return -1;
  if (!GameFlag_TestFar(0x200)) {
    Iwram_FillWords(buf, 0x900, 0);
    GameFlag_SetBitFar(0x200);
  } else {
    Iwram_CopyWords(buf, buf + 0x800, 0x100);
    Iwram_FillWords(buf + 0x100, 0x800, 0);
  }

  p = text;
  pos = 0;
  while ((c = *p++) != 0) {
    if (c > 31)
      pos += gDisplayScroll_GlyphWidths[c - 32];
  }
  if (align == 2)
    x = width - pos;
  else if (align == 1)
    x = (width - pos) / 2;

  // p is set to the text again, though nothing reads it before the glyph rows;
  // the first character is fetched through it
  p = text;
  pos = 0;
  while ((c = *text++) != 0) {
    if (c > 31) {
      glyph = c - 32;
      p = font + glyph * 8;
      dst = buf + x + pos;
      for (row = 0; row < 8; row++) {
        bits = *p++;
        mask = 0x80;
        for (bit = 7; bit >= 0; bit--) {
          if (bits & mask) {
            dst[0x101] = 1;
            dst[0] = 15;
          }
          dst++;
          mask >>= 1;
        }
        dst += 248;
      }
      w = 1;
      if (c > 31)
        w = gDisplayScroll_GlyphWidths[glyph];
      pos += w;
    }
  }

  tiles = width / 8;
  p = buf;
  dst = buf;
  for (row = 0; row < 8; row++) {
    for (bit = 0; bit < tiles * 4; bit++) {
      c = *p++;
      c |= *p++ << 4;
      *dst++ = c;
    }
    dst += 256 - tiles * 4;
    p += 256 - tiles * 8;
  }

  for (row = 0; row < tiles; row++) {
    s32 tile = slot + row;
    u32 *words = (u32 *)buf + row;

    *(u32 *)(0x06010000 + tile * 32) = words[0x000];
    *(u32 *)(0x06010004 + tile * 32) = words[0x040];
    *(u32 *)(0x06010008 + tile * 32) = words[0x080];
    *(u32 *)(0x0601000c + tile * 32) = words[0x0c0];
    *(u32 *)(0x06010010 + tile * 32) = words[0x100];
    *(u32 *)(0x06010014 + tile * 32) = words[0x140];
    *(u32 *)(0x06010018 + tile * 32) = words[0x180];
    *(u32 *)(0x0601001c + tile * 32) = words[0x1c0];
  }
  MemFree(buf);
  return 0;
}
