#ifndef ALCHEMY_MODULE_DEBUG_DEBUG_H
#define ALCHEMY_MODULE_DEBUG_DEBUG_H

/// @file
/// What games/tla/src/debug/ declares for the files that use it, written once.

#include "types.h"

struct Actor;
struct Vector3;

/// The entrances that open a room of names in place of the debug room: each
/// has its own actor placements and the triggers of
/// gIncreaseItemAndLevel_NameTriggers.
enum {
  ENTRANCE_NAMES_FOURTH = 96,
  ENTRANCE_NAMES_THIRD = 97,
  ENTRANCE_NAMES_SECOND = 98,
  ENTRANCE_NAMES_FIRST = 99
};

/// The debug room's actors, numbering the rows of
/// gIncreaseItemAndLevel_ActorPlacements, as The Broken Seal's.
enum IncreaseItemAndLevelActorId {
  ACTOR_ITEM_BROWSER = 8, ///< IncreaseItemAndLevel_SelectItem
  ACTOR_ICON_BROWSER,     ///< IncreaseItemAndLevel_OpenIconBrowser
  /// IncreaseItemAndLevel_OpenPortraitBrowser; Force cast at it runs
  /// IncreaseItemAndLevel_RunMotionTest.
  ACTOR_PORTRAIT_BROWSER,
  /// IncreaseItemAndLevel_RunLevelEditor, drawn larger than the others.
  ACTOR_LEVEL_EDITOR
};

struct EventFlags {
  u8 unk000[0xd36];
  u8 unkd36;
  u8 unkd37;
};

void Func_02000eb4(s32 scene, s32 entrance);

extern u8 gData_02001860[];

void Func_02000ebc(s32, s32);
void Func_02000e5c(s32, s32, s32);
void Func_02000ec4(struct Vector3 *v);
s32 Func_02000efc(struct Actor *object);

#endif
