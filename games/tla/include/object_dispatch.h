#ifndef TLA_OBJECT_DISPATCH_H
#define TLA_OBJECT_DISPATCH_H

#include "types.h"
#include "system.h"

struct AnimationObject;
struct AnimationEntry;

/// A field object's sprite as the dispatcher's setters write it: the same
/// bytes as FieldSprite.
struct DispatchChild {
  u8 unk00[5];
  u8 unk05Low : 2;
  u8 stateFlags : 2; ///< the image's OAM object mode (FieldSprite.blendMode)
  u8 unk05High : 4;
  u8 unk06[0x0b];
  u8 unk11Low : 1;
  u8 displayFlag : 1; ///< the own-scale bit in the projection draft's layout
  u8 unk11High : 6;
  s16 value12; ///< FieldSprite.rotation, 1/65536 turns
  u8 unk14[5];
  u8 dirty;   ///< nonzero redraws its pixels
  u8 value1a; ///< the sprite flags (FieldSprite.nextFrame)
  u8 count;   ///< layers in use
  u8 unk1c[0x0c];
  struct AnimationEntry *entries[4]; ///< its layers; empty ones null
};

struct FieldSprite;

/// A field object as the object dispatcher's far calls take it.
struct DispatchObject {
  u32 value00;      ///< the object script's address
  s16 value04;      ///< the script cursor, in words
  u8 unk06[0x1d];   ///< facing, position, scales and radius; not named here
  u8 priorityFlags; ///< ObjectRuntime.priorityFlags
  u8 unk24[0x0c];   ///< the velocity; not named here
  s32 value30;      ///< speed limit, 16.16 fixed-point pixels per frame
  s32 value34;      ///< acceleration, 16.16 fixed point
  u8 unk38[0x18];   ///< the target, bounce, gravity and a spare word
  union {
    struct DispatchChild *child;     ///< kind 1
    struct DispatchChild **children; ///< kind 2
    struct FieldSprite *sprite;      ///< kind 1
    struct AnimationObject *animation;
  } target;
  u8 kind;     ///< 1 one sprite, 2 a group of sprites
  u8 unk55[2]; ///< motion flags and arrival axis; not named here
  u8 value57;  ///< cleared with a new script
  u8 unk58[3]; ///< snap, collision and steering flags; not named here
  u8 value5b;  ///< nonzero pauses the object; cleared with a new script
  u8 unk5c;
  u8 value5d; ///< cleared with a new script
  u8 unk5e[6];
  s16 value64; ///< the action script's parameter; the camera clears it
  u8 unk66[2];
  s32 argument; ///< the watched object a script approaches or follows
  void (*update)(struct DispatchObject *object); ///< per-frame hook
};

s32 Animation_ApplyChildArgument(struct DispatchChild *, s32);
void Animation_ApplyChildValue(struct DispatchChild *, s32);
s32 InitializeAnimationObjects(struct DispatchChild *);
s32 ResourceMetadata_Register(struct DispatchChild *, s32);
void ObjectDispatch_Initialize(struct DispatchObject *, const s32 *);
void ObjectDispatch_SetSingleChildFlagsFar(struct DispatchObject *, u32);
void Animation_ApplyChildValuesToRecord(struct DispatchChild *, u32);
void Animation_ApplyChildValuesToRecordFar(struct DispatchChild *, u32);
void Animation_ApplyChildValues(struct DispatchObject *, u32);
void Animation_ApplyChildValuesFar(struct DispatchObject *, u32);
void ObjectDispatch_SetSpritePriority(struct DispatchObject *, s32);

extern const s32 gObjectDispatch_CameraScript[];
extern const s32 gObjectDispatch_EndScript[];

#endif
