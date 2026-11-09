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

void Func_0200072c(void) {
  if (GameFlag_Test(0x8cb) != 0)
    MapGrid_CopyCellsTo(72, 63, 36, 38, 1, 2);
  else
    MapGrid_CopyCellsTo(71, 63, 36, 38, 1, 2);
  GameFlag_SetBit(0x203);
}

void Func_02000778(void) {
  MapGrid_CopyCellsTo(70, 63, 36, 38, 1, 2);
  GameFlag_ClearBit(0x203);
}

void Func_020007a0(void) {
  if (GameFlag_Test(0x203) != 0 && Event_GetFlag(0x8cb) == 0) {
    Func_0200110c();
    Event_ResolvePendingActions(1);
    Sound_Play(161);
    Map_CopyCellPictureIndicesRect(72, 63, 36, 38, 1, 2);
    Event_Wait(40);
    Sound_Play(188);
    Map_CopyCellPictureIndicesRect(71, 66, 97, 39, 2, 2);
    wait(2);
    Map_CopyCellPictureIndicesRect(73, 66, 97, 39, 2, 2);
    wait(2);
    Map_CopyCellPictureIndicesRect(75, 66, 97, 39, 2, 2);
    MapGrid_CopyCellAttributes(33, 37, 2, 2, 33, 39);
    wait(1);
    GameFlag_SetBit(0x8cb);
    Func_02001114();
  } else {
    Func_0200110c();
    Event_ResolvePendingActions(0);
    FieldMessage_Show((s32)&MsgFieldEyesOfTruth, 1);
    Func_02001114();
  }
}
