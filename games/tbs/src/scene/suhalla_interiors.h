#ifndef ALCHEMY_SUHALLA_INTERIORS_H
#define ALCHEMY_SUHALLA_INTERIORS_H

#include "types.h"
#include "field_service.h"
#include "field_scene.h"
#include "scene.h"

/// An actor's animation object, seen for the flags and rotation that lay a
/// wounded soldier in his bed.
struct SceneRecord {
  u8 unk00[9];
  u8 flags;
  u8 unk0a[20];
  u16 angle; ///< 1/65536 turns
  u8 unk20[6];
  u8 field26;
};

/// A field actor, as its fields or as its bytes, set up as the houses load.
union SceneActor {
  struct {
    u8 unk00[12];
    s32 y; ///< 16.16 fixed-point pixels
    u8 unk10[64];
    struct SceneRecord *record;
    u8 unk54;
    u8 mode;
    u8 unk56[3];
    u8 flags;
  } fields;
  u8 bytes[90];
};

#endif
