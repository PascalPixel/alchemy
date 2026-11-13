#ifndef ALCHEMY_MENUS_INTERNAL_H
#define ALCHEMY_MENUS_INTERNAL_H

/// @file
/// What every part of the menus object shares: its includes. menus.c
/// includes it before the parts, and each part includes it too.

#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"
#include "battle/battle_unit.h"
#include "version.h"
#include "module/system/system.h"
#include "task.h"
#include "scene.h"
#include "fixed_math.h"
#include "edition.h"
#include "io_reg.h"
#include "system.h"
#include "main_routine_work.h"
#include "battle_system.h"
#include "battle_work.h"
#include "actor.h"
#include "battle_unit.h"
#include "battle_action.h"
#include "graphics/uiwindow.h"
#include "battle/battle_summon.h"
#include "battle/battle_system.h"
#include "window.h"
#include "resource_menu.h"
#include "display_transition.h"
#include "vram_tab.h"
#include "heap_release.h"
#include "dma.h"
#include "resource.h"
#include "uiflow.h"
#include "text/msg_ids.h"
#include "text_render.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "tla_edition.h"
#include "minigame.h"
#include "scene_call.h"
#include "sprite.h"
#include "render_input.h"

struct TargetNavigation {
  u8 unk00[0x28];
  s32 targetViewOffset;
  u8 unk2c[0x20];
  s32 timer;
  u8 unk50[0x88];
  s32 tutorialStep;
  s32 tutorialDelay;
  u8 unke0[0x2c];
  s32 lastTarget;
};

struct TargetBattle {
  u8 unk00[0x58];
  s16 partyUnits[7];
  s16 enemyIds[7];
};

struct TargetPoint {
  s32 x;
  s32 y;
  s32 z;
};

struct TargetTrack {
  u8 x;
  u8 y;
  u8 flags;
  s8 index;
};

struct TargetSprite {
  u8 unk00[4];
  u8 y;
  u8 affine : 2;
  u8 mode : 2;
  u8 high05 : 4;
  u16 x : 9;
  u16 matrix : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 attr2high : 6;
  u16 unk0a;
};

struct TargetAffine {
  u16 scaleX;
  u16 scaleY;
  u16 angle;
  u16 unk06;
};

struct TextObject {
  u8 storage[12];
};

struct WindowLine {
  u16 left;
  u16 span;
};

struct WindowBuffer {
  u8 unk000[4];
  struct WindowLine lines[160];
};

struct MenuSelectionState {
  u8 padding000[0x78];
  void *work;
  u8 padding07c[8];
  u8 resourceIds[8];
  s16 selection;
  s16 itemCount;
  s16 field090;
  s16 resourceBase;
};

struct MenuModeLabelState {
  u8 unk00[0x7c];
  void *window;
  u8 unk80[0x0c];
  s16 mode;
  u8 unk8e[8];
  s16 previousMode;
};

struct CountdownDisplayEntry {
  u8 data[12];
};

struct LinkSignature {
  u8 padding000[8];
  u16 e;
  u16 d;
  u16 v;
  u16 s;
  u8 padding010[8];
};

struct LinkCountdownState {
  struct CountdownDisplayEntry entries[3];
  u8 entryActive[3];
  u8 padding027;
  s32 targetOffset;
  s32 currentOffset;
  u8 padding030[20];
  s32 displayHandle;
  s32 secondaryHandle;
  s32 timer;
  s32 enabled;
};

struct LinkRuntimeState {
  u8 padding000[80];
  u8 side;
  u8 padding051;
  u8 paused;
};

extern u8 gWorkspace_ItemTitle[];
extern u8 gWorkspace_EnergyTitle[];
extern u8 gWorkspace_StatusTitle[];
extern u8 gWorkspace_PageLabel[];
extern u8 gWorkspace_RangeDash[];

/* VIEW(menus): routine Portrait_Load as s32 Portrait_Load(s32 portrait, s32
 * slot); window/screens has s32 Portrait_Load(u32 entry, s32 slot) {. */
s32 Portrait_Load__menus(s32 portrait, s32 slot) __asm__("Portrait_Load");
#define Portrait_Load Portrait_Load__menus

extern s8 gMenu_SaveSlotActionByPosition[];

extern struct LinkCountdownState *gLinkCountdownWork;

struct TargetWindow {
  u8 unk00[8];
  u16 width;
  u16 height;
  s16 x;
  s16 y;
};

#endif // ALCHEMY_MENUS_INTERNAL_H
