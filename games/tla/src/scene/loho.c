/// @file
/// Overlay 0x6a6's scene: its code, then its tables, in ROM order.

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_inline.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "dma.h"
#include "text/msg_ids.h"

extern u32 gData_020017d8[];
extern u32 gData_020017a4[];
extern u32 gData_0200171c[];

void Func_020013f0(s32 a, s32 b, s32 c);
void Func_02001440(s32 a, s32 b);
void Func_020012e0(void);
void Func_02000f38(void);

void Func_02000038(void) { Func_080d7788(19, 3, 16); }

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern char SceneId_Ov6A6Room2;
extern u32 gData_0200183c[];

const u32 *Func_02000050(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6A6Room2)
    return gData_0200183c;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gData_02001900[];
extern struct ActorPlacement gData_02001a80[];

struct EventWork15a {
  u8 unk000[0x15a];
  u16 flag;
};

const struct ActorPlacement *Func_02000080(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6A6Room2)
    return gData_02001a80;
  return gData_02001900;
}

void Func_020000ac(void) {
  Map_CopyCellAttributeRect(69, 70, 10, 12, 24, 7);
  Func_0802b450(69, 70, 10, 12, 24, 7);
  Map_CopyCellAttributeRect(79, 70, 10, 12, 88, 7);
  Func_0802b450(79, 70, 10, 12, 88, 7);
  Map_CopyCellAttributeRect(69, 82, 10, 12, 24, 71);
  Func_0802b450(69, 82, 10, 12, 24, 71);
}

void Func_0200012c(s32 actor) {
  s32 msg = (s32)MsgLoho23;

  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
}

void Func_02000178(void) {
  FieldMessage_Show((s32)MsgLoho27, 1);
  ((struct EventWork15a *)Ram_HeapSlots->eventWork)->flag = 1;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
void Func_020000ac();
s32 Func_02001248();
s32 Func_02001268();
s32 Func_02001278();
s32 Func_02001280();

void Func_020012e8();
void Func_020013c8();

s32 Resource_FindFreeEntry();
s32 Resource_GetTableEntry();
s32 Resource_ResetEntry(u32 slot);

extern char ResourceId_FieldEffectExplosion;
extern u8 Field_Palette685[];
extern u32 gData_02001470[];
extern s32 gData_0200151c[];
extern s32 gData_0200154c[];
extern u32 gData_0200158c[];
extern u32 gData_020015e0[];
extern u32 gData_02001638[];
extern s32 gData_0200166c[];

static __inline__ void W_Func_020013a8(s32 a0, s32 a1, s32 a2) {
  /* FAKEMATCH(wrapper): W_Func_020013a8 forwarded inline; as direct calls to
   * Func_080d47b4, 70 of 1050 instructions differ in tla-en. */
  Func_080d47b4(a0, a1, a2);
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

  t0 = ((s32)Actor_GetById(4));
  Func_020012e0();
  Event_ResolvePendingActions(0);
  p = (struct Unk16 *)t0;
  goto L_020081ec;
L_020081b6:
  if (v_r1 == 0)
    goto L_020081dc;
  if (i == 0)
    goto L_020081ce;
  Actor_WalkToAndWait(4, 0x1d7, 0x15a);
  goto L_020081dc;
L_020081ce:
  Actor_WalkToAndWait(4, 0x1fa, 0x15a);
L_020081dc:
  if (i == 0)
    goto L_020081e2;
  goto L_02008982;
L_020081e2:
  Actor_WalkToAndWait(4, 0x1fa, 0x175);
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
  FieldCamera_MoveTo(0x1e80000, -1, 0x1740000, 1);
  GameFlag_SetBit(0xa5f);
  Actor_MoveToAndWait(4, 0x1e7, 0x179);
  Actor_FaceDirectionAndWait(4, 0xc000);
  Event_Wait(30);
  Sound_Play(140);
  FieldActor_StartRepeatedMotion(16, 2);
  FieldActor_StartRepeatedMotion(18, 2);
  Actor_RunRepeatedMotion(17, 2);
  Actor_FaceActor(16, 4, 0);
  Actor_FaceActor(17, 4, 0);
  Actor_ArmCallback(18, 0x5000, 0);
  Event_Wait(25);
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, 0x102);
  Actor_WalkByAndWait(4, 24, 12);
  Actor_ArmCallbackAndRefresh(4, 0xc000);
  Event_Wait(30);
  Sound_Play(144);
  Actor_ShowItemIconById(21, 0x1cc);
  Actor_SetPosition(21, 0x1e80000, 0x14a0000);
  Actor_SetDestinationOffset(20, 0, 8);
  Actor_SetSpeed(21, 0x30000, 0x18000);
  MapGrid_SetShake(0x20000, 0x20000, 0x10000);
  Actor_SetDestination(21, 0x1e8, 0x12c);
  Camera_MoveWithinMapBounds(0x1e80000, -1, 0x1340000, 1);
  Actor_WaitForMoveById(21);
  Actor_SetPosition(21, 0, 0);
  MapGrid_SetShake(0x70000, 0x70000, 0x10000);
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
          (volatile u32 *)0x40000d4);
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
  Sound_Play(145);
L_020083c4:
  if (i != 30)
    goto L_020083d4;
  Func_02001278(90);
  Sound_Play(141);
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
  FieldCamera_MoveTo(0x1e80000, -1, 0x1860000, 1);
  Actor_SetPositionAndFacing(14, 0xf80000, 0x1e00000, 0);
  Func_020000ac();
  MapGrid_SetShake(0, 0, 0x10000);
  Sound_Play(0x186);
  Event_Wait(180);
  Func_02001280(120);
  Event_SetMessageId((s32)MsgLoho28);
  Event_Wait(120);
  Actor_RunRepeatedMotion(18, 2);
  Actor_FaceDirectionAndWait(18, 0x8000);
  Event_ShowMessage(18, 0);
  Actor_FaceDirection(17, 0x2000, 0);
  Actor_SetAnimation(17, 3);
  Actor_SetModeById(16, 3);
  Event_Wait(30);
  W_Func_020013a8(17, 0x102, 35);
  Event_ShowMessage(17, 0);
  Func_080d47b4(16, 0x107, 35);
  Actor_WalkByAndWait(16, -12, 24);
  Actor_RunRepeatedMotion(16, 2);
  Event_ShowMessage(16, 0);
  Actor_SetAnimation(4, 3);
  Actor_FaceDirectionAndWait(18, 0xb000);
  Event_Wait(30);
  W_Func_020013a8(18, 0x100, 35);
  Event_ShowMessage(18, 0);
  W_Func_020013a8(16, 0x101, 0);
  Func_080d47b4(17, 0x101, 35);
  Actor_FaceActor(16, 18, 0);
  Actor_FaceDirection(17, 0x2000, 0);
  Event_Wait(60);
  Actor_FaceDirection(18, 0x8000, 0);
  Event_Wait(60);
  W_Func_020013a8(18, 0x105, 50);
  W_Func_020013a8(18, 0x102, 35);
  Event_ShowMessage(18, 0);
  Actor_TurnToFaceEachOther(16, 17, 0);
  Event_Wait(40);
  Actor_FaceDirection(16, 0xc000, 0);
  Actor_FaceDirection(17, 0xc000, 0);
  Actor_FaceDirection(18, 0xb000, 0);
  FieldCamera_MoveTo(0x1e80000, -1, 0x1460000, 1);
  Event_Wait(60);
  W_Func_020013a8(16, 0x100, 0);
  Func_080d47b4(17, 0x100, 50);
  W_Func_020013a8(17, 0x102, 35);
  Event_ShowMessage(17, 0);
  Actor_SetAnimation(16, 3);
  Event_ShowMessage(16, 0);
  Actor_FaceActor(18, 4, 0);
  Func_080d47b4(18, 0x109, 35);
  FieldCamera_MoveTo(0x1e80000, -1, 0x16e0000, 1);
  Func_020013c8();
  Actor_FaceDirection(18, 0x5000, 0);
  Event_ShowMessage(18, 0);
  Actor_SetSpeed(17, 0xcccc, 0x6666);
  Actor_OffsetPositionAndReset(17, 0, 16);
  Actor_WaitForMoveById(17);
  Actor_FaceDirectionAndWait(17, 0x2000);
  Actor_SetAnimation(17, 4);
  Event_ShowMessage(17, 0);
  Actor_SetAnimation(16, 3);
  Event_ShowMessage(16, 0);
  Actor_SetAnimation(18, 3);
  Actor_FaceDirection(17, 0x3000, 0);
  Event_ShowMessage(17, 0);
  Func_080d47b4(4, 0x102, 35);
  Event_Wait(60);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(16, 0x4000);
  Event_Wait(20);
  Actor_RunRepeatedMotion(16, 2);
  Event_ShowMessage(16, 0);
  W_Func_020013a8(18, 0x105, 0);
  W_Func_020013a8(17, 0x105, 0);
  Func_080d47b4(16, 0x105, 60);
  W_Func_020013a8(18, 0x106, 35);
  Event_ShowMessage(18, 0);
  Actor_SetModeById(17, 3);
  Event_Wait(40);
  Func_080d47b4(17, 0x106, 35);
  Event_ShowMessage(17, 0);
  Actor_SetModeById(16, 3);
  Event_Wait(30);
  FieldCamera_SetSpeed(0x8000, 0x1000);
  FieldCamera_MoveTo(0x21e0000, -1, 0x1000000, 1);
  zero = 0;
  t147 = ((s32)Actor_GetById(16));
  FIELD_AT(t147, u8, 85) = zero;
  Actor_SetSpeedParameters(16, 0x18000, 0xc000);
  Actor_SetMovementTypeById(16, (s32)gData_02001470);
  Actor_SetSpeed(18, 0x8000, 0x4000);
  Actor_SetMovementTypeById(18, (s32)gData_0200151c);
  Actor_SetSpeed(17, 0x6666, 0x3333);
  Actor_SetMovementTypeById(17, (s32)gData_0200154c);
  Actor_RefreshSelectorById(16);
  Actor_ArmCallbackAndRefresh(16, 0x6000);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(16, 0x8000);
  Event_Wait(10);
  Actor_ArmCallbackAndRefresh(16, 0xa000);
  Event_Wait(10);
  Actor_ArmCallbackAndRefresh(16, 0xe000);
  Event_Wait(10);
  Actor_ArmCallbackAndRefresh(16, 0);
  Event_Wait(10);
  Actor_ArmCallbackAndRefresh(16, 0x2000);
  Event_Wait(10);
  Actor_ArmCallbackAndRefresh(16, 0x5000);
  Event_Wait(40);
  Actor_SetModeById(16, 3);
  Event_Wait(60);
  Event_ShowMessageAndWait(16, 0);
  Actor_RefreshSelectorById(17);
  Actor_RefreshSelectorById(18);
  Actor_TurnToFaceEachOther(17, 18, 0);
  Event_Wait(20);
  goto L_020088f0;
L_020088f0:
  Actor_SetAnimation(18, 3);
  Actor_SetModeById(17, 3);
  Event_Wait(30);
  Actor_FaceActor(17, 4, 0);
  Actor_SetSpeed(16, 0x10000, 0x8000);
  Actor_SetMovementTypeById(16, (s32)gData_0200158c);
  FieldCamera_SetSpeed(0x1cccc, 0x3999);
  FieldCamera_MoveTo(0x21e0000, -1, 0x15e0000, 1);
  Func_020013c8();
  FieldMessage_ShowAtActor(17, 0);
  t186 = Event_AskYesNoWithActorAnimation(4, 0);
  if (t186 != 0)
    goto L_02008992;
  Actor_SetAnimation(18, 3);
  Event_ShowMessageAndWait(18, 0);
  t189 = (s32)Ram_HeapSlots->eventWork;
  t190 = FIELD_AT(t189, u16, 452);
  FIELD_AT(t189, u16, 452) = (t190 + 1);
  goto L_020089c4;
L_02008982:
  Actor_WalkToAndWait(4, 0x1d7, 0x175);
  goto L_0200821e;
L_02008992:
  Actor_SetAnimation(18, 4);
  t193 = (s32)Ram_HeapSlots->eventWork;
  t194 = FIELD_AT(t193, u16, 452);
  FIELD_AT(t193, u16, 452) = (t194 + 1);
  Event_ShowMessage(17, 0);
  Actor_SetAnimation(18, 3);
  Event_ShowMessageAndWait(18, 0);
L_020089c4:
  Actor_WalkBy(18, 0, 24);
  Actor_WalkByAndWait(17, 0, 24);
  Actor_FaceDirection(17, 0, 0);
  Actor_ArmCallback(18, 0xd000, 0);
  Actor_RefreshSelectorById(16);
  Actor_SetMovementTypeById(16, (s32)gData_020015e0);
  Actor_RefreshSelectorById(16);
  Actor_FaceDirection(18, 0xc000, 0);
  Actor_FaceDirection(17, 0, 0);
  Actor_ArmCallback(16, 0x6000, 0);
  Event_Wait(50);
  Actor_SetAnimation(18, 3);
  Actor_SetAnimation(17, 3);
  Actor_SetModeById(16, 3);
  Event_Wait(60);
  FieldActor_WatchAndSetScript(4, 16);
  Actor_WalkTo(18, 0x1f8, 0x172);
  Actor_WalkTo(17, 0x1f8, 0x160);
  Actor_SetMovementTypeById(16, (s32)gData_02001638);
  Actor_WaitForMoveById(17);
  Actor_WaitForMoveById(18);
  Actor_RefreshSelectorById(16);
  Actor_FaceDirectionAndWait(16, 0);
  Actor_SetSpeed(16, 0x4ccc, 0x2666);
  Actor_SetSpeed(17, 0x4ccc, 0x2666);
  Actor_SetSpeed(18, 0x4ccc, 0x2666);
  Event_Wait(40);
  Actor_SetSpeedParameters(20, 0x1999, 0xccc);
  t226 = ((s32)Actor_GetById(20));
  FIELD_AT(t226, u8, 85) = 0;
  Actor_SetMovementTypeById(20, (s32)gData_0200166c);
  Actor_RefreshSelectorById(20);
  Actor_SetSpeed(20, 0x4ccc, 0x2666);
  FieldCamera_SetSpeed(0x4ccc, 0x999);
  Camera_MoveWithinMapBounds(0x1cc0000, -1, 0x15e0000, 1);
  Event_Wait(30);
  t233 = ((s32)Actor_GetById(16));
  FIELD_AT(t233, u8, 90) &= 254;
  Actor_WalkBy(18, -0x28, -0x14);
  Actor_WalkBy(17, -0x28, -0x14);
  Actor_WalkBy(16, -0x28, -0x14);
  Actor_WalkByAndWait(20, -0x28, -0x14);
  Actor_WalkBy(18, -0x8a, 0);
  Actor_WalkBy(17, -0x8a, 0);
  Actor_WalkBy(16, -0x8a, 0);
  Actor_WalkByAndWait(20, -0x8a, 0);
  Actor_SetPosition(16, 0, 0);
  Actor_SetPosition(18, 0, 0);
  Actor_SetPosition(17, 0, 0);
  Actor_SetPosition(20, 0, 0);
  Func_020012e8();
  return;
}

void Func_02000bf4(void) {
  if (GameFlag_Test(0x90a))
    Event_SetMessageId((s32)MsgLoho32);
  else
    Event_SetMessageId((s32)MsgLoho55);
  Event_ShowMessageAndWait(11, 0);
}

void Func_02000c24(void) {
  if (GameFlag_Test(0x90a))
    Event_SetMessageId((s32)MsgLoho36);
  else
    Event_SetMessageId((s32)MsgLoho56);
  Event_ShowMessageAndWait(11, 0);
}

void Func_02000c54(s32 id) {
  s32 high = 192;
  s32 kind = 64;

  /* FAKEMATCH(barrier): the empty asm on the value and kind, so the kind is set
   * between the value's halves; as constant arguments, 2 of 6 instructions
   * differ in tla-en. */
  asm("" : "+l"(high), "+l"(kind));
  Func_020013f0(id, kind, high << 2);
}

void Func_02000c64(s32 id) {
  s32 value = 768;
  s32 kind = 65;

  /* FAKEMATCH(barrier): the empty asm on the value and kind, so the kind is set
   * between the value's halves; as constant arguments, 2 of 7 instructions
   * differ in tla-en. */
  asm("" : "+l"(value), "+l"(kind));
  Func_020013f0(id, kind, value + 1);
}

void Func_02000c74(s32 id) {
  s32 value = 768;
  s32 kind = 66;

  /* FAKEMATCH(barrier): the empty asm on the value and kind, so the kind is set
   * between the value's halves; as constant arguments, 2 of 7 instructions
   * differ in tla-en. */
  asm("" : "+l"(value), "+l"(kind));
  Func_020013f0(id, kind, value + 2);
}

extern char SceneId_Ov6A6Room1;
extern struct Trigger gData_02001ae0[];
extern struct Trigger gData_02001aec[];
extern struct Trigger gData_02001dd4[];

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_02001348_(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

void Func_02000c84(void) {
  GameFlag_SetBit(0x201);
  Actor_GetById(22)->collisionFlags &= 0xfe;
  if (GameFlag_Test(0xff7) == 0)
    Func_02001348_(66, PIXELS(520), PIXELS(136));
}

const struct Trigger *Func_02000cc0(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6A6Room1)
    return gData_02001aec;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6A6Room2)
    return gData_02001dd4;
  return gData_02001ae0;
}

void Func_02000d00(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  u16 facing = (leader->facing + 0x2000) & ~0x3fff;

  if (facing == 0xc000)
    FieldMessage_Show((s32)MsgLoho57, 1);
}

void Func_02000d40(void) {
  s32 count = 8;

  /* FAKEMATCH(barrier): the empty asm on count, so the second argument is set
   * first; as the constant argument 8, 2 of 5 instructions differ in tla-en. */
  asm("" : "+l"(count));
  Func_02001440(13, count);
}

void Func_02001430(s32 a, s32 b);
void Func_02001438(s32 id);

void Func_02000d4c(void) {
  Actor_GetById(gMainRoutineWork.currentOwner);
  if (GameFlag_Test(800)) {
    Func_02001430(30, 8);
  } else {
    if (GameFlag_Test(0xa5f))
      Event_SetMessageId((s32)MsgLoho51);
    else
      Event_SetMessageId((s32)MsgLoho17);
    Event_ShowMessageAndWait(8, 0);
  }
}

void Func_02000da4(s32 actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  u16 facing = (leader->facing + 0x2000) & ~0x3fff;

  if (facing == 0xc000) {
    Func_02001438(actor);
  } else {
    if (GameFlag_Test(0xa5f))
      Event_SetMessageId((s32)MsgLoho53);
    else
      Event_SetMessageId((s32)MsgLoho19);
    Event_ShowMessageAndWait(actor, 0);
  }
}

void Func_02000e0c(void) {
  Actor_SetMovementTypeById(8, gData_020017d8);
  GameFlag_SetBit(800);
}

void Func_02000e24(void) {
  Actor_SetMovementTypeById(8, gData_020017a4);
  GameFlag_SetBit(800);
}

void Func_02000e3c(void) {
  GameFlag_ClearBit(800);
  Actor_SetMovementTypeById(8, gData_0200171c);
}

struct EventWork154 {
  u8 unk000[0x154];
  s16 value;
};

struct LohoPicture {
  const void *data;
  u16 a;
  u16 b;
};

extern struct LohoPicture gData_02001f14;

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020012e8(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_020012a0_(const void *data, s32 a, s32 b) {
  Map_PlayMapEdit(data, a, b);
}

static __inline__ void Offset_(s32 id, s32 dx, s32 dz) {
  Actor_OffsetPositionAndResetMotion(id, dx, dz);
}

void Func_02000e54(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 i;

  Func_020012e0();
  Event_ResolvePendingActions(0);
  // The loop is written with goto so that it carries no loop notes: the bytes
  // rebuild the zero on every pass, and a for loop hoists it and moves the work
  // pointer (differ 4).
  i = 8;
loop:
  {
    struct Actor *object = Actor_GetById(i);

    if (object != NULL)
      object->motionFlags = 0;
  }
  i++;
  if ((u32)i <= 63)
    goto loop;
  work = (u8 *)&((struct EventWork154 *)work)->value;
  Sound_Play(158);
  Func_020012a0_(gData_02001f14.data, gData_02001f14.a, gData_02001f14.b);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Offset_(gMainRoutineWork.currentOwner, 0, -4);
  Sound_Play(123);
  Event_Wait(6);
  Import_EventSetExitRequest(*(s16 *)work);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020012e8();
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

void Func_02000eec(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct EventWork154 *work = Ram_HeapSlots->eventWork;

  /* FAKEMATCH(dead): an empty once-only block, so gMainRoutineWork loads
   * before the event work; as no block, one row is reordered, 2 of 31
   * instructions differ in tla-en. */
  do {
  } while (0);
  Actor_SetModeById(party->currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Sound_Play(123);
  Offset_(gMainRoutineWork.currentOwner, 0, -4);
  Import_EventSetExitRequest(work->value);
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_020012a8_(s32 srcX, s32 srcY, s32 width, s32 height,
                                      s32 destX, s32 destY) {
  Map_CopyCellPictureIndicesRect(srcX, srcY, width, height, destX, destY);
}

void Func_02000f38(void) {
  s32 value = ((struct EventWork154 *)Ram_HeapSlots->eventWork)->value;

  if (value == 2)
    Func_02001348_(8, PIXELS(144), PIXELS(224));
  else
    Func_02001348_(8, PIXELS(224), PIXELS(224));
  Actor_ArmCallback(4, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_GetById(4)->actionFlags &= 0xfe;
  Func_020012a8_(96, 14, 72, 14, 2, 1);
  Actor_OffsetPositionAndResetMotion(4, 0, 4);
  Actor_WaitForMoveById(4);
  Actor_SetSingleChildFlags(Actor_GetById(4), 0);
  Actor_SetModeById(4, 13);
  Actor_OffsetPositionAndResetMotion(4, 0, 16);
  Actor_WaitForMoveById(4);
  Actor_SetModeById(4, 10);
  Event_Wait(14);
  Sound_Play(123);
  Import_EventSetExitRequest(value);
  Func_020012e8();
}

void Func_02000ffc(void) {
  s32 back;
  s32 id;
  s32 x;

  Func_020012e0();
  Event_ResolvePendingActions(0);
  back = 14;
  id = 4;
  x = 0;
  /* FAKEMATCH(barrier): the empty asm on back, id and x, so the other
   * arguments are set before the step is negated; as constant arguments, 2 of
   * 17 instructions differ in tla-en. */
  asm("" : "+l"(back), "+l"(id), "+l"(x));
  Actor_CommitPositionAndActivate(id, x, -back);
  Actor_CommitPositionAndActivate(4, 16, 0);
  Event_Wait(10);
  Func_02000f38();
}

void Func_0200102c(void) {
  s32 back;
  s32 id;
  s32 x;

  Func_020012e0();
  Event_ResolvePendingActions(0);
  back = 14;
  id = 4;
  x = 0;
  /* FAKEMATCH(barrier): the empty asm on back, id and x, so the other
   * arguments are set before the step is negated; as constant arguments, 2 of
   * 18 instructions differ in tla-en. */
  asm("" : "+l"(back), "+l"(id), "+l"(x));
  Actor_CommitPositionAndActivate(id, x, -back);
  Actor_CommitPositionAndActivate(4, -16, 0);
  Event_Wait(10);
  Func_02000f38();
}

void Func_0200105c(void) {
  Func_020012e0();
  Event_ResolvePendingActions(0);
  Actor_ArmCallback(4, FACING_NORTH, 0);
  Event_Wait(10);
  Func_02000f38();
}

void Func_02001080(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 256;
  Actor_SetSingleChildFlags(Actor_GetById(20), 0);
}

void Func_020010a0(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 521;
  Actor_SetSingleChildFlags(Actor_GetById(9), 0);
}

void Actor_SetWideSprite(s32 id, s32 wide);

static __inline__ void Func_02001350_(s32 id, s32 x, s32 z, u16 angle) {
  /* FAKEMATCH(wrapper): forwarded inline, so each call builds its angle
   * first; as plain calls, 14 of 121 instructions differ in tla-en. */
  Actor_SetHorizontalPositionAndAngleWithTerrain(id, x, z, angle);
}

s32 Func_020010c0(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6A6Room1) {
    if (!GameFlag_Test(0x300))
      Actor_SetWideSprite(64, 0);
    if (!GameFlag_Test(0x301))
      Actor_SetWideSprite(65, 1);
    if (!GameFlag_Test(0x302))
      Actor_SetWideSprite(66, 1);
    Func_02001080();
  } else {
    Func_020010a0();
  }
  if (GameFlag_Test(0xa5f)) {
    Actor_SetHorizontalPositionWithTerrain(20, 0, 0);
    Actor_SetHorizontalPositionAndAngleWithTerrain(14, PIXELS(248), PIXELS(480),
                                                   FACING_EAST);
    if (!GameFlag_Test(0x90a)) {
      Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
    } else {
      Func_02001350_(16, PIXELS(459), PIXELS(254), FACING_NORTHEAST);
      Func_02001350_(18, PIXELS(442), PIXELS(203), FACING_WEST);
      Func_02001350_(17, PIXELS(490), PIXELS(188), FACING_SOUTHEAST);
      Actor_GetById(17)->timer = Actor_GetById(17)->x >> 16;
      Actor_GetById(17)->unk66 = Actor_GetById(17)->z >> 16;
      Actor_SetMovementTypeById(17, 2);
    }
  }
  return 0;
}

s32 Func_020011ec(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6A6Room1 && GameFlag_Test(0xa5f))
    Func_020000ac();
  return 0;
}

#include "objcmd.h"

extern u32 gData_020016b8[];
extern s16 gData_02001ef4[];

// clang-format off
u32 gData_02001470[] = {
    39, 2, 48, 0x21b0000, 0x1470000, 1,
    48, 0x2470000, 0x1470000, 1, 48, 0x2470000,
    0x12c0000, 1, 3, 0, 150, 22,
    26, 1, 2, 0x2460000, 0, 0x10a0000,
    4, 0x2460000, 0, 0xf80000, 1, 48,
    0x23a0000, 0xf80000, 1, 48, 0x23a0000, 0x1160000,
    1, 39, 1, 22, 3, 0x6000,
    17,
};

s32 gData_0200151c[] = {
    SC_WAIT(50),
    SC_ARG(2),
    SC_MOVE_BY(PIXELS(4), 0, PIXELS(-40)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gData_0200154c[] = {
    SC_WAIT(70),
    SC_ARG(2),
    SC_MOVE_BY(0, 0, PIXELS(-40)),
    SC_SYNC,
    SC_MOVE_XZ(PIXELS(512), PIXELS(303)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_0200158c[] = {
    39, 2, 4, 0x23a0000, 0, 0xf80000,
    1, 4, 0x2470000, 0, 0xf80000, 1,
    4, 0x2460000, 0, 0x10a0000, 1, 3,
    0, 20, 17,
};

u32 gData_020015e0[] = {
    SC_POSITION(PIXELS(583), 0, PIXELS(327)),
    SC_SET(SW_MOTION, 0),
    SC_POSITION(PIXELS(583), 0, PIXELS(300)),
    SC_MOVE_XZ(PIXELS(583), PIXELS(327)),
    SC_SYNC,
    SC_MOVE_XZ(PIXELS(554), PIXELS(332)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_02001638[] = {
    SC_ARG(2),
    SC_MOVE_XZ(PIXELS(472), PIXELS(331)),
    SC_SYNC,
    SC_MOVE_XZ(PIXELS(472), PIXELS(361)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gData_0200166c[] = {
    SC_MOVE_BY(0, PIXELS(2), 0),
    SC_SYNC,
    SC_END,
    SC_ARG(5),
    SC_WAIT(1),
    SC_END,
    SC_WAIT(5),
    SC_ARG(5),
    SC_SET(SW_SCALE_X, -0x10000),
    SC_END,
};

u32 gData_020016b8[] = {
    SC_LABEL(1),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

u32 gData_0200171c[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(128), 0, PIXELS(120)),
    SC_SYNC,
    SC_ARG(1),
    SC_LABEL(1),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

u32 gData_020017a4[] = {
    SC_SET(SW_SPEED, 0xe666),
    SC_ARG(2),
    SC_MOVE(PIXELS(128), 0, PIXELS(144)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_020017d8[] = {
    SC_SET(SW_SPEED, 0xe666),
    SC_ARG(2),
    SC_MOVE(PIXELS(159), 0, PIXELS(128)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gData_0200183c[] = {
    0x1c01c4, 0x1cc02e4, 0x2ec0024, 0x6ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(266),
    SCENE_EXIT_IF(1, 267, 1, -1),
    SCENE_EXIT_IF(2, 267, 2, -1),
    SCENE_EXIT_IF(3, 267, 3, -1),
    SCENE_EXIT_IF(4, 267, 4, -1),
    SCENE_EXIT_IF(5, 267, 5, -1),
    SCENE_EXIT_IF(6, 267, 6, -1),
    SCENE_EXIT_IF(8, 267, 6, -1),
    SCENE_EXIT_IF(9, 267, 6, -1),
    SCENE_EXIT_IF(7, 2, 59, -1),
    SCENE_EXITS(267),
    SCENE_EXIT_IF(1, 266, 1, -1),
    SCENE_EXIT_IF(2, 266, 2, -1),
    SCENE_EXIT_IF(3, 266, 3, -1),
    SCENE_EXIT_IF(4, 266, 4, -1),
    SCENE_EXIT_IF(5, 266, 5, -1),
    SCENE_EXIT_IF(6, 266, 6, -1),
    SCENE_EXIT_IF(7, 267, 8, -1),
    SCENE_EXIT_IF(8, 267, 7, -1),
    SCENE_EXIT_IF(9, 267, 10, -1),
    SCENE_EXIT_IF(10, 267, 9, -1),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_02001900[] = {
    {221, CONDITION_ALWAYS, 1, PIXELS(56), 0, PIXELS(304), FACING_SOUTHWEST, 1, 0},
    {221, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(392), FACING_SOUTHEAST, 1, 0},
    {221, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(264), FACING_WEST, 1, 0},
    {154, CONDITION_ALWAYS, 1, PIXELS(320), 0, PIXELS(320), FACING_NORTH, 1, 0},
    {221, CONDITION_ALWAYS, 1, PIXELS(176), 0, PIXELS(416), FACING_NORTHWEST, 1, 0},
    {221, CONDITION_ALWAYS, 2, PIXELS(88), 0, PIXELS(568), FACING_NORTH, 1, 0},
    {221, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(544), FACING_EAST, 1, 0},
    {154, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(544), FACING_NORTH, 1, 0},
    {221, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(336), FACING_NORTHWEST, 1, 0},
    {154, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(336), FACING_EAST, 1, 0},
    {221, CONDITION_ALWAYS, 1, PIXELS(528), 0, PIXELS(360), FACING_WEST, 1, 0},
    {246, 0x7c, 1, PIXELS(232), 0, PIXELS(248), FACING_SOUTH, 0, 0},
    {421, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(360), FACING_NORTH, 1, 0},
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(136), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02001a80[] = {
    {130, CONDITION_ALWAYS, (s32)gData_020016b8, PIXELS(128), 0, PIXELS(120), FACING_NORTHWEST, 1, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(120), FACING_SOUTH, 1, 0},
    {202, CONDITION_ALWAYS, 1, PIXELS(448), 0, PIXELS(408), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};

struct Trigger gData_02001ae0[] = {
    {TRIGGERS_END},
};

struct Trigger gData_02001aec[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Func_02000eec)},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_02000eec)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_02000eec)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Func_02000eec)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Func_02000e54)},
    {0x4602, 6, CONDITION_ALWAYS, EVENT(Func_0200105c)},
    {0x8602, 8, CONDITION_ALWAYS, EVENT(Func_0200102c)},
    {0x602, 9, CONDITION_ALWAYS, EVENT(Func_02000ffc)},
    {1, 7, CONDITION_ALWAYS, 7},
    {0, 8, 0x1a5f, MSGNO(MsgLoho29)},
    {0, 9, 0x1a5f, MSGNO(MsgLoho30)},
    {0, 10, 0x1a5f, MSGNO(MsgLoho31)},
    {0, 11, 0x1a5f, EVENT(Func_02000bf4)},
    {0x8d15, 8, 0x1a5f, MSGNO(MsgLoho33)},
    {0x8d15, 9, 0x1a5f, MSGNO(MsgLoho34)},
    {0x8d15, 10, 0x1a5f, MSGNO(MsgLoho35)},
    {0x8d15, 11, 0x1a5f, EVENT(Func_02000c24)},
    {0, 12, 0x1a5f, MSGNO(MsgLoho37)},
    {0, 13, 0x1a5f, MSGNO(MsgLoho38)},
    {0, 14, 0x1a5f, MSGNO(MsgLoho39)},
    {0, 15, 0x1a5f, MSGNO(MsgLoho40)},
    {0, 16, 0x1a5f, MSGNO(MsgLoho41)},
    {0, 18, 0x1a5f, MSGNO(MsgLoho42)},
    {0, 17, 0x1a5f, MSGNO(MsgLoho43)},
    {0x8d15, 12, 0x1a5f, MSGNO(MsgLoho44)},
    {0x8d15, 13, 0x1a5f, MSGNO(MsgLoho45)},
    {0x8d15, 14, 0x1a5f, MSGNO(MsgLoho46)},
    {0x8d15, 15, 0x1a5f, MSGNO(MsgLoho47)},
    {0x8d15, 16, 0x1a5f, MSGNO(MsgLoho48)},
    {0x8d15, 18, 0x1a5f, MSGNO(MsgLoho49)},
    {0x8d15, 17, 0x1a5f, MSGNO(MsgLoho50)},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgLoho01)},
    {0, 9, CONDITION_ALWAYS, MSGNO(MsgLoho02)},
    {0, 10, CONDITION_ALWAYS, MSGNO(MsgLoho03)},
    {0, 11, CONDITION_ALWAYS, MSGNO(MsgLoho04)},
    {0, 12, CONDITION_ALWAYS, MSGNO(MsgLoho09)},
    {0, 13, CONDITION_ALWAYS, MSGNO(MsgLoho10)},
    {0, 14, CONDITION_ALWAYS, MSGNO(MsgLoho11)},
    {0, 15, CONDITION_ALWAYS, MSGNO(MsgLoho12)},
    {0, 16, CONDITION_ALWAYS, MSGNO(MsgLoho21)},
    {0, 18, CONDITION_ALWAYS, MSGNO(MsgLoho22)},
    {0, 17, CONDITION_ALWAYS, EVENT(Func_0200012c)},
    {0, 20, CONDITION_ALWAYS, EVENT(Func_02000178)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgLoho05)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgLoho06)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgLoho07)},
    {0x8d15, 11, CONDITION_ALWAYS, MSGNO(MsgLoho08)},
    {0x8d15, 12, CONDITION_ALWAYS, MSGNO(MsgLoho13)},
    {0x8d15, 13, CONDITION_ALWAYS, MSGNO(MsgLoho14)},
    {0x8d15, 14, CONDITION_ALWAYS, MSGNO(MsgLoho15)},
    {0x8d15, 15, CONDITION_ALWAYS, MSGNO(MsgLoho16)},
    {0x8d15, 16, CONDITION_ALWAYS, MSGNO(MsgLoho24)},
    {0x8d15, 18, CONDITION_ALWAYS, MSGNO(MsgLoho25)},
    {0x8d15, 17, CONDITION_ALWAYS, MSGNO(MsgLoho26)},
    {0x1f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLoho58))},
    {0x1cc14, 20, CONDITION_ALWAYS, EVENT(Func_02000198)},
    {0, 19, CONDITION_ALWAYS, EVENT(Func_02000038)},
    {0x50008805, 101, 0x300, EVENT(Func_02000c54)},
    {0x50008805, 102, 0x301, EVENT(Func_02000c64)},
    {0x50008805, 103, 0x302, EVENT(Func_02000c74)},
    {0x8f15, 22, 0x201, EVENT(Func_02000c84)},
    {TRIGGERS_END},
};

struct Trigger gData_02001dd4[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {1, 6, CONDITION_ALWAYS, 6},
    {1, 7, CONDITION_ALWAYS, 7},
    {1, 8, CONDITION_ALWAYS, 8},
    {1, 9, CONDITION_ALWAYS, 9},
    {1, 10, CONDITION_ALWAYS, 10},
    {0xc402, 20, 0x320, EVENT(Func_02000e24)},
    {0x8402, 22, 0x320, EVENT(Func_02000e0c)},
    {2, 21, 0x1320, EVENT(Func_02000e3c)},
    {2, 23, 0x1320, EVENT(Func_02000e3c)},
    {0, 8, CONDITION_ALWAYS, EVENT(Func_02000d4c)},
    {0x8d15, 8, 0xa5f, MSGNO(MsgLoho18)},
    {0x8d15, 8, 0x1a5f, MSGNO(MsgLoho52)},
    {0, 9, 0x1320, EVENT(Func_02000d40)},
    {0, 10, CONDITION_ALWAYS, EVENT(Func_02000da4)},
    {0x8d15, 10, 0xa5f, MSGNO(MsgLoho20)},
    {0x8d15, 10, 0x1a5f, MSGNO(MsgLoho54)},
    {3, 20, CONDITION_ALWAYS, EVENT(Func_02000d40)},
    {3, 30, CONDITION_ALWAYS, EVENT(Func_02000d00)},
    {TRIGGERS_END},
};

s16 gData_02001ef4[] = {
    64, 50, 2, 2, 2, 64, 52, 2,
    2, 2, 64, 54, 2, 2, 2, -1,
};
// clang-format on

struct LohoPicture gData_02001f14 = {gData_02001ef4, 79, 15};
