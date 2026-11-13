#ifndef TLA_ISLETS_H
#define TLA_ISLETS_H

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "map.h"
#include "scene_tables.h"
#include "input.h"
#include "text/msg_ids.h"
#include "frame.h"
#include "system/vector3.h"

struct IsletsEventWork {
  u8 unk000[0x154];
  s16 exit;
  u8 unk156[4];
  s16 field15a;
  u8 unk15c[0x50];
  s32 field1ac;
};

void Actor_SelectAnimation(struct Actor *object, s32 value);
void Actor_StartScript(struct Actor *object, const void *script);
void Func_020034ec(void);

void Func_02001cac(void);
void Func_02001cfc(void);

extern const u8 gSmokeScript[];
extern struct Trigger gData_02004b14[];
extern struct Trigger gData_02004b20[];
extern struct Trigger gData_02004bb0[];
extern struct Trigger gData_02004ca0[];
extern struct Trigger gData_02004d84[];
extern struct Trigger gData_02004e5c[];
extern u16 gSceneState[];
extern char SceneId_Ov674Room1;
extern char SceneId_Ov674Room2;
extern char SceneId_Ov674Room3;
extern char SceneId_Ov674Room4;
extern char SceneId_Ov674Room5;

void Func_02002c38(void);
void Func_02002f1c(void);
void Func_020030b4(void);
void Func_02003164(void);
void Func_0200329c(void);

struct IsletsMapFlags {
  u8 unk00[0x17];
  u8 unused : 2;
  u8 mode : 2;
};

struct IsletsExit {
  s32 value;
  struct {
    u16 first;
    u16 second;
  } pair;
};

struct PushTarget {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 footprint; ///< index into the footprint table
  s32 id;
  void (*callback)(void);
};

extern struct ActorPlacement gData_02004754[];
extern struct ActorPlacement gData_0200476c[];
extern struct ActorPlacement gData_020047e4[];
extern struct ActorPlacement gData_0200494c[];
extern struct ActorPlacement gData_020049c4[];
extern struct ActorPlacement gData_02004a84[];
extern s32 gData_02005190;
extern s32 gData_02005194;
extern s32 gData_02005198;
extern s32 gData_0200519c;
extern const u8 gData_02004028[];
extern const struct IsletsExit gData_020046bc[];
extern u16 gData_02004070[];

void Func_020020ec(s32 x, s32 z, s32 layers, s32 value);
void Func_020021d0(void);
void Func_02002bd8(void);
void Leader_PushObjectAhead(void);
s32 MapGrid_CheckCollision(struct Actor *object, struct Vector3 *position);
struct Actor *Actor_FindAtCell(struct Vector3 *position, s32 mode);
s32 Func_020037d8(struct PushTarget *target);
void PushTarget_Push(struct PushTarget target);

void Func_02001f58(void);
void Func_02001278(struct Actor *object);
void Actor_EmitSmoke(struct Actor *object);
void Func_020034d8(void);
void Actor_MoveToObject(s32 actor, s32 target);
void FieldPsynergy_StopMindReadWave(void);

#endif
