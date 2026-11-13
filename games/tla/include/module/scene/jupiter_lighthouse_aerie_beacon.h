#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_AERIE_BEACON_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_AERIE_BEACON_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"
#include "module/scene/scene.h"

struct Actor;
struct CellTarget;
struct CellTargetEntry;
struct FieldEffect;

void Func_0200045c(void);

void SceneActor_ParkRecord(struct Actor *actor);

extern s16 gData_02006a6c;
extern s16 gData_02006a6e;
extern struct Actor *gData_02006a70[];
extern struct LightOam Data_02006ab0[];
extern s16 gData_02006b72;

s32 Func_020059bc(struct Actor *actor);
extern void Func_02005754(struct FieldEffect *effect, s32 mode);

/// The cells Func_02005534 watches, and the cell it copies over the one the
/// leader steps on.
extern struct CellTarget Data_02006b74;

void Func_0200581c(void);

void Func_02005194(struct CellTarget *table, struct CellTargetEntry *entry);

#endif
