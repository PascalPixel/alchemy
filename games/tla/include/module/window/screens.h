#ifndef ALCHEMY_MODULE_WINDOW_SCREENS_H
#define ALCHEMY_MODULE_WINDOW_SCREENS_H

/// @file
/// What the screens object defines for the code outside it.

#include "types.h"

struct BobPairObject;
struct RenderInput;
struct WindowSprite;

struct Effect {
  unsigned x : 16;
  unsigned y : 16;
  unsigned z : 16;
  unsigned unk : 16;
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
void BattleMessage_WaitForAdvanceAtFar(s32 x, s32 y);
void Sound_SetBgmVolumeTargetFar(s32 target, s32 step);
void Scene_PlayQueuedMusicFar(void);
void ObjectTable_SnapshotFar(void);
void DjinnFigures_InitializeTopRowFar(void);
void DjinnFigures_ReleaseTopRowFar(void);
void Menu_SetEntryObjectsHiddenFar(s32 shown);
void MenuIcon_SetPositionFar(s32 index, s32 x, s32 y);
s32 FileList_SelectFile(s32 selectedFile, s32 menuMode);
void MenuTitle_Bob(struct BobPairObject *object);
struct WindowSprite *MenuTitle_Create(s32 group, struct RenderInput *input,
                                      s32 x, s32 y);
s32 FileList_SelectAction(void);
struct ArrowSprite {
  u32 link;
  union {
    u32 word;
    struct {
      unsigned y : 8;
      unsigned flags : 8;
      unsigned x : 9;
      unsigned high : 7;
    } bits;
  } attributes;
  union {
    u32 value;
    struct {
      unsigned index : 10;
      unsigned high : 6;
    } bits;
  } tile;
};

#endif
