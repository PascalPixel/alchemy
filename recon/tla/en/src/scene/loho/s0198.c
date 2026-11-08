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
s32 FieldEvent_Wait();
void Func_020000ac();
s32 Func_02001248();
s32 Func_02001268();
s32 Func_02001278();
s32 Func_02001280();
s32 Func_020012c8();
s32 Func_020012e0();
void Func_020012e8();
s32 Func_02001348();
s32 Func_02001350();
s32 Func_02001380();
s32 Func_02001390();
s32 Func_020013a0();
s32 Func_020013a8();
s32 Func_020013b0();
s32 Func_020013b8();
void Func_020013c8();
s32 Func_020013f8();
s32 Func_02001428();
s32 Func_02001448();
s32 GameFlag_SetBit();
s32 Heap_Alloc();
s32 Heap_Release();
s32 Motion_SetVarCbAndRefresh();
s32 Actor_ArmCallback();
s32 Actor_WaitForMoveById();
s32 Actor_CommitPositionAndActivate();
s32 Actor_SetMovementTypeById();
s32 Actor_OffsetPositionAndReset();
s32 Actor_OffsetPositionAndResetMotion();
s32 Actor_ResetAndSetPosition();
s32 Actor_ResetAndSetPositionInMode2();
s32 Actor_SetAngleToward();
s32 Actor_SetPositionAndCommit();
s32 Actor_SetPositionAndReset();
s32 Actor_SetSpeedParameters();
s32 Actor_StartRepeatedMotion();
s32 Actor_GetById();
s32 Actor_WatchAndSetScript();
s32 Actor_TurnToFaceEachOther();
s32 Actor_RefreshSelectorById();
s32 Actor_SetModeById();
s32 Random16Far();
s32 Resource_FindFreeEntry();
s32 Resource_GetTableEntry();
s32 Resource_ResetEntry();
s32 Heap_AllocEwram();
s32 FieldMessage_ShowAtActor();
s32 VramCache_Load();
s32 wait();
extern char ResourceId_FieldEffectExplosion;
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
  /* FAKEMATCH(wrapper): W_Func_020013a8 forwarded inline; as direct calls to
   * Func_020013a8, 70 of 1050 instructions differ in tla-en. */
  Func_020013a8(a0, a1, a2);
}

struct Unk16 {
  u8 unk00[4];
  union {
    u32 words[2];
    u16 halves[4];
    u8 bytes[8];
    struct {
      u16 y : 8;
      u16 attr0 : 8;
      u16 x : 9;
      u16 attr1 : 7;
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
    } oam;
  } sprite;
  s32 unk_0c;
};

void Func_02000198(void) {
  s32 v_r1, v_r2, v_r0, t0;
  struct Unk16 *p;
  struct Unk16 *table;
  s32 tiles, entry, i, value, zero;
  s32 t38, t47, t48, t49, t50, t51;
  s32 t147, t186, t189;
  s32 t190, t193, t194, t226, t233, t234;
  u8 unused[4]; // Unused: gives the stack frame its 4 bytes.

  t0 = Actor_GetById(4);
  Func_020012e0();
  Func_020013f8(0);
  p = (struct Unk16 *)t0;
  goto L_020081ec;
L_020081b6:
  if (v_r1 == 0)
    goto L_020081dc;
  if (i == 0)
    goto L_020081ce;
  Call3(Actor_SetPositionAndReset, 4, 0x1d7, 0x15a);
  goto L_020081dc;
L_020081ce:
  Call3(Actor_SetPositionAndReset, 4, 0x1fa, 0x15a);
L_020081dc:
  if (i == 0)
    goto L_020081e2;
  goto L_02008982;
L_020081e2:
  Call3(Actor_SetPositionAndReset, 4, 0x1fa, 0x175);
  goto L_0200821e;
L_020081ec:
  i = 0;
  v_r1 = 0;
  v_r2 = 0;
  if (FIELD_AT(p, s32, 8) > 0x1e8ffff)
    goto L_020081fc;
  i = 1;
L_020081fc:
  v_r0 = FIELD_AT(p, s32, 16);
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
  GameFlag_SetBit(SPLIT_CONSTANT(0xa5f));
  Call3(Actor_SetPositionAndCommit, 4, 0x1e7, 0x179);
  Call2(Func_020013a0, 4, 0xc000);
  FieldEvent_Wait(30);
  Func_02001448(140);
  Call2(Actor_StartRepeatedMotion, 16, 2);
  Call2(Actor_StartRepeatedMotion, 18, 2);
  Call2(Motion_SetVarCbAndRefresh, 17, 2);
  Call3(Actor_SetAngleToward, 16, 4, 0);
  Call3(Actor_SetAngleToward, 17, 4, 0);
  Actor_ArmCallback(18, 0x5000, 0);
  FieldEvent_Wait(25);
  Call2(Actor_StartRepeatedMotion, 4, 3);
  Call2(Func_020013b0, 4, 0x102);
  Call3(Actor_CommitPositionAndActivate, 4, 24, 12);
  Func_020013a0(4, 0xc000);
  FieldEvent_Wait(30);
  Func_02001448(144);
  Call2(Func_02001428, 21, 0x1cc);
  Call3(Func_02001348, 21, 0x1e80000, 0x14a0000);
  Call3(Actor_OffsetPositionAndResetMotion, 20, 0, 8);
  Call3(Actor_SetSpeedParameters, 21, 0x30000, 0x18000);
  Call3(Func_020012c8, 0x20000, 0x20000, 0x10000);
  Call3(Actor_ResetAndSetPosition, 21, 0x1e8, 0x12c);
  Camera_MoveWithinMapBounds(0x1e80000, -1, 0x1340000, 1);
  Actor_WaitForMoveById(21);
  Call3(Func_02001348, 21, 0, 0);
  Call3(Func_020012c8, 0x70000, 0x70000, 0x10000);
  table = (struct Unk16 *)Heap_AllocEwram(0x600);
  p = (struct Unk16 *)Heap_Alloc(0x1000);
  t38 = Resource_GetTableEntry((s32)&ResourceId_FieldEffectExplosion);
  Func_02001248(t38, p);
  entry = Resource_FindFreeEntry();
  tiles = VramCache_Load(entry, 0x400, p);
  Heap_Release(p);
  p = (struct Unk16 *)&table->unk_0c;
  i = 95;
L_0200838c:
  *(s32 *)p = -((u32)(Random16Far() << 5) >> 16);
  i = (i - 1);
  p = (struct Unk16 *)((s32)p + 16);
  if (i >= 0)
    goto L_0200838c;
  Dma_Set((const void *)(s32)Field_Palette685, (void *)0x50003e0, 0x80000010,
          (volatile u32 *)SPLIT_CONSTANT(0x40000d4));
  i = 0;
  /* FAKEMATCH(dead): this test that never passes, with its edge into the loop,
   * which combine folds away; as the loop entered only at its top, 28 of 1050
   * instructions differ in tla-en. */
  if (((entry & 255) >> 8) == 0)
    goto L_020083b6;
  i = 1;
  goto L_02008452;
L_020083b6:
  if ((31 & i) != 0)
    goto L_020083c4;
  Func_02001448(145);
L_020083c4:
  if (i != 30)
    goto L_020083d4;
  Func_02001278(90);
  Func_02001448(141);
L_020083d4:
  p = table;
L_020083d6:
  if (p->unk_0c != 0)
    goto L_02008430;
  p->sprite.words[0] = 0x40000000;
  p->sprite.words[1] = 0;
  t47 = (u16)((((u32)(Random16Far() * 96) >> 16) + 72) & 0x1ff);
  t48 = p->sprite.halves[1];
  p->sprite.halves[1] = (t48 & -0x200) | t47;
  p->sprite.oam.y = (u32)Random16Far() >> 10;
  t51 = p->sprite.bytes[5];
  t51 |= 240;
  p->sprite.bytes[5] = t51 & -13;
L_02008430:
  // Through long: read as s32, PRE merges this with the read after the call.
  value = *(long *)&p->unk_0c;
  if (value < 0)
    goto L_02008452;
  t49 = (u16)(((value >> 2) * 4 + tiles) & 0x3ff);
  t50 = p->sprite.halves[2];
  p->sprite.halves[2] = (t50 & -0x400) | t49;
  Func_02001268(p, 100);
L_02008452:
  p->unk_0c += 1;
  if (p->unk_0c <= 31)
    goto L_02008474;
  p->unk_0c = -((u32)(Random16Far() * 7) >> 16);
L_02008474:
  p++;
  if ((s32)p <= (s32)&table[95])
    goto L_020083d6;
  wait(1);
  i = (i + 1);
  if (i <= 119)
    goto L_020083b6;
  Resource_ResetEntry(entry);
  Heap_Release(table);
  Call4(Camera_MoveWithinMapBounds, 0x1e80000, -1, 0x1860000, 1);
  Call4(Func_02001350, 14, 0xf80000, 0x1e00000, 0);
  Func_020000ac();
  Call3(Func_020012c8, 0, 0, 0x10000);
  Func_02001448(0x186);
  FieldEvent_Wait(180);
  Func_02001280(120);
  Func_02001380((s32)MsgLoho28);
  FieldEvent_Wait(120);
  Call2(Motion_SetVarCbAndRefresh, 18, 2);
  Call2(Func_020013a0, 18, 0x8000);
  Call2(Func_02001390, 18, 0);
  Call3(Actor_ArmCallback, 17, 0x2000, 0);
  Call2(Actor_SetModeById, 17, 3);
  Actor_SetModeById(16, 3);
  FieldEvent_Wait(30);
  W_Func_020013a8(17, 0x102, 35);
  Call2(Func_02001390, 17, 0);
  Func_020013a8(16, 0x107, 35);
  Call3(Actor_CommitPositionAndActivate, 16, -12, 24);
  Call2(Motion_SetVarCbAndRefresh, 16, 2);
  Call2(Func_02001390, 16, 0);
  Call2(Actor_SetModeById, 4, 3);
  Call2(Func_020013a0, 18, 0xb000);
  FieldEvent_Wait(30);
  W_Func_020013a8(18, 0x100, 35);
  Call2(Func_02001390, 18, 0);
  W_Func_020013a8(16, 0x101, 0);
  Func_020013a8(17, 0x101, 35);
  Call3(Actor_SetAngleToward, 16, 18, 0);
  Call3(Actor_ArmCallback, 17, 0x2000, 0);
  FieldEvent_Wait(60);
  Call3(Actor_ArmCallback, 18, 0x8000, 0);
  FieldEvent_Wait(60);
  W_Func_020013a8(18, 0x105, 50);
  W_Func_020013a8(18, 0x102, 35);
  Call2(Func_02001390, 18, 0);
  Actor_TurnToFaceEachOther(16, 17, 0);
  FieldEvent_Wait(40);
  Call3(Actor_ArmCallback, 16, 0xc000, 0);
  Call3(Actor_ArmCallback, 17, 0xc000, 0);
  Call3(Actor_ArmCallback, 18, 0xb000, 0);
  Call4(Camera_MoveWithinMapBounds, 0x1e80000, -1, 0x1460000, 1);
  FieldEvent_Wait(60);
  W_Func_020013a8(16, 0x100, 0);
  Func_020013a8(17, 0x100, 50);
  W_Func_020013a8(17, 0x102, 35);
  Call2(Func_02001390, 17, 0);
  Call2(Actor_SetModeById, 16, 3);
  Call2(Func_02001390, 16, 0);
  Call3(Actor_SetAngleToward, 18, 4, 0);
  Func_020013a8(18, 0x109, 35);
  Call4(Camera_MoveWithinMapBounds, 0x1e80000, -1, 0x16e0000, 1);
  Func_020013c8();
  Call3(Actor_ArmCallback, 18, 0x5000, 0);
  Call2(Func_02001390, 18, 0);
  Call3(Actor_SetSpeedParameters, 17, SPLIT_CONSTANT(0xcccc), SPLIT_CONSTANT(0x6666));
  Actor_OffsetPositionAndReset(17, 0, 16);
  Actor_WaitForMoveById(17);
  Call2(Func_020013a0, 17, 0x2000);
  Call2(Actor_SetModeById, 17, 4);
  Call2(Func_02001390, 17, 0);
  Call2(Actor_SetModeById, 16, 3);
  Call2(Func_02001390, 16, 0);
  Call2(Actor_SetModeById, 18, 3);
  Call3(Actor_ArmCallback, 17, 0x3000, 0);
  Call2(Func_02001390, 17, 0);
  Func_020013a8(4, 0x102, 35);
  FieldEvent_Wait(60);
  FieldEvent_Wait(10);
  Call2(Func_020013a0, 16, 0x4000);
  FieldEvent_Wait(20);
  Call2(Motion_SetVarCbAndRefresh, 16, 2);
  Call2(Func_02001390, 16, 0);
  W_Func_020013a8(18, 0x105, 0);
  W_Func_020013a8(17, 0x105, 0);
  Func_020013a8(16, 0x105, 60);
  W_Func_020013a8(18, 0x106, 35);
  Call2(Func_02001390, 18, 0);
  Actor_SetModeById(17, 3);
  FieldEvent_Wait(40);
  Func_020013a8(17, 0x106, 35);
  Call2(Func_02001390, 17, 0);
  Actor_SetModeById(16, 3);
  FieldEvent_Wait(30);
  Call2(Func_020013b8, 0x8000, 0x1000);
  Call4(Camera_MoveWithinMapBounds, 0x21e0000, -1, 0x1000000, 1);
  zero = 0;
  t147 = Actor_GetById(16);
  FIELD_AT(t147, u8, 85) = zero;
  Actor_SetSpeedParameters(16, 0x18000, 0xc000);
  Actor_SetMovementTypeById(16, (s32)gData_02001470);
  Call3(Actor_SetSpeedParameters, 18, 0x8000, 0x4000);
  Actor_SetMovementTypeById(18, (s32)gData_0200151c);
  Call3(Actor_SetSpeedParameters, 17, SPLIT_CONSTANT(0x6666), SPLIT_CONSTANT(0x3333));
  Actor_SetMovementTypeById(17, (s32)gData_0200154c);
  Actor_RefreshSelectorById(16);
  Func_020013a0(16, 0x6000);
  FieldEvent_Wait(10);
  Call2(Func_020013a0, 16, 0x8000);
  FieldEvent_Wait(10);
  Func_020013a0(16, 0xa000);
  FieldEvent_Wait(10);
  Func_020013a0(16, 0xe000);
  FieldEvent_Wait(10);
  Func_020013a0(16, 0);
  FieldEvent_Wait(10);
  Func_020013a0(16, 0x2000);
  FieldEvent_Wait(10);
  Func_020013a0(16, 0x5000);
  FieldEvent_Wait(40);
  Actor_SetModeById(16, 3);
  FieldEvent_Wait(60);
  Func_02001390(16, 0);
  Actor_RefreshSelectorById(17);
  Actor_RefreshSelectorById(18);
  Actor_TurnToFaceEachOther(17, 18, 0);
  FieldEvent_Wait(20);
  goto L_020088f0;
L_020088f0:
  Call2(Actor_SetModeById, 18, 3);
  Actor_SetModeById(17, 3);
  FieldEvent_Wait(30);
  Call3(Actor_SetAngleToward, 17, 4, 0);
  Call3(Actor_SetSpeedParameters, 16, 0x10000, 0x8000);
  Actor_SetMovementTypeById(16, (s32)gData_0200158c);
  Call2(Func_020013b8, SPLIT_CONSTANT(0x1cccc), SPLIT_CONSTANT(0x3999));
  Call4(Camera_MoveWithinMapBounds, 0x21e0000, -1, 0x15e0000, 1);
  Func_020013c8();
  FieldMessage_ShowAtActor(17, 0);
  t186 = Event_AskYesNoWithActorAnimation(4, 0);
  if (t186 != 0)
    goto L_02008992;
  Call2(Actor_SetModeById, 18, 3);
  Func_02001390(18, 0);
  t189 = FIELD_AT(0x3000000, s32, 108);
  t190 = FIELD_AT(t189, u16, 452);
  FIELD_AT(t189, u16, 452) = (t190 + 1);
  goto L_020089c4;
L_02008982:
  Call3(Actor_SetPositionAndReset, 4, 0x1d7, 0x175);
  goto L_0200821e;
L_02008992:
  Call2(Actor_SetModeById, 18, 4);
  t193 = FIELD_AT(0x3000000, s32, 108);
  t194 = FIELD_AT(t193, u16, 452);
  FIELD_AT(t193, u16, 452) = (t194 + 1);
  Call2(Func_02001390, 17, 0);
  Call2(Actor_SetModeById, 18, 3);
  Func_02001390(18, 0);
L_020089c4:
  Call3(Actor_OffsetPositionAndReset, 18, 0, 24);
  Call3(Actor_CommitPositionAndActivate, 17, 0, 24);
  Call3(Actor_ArmCallback, 17, 0, 0);
  Actor_ArmCallback(18, 0xd000, 0);
  Actor_RefreshSelectorById(16);
  Actor_SetMovementTypeById(16, (s32)gData_020015e0);
  Actor_RefreshSelectorById(16);
  Call3(Actor_ArmCallback, 18, 0xc000, 0);
  Call3(Actor_ArmCallback, 17, 0, 0);
  Actor_ArmCallback(16, 0x6000, 0);
  FieldEvent_Wait(50);
  Call2(Actor_SetModeById, 18, 3);
  Call2(Actor_SetModeById, 17, 3);
  Actor_SetModeById(16, 3);
  FieldEvent_Wait(60);
  Call2(Actor_WatchAndSetScript, 4, 16);
  Call3(Actor_ResetAndSetPositionInMode2, 18, 0x1f8, 0x172);
  Call3(Actor_ResetAndSetPositionInMode2, 17, 0x1f8, 0x160);
  Actor_SetMovementTypeById(16, (s32)gData_02001638);
  Actor_WaitForMoveById(17);
  Actor_WaitForMoveById(18);
  Actor_RefreshSelectorById(16);
  Call2(Func_020013a0, 16, 0);
  Call3(Actor_SetSpeedParameters, 16, SPLIT_CONSTANT(0x4ccc), SPLIT_CONSTANT(0x2666));
  Call3(Actor_SetSpeedParameters, 17, SPLIT_CONSTANT(0x4ccc), SPLIT_CONSTANT(0x2666));
  Call3(Actor_SetSpeedParameters, 18, SPLIT_CONSTANT(0x4ccc), SPLIT_CONSTANT(0x2666));
  FieldEvent_Wait(40);
  Actor_SetSpeedParameters(20, SPLIT_CONSTANT(0x1999), SPLIT_CONSTANT(0xccc));
  t226 = Actor_GetById(20);
  FIELD_AT(t226, u8, 85) = 0;
  Actor_SetMovementTypeById(20, (s32)gData_0200166c);
  Actor_RefreshSelectorById(20);
  Call3(Actor_SetSpeedParameters, 20, SPLIT_CONSTANT(0x4ccc), SPLIT_CONSTANT(0x2666));
  Call2(Func_020013b8, SPLIT_CONSTANT(0x4ccc), SPLIT_CONSTANT(0x999));
  Camera_MoveWithinMapBounds(0x1cc0000, -1, 0x15e0000, 1);
  FieldEvent_Wait(30);
  t233 = Actor_GetById(16);
  FIELD_AT(t233, u8, 90) &= 254;
  Call3(Actor_OffsetPositionAndReset, 18, -0x28, -0x14);
  Call3(Actor_OffsetPositionAndReset, 17, -0x28, -0x14);
  Call3(Actor_OffsetPositionAndReset, 16, -0x28, -0x14);
  Call3(Actor_CommitPositionAndActivate, 20, -0x28, -0x14);
  Call3(Actor_OffsetPositionAndReset, 18, -0x8a, 0);
  Call3(Actor_OffsetPositionAndReset, 17, -0x8a, 0);
  Call3(Actor_OffsetPositionAndReset, 16, -0x8a, 0);
  Call3(Actor_CommitPositionAndActivate, 20, -0x8a, 0);
  Call3(Func_02001348, 16, 0, 0);
  Call3(Func_02001348, 18, 0, 0);
  Call3(Func_02001348, 17, 0, 0);
  Call3(Func_02001348, 20, 0, 0);
  Func_020012e8();
  return;
}
