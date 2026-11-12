#include "local.h"
#include "io_reg.h"
#include "call.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

extern char SceneId_Ov692Room1;
extern const u8 gData_02005074[];
extern const u8 gData_020050d4[];
extern const u8 gData_02005134[];
extern const u8 gData_0200519c[];
extern const u8 gData_020051fc[];
extern const u8 gData_0200525c[];
extern const u8 gData_020052bc[];
extern const u8 gData_0200531c[];
extern const u8 gData_0200537c[];
extern const u8 gData_020053dc[];
extern const u8 gData_0200543c[];
extern const u8 gData_0200549c[];
extern const u8 gData_020054fc[];
extern u16 gData_02005500[];
extern const u8 gData_02005508[];
extern u16 gData_0200550c[];
extern const u8 gData_02005520[];
extern const u8 gData_02005528[];
extern const u8 gData_0200552e[];

void Func_02000800(s32 id, const u8 *script, s32 flag);
void Func_02004e60(s32 value);
void Func_02004ed8(const u8 *table);
void Func_02000880(u16 *table);
void Func_02004ec0(void);
void Func_02004ec8(s32 a, s32 b, s32 c);
void Func_02004f08(s32 id, s32 value);
s32 Func_02000694(u32 layer, s32 x, s32 y, s32 value);
void Actor_SetActionCallback(struct Actor *object, s32 value);
void Func_020034bc(void);
void Func_020034f0(void);
void Func_02003548(void);
void Func_02003630(void);

#define CLOSED_BLEND()                                                         \
  {                                                                            \
    REG_BLDALPHA = 0x1000;                                                     \
    REG_BLDCNT = 0x3f44;                                                       \
  }

static __inline__ void Raise(s32 id) {
  struct Actor *object = Actor_GetById(id);

  object->motionFlags = 2;
  object->y = object->terrainHeight = 20 << 16;
  object->priorityFlags |= 128;
}

static __inline__ void Lower(s32 id, s32 height) {
  Actor_GetById(id)->motionFlags = 0;
  Actor_GetById(id)->y = Actor_GetById(id)->terrainHeight = height;
}

s32 Func_02003678(void) {
  s32 i;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 516;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room1) {
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
    Func_02000800(8, gData_02005074, 768);
    if (!GameFlag_Test(768)) {
      CLOSED_BLEND();
      Map_CopyCellPictureIndicesRect(71, 64, 47, 98, 1, 1);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room2) {
    s32 j;

    Func_02004e60(0);
    Actor_GetById(15)->update = Func_02003630;
    j = 0;
    do {
      {
        struct Actor *object = Actor_GetById(j + 9);

        object->motionFlags = 0;
        object->y = object->terrainHeight = 0;
      }
    } while (++j != 6);
    if (GameFlag_Test(2544))
      Actor_GetById(15)->z = 132 << 17;
    Func_02004ed8(gData_020054fc);
    Func_02000880(gData_02005500);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
    Raise(16);
    if (GameFlag_Test(768))
      Func_020034bc();
    else
      CLOSED_BLEND();
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room3) {
    Func_02004e60(0);
    if (GameFlag_Test(2545))
      Actor_GetById(11)->z = 166 << 18;
    Func_02004ed8(gData_02005508);
    Func_02004ec0();
    Func_02004ec8(0, 13, 14);
    Func_02000880(gData_02005500);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
    Raise(9);
    Func_02000800(10, gData_020050d4, 769);
    Func_02000800(12, gData_02005134, 769);
    if (GameFlag_Test(769))
      Func_020034f0();
    else
      CLOSED_BLEND();
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room4) {
    Func_02004e60(0);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
    Func_02000880(gData_0200550c);
    if (gMainRoutineWork.entrance <= 7) {
      Func_02000800(10, gData_0200519c, 769);
      Func_02000800(12, gData_020051fc, 769);
      if (GameFlag_Test(769)) {
        Func_020034f0();
      } else {
        CLOSED_BLEND();
        Map_CopyCellPictureIndicesRect(71, 64, 35, 82, 1, 1);
      }
    } else {
      Actor_GetById(23)->priorityFlags |= 32;
      Actor_GetById(24)->priorityFlags |= 32;
      Actor_GetById(25)->priorityFlags |= 32;
      Actor_SetSpritePriority(23, 3);
      Actor_SetSpritePriority(24, 3);
      Actor_SetSpritePriority(25, 3);
      if (GameFlag_Test(2546))
        Actor_GetById(23)->x = 172 << 17;
      if (GameFlag_Test(2547))
        Actor_GetById(24)->x = 172 << 17;
      if (GameFlag_Test(2548))
        Actor_GetById(25)->x = 172 << 17;
      Func_02004ed8(gData_02005520);
      Lower(26, 0);
      Actor_GetById(15)->unk5c[0] = 3;
      Actor_GetById(16)->unk5c[0] = 3;
      Actor_GetById(17)->unk5c[0] = 3;
      Actor_GetById(18)->unk5c[0] = 3;
      Actor_GetById(19)->unk5c[0] = 3;
      Actor_GetById(20)->unk5c[0] = 3;
      Actor_GetById(21)->unk5c[0] = 3;
      Actor_GetById(22)->unk5c[0] = 3;
      Func_02000800(15, gData_0200525c, 770);
      Func_02000800(16, gData_0200525c, 770);
      Func_02000800(17, gData_020052bc, 770);
      Func_02000800(18, gData_020052bc, 770);
      Func_02000800(19, gData_0200531c, 770);
      Func_02000800(20, gData_0200531c, 770);
      Func_02000800(21, gData_0200537c, 770);
      Func_02000800(22, gData_0200537c, 770);
      if (GameFlag_Test(770))
        Func_02003548();
      else
        CLOSED_BLEND();
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room5) {
    Func_02004e60(0);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
    Func_02004f08(10, 1);
    Func_02004f08(11, 1);
    Raise(8);
    Raise(9);
    if (GameFlag_Test(770)) {
      Func_02000694(0, 22, 26, 0);
      Func_02000694(0, 25, 26, 0);
      Actor_SetSpritePriority(64, 3);
    } else {
      CLOSED_BLEND();
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room6) {
    Func_02004e60(0);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
    if (Event_GetFlag(2549)) {
      MapGrid_CopyCellAttributes(46, 74, 1, 1, 46, 73);
      Actor_GetById(9)->x = 190 << 18;
    }
    if (Event_GetFlag(2550))
      Actor_SetPosition(12, 230 << 18, 154 << 18);
    Func_02004ed8(gData_02005528);
    Func_02000800(8, gData_020053dc, 770);
    Func_02000800(10, gData_0200543c, 770);
    Func_02000800(11, gData_0200549c, 770);
    if (GameFlag_Test(770)) {
      Actor_SetModeById(9, 4);
      if (Event_GetFlag(2550))
        Actor_SetModeById(12, 4);
      if (Event_GetFlag(2549))
        Func_02000694(2, 46, 8, 0xff00);
      Func_02000694(0, 10, 45, 0);
      Func_02000694(0, 53, 40, 0);
    } else {
      CLOSED_BLEND();
      Map_CopyCellPictureIndicesRect(71, 64, 50, 102, 1, 1);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room7) {
    i = 0;
  room7:
    {
      struct Actor *object = Actor_GetById(i + 8);

      Func_02004c98(object, 4);
      object->motionFlags = 0;
      object->y = object->terrainHeight = -0x200000;
    }
    if (++i != 3)
      goto room7;
    if (GameFlag_Test(2551)) {
      Actor_SetModeById(23, 4);
      Actor_SetPosition(23, 174 << 18, 162 << 18);
    }
    Func_02004ed8(gData_0200552e);
    Func_02004e60(0);
    if (!Event_GetFlag(771))
      Actor_SetPosition(21, 0, 0);
    if (!Event_GetFlag(772))
      Actor_SetPosition(22, 0, 0);
    if (Event_GetFlag(771) &&
        Event_GetFlag(772)) {
      Lower(11, -0x200000);
      Actor_SetModeById(11, 4);
    } else {
      Func_02000694(0, 38, 18, 0xff00);
    }
    if (GameFlag_Test(2553))
      Actor_SetModeById(12, 0);
    for (i = 0; i != 4; i++)
      Actor_SetModeById(i + 13, 2);
    for (i = 0; i != 11; i++)
      Actor_SetSingleChildFlags(Actor_GetById(i + 12), 0);
    if (!GameFlag_Test(2552)) {
      for (i = 0; i != 9; i++)
        Actor_SetPosition(i + 12, 0, 0);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room8) {
    Func_0200255c();
    Event_SetFlag(324);
    i = 0;
  room8:
    if (GameFlag_Test(i + 514)) {
      s32 y = 24 - i * 2;

      MapGrid_CopyCellsTo(65, 1, 80, y, 1, 2);
      Call4((void (*)())Func_02000694, 0, 16, y, 0xff00);
    }
    if (++i != 6)
      goto room8;
    Actor_SetSingleChildFlags(Actor_GetById(10), 0);
    Actor_SetSingleChildFlags(Actor_GetById(11), 0);
    if (GameFlag_Test(773)) {
      Lower(9, -0x200000);
      Actor_SetModeById(9, 4);
    } else {
      Func_02000694(0, 7, 21, 0xff00);
      Actor_SetPosition(11, 0, 0);
    }
    Lower(8, 0x100000);
    Actor_SetActionCallback(Actor_GetById(8), 9);
  }
  return 0;
}
