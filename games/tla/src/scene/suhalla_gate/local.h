#ifndef SUHALLA_GATE_LOCAL_H
#define SUHALLA_GATE_LOCAL_H

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

void Actor_SetActionCallback(struct Actor *object, s32 a);
void Func_02000800(void);
void Func_02000a90(s32 id, s32 a);
void Func_02000a98(s32 id, s32 a, s32 b);
void Func_02000988(void);
void Func_02000990(void);
void Func_02000a30(void);
void Func_020005e8(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
extern u8 MsgMyThatWas;
extern u8 MsgNotThatWay;

#endif
