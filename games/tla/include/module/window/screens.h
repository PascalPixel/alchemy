#ifndef ALCHEMY_MODULE_WINDOW_SCREENS_H
#define ALCHEMY_MODULE_WINDOW_SCREENS_H

/// @file
/// What games/tla/src/window/ declares for the files that use it, written once.

#include "types.h"

struct RenderInput;
struct WindowSprite;

struct CommandRowIcon {
  u8 unk00[4];
  u8 y;
  u8 objMode : 2; ///< 3: double-size affine
  u8 high05 : 6;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u8 unk08[4];
  u16 resource;
  u16 unused;
  s32 positionX;
  s32 positionY;
  s32 kind;
};

struct CommandRowAffine {
  u16 scaleX;
  u16 scaleY;
  u16 angle;
};

struct CommandRowWork {
  u8 paletteSlots[256];
  s32 paletteCount;
  struct CommandRowIcon entries[6];
  void *window;
  s32 iconCount;
  struct CommandRowAffine affine;
  s32 affineIndex;
  s32 selectedIndex;
  s32 drawnIndex;
  s32 commandKinds[6];
};

/// The battle menu's navigation work (heap slot 57).
struct CommandRowNavigation {
  u8 unk00[0x3c];
  s32 commandSelection[2];
  u8 unk44[8];
  s32 timer; ///< the link battle's time left; 0 takes the first command
  u8 unk50[0x88];
  s32 tutorialStep;
  s32 tutorialDelay;
  s32 commandActor;
  u8 unke4[0x20];
  s32 reserveChoice; ///< -1 while no reserve member is chosen
};

struct CommandRowWindowWork {
  u8 unk00[3];
  u8 dirty;
  u8 unk04;
  u8 flags05;
  u8 holdTilemapUpload;
  u8 unk07;
  u16 tilemap[0x400];
};

struct BgScrollPair {
  s16 x;
  s16 y;
};

struct DjinnMenuWindow {
  u8 unk00[8];
  u16 width;
  u16 height;
  s16 x;
  s16 y;
};

struct DjinnMenuNavigation {
  u8 unk00[0x30];
  s32 row;
  s32 page;
  s32 preferredRow;
  u8 unk3c[0x10];
  s32 timer;
  u8 unk50[0x88];
  s32 tutorialStep;
  s32 tutorialDelay;
  u8 unke0[4];
  u32 chosenDjinn[8];
};

struct DjinnMenuUnit {
  u8 unk000[0xf8];
  u32 djinnHeld[4];
  u32 djinnSet[4];
};

struct DjinnMenuPartyStatus {
  struct DjinnMenuWindow *window;
  u8 unk04[8];
  u16 flags;
};

struct DjinnMenuSprite {
  u8 unk00[4];
  u32 attr01;
  u32 attr23;
};

struct DjinnMenuOam {
  u8 unk00[4];
  u8 y;
  u8 attr0high;
  u16 x : 9;
  u16 attr1high : 7;
  u16 tile : 10;
  u16 attr2high : 6;
};

struct DjinnSlot {
  u16 id;
  u16 state;
};

struct PreviewAbility {
  u8 unk00[2];
  u8 element; ///< 4 for none
  u8 unk03[5];
  u8 range; ///< 255 for everyone
  u8 ppCost;
};

struct ItemMenuWindow {
  u8 unk00[8];
  u16 width;
  u16 height;
  s16 x;
  s16 y;
};

struct ItemMenuNavigation {
  u8 unk00[0x30];
  s32 row;
  s32 page;
  s32 preferredRow;
  u8 unk3c[0x10];
  s32 timer;
};

struct ItemMenuSprite {
  struct ItemMenuSprite *next;
  union {
    struct {
      unsigned long attr01;
      unsigned long attr23;
    } word;
    struct {
      u16 y : 8;
      u16 affine : 2;
      u16 mode : 2;
      u16 mosaic : 1;
      u16 colors : 1;
      u16 shape : 2;
      u16 x : 9;
      u16 affineIndex : 5;
      u16 size : 2;
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
      u16 unused;
    } f;
  } oam;
};

struct ItemMenuOam {
  struct ItemMenuOam *next;
  u16 y : 8;
  u16 affine : 2;
  u16 mode : 2;
  u16 mosaic : 1;
  u16 colors : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unused;
};

struct PsynergyMenuWindow {
  u8 unk00[8];
  u16 width;
  u16 height;
  s16 x;
  s16 y;
};

struct PsynergyMenuNavigation {
  u8 unk00[0x30];
  s32 row;
  s32 page;
  s32 preferredRow;
  u8 unk3c[0x10];
  s32 timer;
};

struct PsynergyMenuUnit {
  u8 unk000[0x3a];
  s16 pp;
  u8 unk03c[0x101];
  u8 psySeal;
};

struct PsynergyAbility {
  u8 unk00;
  u8 flags; ///< bit 7: usable in battle
  u8 element;
  u8 unk03[5];
  u8 range;
  u8 ppCost;
};

struct PsynergyMenuSprite {
  struct PsynergyMenuSprite *next;
  union {
    struct {
      unsigned long attr01;
      unsigned long attr23;
    } word;
    struct {
      u16 y : 8;
      u16 affine : 2;
      u16 mode : 2;
      u16 mosaic : 1;
      u16 colors : 1;
      u16 shape : 2;
      u16 x : 9;
      u16 affineIndex : 5;
      u16 size : 2;
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
      u16 unused;
    } f;
  } oam;
};

struct PsynergyMenuOam {
  struct PsynergyMenuOam *next;
  u16 y : 8;
  u16 affine : 2;
  u16 mode : 2;
  u16 mosaic : 1;
  u16 colors : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unused;
};

struct SummaryElement {
  s16 power;
  s16 resist;
};

struct SummaryPsynergy {
  u16 id;
  u16 unk02;
};

/// A unit as the summary screen reads it.
struct SummaryUnit {
  u8 name[15];
  u8 level;
  u8 unk10[0x24];
  s16 hpMax;
  s16 ppMax;
  s16 hp;
  s16 pp;
  u16 atk;
  u16 def;
  u16 agi;
  u8 luck;
  u8 unk43[5];
  struct SummaryElement element[4];
  struct SummaryPsynergy psynergy[32];
  u16 items[15];
  u8 unkf6[2];
  u32 djinnOwned[4];
  u32 djinnSet[4];
  u8 djinnHeld[4];
  u8 djinnSetCount[4];
  u8 unk120[4];
  u32 experience;
  u8 unk128;
  u8 classId;
  u8 unk12a[6];
  s8 curse;
  s8 poison;
  u8 attackTurns;
  s8 attackStage;
  u8 defenseTurns;
  s8 defenseStage;
  u8 resistanceTurns;
  s8 resistanceStage;
  u8 delusion;
  u8 unk139[2];
  u8 stun;
  u8 sleep;
  u8 psySeal;
  u8 unk13e[2];
  u8 haunt;
  u8 grimReaper;
  u8 unk142[5];
  s8 agilityStage;
  u8 unk148[4];
};

struct SummaryOam {
  u8 unk00[4];
  union {
    struct {
      u32 attr01;
      u32 attr23;
    } word;
    struct {
      u8 y;
      u8 attr0high;
      u16 x : 9;
      u16 affineIndex : 5;
      u16 size : 2;
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
      u16 unk0a;
    } f;
  } oam;
};

struct SummaryPoint {
  s32 x;
  s32 y;
};

struct SummaryCursor {
  s8 groupPositions[8];
  s32 helpEntry;
  s32 drawnHelpEntry;
  s32 group;
  s32 groupSize;
  s32 column;
  s32 row;
};

struct SummaryWindow {
  u8 unk00[8];
  u16 width;
  u16 height;
  s16 x;
  s16 y;
};

struct SummaryWindowWork {
  u8 unk00[3];
  u8 dirty;
  u8 unk04[2];
  u8 holdTilemapUpload;
  u8 textColor;
};

struct SummaryNavigation {
  u8 unk00[0x44];
  void *timerWindow;
  s32 timerHidden;
  s32 timer;
  u8 unk50[0x46];
  u16 list[64];
};

struct SummaryAbility {
  u8 unk00;
  u8 flags; ///< bit 7: usable in battle
  u8 element;
  u8 unk03[5];
  u8 range;
  u8 ppCost;
};

struct SummaryStatCursor {
  u8 entry;
  u8 column;
  u8 row;
  u8 highlightColumn;
  u8 highlightRow;
  u8 highlightWidth;
  u8 unk06[2];
};

struct SummaryCursorStop {
  u8 entry;
  u8 column;
  u8 row;
  u8 unk03;
};

struct SummaryRecoveryEntry {
  u8 element;
  u8 index;
  u8 owner;
  s8 turns;
};

struct SummaryRecoveryList {
  struct SummaryRecoveryEntry entries[72];
  s32 count;
};

struct SummaryRecovery {
  u8 unk000[8];
  struct SummaryRecoveryList list;
};

struct SummaryResourceEntry {
  u16 unk00;
  u16 offset;
};

struct SummonMenuWindow {
  u8 unk00[8];
  u16 width;
  u16 height;
  s16 x;
  s16 y;
};

struct SummonMenuWork {
  u8 unk00[3];
  u8 dirty;
  u8 unk04[2];
  u8 holdTilemapUpload;
};

struct SummonMenuNavigation {
  u8 unk00[0x30];
  s32 row;
  s32 page;
  s32 preferredRow;
  u8 unk3c[0x10];
  s32 timer;
  u8 unk50[0x88];
  s32 tutorialStep;
  s32 tutorialDelay;
};

struct SummonMenuSprite {
  struct SummonMenuSprite *next;
  union {
    struct {
      unsigned long attr01;
      unsigned long attr23;
    } word;
    struct {
      u16 y : 8;
      u16 affine : 2;
      u16 mode : 2;
      u16 mosaic : 1;
      u16 colors : 1;
      u16 shape : 2;
      u16 x : 9;
      u16 affineIndex : 5;
      u16 size : 2;
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
      u16 unused;
    } f;
  } oam;
};

struct SummonMenuOam {
  struct SummonMenuOam *next;
  u16 y : 8;
  u16 affine : 2;
  u16 mode : 2;
  u16 mosaic : 1;
  u16 colors : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unused;
};

struct BobPairObject {
  struct BobPairObject *next;
  u8 unk04[4];
  u8 baseY;
  u8 unk09[11];
  u8 y;
};

struct Effect {
  unsigned x : 16;
  unsigned y : 16;
  unsigned z : 16;
  unsigned unk : 16;
};

struct BarPatternRow {
  u32 word0;
  u32 word1;
};

struct NameCursorMotion {
  struct WindowSprite *anchor;
  u16 x;
  u16 y;
  s16 targetX;
  s16 targetY;
  s8 active;
  s8 kind;
};

union NameCursor {
  struct WindowSprite *sprite;
  struct NameCursorMotion cursor;
};

struct UiWindowBounds {
  s32 handle;
  u16 left;
  u16 top;
  u16 right;
  u16 height;
  u16 flags;
};

struct StatusUnit {
  u8 name[0x34];
  s16 maxHp;
  s16 maxPp;
  s16 hp;
  s16 pp;
};

struct SaveGameWords {
  u32 buildStamp;
  u32 playTime;
};

struct ObjectRowWork {
  u8 unk000[0xf8];
  void *icons[4];
  u8 unk108[0x10];
  s16 x[8];
  s16 y[8];
  s32 scale[4];
};

struct ObjectRowState {
  u8 unk000[248];
  void *objects[4];
  u8 unk108[16];
  s16 x[8];
  s16 y[8];
  s32 scale[4];
};

/// The portion of the file summary (SaveSummary) read here.
struct NumberRow {
  u8 unk00[0x28];
  s8 values[4]; ///< Djinn count for each element.
};

void PartyStatus_CreateWindow(s32 flags);

s32 DebugBrowser_Icons(void);
s32 DebugBrowser_Portraits(void);

s32 YesNoMenu_Run(s32 low, s32 message, s32 w, s32 selection);
s32 Resource_LoadIndexedIntoBuffer(s32 slot, s32 index);

void UpdateLinkSessionCountdown(void);

s32 BattleCommandRow_Run(s32 mode, s32 fleeAllowed);

s32 BattleTarget_RunSelection(s32 selection, s32 mode, u32 spread, u32 kind);

s32 BattleStatusScreen_Run(u16 *list, s32 listCount, s32 member);

s32 Battle_RefuseFlee(void);

s32 BattlePsynergyMenu_Run(s32 unitId, u16 *actions, s32 count);

s32 BattleSummonMenu_Run(s32 unused0, s32 unused1, const u8 *standby);

s32 BattleItemMenu_Run(s32 member, u16 *entries, s32 count);
s32 Item_ClassifyUseAbility(s32 member, s32 item);

s32 ItemIcon_LoadIntoSlot(s32 item, s32 slot);
void IconMenu_DrawIcons(void);

void Link_DrawPageArrows(s32 destination);
s32 Link_CreateCountdownLabelWindow(void);

void Func_08118128(s32 x, s32 y);

extern const u8 gDjinnMenu_BlinkTile[32];
extern u8 gDjinnMenu_ElementXOffsets[4];
extern u8 gDjinnMenu_ElementYOffsets[4];

extern u8 Msg_0d10[];
extern u8 Msg_0d11[];
extern u8 Msg_0d12[];
extern u8 Msg_0d13[];
extern u8 Msg_0d15[];
extern u8 Msg_0d16[];
extern u8 Msg_0d17[];
extern u8 Msg_0d19[];

extern const struct SummaryStatCursor gOwnerStatus_StatCursors[];
extern const struct SummaryCursorStop gOwnerStatus_EquipmentCursors[];
extern const struct SummaryCursorStop gOwnerStatus_ConditionCursors[];
extern const u8 gOwnerStatus_LevelString[];
extern const u16 Data_0805f676[];
extern const u8 gWindow_PairBobOffsets[];
extern const struct BarPatternRow gMenu_BarPatternRows[];

extern const u8 Data_080aa1b8[];

extern const u8 gWindow_ProgressBarRows[];
extern const u8 gWindow_ProgressBarTiles[];

void Sound_SetBgmVolumeTargetFar(s32 target, s32 step);

void Func_080c8698(void);

extern u8 gData_02000504[];

void ObjectTable_SnapshotFar(void);

void Func_080f8070(void);
void Func_080f8078(void);
void Func_080f8080(s32 shown);

void Func_080f8090(s32 index, s32 x, s32 y);

extern u8 Tile_BuildCellPicturesCode[];

s32 FileList_SelectFile(s32 selectedFile, s32 menuMode);

void MenuTitle_Bob(struct BobPairObject *object);
struct WindowSprite *MenuTitle_Create(s32 group, struct RenderInput *input,
                                      s32 x, s32 y);

s32 FileList_SelectAction(void);

#endif
