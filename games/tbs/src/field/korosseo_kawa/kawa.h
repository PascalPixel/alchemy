#ifndef ALCHEMY_KOROSSEO_KAWA_H
#define ALCHEMY_KOROSSEO_KAWA_H

#include "types.h"

struct FieldActor;

void *UnitGet(u32 owner);

typedef struct Position3 {
    s32 x;
    s32 y;
    s32 z;
} Position3;

struct FieldActor *SceneActor_FindSlotAtTilePosition(
    Position3 *pos, struct FieldActor *subject);
struct FieldActor *SceneActor_FindOccupantAheadOfSubject(void);
void SceneActor_MarkObjectAtTiles94To95(s32 actor);

#endif
