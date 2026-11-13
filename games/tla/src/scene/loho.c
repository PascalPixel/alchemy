/// @file
/// The scene code of overlay 0x6a6, in ROM order.

#include "shared/routines.h"
#include "shared/tables.h"
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
#include "event_work.h"
#include "item_icon.h"
#include "module/scene/loho.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "scene_ids.h"
#include "resource.h"

void Actor_ReturnHome(s32 a, s32 b, s32 c);

/// Lull, the Jupiter Djinni behind the wall the cannon breaks, joins the party
/// when spoken to.
void Loho_MeetLull(void) { Djinn_JoinParty(19, 3, 16); }

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const u32 *Scene_GetRegions(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6A6Room2)
    return gData_0200183c;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6A6Room2)
    return gData_02001a80;
  return gData_02001900;
}

void Func_020000ac(void) {
  MapGrid_CopyCellAttributes(69, 70, 10, 12, 24, 7);
  MapGrid_CopyCellsLayered(69, 70, 10, 12, 24, 7);
  MapGrid_CopyCellAttributes(79, 70, 10, 12, 88, 7);
  MapGrid_CopyCellsLayered(79, 70, 10, 12, 88, 7);
  MapGrid_CopyCellAttributes(69, 82, 10, 12, 24, 71);
  MapGrid_CopyCellsLayered(69, 82, 10, 12, 24, 71);
}

/// "Did you want this cannon?"
void Loho_AskWantCannon(s32 actor) {
  s32 msg = (s32)MsgLoho23;

  Event_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

/// "It looks like it's empty."
void Loho_ExamineCannon(void) {
  FieldMessage_Show((s32)MsgLoho27, 1);
  ((struct EventWork15a *)Ram_HeapSlots->eventWork)->flag = 1;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void FieldCamera_WaitForArrival();

/// The Magma Ball used on the cannon: it is loaded and fired at the ancient
/// wall north of town, which is breached, and the miners give the party the
/// cannon for its ship.
/// "Is everyone all right?"
void Loho_FireCannon(void) {
  s32 v_r1, v_r2, v_r0, t0;
  struct Unk16 *p;
  struct Unk16 *table;
  s32 tiles, entry, i, value, zero;
  s32 t38, t47, t48, t49, t50, t51;
  s32 t147, t186, t189;
  s32 t190, t193, t194, t226, t233, t234;
  u8 unused[4]; // Unused: gives the stack frame its 4 bytes.

  t0 = ((s32)Actor_GetById(4));
  Event_Start();
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
  Event_SetFlag(0xa5f);
  Actor_MoveToAndWait(4, 0x1e7, 0x179);
  Actor_FaceDirectionAndWait(4, 0xc000);
  Event_Wait(30);
  Event_PlaySound(140);
  FieldActor_StartRepeatedMotion(16, 2);
  FieldActor_StartRepeatedMotion(18, 2);
  Actor_RunRepeatedMotion(17, 2);
  Actor_FaceActor(16, 4, 0);
  Actor_FaceActor(17, 4, 0);
  Actor_FaceDirection(18, 0x5000, 0);
  Event_Wait(25);
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, 0x102);
  Actor_WalkByAndWait(4, 24, 12);
  Actor_FaceDirectionAndWait(4, 0xc000);
  Event_Wait(30);
  Event_PlaySound(144);
  Actor_ShowItemIconById(21, 0x1cc);
  Actor_SetPosition(21, 0x1e80000, 0x14a0000);
  Actor_SetDestinationOffset(20, 0, 8);
  Actor_SetSpeed(21, 0x30000, 0x18000);
  MapGrid_SetShake(0x20000, 0x20000, 0x10000);
  Actor_SetDestination(21, 0x1e8, 0x12c);
  FieldCamera_MoveTo(0x1e80000, -1, 0x1340000, 1);
  Actor_WaitForMoveById(21);
  Actor_SetPosition(21, 0, 0);
  MapGrid_SetShake(0x70000, 0x70000, 0x10000);
  table = (struct Unk16 *)Heap_AllocEwram(0x600);
  p = (struct Unk16 *)Heap_Alloc(0x1000);
  t38 = Resource_GetTableEntry((s32)&ResourceId_FieldEffectExplosion);
  Resource_DecodeType01(t38, p);
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
  Event_PlaySound(145);
L_020083c4:
  if (i != 30)
    goto L_020083d4;
  Fade_ToWhite(90);
  Event_PlaySound(141);
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
  Event_PlaySound(0x186);
  Event_Wait(180);
  Fade_FromWhite(120);
  Event_SetMessageId((s32)MsgLoho28);
  Event_Wait(120);
  Actor_RunRepeatedMotion(18, 2);
  Actor_FaceDirectionAndWait(18, 0x8000);
  Event_ShowMessage(18, 0);
  Actor_FaceDirection(17, 0x2000, 0);
  Actor_SetAnimation(17, 3);
  Actor_SetAnimation(16, 3);
  Event_Wait(30);
  FieldActor_ShowEmote(17, 0x102, 35);
  Event_ShowMessage(17, 0);
  FieldActor_ShowEmote(16, 0x107, 35);
  Actor_WalkByAndWait(16, -12, 24);
  Actor_RunRepeatedMotion(16, 2);
  Event_ShowMessage(16, 0);
  Actor_SetAnimation(4, 3);
  Actor_FaceDirectionAndWait(18, 0xb000);
  Event_Wait(30);
  FieldActor_ShowEmote(18, 0x100, 35);
  Event_ShowMessage(18, 0);
  FieldActor_ShowEmote(16, 0x101, 0);
  FieldActor_ShowEmote(17, 0x101, 35);
  Actor_FaceActor(16, 18, 0);
  Actor_FaceDirection(17, 0x2000, 0);
  Event_Wait(60);
  Actor_FaceDirection(18, 0x8000, 0);
  Event_Wait(60);
  FieldActor_ShowEmote(18, 0x105, 50);
  FieldActor_ShowEmote(18, 0x102, 35);
  Event_ShowMessage(18, 0);
  Actor_FaceEachOther(16, 17, 0);
  Event_Wait(40);
  Actor_FaceDirection(16, 0xc000, 0);
  Actor_FaceDirection(17, 0xc000, 0);
  Actor_FaceDirection(18, 0xb000, 0);
  FieldCamera_MoveTo(0x1e80000, -1, 0x1460000, 1);
  Event_Wait(60);
  FieldActor_ShowEmote(16, 0x100, 0);
  FieldActor_ShowEmote(17, 0x100, 50);
  FieldActor_ShowEmote(17, 0x102, 35);
  Event_ShowMessage(17, 0);
  Actor_SetAnimation(16, 3);
  Event_ShowMessage(16, 0);
  Actor_FaceActor(18, 4, 0);
  FieldActor_ShowEmote(18, 0x109, 35);
  FieldCamera_MoveTo(0x1e80000, -1, 0x16e0000, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(18, 0x5000, 0);
  Event_ShowMessage(18, 0);
  Actor_SetSpeed(17, 0xcccc, 0x6666);
  Actor_WalkBy(17, 0, 16);
  Actor_WaitForMoveById(17);
  Actor_FaceDirectionAndWait(17, 0x2000);
  Actor_SetAnimation(17, 4);
  Event_ShowMessage(17, 0);
  Actor_SetAnimation(16, 3);
  Event_ShowMessage(16, 0);
  Actor_SetAnimation(18, 3);
  Actor_FaceDirection(17, 0x3000, 0);
  Event_ShowMessage(17, 0);
  FieldActor_ShowEmote(4, 0x102, 35);
  Event_Wait(60);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(16, 0x4000);
  Event_Wait(20);
  Actor_RunRepeatedMotion(16, 2);
  Event_ShowMessage(16, 0);
  FieldActor_ShowEmote(18, 0x105, 0);
  FieldActor_ShowEmote(17, 0x105, 0);
  FieldActor_ShowEmote(16, 0x105, 60);
  FieldActor_ShowEmote(18, 0x106, 35);
  Event_ShowMessage(18, 0);
  Actor_SetAnimation(17, 3);
  Event_Wait(40);
  FieldActor_ShowEmote(17, 0x106, 35);
  Event_ShowMessage(17, 0);
  Actor_SetAnimation(16, 3);
  Event_Wait(30);
  FieldCamera_SetSpeed(0x8000, 0x1000);
  FieldCamera_MoveTo(0x21e0000, -1, 0x1000000, 1);
  zero = 0;
  t147 = ((s32)Actor_GetById(16));
  FIELD_AT(t147, u8, 85) = zero;
  Actor_SetSpeed(16, 0x18000, 0xc000);
  Actor_SetScript(16, (s32)gData_02001470);
  Actor_SetSpeed(18, 0x8000, 0x4000);
  Actor_SetScript(18, (s32)gData_0200151c);
  Actor_SetSpeed(17, 0x6666, 0x3333);
  Actor_SetScript(17, (s32)gData_0200154c);
  Actor_RefreshSelectorById(16);
  Actor_FaceDirectionAndWait(16, 0x6000);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(16, 0x8000);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(16, 0xa000);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(16, 0xe000);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(16, 0);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(16, 0x2000);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(16, 0x5000);
  Event_Wait(40);
  Actor_SetAnimation(16, 3);
  Event_Wait(60);
  Event_ShowMessage(16, 0);
  Actor_RefreshSelectorById(17);
  Actor_RefreshSelectorById(18);
  Actor_FaceEachOther(17, 18, 0);
  Event_Wait(20);
  goto L_020088f0;
L_020088f0:
  Actor_SetAnimation(18, 3);
  Actor_SetAnimation(17, 3);
  Event_Wait(30);
  Actor_FaceActor(17, 4, 0);
  Actor_SetSpeed(16, 0x10000, 0x8000);
  Actor_SetScript(16, (s32)gData_0200158c);
  FieldCamera_SetSpeed(0x1cccc, 0x3999);
  FieldCamera_MoveTo(0x21e0000, -1, 0x15e0000, 1);
  FieldCamera_WaitForArrival();
  Event_OpenMessage(17, 0);
  t186 = Event_AskYesNoWithActorAnimation(4, 0);
  if (t186 != 0)
    goto L_02008992;
  Actor_SetAnimation(18, 3);
  Event_ShowMessage(18, 0);
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
  Event_ShowMessage(18, 0);
L_020089c4:
  Actor_WalkBy(18, 0, 24);
  Actor_WalkByAndWait(17, 0, 24);
  Actor_FaceDirection(17, 0, 0);
  Actor_FaceDirection(18, 0xd000, 0);
  Actor_RefreshSelectorById(16);
  Actor_SetScript(16, (s32)gData_020015e0);
  Actor_RefreshSelectorById(16);
  Actor_FaceDirection(18, 0xc000, 0);
  Actor_FaceDirection(17, 0, 0);
  Actor_FaceDirection(16, 0x6000, 0);
  Event_Wait(50);
  Actor_SetAnimation(18, 3);
  Actor_SetAnimation(17, 3);
  Actor_SetAnimation(16, 3);
  Event_Wait(60);
  FieldActor_WatchAndSetScript(4, 16);
  Actor_WalkTo(18, 0x1f8, 0x172);
  Actor_WalkTo(17, 0x1f8, 0x160);
  Actor_SetScript(16, (s32)gData_02001638);
  Actor_WaitForMoveById(17);
  Actor_WaitForMoveById(18);
  Actor_RefreshSelectorById(16);
  Actor_FaceDirectionAndWait(16, 0);
  Actor_SetSpeed(16, 0x4ccc, 0x2666);
  Actor_SetSpeed(17, 0x4ccc, 0x2666);
  Actor_SetSpeed(18, 0x4ccc, 0x2666);
  Event_Wait(40);
  Actor_SetSpeed(20, 0x1999, 0xccc);
  t226 = ((s32)Actor_GetById(20));
  FIELD_AT(t226, u8, 85) = 0;
  Actor_SetScript(20, (s32)gData_0200166c);
  Actor_RefreshSelectorById(20);
  Actor_SetSpeed(20, 0x4ccc, 0x2666);
  FieldCamera_SetSpeed(0x4ccc, 0x999);
  FieldCamera_MoveTo(0x1cc0000, -1, 0x15e0000, 1);
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
  Event_End();
  return;
}

/// "Now that the wall's been breached, we can get on with the mining! Good
/// work!"
void Loho_TalkAboutBreachedWall(void) {
  if (Event_GetFlag(0x90a))
    Event_SetMessageId((s32)MsgLoho32);
  else
    Event_SetMessageId((s32)MsgLoho55);
  Event_ShowMessage(11, 0);
}

void Loho_MindReadMiner(void) {
  if (Event_GetFlag(0x90a))
    Event_SetMessageId((s32)MsgLoho36);
  else
    Event_SetMessageId((s32)MsgLoho56);
  Event_ShowMessage(11, 0);
}

void Func_02000c54(s32 id) {
  s32 high = 192;
  s32 kind = 64;

  /* FAKEMATCH(barrier): the empty asm on the value and kind, so the kind is set
   * between the value's halves; as constant arguments, 2 of 6 instructions
   * differ in tla-en. */
  asm("" : "+l"(high), "+l"(kind));
  Actor_ReturnHome(id, kind, high << 2);
}

DEFINE_FUNC_02000C64(Func_02000c64, 65, 1)

DEFINE_FUNC_02000C64(Func_02000c74, 66, 2)

/// Lift raises the rock (flag 0x201), and the Golem Core under it shows until
/// it is dug up (flag 0xff7).
void Loho_LiftRock(void) {
  Event_SetFlag(0x201);
  Actor_GetById(22)->collisionFlags &= 0xfe;
  if (Event_GetFlag(0xff7) == 0)
    Actor_SetPosition(66, PIXELS(520), PIXELS(136));
}

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6A6Room1)
    return gData_02001aec;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6A6Room2)
    return gData_02001dd4;
  return gData_02001ae0;
}

/// "People staying at the inn can check in at the counter."
void Loho_ReadInnSign(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  u16 facing = (leader->facing + 0x2000) & ~0x3fff;

  if (facing == 0xc000)
    FieldMessage_Show((s32)MsgLoho57, 1);
}

void Loho_TalkToInnkeeper(void) {
  s32 count = 8;

  /* FAKEMATCH(barrier): the empty asm on count, so the second argument is set
   * first; as the constant argument 8, 2 of 5 instructions differ in tla-en. */
  asm("" : "+l"(count));
  Inn_CheckIn(13, count);
}

/// "Commerce really took off when we started this mining colony in Loho."
void Loho_TalkAboutCommerce(void) {
  Actor_GetById(gMainRoutineWork.currentOwner);
  if (Event_GetFlag(800)) {
    Shop_Run(30, 8);
  } else {
    if (Event_GetFlag(0xa5f))
      Event_SetMessageId((s32)MsgLoho51);
    else
      Event_SetMessageId((s32)MsgLoho17);
    Event_ShowMessage(8, 0);
  }
}

/// "If you're only here for the cannon, you can forget about it."
void Loho_TalkAboutCannon(s32 actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  u16 facing = (leader->facing + 0x2000) & ~0x3fff;

  if (facing == 0xc000) {
    Sanctum_Run(actor);
  } else {
    if (Event_GetFlag(0xa5f))
      Event_SetMessageId((s32)MsgLoho53);
    else
      Event_SetMessageId((s32)MsgLoho19);
    Event_ShowMessage(actor, 0);
  }
}

void Func_02000e0c(void) {
  Actor_SetScript(8, gData_020017d8);
  Event_SetFlag(800);
}

void Func_02000e24(void) {
  Actor_SetScript(8, gData_020017a4);
  Event_SetFlag(800);
}

void Func_02000e3c(void) {
  Event_ClearFlag(800);
  Actor_SetScript(8, gData_0200171c);
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_020012a0_(const void *data, s32 a, s32 b) {
  MapGrid_AnimateCells(data, a, b);
}

void Func_02000e54(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 i;

  Event_Start();
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
  work = (u8 *)&((struct EventWork154 *)work)->returnEntrance;
  Event_PlaySound(158);
  Func_020012a0_(gData_02001f14.data, gData_02001f14.a, gData_02001f14.b);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 0, -4);
  Event_PlaySound(123);
  Event_Wait(6);
  Import_EventSetExitRequest(*(s16 *)work);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

void Loho_EnterDoor(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct EventWork154 *work = Ram_HeapSlots->eventWork;

  /* FAKEMATCH(dead): an empty once-only block, so gMainRoutineWork loads
   * before the event work; as no block, one row is reordered, 2 of 31
   * instructions differ in tla-en. */
  do {
  } while (0);
  Actor_SetAnimation(party->currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Event_PlaySound(123);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 0, -4);
  Import_EventSetExitRequest(work->returnEntrance);
}

void Func_02000f38(void) {
  s32 value = ((struct EventWork154 *)Ram_HeapSlots->eventWork)->returnEntrance;

  if (value == 2)
    Actor_SetPosition(8, PIXELS(144), PIXELS(224));
  else
    Actor_SetPosition(8, PIXELS(224), PIXELS(224));
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_GetById(4)->actionFlags &= 0xfe;
  MapGrid_CopyCellsTo(96, 14, 72, 14, 2, 1);
  Actor_SetDestinationOffset(4, 0, 4);
  Actor_WaitForMoveById(4);
  Actor_SetSingleChildFlags(Actor_GetById(4), 0);
  Actor_SetAnimation(4, 13);
  Actor_SetDestinationOffset(4, 0, 16);
  Actor_WaitForMoveById(4);
  Actor_SetAnimation(4, 10);
  Event_Wait(14);
  Event_PlaySound(123);
  Import_EventSetExitRequest(value);
  Event_End();
}

void Func_02000ffc(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, 0, -14);
  Actor_WalkByAndWait(4, 16, 0);
  Event_Wait(10);
  Func_02000f38();
}

void Func_0200102c(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, 0, -14);
  Actor_WalkByAndWait(4, -16, 0);
  Event_Wait(10);
  Func_02000f38();
}

void Func_0200105c(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceDirection(4, FACING_NORTH, 0);
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
  Actor_SetPositionAndFacing(id, x, z, angle);
}

s32 Scene_Initialize(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6A6Room1) {
    if (!Event_GetFlag(0x300))
      Actor_SetWideSprite(64, 0);
    if (!Event_GetFlag(0x301))
      Actor_SetWideSprite(65, 1);
    if (!Event_GetFlag(0x302))
      Actor_SetWideSprite(66, 1);
    Func_02001080();
  } else {
    Func_020010a0();
  }
  if (Event_GetFlag(0xa5f)) {
    Actor_SetPosition(20, 0, 0);
    Actor_SetPositionAndFacing(14, PIXELS(248), PIXELS(480),
                                                   FACING_EAST);
    if (!Event_GetFlag(0x90a)) {
      Actor_SetPosition(17, 0, 0);
      Actor_SetPosition(16, 0, 0);
      Actor_SetPosition(18, 0, 0);
    } else {
      Func_02001350_(16, PIXELS(459), PIXELS(254), FACING_NORTHEAST);
      Func_02001350_(18, PIXELS(442), PIXELS(203), FACING_WEST);
      Func_02001350_(17, PIXELS(490), PIXELS(188), FACING_SOUTHEAST);
      Actor_GetById(17)->timer = Actor_GetById(17)->x >> 16;
      Actor_GetById(17)->unk66 = Actor_GetById(17)->z >> 16;
      Actor_SetScript(17, 2);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6A6Room1 && Event_GetFlag(0xa5f))
    Func_020000ac();
  return 0;
}

#include "objcmd.h"

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

DEFINE_G_DATA_020017A4(gData_020017a4, 128, 144)

DEFINE_G_DATA_020017A4(gData_020017d8, 159, 128)

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
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Loho_EnterDoor)},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Loho_EnterDoor)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Loho_EnterDoor)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Loho_EnterDoor)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Func_02000e54)},
    {0x4602, 6, CONDITION_ALWAYS, EVENT(Func_0200105c)},
    {0x8602, 8, CONDITION_ALWAYS, EVENT(Func_0200102c)},
    {0x602, 9, CONDITION_ALWAYS, EVENT(Func_02000ffc)},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, 8, 0x1a5f, MSGNO(MsgLoho29)},
    {TRIGGER_TALK, 9, 0x1a5f, MSGNO(MsgLoho30)},
    {TRIGGER_TALK, 10, 0x1a5f, MSGNO(MsgLoho31)},
    {TRIGGER_TALK, 11, 0x1a5f, EVENT(Loho_TalkAboutBreachedWall)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x1a5f, MSGNO(MsgLoho33)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x1a5f, MSGNO(MsgLoho34)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x1a5f, MSGNO(MsgLoho35)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x1a5f, EVENT(Loho_MindReadMiner)},
    {TRIGGER_TALK, 12, 0x1a5f, MSGNO(MsgLoho37)},
    {TRIGGER_TALK, 13, 0x1a5f, MSGNO(MsgLoho38)},
    {TRIGGER_TALK, 14, 0x1a5f, MSGNO(MsgLoho39)},
    {TRIGGER_TALK, 15, 0x1a5f, MSGNO(MsgLoho40)},
    {TRIGGER_TALK, 16, 0x1a5f, MSGNO(MsgLoho41)},
    {TRIGGER_TALK, 18, 0x1a5f, MSGNO(MsgLoho42)},
    {TRIGGER_TALK, 17, 0x1a5f, MSGNO(MsgLoho43)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x1a5f, MSGNO(MsgLoho44)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x1a5f, MSGNO(MsgLoho45)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x1a5f, MSGNO(MsgLoho46)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x1a5f, MSGNO(MsgLoho47)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x1a5f, MSGNO(MsgLoho48)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, 0x1a5f, MSGNO(MsgLoho49)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x1a5f, MSGNO(MsgLoho50)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgLoho01)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgLoho02)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgLoho03)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgLoho04)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgLoho09)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgLoho10)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgLoho11)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgLoho12)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgLoho21)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgLoho22)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(Loho_AskWantCannon)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, EVENT(Loho_ExamineCannon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgLoho05)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgLoho06)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgLoho07)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgLoho08)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgLoho13)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgLoho14)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgLoho15)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgLoho16)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgLoho24)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgLoho25)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgLoho26)},
    {0x1f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLoho58))},
    {0x1cc14, 20, CONDITION_ALWAYS, EVENT(Loho_FireCannon)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, EVENT(Loho_MeetLull)},
    {0x50008805, 101, 0x300, EVENT(Func_02000c54)},
    {0x50008805, 102, 0x301, EVENT(Func_02000c64)},
    {0x50008805, 103, 0x302, EVENT(Func_02000c74)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), 22, 0x201, EVENT(Loho_LiftRock)},
    {TRIGGERS_END},
};

struct Trigger gData_02001dd4[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {0xc402, 20, 0x320, EVENT(Func_02000e24)},
    {0x8402, 22, 0x320, EVENT(Func_02000e0c)},
    {TRIGGER_STEP, 21, 0x1320, EVENT(Func_02000e3c)},
    {TRIGGER_STEP, 23, 0x1320, EVENT(Func_02000e3c)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(Loho_TalkAboutCommerce)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0xa5f, MSGNO(MsgLoho18)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x1a5f, MSGNO(MsgLoho52)},
    {TRIGGER_TALK, 9, 0x1320, EVENT(Loho_TalkToInnkeeper)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(Loho_TalkAboutCannon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0xa5f, MSGNO(MsgLoho20)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x1a5f, MSGNO(MsgLoho54)},
    {TRIGGER_EXAMINE, 20, CONDITION_ALWAYS, EVENT(Loho_TalkToInnkeeper)},
    {TRIGGER_EXAMINE, 30, CONDITION_ALWAYS, EVENT(Loho_ReadInnSign)},
    {TRIGGERS_END},
};

s16 gData_02001ef4[] = {
    64, 50, 2, 2, 2, 64, 52, 2,
    2, 2, 64, 54, 2, 2, 2, -1,
};
// clang-format on

struct LohoPicture gData_02001f14 = {gData_02001ef4, 79, 15};
