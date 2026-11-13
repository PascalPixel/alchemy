#ifndef ALCHEMY_MODULE_SCENE_CHAMPA_CLIFF_CAVES_H
#define ALCHEMY_MODULE_SCENE_CHAMPA_CLIFF_CAVES_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

extern struct ActorPlacement gData_0200375c[];
extern struct ActorPlacement gData_020037a4[];
extern struct ActorPlacement gData_020038ac[];
extern struct ActorPlacement gData_0200390c[];
extern struct ActorPlacement gData_02003984[];
extern struct ActorPlacement gData_02003aec[];
extern struct ActorPlacement gData_02003b94[];
extern struct ActorPlacement gData_02003c0c[];
extern struct Trigger gData_02003e7c[];
extern struct Trigger gData_02003ffc[];
extern struct Trigger gData_02004104[];

void Func_02001774(void);
void Func_02001868(void);
void Func_020005e8(s32 value);
void Func_0200067c(s32 mode);

void FieldCamera_WaitForArrival(void);

void Func_020032a4(void);
void Func_02000804(s32 item);
void Func_02000978(s32 item);
void Func_020029f8(s32 id, s32 mode);
void Func_02002c88(void);
void Func_02002de4(void);
void Func_02002d34(void);

void Func_02000208(void);

void Func_020003bc(void);

void Func_020003ec(void);
void Func_0200115c(void);

extern s16 gData_02003ce4[];
extern s16 gData_02003cfa[];

struct FlashSprite;

void FieldEffect_StartBlankOverlay(s32 a);
void Field_StartColorTransition(s32 a, s32 b);
extern char ResourceId_FireColumn;

#endif
