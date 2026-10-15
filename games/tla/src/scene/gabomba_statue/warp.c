#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "input.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Func_020011bc(s32 x, s32 z);

void Func_02001284(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (z == 11) {
    if (x == 18 && (gInput.held & 16) == 0)
      return;
    if (x == 20 && (gInput.held & 32) == 0)
      return;
  } else if (x == 19 && z == 12 && (gInput.held & 64) == 0) {
    return;
  }
  Func_020011bc(312, 180);
}

void Func_020012ec(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (x == 29) {
    if (z == 11 && (gInput.held & 128) == 0)
      return;
    if (z == 13 && (gInput.held & 64) == 0)
      return;
  } else if (z == 12) {
    if (x == 28 && (gInput.held & 16) == 0)
      return;
    if (x == 30 && (gInput.held & 32) == 0)
      return;
  }
  Func_020011bc(472, 196);
}

void Func_02001364(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (z == 7) {
    if (x == 32 && (gInput.held & 16) == 0)
      return;
  } else if (x == 33 && z == 8 && (gInput.held & 64) == 0) {
    return;
  }
  Func_020011bc(536, 112);
}

void Func_020013bc(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (z == 13) {
    if (x == 5 && (gInput.held & 16) == 0)
      return;
    if (x == 7 && (gInput.held & 32) == 0)
      return;
  } else if (x == 6) {
    if (z == 12 && (gInput.held & 128) == 0)
      return;
    if (z == 14 && (gInput.held & 64) == 0)
      return;
  }
  Func_020011bc(104, 208);
}

void Func_02001430(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (x == 42) {
    if (z == 12 && (gInput.held & 128) == 0)
      return;
    if (z == 14 && (gInput.held & 64) == 0)
      return;
  } else if (x == 43 && z == 13 && (gInput.held & 16) == 0) {
    return;
  }
  Func_020011bc(680, 208);
}

void Func_02001498(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (z == 7) {
    if (x == 23 && (gInput.held & 16) == 0)
      return;
    if (x == 25 && (gInput.held & 32) == 0)
      return;
  } else if (x == 24 && z == 8 && (gInput.held & 64) == 0) {
    return;
  }
  Func_020011bc(392, 112);
}

void Func_02001500(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (z == 9) {
    if (x == 5 && (gInput.held & 16) == 0)
      return;
    if (x == 7 && (gInput.held & 32) == 0)
      return;
  } else if (x == 6 && z == 10 && (gInput.held & 64) == 0) {
    return;
  }
  Func_020011bc(104, 146);
}

void Func_02001564(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (x == 42 && z == 10 && (gInput.held & 64) == 0)
    return;
  if (x == 43 && z == 9 && (gInput.held & 32) == 0)
    return;
  Func_020011bc(680, 146);
}

void Func_020015b8(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (x == 27) {
    if (z == 17 && (gInput.held & 128) == 0)
      return;
    if (z == 19 && (gInput.held & 64) == 0)
      return;
  } else if (z == 18) {
    if (x == 26 && (gInput.held & 16) == 0)
      return;
    if (x == 28 && (gInput.held & 32) == 0)
      return;
  }
  Func_020011bc(440, 288);
}
