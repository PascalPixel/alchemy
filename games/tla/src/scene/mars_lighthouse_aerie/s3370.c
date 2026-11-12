#include "types.h"
#include "actor.h"
#include "frame.h"
#include "event_inline.h"

struct MarsLighthouseAerieSprite {
  u8 unk00[9];
  u8 unk09_0 : 2;
  u8 layer : 2;
  u8 unk09_4 : 4;
  u8 unk0a[0x10];
  u8 unk1a;
};

extern u32 Data_02006ccc;
extern u32 Data_02006b68;
extern const u8 gData_02005b54[];
extern const u8 gData_02006180[];

u32 Random16Far(void);
void SceneActor_ParkRecord(struct Actor *object);
void Func_0200556c(struct Actor *object, const u8 *script);
void Func_020000e4(void);

static __inline__ void SetScale(s32 a, s32 b, s32 c) {
  /* FAKEMATCH(wrapper): forwarded inline; as a direct call, 41 of 207
   * instructions differ in tla-en. */
  Map_SetScreenShake(a, b, c);
}

void Func_02003370(void) {
  struct Actor *anchor = Actor_GetById(8);
  s32 spawn = 0;
  struct Actor *object;
  struct MarsLighthouseAerieSprite *sprite;

  switch (Data_02006ccc) {
  case 0:
    Event_PlaySound(220);
    SetScale(0x60000, 0x60000, 0x10000);
    PaletteFade_ApplyTarget(0x2063ff, 1);
    PaletteFade_Interpolate(8);
    break;
  case 8:
    PaletteFade_ApplyTarget(0x10000, 1);
    PaletteFade_Interpolate(8);
    break;
  case 16:
    SetScale(0x10000, 0x10000, 0x10000);
    break;
  case 24:
    anchor->x = 0x2000000;
    anchor->y = -0x1680000;
    anchor->z = 0xcc0000;
    anchor->scaleX = 0x10000;
    anchor->scaleY = 0x10000;
    SceneActor_ParkRecord(anchor);
    Actor_SetScript(8, gData_02005b54);
    break;
  case 25:
    Data_02006ccc--;
    if (anchor->y > 0) {
      PaletteFade_ApplyTarget(0x203210, 0);
      PaletteFade_Interpolate(16);
      Data_02006ccc++;
    } else {
      if ((gFrameCount & 7) == 0)
        Event_PlaySound(246);
      spawn = 1;
      anchor->y += 0x24000;
    }
    break;
  case 26:
    Data_02006ccc--;
    if (anchor->y > 0x140000) {
      Data_02006ccc = 999;
      FieldTask_Remove(Func_02003370);
    } else {
      if ((gFrameCount & 7) == 0)
        Event_PlaySound(246);
      anchor->y += 0x24000;
      spawn = 1;
    }
    break;
  }
  if (spawn) {
    /* FAKEMATCH(wrapper): the spawn forwarded through Value4; as a direct call,
     * 2 of 207 instructions differ in tla-en. */
    object = FieldActor_Create(0x2a0, anchor->x, anchor->y - ((Random16Far() * 80 >> 16) << 16) - 0x80000, anchor->z);
    if (object != NULL) {
      u8 zero;
      s32 mask;

      sprite = object->animation;
      Func_0200556c(object, gData_02006180);
      Actor_SetSpriteColor(object, 1);
      object->motionFlags = 0;
      {
        /* FAKEMATCH(temporary): the masked value held in a local; as
         * object->action = Random16Far() & 0x0ffff000, 36 of 207 instructions
         * differ in tla-en. */
        s32 action = Random16Far() & 0x0ffff000;

        object->action = action;
      }
      zero = 0;
      object->unk66 = 0;
      {
        /* FAKEMATCH(temporary): the byte held in a local; as a direct store, 2
         * of 207 instructions differ in tla-en. */
        u8 phase = Random16Far() >> 13;

        ((u8 *)object)[98] = phase;
      }
      object->update = Func_020000e4;
      object->speedLimit = Math_Sin(Random16Far() * 0xffff >> 20) * 24;
      object->speedLimit = anchor->speedLimit >> 16;
      sprite->unk1a = zero;
      mask = -13;
      ((u8 *)sprite)[9] = (((u8 *)sprite)[9] & mask) | 4;
    }
  }
  if (++Data_02006ccc > Data_02006b68)
    FieldTask_Remove(Func_02003370);
}

#include "mars_lighthouse_aerie.h"
#include "scene_tables.h"

void Func_020035ec(s32 id) {
  struct Actor *object = Actor_Get(id);

  FieldActor_SetSpritePriority(id, 1);
  FieldActor_SetPositionAndResetMotion(object, object->x - PIXELS(16), object->y,
                                  object->z + PIXELS(32));
}

void Func_0200361c(s32 id) {
  struct Actor *object = Actor_Get(id);

  FieldActor_SetSpritePriority(id, 1);
  FieldActor_SetPositionAndResetMotion(object, object->x + PIXELS(16), object->y,
                                  object->z + PIXELS(32));
}

void Func_02003648(s32 id) { Actor_GetById(id)->motionFlags = 3; }

#include "system/main_routine_work.h"
#include "ram_buffer.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

struct FieldActor;
void Animation_SetStateFlags(struct FieldActor *object, s32 flags);
void Func_02000610(s32 id);
void Func_02000640(s32 id);
s32 Func_0200557c();
void Func_02005734();
void Func_02005794();
s32 PartyInventory_Remove();
s32 HeapSlot_AllocFar();
s32 HeapSlot_Release();
void Func_0200326c(void);
extern u8 MsgTheBeaconIs[];
extern u8 gData_020060b4[];
extern u8 gData_020060fc[];
extern u8 gData_02006120[];
extern u8 gData_02006150[];

#define OWNER gMainRoutineWork.currentOwner

/// A halfword whose store may alias any read after it.
union StoredHalfword {
  u16 value;
  s32 word;
};

struct OverlayActor {
  u8 unk0[8];
  s32 x, y, z, terrainHeight, scaleX, scaleY;
  u8 unk20[3];
  u8 priorityFlags;
  u8 unk24[12];
  s32 speedLimit, acceleration;
  u8 unk38[16];
  s32 gravity;
  s32 unk4c;
  s32 animation;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[6];
  u8 unk5c;
  u8 unk5d[15];
  s32 update;
};
#define A(p) ((struct OverlayActor *)(p))

struct OverlaySprite {
  u8 unk0[5];
  u8 unk5_0 : 5;
  u8 unk5_5 : 1;
  u8 unk5_6 : 2;
  u8 unk6[3];
  u8 unk9_0 : 2;
  u8 unk9_2 : 2;
  u8 unk9_4 : 4;
  u8 unkA[6];
  u8 vramBlock;
  u8 unk11[10];
  u8 unk1b;
};

/// The heap slots read as words.
struct HeapWords {
  u8 unk00[108];
  s32 eventWord;
};

/// The event work's step counter.
struct AerieEventWork {
  u8 unk000[452];
  union {
    union StoredHalfword step;
    u16 stepRead;
  };
};

void Func_02003658(void) {
  struct AerieEventWork *work, *later;
  struct OverlaySprite *sprite;
  s32 created, zero, firstBeam, object, tiles, glowTask, message;
  s32 anchor, y, x, z, spawned, block;
  s32 beam, beam2, piece;
  s32 piece2, piece3;
  s32 owner, actor;

  FieldCamera_MoveTo(-1, -1, -1, 0);
  System_Wait(1);
  Actor_SetPosition(28, 0, 0);
  Actor_SetPosition(29, 0, 0);
  Actor_SetPosition(30, 0, 0);
  FieldCamera_SetSpeed(0x19999, 0x3333);
  A(((s32)FieldCamera_GetActor()))->motionFlags = 0;
  FieldCamera_MoveTo(0x2000000, 0x600000, 0x1380000, 1);
  Func_02005734();
  System_Wait(1);
  *(u16 *)0x4000052 = 0xc08;
  *(u16 *)0x4000050 = 0x3f10;
  MapGrid_CopyCellAttributes(18, 40, 18, 16, 23, 10);
  Func_02003648(7);
  Func_02003648(5);
  Func_02003648(6);
  firstBeam = 23;
  zero = 0;
  Func_02003648(19);
  Func_02003648(0);
  Func_02003648(3);
  Func_02003648(2);
  Func_02003648(1);
  Event_PrepareActorAndApplyValue(0, 0);
  Actor_SetSpeed((OWNER), 0xcccc, 0x6666);
  Actor_WalkToAndWait((OWNER), 0x200, 0x118);
  Actor_FaceDirectionAndWait((OWNER), 0xc000);
  Actor_RunRepeatedMotion((OWNER), 2);
  FieldEvent_Wait(20);
  PartyInventory_Remove(222);
  anchor = (s32)Actor_Get(4);
  y = A(anchor)->y;
  x = A(anchor)->x;
  y += 0x80000;
  z = A(anchor)->z;
  spawned = FieldActor_Create(0x1e9, x, y, z);
  created = spawned;
  object = spawned;
  if (created == 0)
    goto L_0200b8ac;
  sprite = (struct OverlaySprite *)A(object)->animation;
  sprite->unk5_5 = 0;
  sprite->unk1b = zero;
  sprite->unk9_4 = 0;
  sprite->unk9_2 = 2;
  A(object)->priorityFlags &= 254;
  A(object)->motionFlags = zero;
  A(object)->unk5c = 1;
  A(object)->priorityFlags |= 2;
  Actor_SetSingleChildFlags(created, 0);
  A(object)->speedLimit = 0x6666;
  A(object)->acceleration = 0xccc;
  A(object)->gravity = 0x8000;
  block = HeapSlot_AllocFar(68, 0x608);
  Item_LoadIcon(247);
  tiles = (block + 0x400);
  Vram_Load(sprite->vramBlock, 128, tiles);
  HeapSlot_Release(68);
  A(object)->motionFlags = zero;
  FieldActor_SetMoveTarget(object, 0x2000000, 0x800000, 0x1180000);
  Actor_WaitForMove(object);
  A(object)->motionFlags = 4;
  FieldEvent_Wait(60);
  A(object)->motionFlags = zero;
  FieldActor_SetMoveTarget(object, 0x2000000, 0x800000, 0x1080000);
  Actor_WaitForMove(object);
  FieldEvent_Wait(20);
  FieldActor_SetMoveTarget(object, 0x2000000, 0x500000, 0xfc0000);
  Actor_WaitForMove(object);
  FieldEvent_Wait(20);
  Func_0200557c(object);
L_0200b8ac:
  FieldEvent_Wait(40);
  FieldActor_SetSpritePriority(23, 2);
  FieldActor_SetSpritePriority(24, 2);
  FieldActor_SetSpritePriority(25, 2);
  FieldActor_SetSpritePriority(26, 2);
  FieldActor_SetSpritePriority(27, 2);
  Event_PlaySound(141);
  MapGrid_SetShake(0x10000, 0x10000, 0x10000);
  Actor_SetSpriteColor(((s32)Actor_Get(23)), 7);
  beam = (s32)Actor_Get(23);
  A(beam)->scaleX = 0x1999;
  A(beam)->scaleY = 0xffff0000;
  A(beam)->motionFlags = zero;
  FieldActor_SetPositionAndResetMotion(beam, 0x2000000, 0x400000, 0xcc0000);
  Actor_SetSpriteColor(((s32)Actor_Get(24)), 7);
  beam2 = (s32)Actor_Get(24);
  A(beam2)->scaleX = 0x1999;
  A(beam2)->scaleY = 0xffff0000;
  A(beam2)->motionFlags = zero;
  FieldActor_SetPositionAndResetMotion(beam2, 0x2000000, 0x600000, 0xcc0000);
  Task_AddFrame(Func_0200326c, 0x480);
  Actor_FaceDirection(7, 0xe000, 0);
  Actor_FaceDirection(5, 0xe000, 0);
  Actor_FaceDirection(6, 0xe000, 0);
  Actor_FaceDirection(19, 0xd000, 0);
  Actor_FaceDirection(0, 0xa000, 0);
  Actor_FaceDirection(3, 0xa000, 0);
  Actor_FaceDirection(1, 0xa000, 0);
  Actor_FaceDirection(1, 0xa000, 80);
  Actor_SetScript(23, gData_020060b4);
  Actor_SetScript(24, gData_020060b4);
  Event_PlaySound(145);
  MapGrid_SetShake(0x60000, 0x60000, 0x10000);
  PaletteFade_ApplyTarget(0x4063ff, 0);
  PaletteFade_Interpolate(16);
  System_Wait(20);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(24);
  System_Wait(60);
  Event_PlaySound(141);
  Event_SetFlag(0x200);
  A(((s32)Actor_Get(23)))->y = 0xffc80000;
  A(((s32)Actor_Get(24)))->y = 0xffe80000;
  Actor_SetSpriteColor(((s32)Actor_Get(25)), 7);
  piece = (s32)Actor_Get(25);
  A(piece)->scaleY = 0xffff0000;
  A(piece)->scaleX = (A(((s32)Actor_Get(23)))->scaleX);
  A(piece)->motionFlags = zero;
  FieldActor_SetPositionAndResetMotion(piece, 0x2000000, 0x80000, 0xcc0000);
  Actor_SetSpriteColor(((s32)Actor_Get(26)), 7);
  piece2 = (s32)Actor_Get(26);
  A(piece2)->scaleY = 0xffff0000;
  A(piece2)->scaleX = (A(((s32)Actor_Get(23)))->scaleX);
  A(piece2)->motionFlags = zero;
  FieldActor_SetPositionAndResetMotion(piece2, 0x2000000, 0x280000, 0xcc0000);
  Actor_SetSpriteColor(((s32)Actor_Get(27)), 7);
  piece3 = (s32)Actor_Get(27);
  A(piece3)->scaleY = 0xffff0000;
  A(piece3)->scaleX = (A(((s32)Actor_Get(23)))->scaleX);
  A(piece3)->motionFlags = zero;
  FieldActor_SetPositionAndResetMotion(piece3, 0x2000000, 0x480000, 0xcc0000);
  MapGrid_CopyCellsTo(106, 36, 87, 7, 17, firstBeam);
  MapGrid_CopyCellsTo(42, 98, 23, 69, 17, firstBeam);
  MapGrid_CopyCellAttributes(0, 40, 18, 20, firstBeam, 10);
  Func_020035ec(7);
  Func_020035ec(5);
  Func_020035ec(6);
  glowTask = (s32)Func_0200326c;
  Func_020035ec(20);
  Func_020035ec(21);
  Func_020035ec(19);
  Func_0200361c(2);
  Func_0200361c(1);
  Func_0200361c(22);
  Func_0200361c(0);
  Func_0200361c(3);
  owner = OWNER;
  {
    s32 speed = 0xe000;
    Actor_SetPositionAndFacing(owner, 0x1e40000, 0x1380000, speed);
  }
  FieldActor_SetSpritePriority((OWNER), 1);
  System_Wait(1);
  MapGrid_SetShake(0x40000, 0x40000, 0x10000);
  PaletteFade_ApplyTarget(0x4063ff, 0);
  PaletteFade_Interpolate(120);
  Actor_SetScript(23, gData_020060fc);
  Actor_SetScript(24, gData_020060fc);
  Actor_SetScript(25, gData_020060fc);
  Actor_SetScript(26, gData_020060fc);
  Actor_SetScript(27, gData_020060fc);
  System_Wait(120);
  MapGrid_SetShake(0x30000, 0x30000, 0x10000);
  PaletteFade_ApplyTarget(0x203210, 0);
  PaletteFade_Interpolate(120);
  System_Wait(120);
  MapGrid_SetShake(0x20000, 0x20000, 0x10000);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(120);
  System_Wait(120);
  MapGrid_SetShake(0x10000, 0x10000, 0x10000);
  Event_PlaySound(78);
  A(((s32)Actor_Get(23)))->scaleY = 0x51e;
  Actor_SetScript(24, gData_02006120);
  Actor_SetScript(25, gData_02006120);
  Actor_SetScript(26, gData_02006120);
  Actor_RunScriptAndWait(27, gData_02006120);
  Event_PlaySound(0x186);
  Actor_SetSpriteColor(((s32)Actor_Get(23)), 15);
  FieldEvent_Wait(20);
  Actor_RunScriptAndWait(23, gData_02006150);
  FieldTask_Remove(glowTask);
  actor = (s32)Actor_Get(8);
  A(actor)->motionFlags = zero;
  Animation_SetStateFlags(actor, 1);
  Actor_SetSpriteColor(((s32)Actor_Get(8)), 7);
  FieldActor_SetSpritePriority(8, 2);
  *(s32 *)(s32)&Data_02006ccc = zero;
  *(s32 *)(s32)&Data_02006b68 = 240;
  Task_AddFrame(Func_02003370, 0x480);
L_0200bcea:
  System_Wait(1);
  if ((u32)(*(s32 *)(s32)&Data_02006ccc) <= (u32)100)
    goto L_0200bcea;
  Event_PlaySound(72);
  FieldEvent_Wait(120);
  Actor_ShowEmote(3, 0x109, 40);
  FieldEvent_SetMessageId((s32)MsgTheBeaconIs);
  Event_ShowMessage(3, 0);
  FieldActor_ShowEmote(2, 0x105, 40);
  Event_ShowMessage(2, 0);
  Event_ShowMessage(6, 0);
  FieldActor_ShowEmote(7, 0x109, 20);
  Event_ShowMessage(7, 0);
  FieldActor_ShowEmote(1, 0x103, 40);
  Event_ShowMessage(1, 0);
  Actor_FaceDirection(7, 0, 0);
  Actor_FaceDirection((OWNER), 0, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirection(6, 0, 0);
  Actor_FaceDirection(19, 0, 0);
  Actor_FaceDirection(3, 0x8000, 0);
  Actor_FaceDirection(0, 0x8000, 0);
  Actor_FaceDirection(1, 0x8000, 0);
  Actor_FaceDirection(2, 0x8000, 20);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation((OWNER), 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(19, 3);
  Actor_SetAnimation(3, 3);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimation(1, 3);
  Actor_SetAnimationAndWait(2, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(7, 0x4000, 0);
  Actor_FaceDirection((OWNER), 0x4000, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirection(6, 0xc000, 0);
  Actor_FaceDirection(19, 0xb000, 0);
  Actor_FaceDirection(3, 0x4000, 0);
  Actor_FaceDirection(1, 0xe000, 0);
  Actor_FaceDirectionAndWait(0, 0);
  FieldEvent_ShowMessageThenWait(0x4005, 0, 10);
  MapGrid_SetShake(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  FieldPsynergy_StartMindReadWave(23, 24);
  Message_ShowCentered(((s32)MsgTheBeaconIs + 6), 1);
  Func_02005794();
  FieldEvent_Wait(20);
  FieldPsynergy_StartMindReadWave(23, 24);
  Message_ShowCentered(((s32)MsgTheBeaconIs + 7), 1);
  Func_02005794();
  work = Ram_HeapSlots->eventWork;
  work->step.value = work->stepRead + 2;
  Func_02000610((OWNER));
  Func_02000610(7);
  Func_02000610(5);
  Func_02000610(6);
  Func_02000610(19);
  Func_02000610(1);
  Func_02000610(2);
  Func_02000610(3);
  Actor_FaceDirection(0, 0xc000, 40);
  Actor_FaceDirection(0, 0x4000, 40);
  Actor_FaceDirectionAndWait(0, 0x4000);
  FieldEvent_ShowMessageThenWait(0x6000, 0, 10);
  FieldPsynergy_StartMindReadWave(23, 24);
  message = ((s32)MsgTheBeaconIs + 9);
  Message_ShowCentered(message, 1);
  Func_02005794();
  Func_02000640(3);
  Actor_FaceDirectionAndWait(0x6003, 0x4000);
  A(((s32)Actor_Get(3)))->update = 0;
  // Read as a word, so the store to the actor before it may alias.
  later =
      (struct AerieEventWork *)((struct HeapWords *)Ram_HeapSlots)->eventWord;
  later->stepRead = later->stepRead + 1;
  FieldActor_ShowEmote(3, 0x100, 20);
  FieldEvent_ShowMessageThenWait(3, 0, 20);
  Event_PlaySound(201);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(20);
  System_Wait(20);
  FieldEvent_SetExitRequest(3);
  return;
}

#undef A

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
s32 Func_0200558c();
void Func_020004a4(void);
extern u8 MsgWhatCouldHe[];
extern u8 gData_020061d4[];

static __inline__ void W_Func_02005714(s32 a0, s32 a1, s32 a2) {
  /* FAKEMATCH(wrapper): W_Func_02005714 forwarded inline; as direct calls to
   * Func_080d47b4, 195 of 1176 instructions differ in tla-en. */
  Func_080d47b4(a0, a1, a2);
}

void Func_02003ff4(void) {
  s32 owner, i, ownerInLoop, ownerAfterLoop, quarterTurn;
  s32 threeSixteenthsTurn, actor, t2, t3;
  s32 t4, t5, t6, t7, t8, t9, t10, t17, t21, t25;
  s32 t31, t49, t62, t76, t87, t101, t104, t107, t156;
  s32 t158, t160, t168, t184, t186, t189, t195, t205;
  s32 t215, t228, t238, t250, t263, t271, t274, t284, t292;
  s32 t296;

  Func_0200560c();
  FieldEvent_ResolvePendingActions(0);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;
    owner = (s32)&party->currentOwner;
  }
  quarterTurn = 0x4000;
  t2 = *(s32 *)owner;
  t3 = Actor_Get(t2);
  ((struct Actor *)t3)->angle = quarterTurn;
  t4 = Actor_Get(7);
  ((struct Actor *)t4)->angle = quarterTurn;
  t5 = Actor_Get(5);
  ((struct Actor *)t5)->angle = quarterTurn;
  t6 = Actor_Get(6);
  ((struct Actor *)t6)->angle = quarterTurn;
  t7 = Actor_Get(19);
  threeSixteenthsTurn = 0x3000;
  ((struct Actor *)t7)->angle = threeSixteenthsTurn;
  t8 = Actor_Get(0);
  ((struct Actor *)t8)->angle = quarterTurn;
  t9 = Actor_Get(1);
  ((struct Actor *)t9)->angle = quarterTurn;
  t10 = Actor_Get(2);
  ((struct Actor *)t10)->angle = quarterTurn;
  System_Wait(1);
  Actor_SetAnimation(20, 8);
  Actor_SetAnimation(21, 8);
  Actor_SetAnimation(22, 7);
  Actor_SetPosition(19, 0x1f20000, 0x1400000);
  Actor_SetPosition(1, 0x21a0000, 0x1400000);
  t17 = *(s32 *)owner;
  Actor_SetPosition(t17, 0x1fa0000, 0x1180000);
  System_Wait(1);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  t21 = FieldCamera_GetActor();
  ((struct Actor *)t21)->motionFlags = 0;
  FieldCamera_MoveTo(0x2000000, 0x600000, 0x1200000, 0);
  Func_0200558c();
  System_Wait(1);
  t25 = (s32)Ram_HeapSlots->eventWork;
  FIELD_AT(t25, s32, 428) = 0x201;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  Actor_FaceDirection(7, 0, 0);
  Actor_FaceDirection(6, 0, 0);
  t31 = *(s32 *)owner;
  Actor_FaceDirection(t31, 0, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirection(19, 0, 0);
  Actor_FaceDirection(0, 0x8000, 0);
  Actor_FaceDirection(3, 0x8000, 0);
  Actor_FaceDirection(1, 0x8000, 0);
  Actor_FaceDirection(2, 0x8000, 40);
  Actor_FaceDirectionAndWait(6, 0xe000);
  FieldEvent_SetMessageId((s32)MsgWhatCouldHe);
  Event_ShowMessage(6, 0);
  Actor_FaceDirection(3, 0x6000, 20);
  Actor_SetAnimationAndWait(3, 4);
  FieldActor_Jump(19, 4, 10);
  Event_ShowMessage(19, 0);
  Actor_FaceDirection(6, 0, 0);
  Actor_FaceDirection(5, 0x2000, 0);
  Actor_FaceDirection(7, quarterTurn, 0);
  t49 = *(s32 *)owner;
  Actor_FaceDirection(t49, quarterTurn, 0);
  Actor_FaceDirection(0, 0x6000, 0);
  Actor_FaceDirection(2, 0x6000, 0);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(0x2000000, -1, 0x1400000, 1);
  Func_02005734();
  Actor_FaceDirectionAndWait(19, 0xd000);
  Actor_SetAnimation(19, 3);
  Event_ShowMessage(19, 0);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  t62 = *(s32 *)owner;
  Actor_SetAnimation(t62, 3);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimation(3, 3);
  Actor_SetAnimation(2, 3);
  Actor_SetAnimationAndWait(1, 3);
  Actor_SetAttachedEffect(19, 0x102);
  FieldEvent_Wait(20);
  Event_ShowMessage(19, 0);
  Event_ShowMessage(7, 0);
  Actor_FaceDirectionAndWait(19, 0xb000);
  Actor_SetAnimationAndWait(19, 3);
  Actor_ShowEmote(1, 0x103, 20);
  Event_ShowMessage(1, 0);
  t76 = Actor_Get(28);
  Actor_SetSpriteColor(t76, 15);
  Actor_SetPosition(28, 0x2000000, 0x1680000);
  System_Wait(1);
  PaletteFade_ApplySource(0x10000, 0);
  FieldPsynergy_StartMindReadWave(23, 24);
  Event_ShowMessage(28, 0);
  Func_02005794();
  Actor_FaceDirection(2, quarterTurn, 0);
  FieldActor_Jump(2, 4, 10);
  Event_ShowMessage(2, 0);
  t87 = *(s32 *)owner;
  Actor_FaceDirection(t87, quarterTurn, 0);
  Actor_FaceDirection(7, quarterTurn, 0);
  Actor_FaceDirection(5, quarterTurn, 0);
  Actor_FaceDirection(6, quarterTurn, 0);
  Actor_FaceDirection(19, threeSixteenthsTurn, 0);
  Actor_FaceDirection(1, quarterTurn, 0);
  Actor_FaceDirection(3, quarterTurn, 0);
  Actor_FaceDirectionAndWait(0, quarterTurn);
  W_Func_02005714(0, 0x109, 20);
  Event_ShowMessage(0, 0);
  FieldPsynergy_StartMindReadWave(23, 24);
  Event_ShowMessage(28, 0);
  Func_02005794();
  t101 = *(s32 *)owner;
  Actor_FaceDirection(t101, 0, 0);
  Actor_FaceDirection(0, 0x8000, 20);
  t104 = *(s32 *)owner;
  Actor_SetAnimation(t104, 3);
  Actor_SetAnimationAndWait(0, 3);
  t107 = *(s32 *)owner;
  Actor_FaceDirection(t107, quarterTurn, 0);
  Actor_FaceDirectionAndWait(0, quarterTurn);
  FieldPsynergy_StartMindReadWave(23, 24);
  Event_ShowMessage(28, 0);
  Func_02005794();
  actor = Actor_Get(5);
  ((struct Actor *)actor)->angle = 0x6000;
  Actor_SetAnimation(5, 22);
  FieldEvent_Wait(10);
  FieldActor_StartRepeatedMotion(5, 2);
  Event_ShowMessage(5, 0);
  FieldPsynergy_StartMindReadWave(23, 24);
  Event_ShowMessage(28, 0);
  Func_02005794();
  ((struct Actor *)actor)->angle = quarterTurn;
  Actor_SetAnimation(5, 1);
  W_Func_02005714(5, 0x100, 20);
  Event_ShowMessage(5, 0);
  FieldPsynergy_StartMindReadWave(23, 24);
  Event_ShowMessage(28, 0);
  Func_02005794();
  Event_ShowMessage(1, 0);
  FieldPsynergy_StartMindReadWave(23, 24);
  Event_ShowMessage(28, 0);
  Func_02005794();
  W_Func_02005714(1, 0x100, 20);
  Event_ShowMessage(1, 0);
  FieldPsynergy_StartMindReadWave(23, 24);
  Event_ShowMessage(28, 0);
  Func_02005794();
  Actor_ShowEmote(19, 0x100, 20);
  Event_ShowMessage(19, 0);
  FieldPsynergy_StartMindReadWave(23, 24);
  Event_ShowMessage(28, 0);
  Func_02005794();
  FieldEvent_Wait(40);
  FieldCamera_MoveTo(0x2000000, -1, 0x1280000, 1);
  Actor_FaceDirection(19, 0x5000, 20);
  Actor_FaceDirection(19, threeSixteenthsTurn, 40);
  Actor_FaceDirectionAndWait(19, 0xd000);
  Event_ShowMessage(19, 0);
  Actor_FaceDirectionAndWait(5, 0x2000);
  Event_ShowMessage(0x4005, 0);
  Actor_FaceDirectionAndWait(0, 0x8000);
  Event_ShowMessage(0x8000, 0);
  Actor_FaceDirectionAndWait(7, 0);
  Event_ShowMessage(7, 0);
  Actor_FaceDirectionAndWait(2, 0x8000);
  Actor_ShowEmote(2, 0x109, 40);
  Event_OpenMessage(2, 0);
  t156 = *(s32 *)owner;
  Actor_FaceDirectionAndWait(t156, 0);
  t158 = *(s32 *)owner;
  FieldEvent_AskYesNoWithActorAnimation(t158, 0);
  t160 = *(s32 *)owner;
  Actor_WaitForAnimationChange(t160);
  PaletteFade_ApplyTarget(0x10004, 1);
  PaletteFade_Interpolate(40);
  Event_PlaySound(78);
  Event_PlaySound(141);
  MapGrid_SetShake(0x10000, 0x10000, 0x10000);
  FieldEvent_Wait(20);
  t168 = *(s32 *)owner;
  Func_02000610(t168);
  Func_02000610(5);
  Func_02000610(6);
  Func_02000610(7);
  Func_02000610(19);
  Func_02000610(0);
  Func_02000610(1);
  Func_02000610(2);
  Func_02000610(3);
  FieldEvent_Wait(20);
  FieldCamera_SetSpeed(0x19999, 0x3333);
  FieldCamera_MoveTo(0x2000000, -1, 0x1200000, 1);
  Func_02005734();
  FieldTask_Remove((s32)Func_020004a4);
  Event_PlaySound(140);
  t184 = Actor_Get(8);
  Actor_SetSpriteColor(t184, 12);
  t186 = Actor_Get(9);
  Actor_SetSpriteColor(t186, 12);
  FieldActor_SetSpritePriority(23, 2);
  t189 = Actor_Get(23);
  Actor_SetSpriteColor(t189, 7);
  actor = Actor_Get(23);
  ((struct Actor *)actor)->scaleY = 0xffff0000;
  ((struct Actor *)actor)->scaleX = 0x1999;
  ((struct Actor *)actor)->motionFlags = 0;
  ((struct Actor *)actor)->priorityFlags |= 2;
  FieldActor_SetPositionAndResetMotion(actor, 0x2000000, 0x400000, 0xcc0000);
  i = 0;
L_0200c66c:
  if (i != 20)
    goto L_0200c72a;
  MapGrid_SetShake(0x20000, 0x20000, 0x10000);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;
    ownerInLoop = (s32)&party->currentOwner;
  }
  t195 = *(s32 *)ownerInLoop;
  W_Func_02005714(t195, 0x100, 0);
  W_Func_02005714(5, 0x100, 0);
  W_Func_02005714(6, 0x100, 0);
  W_Func_02005714(7, 0x100, 0);
  W_Func_02005714(19, 0x100, 0);
  W_Func_02005714(0, 0x100, 0);
  W_Func_02005714(1, 0x100, 0);
  W_Func_02005714(2, 0x100, 0);
  Actor_ShowEmote(3, 0x100, 0);
  t205 = *(s32 *)ownerInLoop;
  Func_02000640(t205);
  Func_02000640(5);
  Func_02000640(6);
  Func_02000640(7);
  Func_02000640(19);
  Func_02000640(0);
  Func_02000640(1);
  Func_02000640(2);
  Func_02000640(3);
L_0200c72a:
  t215 = ((struct Actor *)actor)->scaleX;
  ((struct Actor *)actor)->scaleX = (t215 + 0x400);
  System_Wait(1);
  i = (i + 1);
  if ((u32)i <= (u32)39)
    goto L_0200c66c;
  Event_PlaySound(63);
  Actor_SetScript(23, (s32)gData_020061d4);
  FieldEvent_Wait(80);
  FieldActor_Jump(1, 4, 10);
  FieldActor_Jump(1, 6, 10);
  Actor_FaceDirectionAndWait(1, 0x8000);
  Event_ShowMessage(1, 0);
  Event_ShowMessage(7, 0);
  Actor_FaceDirectionAndWait(6, 0xa000);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(19, 0);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;
    ownerAfterLoop = (s32)&party->currentOwner;
  }
  t228 = *(s32 *)ownerAfterLoop;
  Actor_FaceDirection(t228, 0, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirection(6, 0, 0);
  Actor_FaceDirection(7, 0, 0);
  Actor_FaceDirection(19, 0, 0);
  Actor_FaceDirection(0, 0x8000, 0);
  Actor_FaceDirection(1, 0x8000, 0);
  Actor_FaceDirection(2, 0x8000, 0);
  Actor_FaceDirection(3, 0x8000, 40);
  t238 = *(s32 *)ownerAfterLoop;
  Actor_FaceDirection(t238, 0xc000, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(6, 0xc000, 0);
  Actor_FaceDirection(7, 0xc000, 0);
  Actor_FaceDirection(19, 0xc000, 0);
  Actor_FaceDirection(0, 0xc000, 0);
  Actor_FaceDirection(1, 0xc000, 0);
  Actor_FaceDirection(2, 0xc000, 0);
  Actor_FaceDirection(3, 0xc000, 20);
  Actor_FaceDirectionAndWait(3, 0x8000);
  Event_ShowMessage(3, 0);
  t250 = *(s32 *)ownerAfterLoop;
  Actor_FaceDirection(t250, 0, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirection(6, 0, 0);
  Actor_FaceDirection(7, 0, 0);
  Actor_FaceDirection(19, 0, 0);
  Actor_FaceDirection(0, 0x8000, 0);
  Actor_FaceDirection(1, 0x8000, 0);
  Actor_FaceDirection(2, 0x8000, 0);
  Actor_FaceDirection(3, 0x8000, 20);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  t263 = *(s32 *)ownerAfterLoop;
  Actor_SetAnimation(t263, 3);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimation(3, 3);
  Actor_SetAnimation(2, 3);
  Actor_SetAnimationAndWait(1, 3);
  FieldCamera_MoveTo(0x2000000, -1, 0x1380000, 1);
  Actor_SetPosition(8, 0, 0);
  t271 = Actor_Get(9);
  Actor_SetSpriteColor(t271, 7);
  FieldActor_SetSpritePriority(9, 1);
  t274 = Actor_Get(9);
  ((struct Actor *)t274)->priorityFlags &= 253;
  Event_PlaySound(140);
  MapGrid_SetShake(0x30000, 0x30000, 0x10000);
  PaletteFade_ApplyTarget(0x203210, 0);
  PaletteFade_Interpolate(40);
  Actor_FaceDirection(7, 0x4000, 0);
  Actor_FaceDirection(5, 0x2000, 0);
  Actor_FaceDirection(6, 0xe000, 0);
  Actor_FaceDirection(19, 0xb000, 0);
  t284 = *(s32 *)ownerAfterLoop;
  Actor_FaceDirection(t284, 0x4000, 0);
  Actor_FaceDirection(3, 0x4000, 0);
  Actor_FaceDirection(6, 0x8000, 0);
  Actor_FaceDirection(0, 0, 0);
  Actor_FaceDirection(1, 0xc000, 80);
  FieldActor_Jump(1, 4, 0);
  Actor_SetSpeed(1, 0x33333, 0x19999);
  t292 = Actor_Get(1);
  FIELD_AT(t292, u8, 90) &= 254;
  Actor_WalkToAndWait(1, 0x21e, 0x148);
  FieldEvent_Wait(1);
  t296 = Actor_Get(1);
  FIELD_AT(t296, u8, 90) |= 1;
  Event_ShowMessage(1, 0);
  Actor_FaceDirection(0, 0x4000, 0);
  Actor_FaceDirection(3, 0x2000, 0);
  Actor_FaceDirection(2, 0x4000, 40);
  Actor_FaceDirection(3, 0x6000, 0);
  Actor_FaceDirection(2, 0xa000, 0);
  Actor_FaceDirection(0, 0xe000, 20);
  Actor_ShowEmote(0, 0x103, 0);
  Actor_FaceDirectionAndWait(0, 0x4000);
  FieldActor_Jump(0, 4, 10);
  Event_ShowMessage(0x1000, 0);
  Actor_SetAnimation(1, 4);
  Event_ShowMessage(1, 0);
  Actor_FaceDirection(3, 0x4000, 0);
  Actor_ShowEmote(3, 0x100, 20);
  Event_ShowMessage(3, 0);
  Actor_SetAttachedEffect(7, 0x102);
  FieldEvent_Wait(20);
  Event_ShowMessage(7, 0);
  FieldActor_Jump(5, 4, 10);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(0x8002, 0);
  Actor_FaceDirectionAndWait(19, 0xd000);
  FieldActor_Jump(19, 2, 10);
  FieldActor_Jump(19, 4, 10);
  Event_ShowMessage(19, 0);
  Event_PlaySound(0x12a);
  Event_PlaySound(78);
  System_Wait(1);
  Event_PlaySound(140);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(20);
  System_Wait(240);
  *(u16 *)0x4000000 = 0;
  PaletteFade_ApplyTarget(0, 0);
  PaletteFade_Interpolate(8);
  System_Wait(8);
  FieldEvent_SetExitRequest(4);
  return;
}

extern const u8 MsgWillYouCast[];

void Func_02004bc4(void) {
  Func_0200560c();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceDirectionAndWait(19, FACING_SOUTH + FACING_STEP);
  FieldEvent_SetMessageId(MsgDoNotLet);
  Event_ShowMessage(0x8013, 0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 480, 348);
  MarsLighthouseAerie_ObjTurn(19, FACING_NORTHWEST + FACING_STEP);
  Func_02005614();
}

void Func_02004c2c(void) {
  Func_0200560c();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceDirectionAndWait(19, FACING_SOUTHEAST + FACING_STEP);
  FieldEvent_SetMessageId(MsgDoNotLet);
  Event_ShowMessage(0x8013, 0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 544, 348);
  MarsLighthouseAerie_ObjTurn(19, FACING_NORTHWEST + FACING_STEP);
  Func_02005614();
}

void Func_02004c94(void) {
  Func_0200560c();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetPosition(18, PIXELS(512), PIXELS(232));
  System_Wait(1);
  FieldEvent_SetMessageId(MsgWillYouCast);
  Event_OpenMessage(18, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_SetPosition(18, 0, 0);
    System_Wait(1);
    Func_02003658();
  }
  Actor_SetPosition(18, 0, 0);
  System_Wait(1);
  Event_SetFlag(0x202);
  Func_02005614();
}

#include "version.h"

void Func_02004d14(void) { GameFlag_ClearBit(0x202); }

// From patch 1, as is Func_02004d50.
#if TLA_PATCH_1
void Func_02004d24(s32 id) {
  u8 *work = Ram_HeapSlots->windowTiles;

  if (((struct MarsLighthouseAerieParty *)&gMainRoutineWork)->held == id)
    work[0x138a] = 1;
}
#endif

// From patch 1; the Japanese overlay has no such function.
#if TLA_PATCH_1

/// A halfword held in its own local: its load, the pool's mask and its store
/// keep their halfword forms.
struct HeldHalfword {
  u16 value;
};

void Func_02004d50(s32 id) {
  u8 *work = Ram_HeapSlots->windowTiles;
  struct MarsLighthouseAerieParty *party =
      (struct MarsLighthouseAerieParty *)&gMainRoutineWork;
  s32 held = party->held;
  /* FAKEMATCH(temporary): the word held in a halfword struct; as a plain store
   * of -1, 42 of 32 instructions differ in tla-en. */
  struct HeldHalfword raw = *(struct HeldHalfword *)&party->held;
  struct HeldHalfword ones;

  if (held == id) {
    s32 zero = 0;

    work[0x138a] = zero;
    ones.value = 0xffff;
    party->held = raw.value | ones.value;
    Actor_Get(held)->update = (void *)zero;
    Actor_SetSpriteColor(Actor_Get(held), 0);
  }
}

#endif

void Func_02004da0(void) {
  Func_0200560c();
  FieldEvent_ResolvePendingActions(0);
#if TLA_PATCH_1
  Func_02004d24(2);
#endif
  Actor_FaceActor(2, gMainRoutineWork.currentOwner, 0);
  FieldEvent_SetMessageId(MsgIUnderstandYour);
  Event_ShowMessage(2, 0);
#if TLA_PATCH_1
  Actor_FaceDirectionAndWait(2, FACING_WEST);
  Func_02004d50(2);
#else
  MarsLighthouseAerie_ObjTurn(2, FACING_WEST);
#endif
  Func_02005614();
}

void Func_02004df0(void) {
  Func_0200560c();
  FieldEvent_ResolvePendingActions(0);
#if TLA_PATCH_1
  Func_02004d24(20);
#endif
  FieldEvent_SetMessageId(MsgUhhhhnnnICan);
  Event_ShowMessage(20, 0);
#if TLA_PATCH_1
  Func_02004d50(20);
#endif
  Func_02005614();
}

void Func_02004e20(void) {
  Func_0200560c();
  FieldEvent_ResolvePendingActions(0);
#if TLA_PATCH_1
  Func_02004d24(22);
#endif
  FieldEvent_SetMessageId(MsgKoffKoffYou);
  Event_ShowMessage(22, 0);
#if TLA_PATCH_1
  Func_02004d50(22);
#endif
  Func_02005614();
}

void Func_02004e50(void) {
  Func_0200560c();
  FieldEvent_ResolvePendingActions(0);
#if TLA_PATCH_1
  Func_02004d24(21);
#endif
  FieldEvent_SetMessageId(MsgUnnnnIAm);
  Event_ShowMessage(21, 0);
#if TLA_PATCH_1
  Func_02004d50(21);
#endif
  Func_02005614();
}

// From patch 1; the Japanese overlay has no such function.
#if TLA_PATCH_1

void Func_02004e80(s32 from, s32 to) {
  struct Actor *found = Actor_Get(from);
  struct Actor *object;
  struct Actor *target;
  struct MainRoutineWork *party = &gMainRoutineWork;

  party->savedByte = 0;
  object = found;
  ((struct MarsLighthouseAerieParty *)party)->held = to;
  target = Actor_Get(to);
  target->update = object->update;
  object->update = NULL;
}
#endif

// These four, their rows, and Func_02004e80 are from patch 1.
#if TLA_PATCH_1

void Func_02004ec0(void) { Func_02004e80(28, 2); }

void Func_02004ecc(void) { Func_02004e80(17, 20); }

void Func_02004ed8(void) { Func_02004e80(30, 22); }

void Func_02004ee4(void) { Func_02004e80(29, 21); }
#endif

#include "dma.h"
#include "io_reg.h"

extern u16 *Data_02006ce4;
extern u16 *Data_02006ce8;
extern u16 *Data_02006cd4;

void Func_020051e0(void);

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
void Func_02004ef0(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  u8 *saved = Ram_HeapSlots->block32;

  gData_02006ce0 = slots;
  Data_02006ce4 = colours;
  Data_02006ce8 = a;
  Data_02006cd4 = b;
  if (*slots != 0xffff && *colours != 0xffff) {
  next:
    {
      int red = *colours++;
      int index = *slots++;
      int green = *colours++;
      int blue = *colours++;
      int value;

      /* FAKEMATCH(dead): the colour's build and store in a once-only
       * block, so both lists are stepped right after their loads and the
       * next entry's marker loads before 0xffff is built; as plain
       * statements, 22 of 80 instructions differ in tla-en. */
      do {
        value = (blue << 10) | (green << 5) | red;
        ((u16 *)BG_PLTT)[index] = value;
      } while (0);
    }
    if (*slots != 0xffff && *colours != 0xffff)
      goto next;
  }
  Dma_Set((const void *)BG_PLTT, saved, DMA_COPY_WORDS(112), REG_DMA3);
  Dma_Set((const void *)OBJ_PLTT, saved + 448, DMA_COPY_WORDS(112), REG_DMA3);
  PaletteFade_ApplyTarget(0x10000, 0);
  if ((s8)gMainRoutineWork.unk2c2 == 2)
    Func_020051e0();
}

#include "task.h"

void Func_02004fac(void) {
  gData_02006cf0 = 0;
  gData_02006cec = Func_0200508c(gData_02006ce0);
  gData_02006cdc = 15;
  gData_02006cd8 = 16;
  gData_02006cd0 = 1;
  Task_AddFrame(Func_020050b0, TASK_PASS_VBLANK);
}

void Func_02005010(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 256;
  FieldTask_Remove(Func_020050b0);
}

void Func_02005030(void) {
  gData_02006cf0 = 0;
  gData_02006cec = Func_0200508c(gData_02006ce0);
  gData_02006cdc = 2;
  gData_02006cd8 = 2;
  gData_02006cd0 = 1;
  Task_AddFrame(Func_020050b0, TASK_PASS_VBLANK);
}

s32 Func_0200508c(u16 *list) {
  s32 count = 0;

  while (*list != 0xffff) {
    list++;
    count++;
  }
  count--;
  return count;
}

void Func_020050b0(void) {
  s32 step;
  u32 i;
  u16 *src;
  volatile u16 *dst;
  int red, green, blue;

  step = 0;
  if (gData_02006cd8 != 0) {
    if (--gData_02006cd8 == 0) {
      step = 1;
      gData_02006cd8 = gData_02006cdc;
      if (gData_02006cdc > 2)
        gData_02006cdc--;
    }
  }
  if (step == 0)
    return;
  src = Data_02006ce8;
  for (i = 0; i < gData_02006cec; i++) {
    dst = &BG_PLTT[Data_02006cd4[gData_02006cf0 * gData_02006cec + i]];
    red = *src++;
    green = *src++;
    blue = *src++;
    /* FAKEMATCH(dead): the store in a once-only block, whose end holds the
     * order around it; as the plain statement, 2 of 120 instructions differ in
     * tla-en. */
    do {
      *dst = (blue << 10) | (green << 5) | red;
    } while (0);
  }
  dst = &BG_PLTT[gData_02006ce0[gData_02006cec]];
  if (gData_02006cd0 != 0)
    src = Data_02006ce4 + gData_02006cec * 3;
  else
    src = Data_02006ce8 + gData_02006cec * 3;
  red = *src++;
  green = *src++;
  /* FAKEMATCH(dead): the last read in a once-only block, whose end holds
   * the order around it; as the plain statement, 2 of 120 instructions differ
   * in tla-en. */
  do {
    blue = *src++;
  } while (0);
  *dst = (blue << 10) | (green << 5) | red;
  gData_02006cd0 ^= 1;
  if (++gData_02006cf0 == gData_02006cec)
    gData_02006cf0 = 0;
}

struct MarsLighthouseAerieEventWork {
  u8 unk000[0x1ac];
  s32 value1ac;
};

void Func_020057c4(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_0200575c_(s32 a, s32 b) {
  BattleEffect_ApplyColorToTargetBuffer(a, b);
}

void Func_020051e0(void) {
  struct MarsLighthouseAerieEventWork *work = Ram_HeapSlots->eventWork;
  s32 saved;

  saved = work->value1ac;
  PaletteFade_ApplyTarget(0x202108, 1);
  Func_0200575c_(0x10000, 2);
  ((struct MarsLighthouseAerieEventWork *)Ram_HeapSlots->eventWork)->value1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02005030();
  Func_020057c4();
  System_Wait(40);
  Func_02005010();
  PaletteFade_ApplyTarget(0x10000, 1);
  PaletteFade_Interpolate(16);
  System_Wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->unk2c2 = 0;
  }
  work->value1ac = saved;
}

#include "system/vector3.h"

struct SparkleSprite {
  u8 unk00[12];
  s32 unk0c;
  s32 unk10;
  s32 unk14;
  s32 unk18;
  u8 unk1c[2];
  u16 unk1e;
  u8 unk20[8];
};

struct SparkleWork {
  struct SparkleSprite sprites[4];
  s16 unka0[2];
  s16 vramEntry;
  s16 unka6;
  struct Actor *object;
};

struct CounterBits {
  u8 low : 1;
  u8 rest : 7;
};

void Animation_ApplyChildValues(struct Actor *object, s32 value);
s32 Resource_ResetEntry(u32 slot);
void Func_0200579c(struct Vector3 *v);
void Func_020057ac(struct SparkleSprite *sprite);
s32 Func_020057bc(struct Actor *object);

void Func_02005268(struct Actor *object) {
  struct Vector3 position;
  s32 x, z, sine;
  s32 i, j;
  struct SparkleWork *work = (struct SparkleWork *)object->watchedObject;
  u8 *count = (u8 *)object + 99;
  struct SparkleSprite *sprite;

  if (*count & 1)
    Animation_ApplyChildValues(
        object, (u8)((u32)((struct CounterBits *)count)->rest % 6));
  if (((u8 *)object)[98] == 0) {
    if ((*count & 15) == 0) {
      Event_PlaySound(0x256);
      work->unka0[work->unka6] = 0;
      work->unka6 ^= 1;
    }
  } else if (((u8 *)object)[98] == 1) {
    if (work->unka0[0] > 31 && work->unka0[1] > 31) {
      Animation_ApplyChildValues(object, 0);
      Resource_ResetEntry(work->vramEntry);
      object->update = 0;
      return;
    }
  }
  (*count)++;
  for (i = 0; i < 2; i++) {
    sine = Math_Sin(work->unka0[i] << 10);
    if (work->unka0[i] >= 0) {
      if (work->unka0[i] <= 31) {
        struct Vector3 *p = &position;

        p->x = object->x;
        p->y = object->y + (work->unka0[i] << 16);
        p->z = object->z;
        Func_0200579c(p);
        x = p->x;
        j = 0;
        z = p->z;
        sprite = &work->sprites[i * 2];
        for (; j <= 1; j++) {
          sprite->unk0c = x;
          sprite->unk10 = z;
          sprite->unk14 = sine;
          sprite->unk18 = sine;
          if (j == 0) {
            sprite->unk1e = Func_020057bc(object) - 1;
          } else {
            sprite->unk1e = Func_020057bc(object) + 1;
            sprite->unk10 -= 0x10000;
            sprite->unk18 = -sprite->unk18;
          }
          Func_020057ac(sprite);
          sprite++;
        }
        work->unka0[i]++;
      }
    }
  }
}
