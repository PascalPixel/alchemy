#ifndef ALCHEMY_UNIT_FIELD_MAKYURI_IRIGUCHI_PILLAR_H
#define ALCHEMY_UNIT_FIELD_MAKYURI_IRIGUCHI_PILLAR_H

#include "types.h"

struct AnchoredEffect;
struct FieldActor;

s32 SceneEffect_AdvanceAnchoredRiseFrame(struct AnchoredEffect *work);
s32 OverlayObject_RampWords24And28Over16Frames(struct FieldActor *o);
s32 OverlayObject_AdvanceScaleCounter(struct FieldActor *o);
s32 Makyuri_FollowLeader(struct FieldActor *actor);
void MakyuriHeya_SpawnSparkPair(void);
void MakyuriIriguchi_RaisePillar(void);

#endif
