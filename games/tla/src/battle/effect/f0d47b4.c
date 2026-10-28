#include "objdisp.h"

struct BattleEffectVisual {
  u8 unk00[9];
  u8 flags;
  u8 unk0a[16];
  u8 value26;
};

struct BattleEffectResource {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[60];
  struct BattleEffectVisual *visual;
};

struct BattleEffectWatchedObject {
  u8 unk00[80];
  struct BattleEffectVisual *visual;
  u8 value54;
  u8 value55;
  u8 unk56[14];
  u16 counter;
  u16 resourceId;
  struct BattleEffectResource *resource;
  void (*callback)(void);
};

struct BattleEffectWatchedObject *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
void Actor_SetMode(struct BattleEffectWatchedObject *object, s32 mode);
void Sound_PlayFar(s32 cue);
void Event_WaitFrames(s32 state);
void BattleEffect_CopyWatchedObjectPosition(void);
extern const u8 gData_080f3310[];

/// The Broken Seal's BattleEffect_SpawnLinked with this game's object kind
/// (488): spawns an effect object at object resourceId that follows it,
/// waits state, and returns the new object.
struct BattleEffectWatchedObject *Func_080d47b4(s32 resourceId, s32 flags,
                                               s32 state) {
  struct BattleEffectResource *resource;
  struct BattleEffectWatchedObject *object;

  if ((flags & 0xff) == 6) {
    Sound_PlayFar(110);
  }

  resource = ObjectTable_Get(resourceId);
  if (resource != 0) {
    object = Func_080200c0(488, resource->x, resource->y, resource->z);

    if (object != 0) {
      Actor_StartScriptFar((struct ActorView *)object,
                                   (u32)gData_080f3310);
      Actor_SetMode(object, flags & 15);
      object->value55 = 0;
      object->counter = 0;
      object->resourceId = resourceId;
      object->callback = BattleEffect_CopyWatchedObjectPosition;
      object->resource = resource;
      object->visual->value26 = 0;

      if ((flags & 0x100) != 0) {
        s32 mask = 13;
        u8 visualFlags = object->visual->flags;

        mask = -mask;
        mask &= visualFlags;
        mask |= 4;
        object->visual->flags = mask;
      } else {
        s32 copiedFlags = 12;
        u8 sourceFlags = resource->visual->flags;
        u8 flags;
        s32 clearMask = 13;

        copiedFlags &= sourceFlags;
        flags = object->visual->flags;
        clearMask = -clearMask;
        clearMask &= flags;
        clearMask |= copiedFlags;
        object->visual->flags = clearMask;
      }
    }
    Event_WaitFrames(state);
  }
  return object;
}
