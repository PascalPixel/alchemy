#ifndef ALCHEMY_OBJECT_DISPATCH_H
#define ALCHEMY_OBJECT_DISPATCH_H

#include "unit/field/field.h"
#include "types.h"

struct DispatchObject {
  u32 value00;
  s16 value04;
  u8 unk06[0x2a];
  s32 value30;
  s32 value34;
  u8 unk38[0x18];
  union {
    struct AnimationObject *child;
    void **children;
  } target;
  u8 kind;
  u8 unk55[2];
  u8 value57;
  u8 unk58[3];
  u8 value5b;
  u8 unk5c;
  u8 value5d;
  u8 unk5e[6];
  s16 value64;
  u8 unk66[2];
  s32 argument;
  u8 unk6c[4];
};

extern const s32 gObjectDispatch_KeyCheckScript[];
extern const s32 gObjectDispatch_KeysScript[];
extern const s32 gObjectDispatch_PlayerScript[];
extern const s32 gObjectDispatch_WorldMapScript[];
extern const s32 gObjectDispatch_CameraScript[];
extern const s32 gObjectDispatch_ApproachLinkScript[];
extern const s32 gObjectDispatch_EndScript[];

#endif
