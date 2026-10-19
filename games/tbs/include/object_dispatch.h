#ifndef ALCHEMY_OBJECT_DISPATCH_H
#define ALCHEMY_OBJECT_DISPATCH_H

/// @file
/// The object dispatcher's scripts: key checks, the player, the world map and
/// the camera.

#include "unit/field/field.h"
#include "types.h"

/// A field object as the object dispatcher's far calls take it.
struct DispatchObject {
  u32 value00;    ///< the object script's address
  s16 value04;    ///< the script cursor, in words
  u8 unk06[0x2a]; ///< facing, position, scales and motion; not named here
  s32 value30;    ///< speed limit, 16.16 fixed-point pixels per frame
  s32 value34;    ///< acceleration, 16.16 fixed point
  u8 unk38[0x18]; ///< the target, bounce, gravity and a spare word
  union {
    struct AnimationObject *child; ///< kind 1
    void **children;               ///< kind 2: four sprites, empty ones null
  } target;
  u8 kind;      ///< low four bits: 0 no sprite, 1 one, 2 up to four
  u8 unk55[2];  ///< motion flags and arrival axis; not named here
  u8 value57;   ///< the script's last flag test; cleared with a new script
  u8 unk58[3];  ///< snap, collision and steering flags; not named here
  u8 value5b;   ///< paused; cleared with a new script
  u8 unk5c;     ///< nonzero keeps its sprite's VRAM; not named here
  u8 value5d;   ///< the script's SC_LOOP count; cleared with a new script
  u8 unk5e[6];  ///< the script's wait, refusals, frozen; not named here
  s16 value64;  ///< the action script's parameter; the camera clears it
  u8 unk66[2];  ///< the player's footprint side; not named here
  s32 argument; ///< the linked object a script approaches or follows
  u8 unk6c[4];  ///< the update hook; not named here
};

extern const s32 gObjectDispatch_KeyCheckScript[];
extern const s32 gObjectDispatch_KeysScript[];
extern const s32 gObjectDispatch_PlayerScript[];
extern const s32 gObjectDispatch_WorldMapScript[];
extern const s32 gObjectDispatch_CameraScript[];
extern const s32 gObjectDispatch_ApproachLinkScript[];
extern const s32 gObjectDispatch_EndScript[];

#endif
