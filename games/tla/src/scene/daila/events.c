#include "types.h"
#include "field_event.h"
#include "event_inline.h"

void Func_02000924(void) {
  Import_EventStart();
  Event_ResolvePendingActions(0);
  MapGrid_CopyCellAttributes(10, 19, 1, 1, 11, 19);
  Actor_SetHorizontalPositionWithTerrain(22, 0, 0);
  GameFlag_SetBit(0x210);
  Import_EventEnd();
}

void Func_02000960(void) {
  Import_EventStart();
  Event_ResolvePendingActions(0);
  MapGrid_CopyCellAttributes(25, 20, 1, 1, 26, 20);
  Actor_SetHorizontalPositionWithTerrain(23, 0, 0);
  GameFlag_SetBit(0x211);
  Import_EventEnd();
}

void Func_020009a0(void) {
  Import_EventStart();
  Event_ResolvePendingActions(0);
  MapGrid_CopyCellAttributes(28, 23, 1, 1, 29, 23);
  Actor_SetHorizontalPositionWithTerrain(24, 0, 0);
  GameFlag_SetBit(0x212);
  Import_EventEnd();
}

void Func_020009e0(void) {
  Import_EventStart();
  Event_ResolvePendingActions(0);
  MapGrid_CopyCellAttributes(17, 11, 1, 1, 18, 11);
  Actor_SetHorizontalPositionWithTerrain(25, 0, 0);
  GameFlag_SetBit(0x213);
  Import_EventEnd();
}

void Func_02000a20(void) {
  Import_EventStart();
  Event_ResolvePendingActions(0);
  MapGrid_CopyCellAttributes(30, 17, 1, 1, 31, 17);
  Actor_SetHorizontalPositionWithTerrain(26, 0, 0);
  GameFlag_SetBit(0x214);
  Import_EventEnd();
}

void Func_02000a5c(void) {
  Import_EventStart();
  Event_ResolvePendingActions(0);
  MapGrid_CopyCellAttributes(16, 23, 1, 1, 17, 23);
  Actor_SetHorizontalPositionWithTerrain(31, 0, 0);
  GameFlag_SetBit(0x215);
  Import_EventEnd();
}
