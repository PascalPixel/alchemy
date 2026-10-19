#ifndef TLA_OBJECT_DISPATCH_H
#define TLA_OBJECT_DISPATCH_H

#include "types.h"

struct DispatchChild {
  u8 unk00[5];
  u8 unk05Low : 2;
  u8 stateFlags : 2;
  u8 unk05High : 4;
  u8 unk06[0x0b];
  u8 unk11Low : 1;
  u8 displayFlag : 1;
  u8 unk11High : 6;
  s16 value12;
  u8 unk14[6];
  u8 value1a;
  u8 unk1b[0x0d];
  s16 *animationIndex;
};

struct DispatchObject {
  u32 value00;
  s16 value04;
  u8 unk06[0x2a];
  s32 value30;
  s32 value34;
  u8 unk38[0x18];
  union {
    struct DispatchChild *child;
    struct DispatchChild **children;
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

s32 Animation_ApplyChildArgument(struct DispatchChild *, s32);
void Animation_ApplyChildValue(struct DispatchChild *, s32);
s32 InitializeAnimationObjects(struct DispatchChild *);
s32 ResourceMetadata_Register(struct DispatchChild *);
void ObjectDispatch_Initialize(struct DispatchObject *, u32);
s32 WaitFrames(s32);

extern const s32 gObjectDispatch_Table4[];
extern const s32 gObjectDispatch_Table6[];

#endif

void ObjectDispatch_SelectAnimation(struct DispatchObject *object,
                                            s32 argument) {
  struct DispatchChild **items;
  struct DispatchChild *item;
  s32 count;

  if (object != 0) {
    switch (object->kind & 0xf) {
    case 1:
      Animation_ApplyChildArgument(object->target.child, argument);
      break;
    case 2:
      items = object->target.children;
      count = 3;
      do {
        item = *items++;
        if (item != 0)
          Animation_ApplyChildArgument(item, argument);
        count--;
      } while (count >= 0);
      break;
    }
  }
}

void ObjectDispatch_SetAnimationSpeed(struct DispatchObject *object,
                                         s32 value) {
  s32 count;
  struct DispatchChild *child;
  struct DispatchChild **children;

  if (object != 0) {
    switch (object->kind & 0xf) {
    case 1:
      Animation_ApplyChildValue(object->target.child, value);
      return;
    case 2:
      children = object->target.children;
      count = 3;
      do {
        child = *children++;
        if (child != 0)
          Animation_ApplyChildValue(child, value);
        count--;
      } while (count >= 0);
      break;
    }
  }
}

void ObjectDispatch_SetAnimationAndSpeed(struct DispatchObject *object,
                                        s32 argument, s32 value) {
  struct DispatchChild **items;
  struct DispatchChild *item;
  s32 count;

  if (object != 0) {
    switch (object->kind & 0xf) {
    case 1:
      Animation_ApplyChildArgument(object->target.child, argument);
      Animation_ApplyChildValue(object->target.child, value);
      break;
    case 2:
      items = object->target.children;
      for (count = 3; count >= 0; count--) {
        item = *items++;
        if (item != 0) {
          Animation_ApplyChildArgument(item, argument);
          Animation_ApplyChildValue(item, value);
        }
      }
      break;
    }
  }
}

void ObjectDispatch_SetChildSignedValue(struct DispatchObject *object,
                                        u32 value) {
  if (object != 0 && (object->kind & 0xf) == 1)
    object->target.child->value12 = value;
}

void Animation_SetIndexAndInitObjects(struct DispatchObject *object,
                                      s32 index) {
  struct DispatchChild *child;

  if (object != NULL && (object->kind & 0xf) == 1) {
    child = object->target.child;
    if (index >= 0) {
      *child->animationIndex = index;
      InitializeAnimationObjects(child);
    }
  }
}

void ObjectDispatch_RegisterChildMetadata(struct DispatchObject *object,
                                          s32 value) {
  if (object != 0 && (object->kind & 0xf) == 1) {
    struct DispatchChild *child = object->target.child;
    if (value >= 0)
      ResourceMetadata_Register(child);
  }
}
