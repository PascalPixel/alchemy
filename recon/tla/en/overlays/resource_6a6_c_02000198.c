// Differ 158 of 1050 (was 178): Func_020013a8's calls go through an inline
// of its own that calls it directly, as in overlay 6a5's scenes (through Call3
// its argument registers load in another order). Older notes:
// Draft of overlay 6a6's Func_02000198, translated from its listing: differ 178
// of 1050 in tla-en (was 181). Read-modify-writes are compound assignments
// where that loads as the native does. Left: register choice and order through
// the function.
#include "types.h"
#include "dma.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Camera_MoveWithinMapBounds();
s32 Event_AskYesNoWithActorAnimation();
s32 FieldEvent_WaitFrames();
s32 Func_020000ac();
s32 Func_02001248();
s32 Func_02001268();
s32 Func_02001278();
s32 Func_02001280();
s32 Func_020012c8();
s32 Func_020012e0();
s32 Func_020012e8();
s32 Func_02001348();
s32 Func_02001350();
s32 Func_02001380();
s32 Func_02001390();
s32 Func_020013a0();
s32 Func_020013a8();
s32 Func_020013b0();
s32 Func_020013b8();
s32 Func_020013c8();
s32 Func_020013f8();
s32 Func_02001428();
s32 Func_02001448();
s32 GameFlag_SetBit();
s32 Heap_Alloc();
s32 Heap_Release();
s32 Motion_SetVarCbAndRefresh();
s32 ObjectMotion_ArmCallback();
s32 ObjectMotion_CommitCurrentPositionAndActivate();
s32 ObjectMotion_CommitPositionAndActivate();
s32 ObjectMotion_EnableActionAndSetCallback();
s32 ObjectMotion_OffsetPositionAndReset();
s32 ObjectMotion_OffsetPositionAndResetMotion();
s32 ObjectMotion_ResetAndSetPosition();
s32 ObjectMotion_ResetAndSetPositionInMode2();
s32 ObjectMotion_SetAngleToward();
s32 ObjectMotion_SetPositionAndCommit();
s32 ObjectMotion_SetPositionAndReset();
s32 ObjectMotion_SetSpeedParameters();
s32 ObjectMotion_SetVariantCallback();
s32 Object_GetById();
s32 Object_LinkObjectAndSetCallback();
s32 Object_TurnToFaceEachOther();
s32 Object_RefreshSelectorById();
s32 Object_SetModeById();
s32 Random16Far();
s32 Resource_FindFreeEntry();
s32 Resource_GetTableEntry();
s32 Resource_ResetEntry();
s32 Heap_AllocEwram();
s32 FieldMessage_ShowAtActor();
s32 VramCache_Load();
s32 WaitFrames();
extern u8 Field_Palette685[];
extern u8 MsgLoho28[];
extern u8 gData_02001470[];
extern u8 gData_0200151c[];
extern u8 gData_0200154c[];
extern u8 gData_0200158c[];
extern u8 gData_020015e0[];
extern u8 gData_02001638[];
extern u8 gData_0200166c[];

static __inline__ void W_Func_020013a8(s32 a0, s32 a1, s32 a2) {
  Func_020013a8(a0, a1, a2);
}

void Func_02000198(void) {
  s32 v_r5, v_r1, v_r6, v_r2, v_r0, v_r7, v_r8, v_r10, v_r3, t0;
  s32 t5, t6, t36, t37, t38, t40, t41, t43, t47, t48;
  s32 t49, t50, t51, t52, t53, t55, t56, t147, t186, t189;
  s32 t190, t193, t194, t226, t233, t234;

  t0 = Value1(Object_GetById, 4);
  Func_020012e0();
  Call1(Func_020013f8, 0);
  v_r5 = t0;
  goto L_020081ec;
L_020081b6:
  if (v_r1 == 0)
    goto L_020081dc;
  if (v_r6 == 0)
    goto L_020081ce;
  Call3(ObjectMotion_SetPositionAndReset, 4, 0x1d7, 0x15a);
  goto L_020081dc;
L_020081ce:
  Call3(ObjectMotion_SetPositionAndReset, 4, 0x1fa, 0x15a);
L_020081dc:
  if (v_r6 == 0)
    goto L_020081e2;
  goto L_02008982;
L_020081e2:
  v_r1 = 0x1fa;
  v_r0 = 4;
  v_r2 = 118;
  goto L_0200898a;
L_020081ec:
  t5 = FIELD_AT(v_r5, s32, 8);
  v_r6 = 0;
  v_r1 = 0;
  v_r2 = 0;
  if (t5 > 0x1e8ffff)
    goto L_020081fc;
  v_r6 = 1;
L_020081fc:
  t6 = FIELD_AT(v_r5, s32, 16);
  v_r0 = t6;
  if (v_r0 > 0x15affff)
    goto L_02008210;
  v_r1 = 1;
  goto L_0200821a;
L_02008210:
  if (v_r0 <= 0x1720000)
    goto L_0200821a;
  v_r2 = 1;
L_0200821a:
  if (v_r2 == 0)
    goto L_020081b6;
L_0200821e:
  Call4(Camera_MoveWithinMapBounds, 0x1e80000, -1, 0x1740000, 1);
  Call1(GameFlag_SetBit, 0xa5f);
  Call3(ObjectMotion_SetPositionAndCommit, 4, 0x1e7, 0x179);
  Call2(Func_020013a0, 4, 0xc000);
  Call1(FieldEvent_WaitFrames, 30);
  Call1(Func_02001448, 140);
  Call2(ObjectMotion_SetVariantCallback, 16, 2);
  Call2(ObjectMotion_SetVariantCallback, 18, 2);
  Call2(Motion_SetVarCbAndRefresh, 17, 2);
  Call3(ObjectMotion_SetAngleToward, 16, 4, 0);
  Call3(ObjectMotion_SetAngleToward, 17, 4, 0);
  Call3(ObjectMotion_ArmCallback, 18, 0x5000, 0);
  Call1(FieldEvent_WaitFrames, 25);
  Call2(ObjectMotion_SetVariantCallback, 4, 3);
  Call2(Func_020013b0, 4, 0x102);
  Call3(ObjectMotion_CommitPositionAndActivate, 4, 24, 12);
  Call2(Func_020013a0, 4, 0xc000);
  Call1(FieldEvent_WaitFrames, 30);
  Call1(Func_02001448, 144);
  Call2(Func_02001428, 21, 0x1cc);
  Call3(Func_02001348, 21, 0x1e80000, 0x14a0000);
  Call3(ObjectMotion_OffsetPositionAndResetMotion, 20, 0, 8);
  Call3(ObjectMotion_SetSpeedParameters, 21, 0x30000, 0x18000);
  Call3(Func_020012c8, 0x20000, 0x20000, 0x10000);
  Call3(ObjectMotion_ResetAndSetPosition, 21, 0x1e8, 0x12c);
  Call4(Camera_MoveWithinMapBounds, 0x1e80000, -1, 0x1340000, 1);
  Call1(ObjectMotion_CommitCurrentPositionAndActivate, 21);
  Call3(Func_02001348, 21, 0, 0);
  Call3(Func_020012c8, 0x70000, 0x70000, 0x10000);
  t36 = Value1(Heap_AllocEwram, 0x600);
  t37 = Value1(Heap_Alloc, 0x1000);
  t38 = Value1(Resource_GetTableEntry, 0x1e8);
  Call2(Func_02001248, t38, t37);
  t40 = Value0(Resource_FindFreeEntry);
  t41 = Value3(VramCache_Load, t40, 0x400, t37);
  Call1(Heap_Release, t37);
  v_r10 = t41;
  v_r8 = t40;
  v_r7 = t36;
  v_r5 = (t36 + 12);
  v_r6 = 95;
L_0200838c:
  t43 = Value0(Random16Far);
  *(s32 *)v_r5 = -((u32)(t43 << 5) >> 16);
  v_r6 = (v_r6 - 1);
  v_r5 = (v_r5 + 16);
  if (v_r6 >= 0)
    goto L_0200838c;
  Dma_Set((const void *)(s32)Field_Palette685, (void *)0x50003e0, 0x80000010,
          (volatile u32 *)0x40000d4);
  v_r6 = 0;
L_020083b6:
  if ((31 & v_r6) != 0)
    goto L_020083c4;
  Call1(Func_02001448, 145);
L_020083c4:
  if (v_r6 != 30)
    goto L_020083d4;
  Call1(Func_02001278, 90);
  Call1(Func_02001448, 141);
L_020083d4:
  v_r5 = v_r7;
L_020083d6:
  t47 = FIELD_AT(v_r5, s32, 12);
  v_r2 = t47;
  if (v_r2 != 0)
    goto L_02008430;
  FIELD_AT(v_r5, s32, 4) = 0x40000000;
  FIELD_AT(v_r5, s32, 8) = v_r2;
  t48 = Value0(Random16Far);
  t49 = FIELD_AT(v_r5, u16, 6);
  FIELD_AT(v_r5, u16, 6) =
      ((t49 & -0x200) |
       ((((u32)(((t48 << 1) + t48) << 5) >> 16) + 72) & 0x1ff));
  t50 = Value0(Random16Far);
  t51 = FIELD_AT(v_r5, u8, 9);
  FIELD_AT(v_r5, u8, 4) = ((u32)t50 >> 10);
  FIELD_AT(v_r5, u8, 9) = ((t51 | 240) & -13);
  goto L_02008430;
L_02008430:
  t52 = FIELD_AT(v_r5, s32, 12);
  v_r3 = t52;
  if (v_r3 < 0)
    goto L_02008452;
  t53 = FIELD_AT(v_r5, u16, 8);
  FIELD_AT(v_r5, u16, 8) =
      ((t53 & -0x400) | ((((v_r3 >> 2) << 2) + v_r10) & 0x3ff));
  Call2(Func_02001268, v_r5, 100);
L_02008452:
  t55 = FIELD_AT(v_r5, s32, 12);
  FIELD_AT(v_r5, s32, 12) = (t55 + 1);
  if ((t55 + 1) <= 31)
    goto L_02008474;
  t56 = Value0(Random16Far);
  FIELD_AT(v_r5, s32, 12) = -((u32)((t56 << 3) - t56) >> 16);
  goto L_02008474;
L_02008474:
  v_r5 = (v_r5 + 16);
  if (v_r5 <= (v_r7 + 0x5f0))
    goto L_020083d6;
  Call1(WaitFrames, 1);
  v_r6 = (v_r6 + 1);
  if (v_r6 <= 119)
    goto L_020083b6;
  Call1(Resource_ResetEntry, v_r8);
  Call1(Heap_Release, v_r7);
  Call4(Camera_MoveWithinMapBounds, 0x1e80000, -1, 0x1860000, 1);
  Call4(Func_02001350, 14, 0xf80000, 0x1e00000, 0);
  Func_020000ac();
  Call3(Func_020012c8, 0, 0, 0x10000);
  Call1(Func_02001448, 0x186);
  Call1(FieldEvent_WaitFrames, 180);
  Call1(Func_02001280, 120);
  Call1(Func_02001380, (s32)MsgLoho28);
  Call1(FieldEvent_WaitFrames, 120);
  Call2(Motion_SetVarCbAndRefresh, 18, 2);
  Call2(Func_020013a0, 18, 0x8000);
  Call2(Func_02001390, 18, 0);
  Call3(ObjectMotion_ArmCallback, 17, 0x2000, 0);
  Call2(Object_SetModeById, 17, 3);
  Call2(Object_SetModeById, 16, 3);
  Call1(FieldEvent_WaitFrames, 30);
  W_Func_020013a8(17, 0x102, 35);
  Call2(Func_02001390, 17, 0);
  Func_020013a8(16, 0x107, 35);
  Call3(ObjectMotion_CommitPositionAndActivate, 16, -12, 24);
  Call2(Motion_SetVarCbAndRefresh, 16, 2);
  Call2(Func_02001390, 16, 0);
  Call2(Object_SetModeById, 4, 3);
  Call2(Func_020013a0, 18, 0xb000);
  Call1(FieldEvent_WaitFrames, 30);
  W_Func_020013a8(18, 0x100, 35);
  Call2(Func_02001390, 18, 0);
  W_Func_020013a8(16, 0x101, 0);
  Func_020013a8(17, 0x101, 35);
  Call3(ObjectMotion_SetAngleToward, 16, 18, 0);
  Call3(ObjectMotion_ArmCallback, 17, 0x2000, 0);
  Call1(FieldEvent_WaitFrames, 60);
  Call3(ObjectMotion_ArmCallback, 18, 0x8000, 0);
  Call1(FieldEvent_WaitFrames, 60);
  W_Func_020013a8(18, 0x105, 50);
  W_Func_020013a8(18, 0x102, 35);
  Call2(Func_02001390, 18, 0);
  Call3(Object_TurnToFaceEachOther, 16, 17, 0);
  Call1(FieldEvent_WaitFrames, 40);
  Call3(ObjectMotion_ArmCallback, 16, 0xc000, 0);
  Call3(ObjectMotion_ArmCallback, 17, 0xc000, 0);
  Call3(ObjectMotion_ArmCallback, 18, 0xb000, 0);
  Call4(Camera_MoveWithinMapBounds, 0x1e80000, -1, 0x1460000, 1);
  Call1(FieldEvent_WaitFrames, 60);
  W_Func_020013a8(16, 0x100, 0);
  Func_020013a8(17, 0x100, 50);
  W_Func_020013a8(17, 0x102, 35);
  Call2(Func_02001390, 17, 0);
  Call2(Object_SetModeById, 16, 3);
  Call2(Func_02001390, 16, 0);
  Call3(ObjectMotion_SetAngleToward, 18, 4, 0);
  Func_020013a8(18, 0x109, 35);
  Call4(Camera_MoveWithinMapBounds, 0x1e80000, -1, 0x16e0000, 1);
  Func_020013c8();
  Call3(ObjectMotion_ArmCallback, 18, 0x5000, 0);
  Call2(Func_02001390, 18, 0);
  Call3(ObjectMotion_SetSpeedParameters, 17, 0xcccc, 0x6666);
  Call3(ObjectMotion_OffsetPositionAndReset, 17, 0, 16);
  Call1(ObjectMotion_CommitCurrentPositionAndActivate, 17);
  Call2(Func_020013a0, 17, 0x2000);
  Call2(Object_SetModeById, 17, 4);
  Call2(Func_02001390, 17, 0);
  Call2(Object_SetModeById, 16, 3);
  Call2(Func_02001390, 16, 0);
  Call2(Object_SetModeById, 18, 3);
  Call3(ObjectMotion_ArmCallback, 17, 0x3000, 0);
  Call2(Func_02001390, 17, 0);
  W_Func_020013a8(4, 0x102, 35);
  Call1(FieldEvent_WaitFrames, 60);
  Call1(FieldEvent_WaitFrames, 10);
  Call2(Func_020013a0, 16, 0x4000);
  Call1(FieldEvent_WaitFrames, 20);
  Call2(Motion_SetVarCbAndRefresh, 16, 2);
  Call2(Func_02001390, 16, 0);
  W_Func_020013a8(18, 0x105, 0);
  W_Func_020013a8(17, 0x105, 0);
  Func_020013a8(16, 0x105, 60);
  W_Func_020013a8(18, 0x106, 35);
  Call2(Func_02001390, 18, 0);
  Call2(Object_SetModeById, 17, 3);
  Call1(FieldEvent_WaitFrames, 40);
  Func_020013a8(17, 0x106, 35);
  Call2(Func_02001390, 17, 0);
  Call2(Object_SetModeById, 16, 3);
  Call1(FieldEvent_WaitFrames, 30);
  Call2(Func_020013b8, 0x8000, 0x1000);
  Call4(Camera_MoveWithinMapBounds, 0x21e0000, -1, 0x1000000, 1);
  t147 = Value1(Object_GetById, 16);
  FIELD_AT(t147, u8, 85) = 0;
  ObjectMotion_SetSpeedParameters(16, 0x18000, 0xc000);
  Call2(ObjectMotion_EnableActionAndSetCallback, 16, (s32)gData_02001470);
  Call3(ObjectMotion_SetSpeedParameters, 18, 0x8000, 0x4000);
  Call2(ObjectMotion_EnableActionAndSetCallback, 18, (s32)gData_0200151c);
  Call3(ObjectMotion_SetSpeedParameters, 17, 0x6666, 0x3333);
  Call2(ObjectMotion_EnableActionAndSetCallback, 17, (s32)gData_0200154c);
  Call1(Object_RefreshSelectorById, 16);
  Call2(Func_020013a0, 16, 0x6000);
  Call1(FieldEvent_WaitFrames, 10);
  Call2(Func_020013a0, 16, 0x8000);
  Call1(FieldEvent_WaitFrames, 10);
  Call2(Func_020013a0, 16, 0xa000);
  Call1(FieldEvent_WaitFrames, 10);
  Call2(Func_020013a0, 16, 0xe000);
  Call1(FieldEvent_WaitFrames, 10);
  Call2(Func_020013a0, 16, 0);
  Call1(FieldEvent_WaitFrames, 10);
  Call2(Func_020013a0, 16, 0x2000);
  Call1(FieldEvent_WaitFrames, 10);
  Call2(Func_020013a0, 16, 0x5000);
  Call1(FieldEvent_WaitFrames, 40);
  Call2(Object_SetModeById, 16, 3);
  Call1(FieldEvent_WaitFrames, 60);
  Call2(Func_02001390, 16, 0);
  Call1(Object_RefreshSelectorById, 17);
  Call1(Object_RefreshSelectorById, 18);
  Call3(Object_TurnToFaceEachOther, 17, 18, 0);
  Call1(FieldEvent_WaitFrames, 20);
  goto L_020088f0;
L_020088f0:
  Call2(Object_SetModeById, 18, 3);
  Call2(Object_SetModeById, 17, 3);
  Call1(FieldEvent_WaitFrames, 30);
  Call3(ObjectMotion_SetAngleToward, 17, 4, 0);
  Call3(ObjectMotion_SetSpeedParameters, 16, 0x10000, 0x8000);
  Call2(ObjectMotion_EnableActionAndSetCallback, 16, (s32)gData_0200158c);
  Call2(Func_020013b8, 0x1cccc, 0x3999);
  Call4(Camera_MoveWithinMapBounds, 0x21e0000, -1, 0x15e0000, 1);
  Func_020013c8();
  FieldMessage_ShowAtActor(17, 0);
  t186 = Value2(Event_AskYesNoWithActorAnimation, 4, 0);
  if (t186 != 0)
    goto L_02008992;
  Call2(Object_SetModeById, 18, 3);
  Call2(Func_02001390, 18, 0);
  t189 = FIELD_AT(0x3000000, s32, 108);
  t190 = FIELD_AT(t189, u16, 452);
  FIELD_AT(t189, u16, 452) = (t190 + 1);
  goto L_020089c4;
L_02008982:
  v_r1 = 0x1d7;
  v_r0 = 4;
  v_r2 = 118;
L_0200898a:
  Call3(ObjectMotion_SetPositionAndReset, v_r0, v_r1, (v_r2 + 255));
  goto L_0200821e;
L_02008992:
  Call2(Object_SetModeById, 18, 4);
  t193 = FIELD_AT(0x3000000, s32, 108);
  t194 = FIELD_AT(t193, u16, 452);
  FIELD_AT(t193, u16, 452) = (t194 + 1);
  Call2(Func_02001390, 17, 0);
  Call2(Object_SetModeById, 18, 3);
  Call2(Func_02001390, 18, 0);
L_020089c4:
  Call3(ObjectMotion_OffsetPositionAndReset, 18, 0, 24);
  Call3(ObjectMotion_CommitPositionAndActivate, 17, 0, 24);
  Call3(ObjectMotion_ArmCallback, 17, 0, 0);
  Call3(ObjectMotion_ArmCallback, 18, 0xd000, 0);
  Call1(Object_RefreshSelectorById, 16);
  Call2(ObjectMotion_EnableActionAndSetCallback, 16, (s32)gData_020015e0);
  Call1(Object_RefreshSelectorById, 16);
  Call3(ObjectMotion_ArmCallback, 18, 0xc000, 0);
  Call3(ObjectMotion_ArmCallback, 17, 0, 0);
  Call3(ObjectMotion_ArmCallback, 16, 0x6000, 0);
  Call1(FieldEvent_WaitFrames, 50);
  Call2(Object_SetModeById, 18, 3);
  Call2(Object_SetModeById, 17, 3);
  Call2(Object_SetModeById, 16, 3);
  Call1(FieldEvent_WaitFrames, 60);
  Call2(Object_LinkObjectAndSetCallback, 4, 16);
  Call3(ObjectMotion_ResetAndSetPositionInMode2, 18, 0x1f8, 0x172);
  Call3(ObjectMotion_ResetAndSetPositionInMode2, 17, 0x1f8, 0x160);
  Call2(ObjectMotion_EnableActionAndSetCallback, 16, (s32)gData_02001638);
  Call1(ObjectMotion_CommitCurrentPositionAndActivate, 17);
  Call1(ObjectMotion_CommitCurrentPositionAndActivate, 18);
  Call1(Object_RefreshSelectorById, 16);
  Call2(Func_020013a0, 16, 0);
  Call3(ObjectMotion_SetSpeedParameters, 16, 0x4ccc, 0x2666);
  Call3(ObjectMotion_SetSpeedParameters, 17, 0x4ccc, 0x2666);
  Call3(ObjectMotion_SetSpeedParameters, 18, 0x4ccc, 0x2666);
  Call1(FieldEvent_WaitFrames, 40);
  Call3(ObjectMotion_SetSpeedParameters, 20, 0x1999, 0xccc);
  t226 = Value1(Object_GetById, 20);
  FIELD_AT(t226, u8, 85) = 0;
  Call2(ObjectMotion_EnableActionAndSetCallback, 20, (s32)gData_0200166c);
  Call1(Object_RefreshSelectorById, 20);
  Call3(ObjectMotion_SetSpeedParameters, 20, 0x4ccc, 0x2666);
  Call2(Func_020013b8, 0x4ccc, 0x999);
  Call4(Camera_MoveWithinMapBounds, 0x1cc0000, -1, 0x15e0000, 1);
  Call1(FieldEvent_WaitFrames, 30);
  t233 = Value1(Object_GetById, 16);
  FIELD_AT(t233, u8, 90) &= 254;
  Call3(ObjectMotion_OffsetPositionAndReset, 18, -0x28, -0x14);
  Call3(ObjectMotion_OffsetPositionAndReset, 17, -0x28, -0x14);
  Call3(ObjectMotion_OffsetPositionAndReset, 16, -0x28, -0x14);
  Call3(ObjectMotion_CommitPositionAndActivate, 20, -0x28, -0x14);
  Call3(ObjectMotion_OffsetPositionAndReset, 18, -0x8a, 0);
  Call3(ObjectMotion_OffsetPositionAndReset, 17, -0x8a, 0);
  Call3(ObjectMotion_OffsetPositionAndReset, 16, -0x8a, 0);
  Call3(ObjectMotion_CommitPositionAndActivate, 20, -0x8a, 0);
  Call3(Func_02001348, 16, 0, 0);
  Call3(Func_02001348, 18, 0, 0);
  Call3(Func_02001348, 17, 0, 0);
  Call3(Func_02001348, 20, 0, 0);
  Func_020012e8();
  return;
}
