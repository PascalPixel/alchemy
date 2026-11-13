#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_9_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_9_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"

struct Actor;
struct CellTarget;
struct CellTargetEntry;
struct LightOam;

void Func_02001a28(s32 x, s32 z, s32 effect);

void Func_02000460(s32 mode);

extern s16 gData_0200265c;
extern struct Actor *gData_02002660[];

s32 Func_02001c80(struct Actor *actor);
void Func_02001b40(struct LightOam *oam, s32 depth);

void Func_02001550(void);

extern struct CellTarget Data_02002764;

void Func_020015d8(struct CellTarget *table, struct CellTargetEntry *entry);

void Func_0200007c(void);

void Func_02000538(void);

extern u32 gData_0200211c[];
extern u32 gData_02002158[];

#endif
