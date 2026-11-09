#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

struct EventWork154 {
  u8 unk000[0x154];
  s16 returnEntrance;
};

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 flag;
};

struct AnimationRecord18 {
  u8 unk00[0x12];
  u16 frame;
};

extern u8 MsgFieldEyesOfTruth;
extern const u8 gData_020013c0[];
extern const u8 gData_020013c4[];
extern const u8 gData_020013ca[];
extern const u8 gData_0200121c[];

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200110c(void);
void Func_02001114(void);
void Func_0200113c(s32 id, s32 value);
void Func_0200115c(s32 id);
s32 Func_02001184(s32 owner);
void Func_020011b4(void);
void Func_020011bc(s32 a, s32 b, s32 c, s32 d);
void Func_020011c4(const void *data);
void Func_020011dc(const void *data);
s32 Func_020011fc(void);
void Func_02001204(void);
void Func_02000038(const void *data);
void Func_0200014c(const void *data);
void Func_02000270(struct Actor *object);
void Func_020008d0(struct Actor *object);
void Func_02000958(void);
void Func_02000a28(void);
void Func_02000c94(void);
void Func_02000cf4(void);

void Func_02000874(void) {
  struct Actor *object;
  s32 x;

  if (Event_GetFlag(0x8cc) == 0) {
    object = Actor_GetById(64);
    x = object->x;
    if (x < (186 << 16)) {
      Func_0200110c();
      Event_ResolvePendingActions(0);
      Sound_Play(204);
      object->motionFlags = 3;
      wait(10);
      Actor_SetHorizontalPositionWithTerrain(64, 0, 0);
      Event_SetFlag(0x8cc);
      Func_02001114();
    }
  }
}
