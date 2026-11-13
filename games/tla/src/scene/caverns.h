#ifndef TLA_CAVERNS_H
#define TLA_CAVERNS_H

#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "main_routine_work.h"
#include "event_inline.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct CavernsMapLayer {
  u8 unk00[0x30];
  u8 *cells;
  u8 unk34[4];
};

struct CavernsMapWork {
  u8 unk000[0x108];
  struct CavernsMapLayer layers[3];
};

extern char SceneId_Ov6a9Room1;
extern char SceneId_Ov6a9Room2;
extern char SceneId_Ov6a9Room3;
extern char SceneId_Ov6a9Room4;
extern u8 gMapCellBuffer[];
extern s16 gData_02001220[];
extern s16 gData_02001264[];
extern u32 gData_02001328[];
extern u32 gData_02001358[];
extern u32 gData_020013b8[];
extern s32 gData_02001430[];
extern s32 gData_0200149c[];
extern s32 gData_020014d8[];
extern s32 gData_020014fc[];

void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Func_02000454(const s16 *data);
void Func_02000d10(void);
void Func_02000ea0(void);
void Actor_SelectAnimation(struct Actor *object, s32 value);
s32 MapGrid_CheckCollision(struct Actor *object, s16 *position);
void Func_0200114c(void);
void Func_02001154(s32 a, s32 b, s32 c, s32 d);
void Func_02000ba8(void);
void Func_02000f50(void);
void Func_02000f98(void);
s32 Func_02000c38(struct Actor *object, s16 *position);

#endif
