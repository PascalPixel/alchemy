#ifndef ALCHEMY_FX_SCENE_H
#define ALCHEMY_FX_SCENE_H

#include "types.h"
#include "layout_guard.h"
#include "effect_slot.h"

struct AnimationObject;
union FieldObject;

struct ArcEffectObject {
  u8 unk00[6];
  u16 angle;
  u8 unk08[0x48];
  struct AnimationObject *records;
  u8 unk54[0x10];
  s16 phase;
  s16 step;
  struct FieldActor *linkedObject;
  void (*callback)(union FieldObject *object);
};

struct BattleFxScene {
  s32 angle;
  s32 x;
  s32 y;
  s32 z;
  void *mainObject;
  void *child;
  s16 firstObjectId;
  s16 secondObjectId;
  u16 action;
  s16 animation;
  s8 enabled;
  u8 mode;
  s8 visible;
  s8 active;
  void (*finishCallback)(void);
  u8 unk28[0x0c];
  s8 childOption;
  s8 childMode;
  u8 unk36[2];
  void (*savedCallback)(union FieldObject *object);
  const s32 *savedScript;
  u8 unk40[4];
  u8 savedPalette;
  u8 lit;
  u16 tileSlot;
  u16 unk48;
  u16 freeBlocks;
  s32 savedShakeX;
  s32 savedShakeY;
  s32 savedShakeDecay;
  struct EffectSlot slots[24];
  u8 unk718[2];
  s16 arcTileSlot;
  s8 messageMode;
  u8 unk71d[3];
};

LAYOUT_SIZE_GUARD(BattleFxScene_Size, struct BattleFxScene, 0x720);
LAYOUT_OFFSET_GUARD(BattleFxScene_MainObject, struct BattleFxScene, mainObject,
                    0x10);
LAYOUT_OFFSET_GUARD(BattleFxScene_Action, struct BattleFxScene, action, 0x1c);
LAYOUT_OFFSET_GUARD(BattleFxScene_SavedCallback, struct BattleFxScene,
                    savedCallback, 0x38);
LAYOUT_OFFSET_GUARD(BattleFxScene_TileSlot, struct BattleFxScene, tileSlot,
                    0x46);
LAYOUT_OFFSET_GUARD(BattleFxScene_SavedShakeX, struct BattleFxScene,
                    savedShakeX, 0x4c);
LAYOUT_OFFSET_GUARD(BattleFxScene_Slots, struct BattleFxScene, slots, 0x58);
LAYOUT_OFFSET_GUARD(BattleFxScene_ArcTileSlot, struct BattleFxScene,
                    arcTileSlot, 0x71a);
LAYOUT_OFFSET_GUARD(BattleFxScene_MessageMode, struct BattleFxScene,
                    messageMode, 0x71c);

static __inline__ s32 BattleFx_Interpolate(s32 to, s32 from, s32 step) {
  return from + (to - from) * step / 10;
}

#endif
