#include "types.h"
#include "object_runtime.h"
#include "call.h"

extern s32 gData_0200519c, gData_02005194, gData_02005198, gData_02005190;
struct ObjectRuntime *Object_GetById(u32 id);
void Func_020020ec(s32 x, s32 z, s32 layer, s32 value);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02003e64(void);
void Func_02003e6c(void);
void Func_02003fa4(s32 skip);
void Func_02003e24();
void WaitFrames(s32 frames);
void Func_02003ffc(s32 cue);

void Func_020021d0(void) {
  struct ObjectRuntime *first = Object_GetById(12);
  struct ObjectRuntime *second = Object_GetById(13);
  s32 firstX = first->x >> 20;
  s32 firstZ = first->z >> 20;
  s32 secondX = second->x >> 20;
  s32 secondZ = second->z >> 20;
  s32 both;

  if (gData_0200519c >> 20 == 12 && gData_02005194 >> 20 == 24) {
    Func_020020ec(gData_0200519c, gData_02005194, 1, 21);
    Func_020020ec(gData_0200519c, gData_02005194, 2, 0);
  } else {
    /* FAKEMATCH(pin): the address pinned to r3 is loaded afresh, as in the
     * native; as a plain read of gData_02005194, 6 of 260 instructions differ
     * in tla-en. */
    register s32 *z asm("r3") = &gData_02005194;

    Func_020020ec(gData_0200519c, *z, 3, 0);
  }
  if (gData_02005198 >> 20 == 7 && gData_02005190 >> 20 == 27) {
    Func_020020ec(gData_02005198, gData_02005190, 1, 21);
    Func_020020ec(gData_02005198, gData_02005190, 2, 0);
  } else {
    /* FAKEMATCH(pin): the address pinned to r3 is loaded afresh, as in the
     * native; as a plain read of gData_02005190, 6 of 260 instructions differ
     * in tla-en. */
    register s32 *z asm("r3") = &gData_02005190;

    Func_020020ec(gData_02005198, *z, 3, 0);
  }
  Func_020020ec(first->x, first->z, 3, 255);
  Func_020020ec(second->x, second->z, 3, 255);
  if (!GameFlag_Test(0x210) && firstX == 12 && firstZ == 26) {
    both = 0;
    Func_02003e64();
    Func_02003fa4(0);
    if (secondX == 12 && secondZ == 26) {
      Func_02003e24(7, 2, 1, 1, firstX, firstZ);
      both = 1;
    }
    first->motionFlags = 3;
    WaitFrames(10);
    Func_02003ffc(161);
    WaitFrames(10);
    first->motionFlags = 0;
    GameFlag_SetBit(0x210);
    if (both) {
      Func_020020ec(first->x, first->z, 3, 0);
      Func_020020ec(second->x, second->z, 3, 0);
      Call6(Func_02003e24, 7, 3, 1, 1, 12, 26);
      GameFlag_SetBit(0x212);
      ((u8 *)first)[0x62] = 1;
      ((u8 *)second)[0x62] = 1;
    }
    Func_02003e6c();
  }
  if (!GameFlag_Test(0x211) && secondX == 7 && secondZ == 29) {
    both = 0;
    Func_02003e64();
    Func_02003fa4(0);
    if (firstX == 7 && firstZ == 29) {
      Func_02003e24(7, 2, 1, 1, firstX, firstZ);
      both = 1;
    }
    second->motionFlags = 3;
    WaitFrames(10);
    Func_02003ffc(161);
    WaitFrames(10);
    second->motionFlags = 0;
    GameFlag_SetBit(0x211);
    if (both) {
      Func_020020ec(first->x, first->z, 3, 0);
      Func_020020ec(second->x, second->z, 3, 0);
      Call6(Func_02003e24, 7, 3, 1, 1, 7, 29);
      GameFlag_SetBit(0x212);
      ((u8 *)first)[0x62] = 1;
      ((u8 *)second)[0x62] = 1;
    }
    Func_02003e6c();
  }
}
