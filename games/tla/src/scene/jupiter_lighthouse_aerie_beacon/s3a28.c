/// @file
/// Scene code of overlay 0x6a5, in ROM order, from s3a28's offset up to the next object the overlay links.

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_work.h"
#include "event_inline.h"
#include "call.h"
#include "text/msg_ids.h"

/// Writes to out the ids of objects 0 to 63 standing in columns 4 to 8 and in
/// the three rows from 72 less the row of z. Nothing in this overlay calls it.
// As ☀️'s MercuryLighthouseAerie_CollectBandSlots does.
void Func_02003a28(s32 *out, s32 zArg) {
  s32 z = zArg;
  struct EventWork *work = Ram_HeapSlots->eventWork;
  u32 slot;

  z = 64 - (z >> 20);
  for (slot = 0; slot <= 63; slot++) {
    struct Actor *object = work->objects[slot];

    if (object != NULL) {
      s32 column = object->x >> 20;
      s32 row = object->z >> 20;

      if (column >= 4 && column <= 8 && z + 8 <= row && row < z + 11)
        *out++ = slot;
    }
  }
}

struct SceneActor {
  u8 pad0[16];
  s32 unk_10;
  u8 pad14[35 - 20];
  u8 unk_23;
  u8 pad24[64 - 36];
  s32 unk_40;
  u8 pad44[85 - 68];
  u8 unk_55;
};
struct SceneWork {
  u8 pad0[12];
  s32 unk_0c;
};
struct FieldWork {
  u8 pad0[0x178];
  struct SceneWork unk_178;
};
struct Iwram {
  u8 pad0[32];
  struct FieldWork *field;
};
extern s32 Data_02006a1c;
extern s32 gData_020059fc[];
extern s32 gData_02005a0c[];
extern s32 gData_02005a14[];
extern s32 gData_02005a1c[];
extern s32 gData_020069dc[];

void Func_02003a6c(s32 mode) {
  s32 speed;
  u32 count;
  s32 *list;
  s32 *p;
  s32 *q;
  u32 i;
  u32 j;
  struct SceneWork *work;

  work = &((struct Iwram *)Ram_HeapSlots)->field->unk_178;
  speed = 0x1999;
  count = 0;
  Data_02006a1c = work->unk_0c;
  switch (mode) {
  case 0:
  case 3:
    list = gData_020059fc;
    break;
  case 1:
    list = gData_02005a0c;
    break;
  case 2:
    list = gData_02005a14;
    break;
  default:
    list = gData_02005a1c;
    break;
  }

  for (i = 0; i <= 4 && list[i] != 64; i++) {
    ((struct SceneActor *)Actor_Get(list[i]))->unk_55 = 0;
    FieldActor_SetSpritePriority(list[i], 1);
    count++;
  }

  for (i = 0; i <= 5; i++) {
    ((struct SceneActor *)Actor_Get(gData_020069dc[i]))->unk_23 |= 32;
    FieldActor_SetSpritePriority(gData_020069dc[i], 0);
  }

  Event_PlaySound(223);

  for (i = 0; i <= 227; i++) {
    work->unk_0c -= speed;
    for (j = 0; j < count; j++) {
      struct SceneActor *actor;

      ((struct SceneActor *)Actor_Get(list[j]))->unk_10 += speed;
      actor = ((struct SceneActor *)Actor_Get(list[j]));
      actor->unk_40 = ((struct SceneActor *)Actor_Get(list[j]))->unk_10;
    }
    if ((i & 3) == 3)
      speed += 0x1999;
    if (speed > 0x17fff)
      speed = 0x18000;
    System_Wait(1);
    if (mode == 0) {
      if (i == 40)
        FieldCamera_MoveTo(128 << 18, 208 << 15, 152 << 17, 1);
      if (i == 120)
        break;
    } else if (mode == 1) {
      if (i == 40)
        FieldCamera_MoveTo(128 << 18, 208 << 15, 176 << 17, 1);
      if (i == 120)
        break;
    } else if (mode == 3) {
      if (i == 40)
        FieldCamera_FollowActor(0, 1);
      if (i == 120)
        break;
    }
  }

  for (i = 0; i <= 5; i++) {
    ((struct SceneActor *)Actor_Get(gData_020069dc[i]))->unk_23 &= 223;
    FieldActor_SetSpritePriority(gData_020069dc[i], 1);
  }

  for (i = 0; i < count; i++) {
    Actor_SetPosition(list[i], 0, 0);
    ((struct SceneActor *)Actor_Get(list[i]))->unk_55 = 0;
  }
}

struct HeapSlotWords {
  s32 unk00[8];
  u8 *map;
};

struct ScrollWork {
  s32 unk00[3];
  s32 height;
};

extern s32 Data_02006a1c;
extern s32 gData_02005a20[];

void Func_0200577c(void);

void Func_02003c58(s32 mode) {
  struct ScrollWork *work = (struct ScrollWork *)(((struct HeapSlotWords *)Ram_HeapSlots)->map + 376);
  s32 speed = 0x18000;
  u32 count = 0;
  u32 i;
  s32 *ids = gData_02005a20;

  for (i = 0; i <= 4 && ids[i] != 64; i++) {
    Actor_Get(ids[i])->motionFlags = 0;
    count++;
  }
  if (mode != 0) {
    Event_PlaySound(223);
    i = 0;
    work->height += 0x18000;
    while (work->height < Data_02006a1c) {
      u32 j;

      for (j = 0; j < count; j++) {
        struct Actor *actor;

        Actor_Get(ids[j])->z -= speed;
        actor = Actor_Get(ids[j]);
        actor->targetZ = Actor_Get(ids[j])->z;
      }
      if ((i & 3) == 3 && Data_02006a1c < speed * 15 * 2 + work->height)
        speed = speed * 15 / 16;
      if (speed <= 0xccb)
        speed = 0xccc;
      if (mode == 1 && i == 40)
        FieldCamera_MoveTo(0x1200000, 0x680000, 0x1700000, 1);
      System_Wait(1);
      i++;
      if (i > 119)
        break;
      work->height += speed;
    }
  }
  work->height = Data_02006a1c;
  Func_0200577c();
  System_Wait(2);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_020059b4();
s32 Func_02004644();
s32 Actor_SetSingleChildFlags();
s32 Func_02000cd0();
s32 Party_AddActiveOwner();
s32 Func_02001388();
s32 Func_020016e8();
s32 Func_02002ae4();
s32 Func_02001bc4();
extern u8 gMainRoutineWork[];
extern u8 Func_020005b8[];

s32 Func_02003d9c(void) {
  s32 t11;
  s32 t2;
  s32 t3;
  s32 t4;
  s32 t5;
  s32 t6;
  s32 t7;
  s32 t8;
  s32 t9;
  s32 t25;

  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1ac), s32, 0) = 0x201;
  Call2(Func_020059b4, 0xf, 1);
  Func_020059b4(0x10, 1);
  t2 = Actor_Get(FIELD_AT(gMainRoutineWork, s32, 532));
  FIELD_AT((t2 + 0x23), u8, 0) = (0x40 | FIELD_AT((t2 + 0x23), u8, 0));
  t3 = Actor_Get(17);
  FIELD_AT((t3 + 0x23), u8, 0) = (FIELD_AT((t3 + 0x23), u8, 0) | 0x80);
  t4 = Actor_Get(18);
  FIELD_AT((t4 + 0x23), u8, 0) = (FIELD_AT((t4 + 0x23), u8, 0) | 0x80);
  t5 = Actor_Get(19);
  FIELD_AT((t5 + 0x23), u8, 0) = (FIELD_AT((t5 + 0x23), u8, 0) | 0x80);
  t6 = Actor_Get(20);
  FIELD_AT((t6 + 0x23), u8, 0) = (FIELD_AT((t6 + 0x23), u8, 0) | 0x80);
  t7 = Actor_Get(21);
  FIELD_AT((t7 + 0x23), u8, 0) = (FIELD_AT((t7 + 0x23), u8, 0) | 0x80);
  t8 = Actor_Get(22);
  FIELD_AT((t8 + 0x23), u8, 0) = (0x80 | FIELD_AT((t8 + 0x23), u8, 0));
  t9 = Event_GetFlag(0xa21);
  if (t9 == 0) {
    Func_02004644();
  } else {
    t11 = Actor_Get(24);
    FIELD_AT(t11, s32, 24) = 0;
  }
  if ((u32)((FIELD_AT(gMainRoutineWork, u16, 482) - 1) << 16) > 0x10000) {
    Actor_SetPosition(24, 0, 0);
  }
  Actor_SetSingleChildFlags(Actor_Get(36), 0);
  Actor_SetSingleChildFlags(Actor_Get(36), 0);
  Task_AddFrame((s32)Func_020005b8, 0x480);
  if (FIELD_AT(gMainRoutineWork, s16, 482) == 9) {
    Func_02000cd0();

    FIELD_AT(gMainRoutineWork, u16, 608) = (s32)MsgRobe;
    FIELD_AT(gMainRoutineWork, u16, 610) = 1;
  } else {
    if (FIELD_AT(gMainRoutineWork, s16, 482) == 0xa) {
      Party_AddActiveOwner(5);
      Func_02001388();
    } else {
      if (FIELD_AT(gMainRoutineWork, s16, 482) == 0xb) {
        Party_AddActiveOwner(6);
        Func_020016e8();
      } else {
        if (FIELD_AT(gMainRoutineWork, s16, 482) == 0xc) {
          Func_02002ae4();
        } else {
          if (FIELD_AT(gMainRoutineWork, s16, 482) == 0xd) {
            Func_02001bc4();
          } else {
            t25 = Event_GetFlag(0xa21);
            if (t25 != 0) {
              Actor_SetPosition(27, 0, 0);
              Actor_SetPosition(28, 0, 0);
            }
          }
        }
      }
    }
  }
  return 0;
}
