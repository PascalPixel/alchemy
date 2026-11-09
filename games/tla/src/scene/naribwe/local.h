#ifndef NARIBWE_LOCAL_H
#define NARIBWE_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

extern const struct SceneEntrance gSceneEntrances[];
extern const u32 gSceneExits[];
extern const struct ActorPlacement gActorPlacements[];
extern const struct Trigger gSceneEvents[];

struct WarpEntry {
  const void *script;
  u16 x;
  u16 z;
};

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200054c(void);
void Func_02000554(void);
void Func_0200059c(s32 id);
void Func_020005cc(void);
void Func_020005d4(s32 a, s32 b, s32 c, s32 d);
void Func_020005e4(s32 a, s32 b);
void Func_020005ec(s32 a);
void Func_02000464(void);

extern const u8 gData_020006cc[];
extern const u8 gData_020008c4[];
extern const u8 gData_02000954[];
extern const u8 gData_02000b4c[];
extern const u8 gData_02000cfc[];
extern const struct WarpEntry gData_0200064c[];

extern u8 MsgNaribwe02;
extern u8 MsgNaribwe03;
extern u8 MsgNaribwe07;
extern u8 MsgNaribwe08;
extern u8 MsgNaribwe09;
extern u8 MsgNaribwe21;
extern u8 MsgNaribwe43;
extern u8 MsgNaribwe23;
extern u8 MsgNaribwe45;

#endif
