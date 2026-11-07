#include "types.h"
#include "actor.h"
#include "call.h"
#include "flag_call.h"
#include "scene_ids.h"
#include "field/event_work.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"

extern u8 gData_02003d9e[];
extern u8 gData_02003d98[];
extern u8 gData_02003dba[];
extern u8 gData_02003db2[];
extern u8 gData_02003dac[];
extern u8 gData_02003dd2[];
extern u8 gData_02003dc4[];
extern u8 gData_02003de4[];
extern u8 gData_02003dd6[];
extern u8 gData_02003dee[];
extern u8 gData_02003de8[];
extern u8 gData_02003e02[];
struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
s32 Func_02000668();
void Func_02000ee4();
void Func_02003b0c();
void Func_02003c0c();
void Func_02003aa4();
s32 Func_020006cc();
void Func_02002518();
void Func_02002d08();
void Func_02003bec();
void Func_02003bf4();
void Func_0200244c();
void Func_020022ac();
void Func_02003be4();
void Func_02003bfc();
void Func_0200079c();
void Func_020024c8();
void Func_02003a94();

s32 Func_02002618(void) {
  struct MainRoutineWork *work;
  s32 scene;
  s32 *owner, *leader;
  s16 *entrance;
  struct Actor *actor;
  /* FAKEMATCH(temporary): one local for a table, a step and a size; as one
   * local each, 92 of 732 instructions differ in tla-en. */
  s32 other, rise;
  s32 offset;
  s32 value1, value2, value3, value4;
  u8 zero;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  work = &gMainRoutineWork;
  work->retreatScene = (s32)&SceneId_Ov691Room1;
  work->retreatEntrance = 1;
  scene = work->scene;
  zero = 0;
  if (scene == (s32)&SceneId_Ov691Room1) {
    value1 = 32;
    Actor_GetById(work->currentOwner)->priorityFlags |= value1;
    Actor_GetById(8)->priorityFlags |= value1;
    Actor_GetById(10)->priorityFlags |= value1;
    Func_02000668(gData_02003d9e);
    if (Flag_IsSet(0x109) == 0)
      Call3((void (*)())Func_02000ee4, 21, 13, 0);
    if (Flag_IsSet(0x8d5))
      Call3((void (*)())Func_02003b0c, 8, 0x1080000, 0x1680000);
    if (Flag_IsSet(0x8d6))
      Call3((void (*)())Func_02003b0c, 10, 0xf80000, 0x1980000);
    other = (s32)gData_02003d98;
    value2 = 78;
    Func_02003c0c(other);
    Call6((void (*)())Func_02003aa4, 98, 21, 4, 3, value2, 21);
    Call6((void (*)())Func_02003aa4, 98, 27, 4, 3, value2, 24);
    Func_020006cc(other);
    if (Flag_IsSet(0x109) == 0) {
      actor = Actor_GetById(20);
      actor->x += 0x10000;
      actor->motionFlags = zero;
      actor->collisionFlags = zero;
      actor->terrainHeight = actor->y += 0x180000;
      Actor_SetSingleChildFlags(actor, 0);
    }
    Call1((void (*)())Func_02002518, 20);
    switch (work->entrance) {
    case 3:
    case 4:
      offset = 0x100000;
      goto move;
    }
    goto settle;
  } else if (scene == (s32)&SceneId_Ov691Room2) {
    owner = &work->currentOwner;
    Actor_GetById(*owner)->priorityFlags |= 32;
    entrance = &work->entrance;
    if (work->entrance <= 3) {
      s32 id;

      Func_02002d08(gData_02003dba);
      Func_02000668(gData_02003db2);
      Func_02003bec();
      Call3((void (*)())Func_02003bf4, 0, 11, 12);
      if (Flag_IsSet(0x8d7))
        Call3((void (*)())Func_02003b0c, 15, 0x1b80000, 0x2180000);
      Func_02003c0c(gData_02003dac);
      id = *entrance;
      if (id == 1) {
        if (Flag_IsSet(0x109) == 0) {
          actor = Actor_GetById(*owner);
          actor->z += 0x100000;
        }
        Call1((void (*)())Func_0200244c, -0x100000);
      } else if (id == 2) {
        if (Flag_IsSet(0x109) == 0) {
          actor = Actor_GetById(*owner);
          actor->z += 0x100000;
        }
        Call1((void (*)())Func_0200244c, -0x600000);
      }
    } else {
      if (*entrance <= 8) {
        s32 id;

        if (Flag_IsSet(0x109) == 0) {
          Actor_GetById(8)->motionFlags = zero;
          rise = 0x500000;
          Actor_GetById(8)->y += rise;
          Actor_GetById(9)->motionFlags = zero;
          Actor_GetById(9)->y += rise;
          Call3((void (*)())Func_02000ee4, 19, 8, 0);
          Actor_GetById(10)->motionFlags = zero;
          rise = 0xa00000;
          Actor_GetById(10)->y += rise;
          Actor_GetById(11)->motionFlags = zero;
          Actor_GetById(11)->y += rise;
          Call3((void (*)())Func_02000ee4, 20, 10, 1);
          Actor_GetById(12)->motionFlags = zero;
          rise = 0xf00000;
          Actor_GetById(12)->y += rise;
          Actor_GetById(13)->motionFlags = zero;
          Actor_GetById(13)->y += rise;
          Call3((void (*)())Func_02000ee4, 21, 12, 0);
        }
        if (Flag_IsSet(0x8d8))
          Call3((void (*)())Func_02003b0c, 16, 0x3780000, 0x2380000);
        Func_02003c0c(gData_02003dd2);
        Func_02000668(gData_02003dc4);
        if (Flag_IsSet(0x109) == 0) {
          actor = Actor_GetById(15);
          other = 0x10000;
          actor->x += other;
          actor->motionFlags = zero;
          actor->collisionFlags = zero;
          rise = 0x180000;
          actor->terrainHeight = actor->y += rise;
          Actor_SetSingleChildFlags(actor, 0);
          actor = Actor_GetById(18);
          actor->x += other;
          actor->motionFlags = zero;
          actor->collisionFlags = zero;
          actor->terrainHeight = actor->y += rise;
          Actor_SetSingleChildFlags(actor, 0);
        }
        Call1((void (*)())Func_02002518, 15);
        Call1((void (*)())Func_02002518, 18);
        id = *entrance;
        if (id == 5) {
          Call1((void (*)())Func_0200244c, 0x1200000);
        } else if (id == 6) {
          if (Flag_IsSet(0x109) == 0)
            Func_020022ac();
        } else if (id == 7) {
          if (Flag_IsSet(0x109) == 0) {
            actor = Actor_GetById(*owner);
            actor->z += 0x100000;
          }
          Call1((void (*)())Func_0200244c, -0x300000);
        } else if (id == 8) {
          offset = 0x680000;
          goto move;
        }
      } else if (*entrance <= 12) {
        if (Flag_IsSet(0x8d9))
          Call3((void (*)())Func_02003b0c, 8, 0x1080000, 0x2480000);
        rise = (s32)gData_02003de4;
        Func_02003c0c(rise);
        Func_020006cc(rise);
        Func_02003be4();
        Call4((void (*)())Func_02003bfc, 0, 0x250, 14, 13);
        Func_02000668(gData_02003dd6);
        if (Flag_IsSet(0x109) == 0) {
          actor = Actor_GetById(10);
          other = 0x10000;
          actor->x += other;
          actor->motionFlags = zero;
          actor->collisionFlags = zero;
          rise = 0x180000;
          actor->terrainHeight = actor->y += rise;
          Actor_SetSingleChildFlags(actor, 0);
          actor = Actor_GetById(12);
          actor->x += other;
          actor->motionFlags = zero;
          actor->collisionFlags = zero;
          actor->terrainHeight = actor->y += rise;
          Actor_SetSingleChildFlags(actor, 0);
        }
        Call1((void (*)())Func_02002518, 10);
        Call1((void (*)())Func_02002518, 12);
        if (*entrance == 9 || *entrance == 10) {
          offset = 0x300000;
          goto move;
        }
      } else {
        Func_02000668(gData_02003dee);
        if (Flag_IsSet(0x109) == 0) {
          actor = Actor_GetById(13);
          actor->x += 0x10000;
          actor->motionFlags = zero;
          actor->collisionFlags = zero;
          actor->terrainHeight = actor->y += 0x180000;
          Actor_SetSingleChildFlags(actor, 0);
        }
        Call1((void (*)())Func_02002518, 13);
        if (Flag_IsSet(0x8da))
          Call3((void (*)())Func_02003b0c, 8, 0x2880000, 0x2580000);
        if (Flag_IsSet(0x8db))
          Call3((void (*)())Func_02003b0c, 9, 0x2380000, 0x2680000);
        Func_0200079c(gData_02003de8);
        if (Flag_IsSet(0x250))
          Call6((void (*)())Func_02003aa4, 121, 48, 1, 3, 123, 48);
        if (Flag_IsSet(0x251))
          Call6((void (*)())Func_02003aa4, 121, 48, 1, 3, 125, 48);
        if (Flag_IsSet(0x252))
          Call6((void (*)())Func_02003aa4, 108, 56, 1, 3, 109, 56);
        if (*entrance == 14) {
          Call1((void (*)())Func_0200244c, 0x300000);
        } else if (*entrance == 16 || *entrance == 17) {
          if (Flag_IsSet(0x109) == 0) {
            actor = Actor_GetById(work->currentOwner);
            actor->z += 0x100000;
          }
          offset = -0x200000;
        move:
          Func_0200244c(offset);
        }
      }
    settle:
      Func_020024c8();
    }
  } else if (scene == (s32)&SceneId_Ov691Room3) {
    if (Flag_IsSet(0x8dc)) {
      Call3((void (*)())Func_02003b0c, 8, 0, 0);
      value3 = 42;
      Call6((void (*)())Func_02003aa4, 0, 32, 5, 8, value3, 17);
      Call6((void (*)())Func_02003aa4, 0, 96, 5, 8, value3, 81);
      value4 = 5;
      other = 8;
      Call6((void (*)())Func_02003a94, 0, 32, 42, 17, value4, other);
      Call6((void (*)())Func_02003a94, 0, 96, 42, 81, value4, other);
    }
    leader = &work->currentOwner;
    Actor_GetById(*leader)->priorityFlags |= 32;
    Func_02000668(gData_02003e02);
    if (work->entrance <= 2) {
      if (Flag_IsSet(0x109) == 0) {
        actor = Actor_GetById(*leader);
        actor->z += 0x100000;
      }
      Call1((void (*)())Func_0200244c, -0x200000);
    }
  }
  return 0;
}
