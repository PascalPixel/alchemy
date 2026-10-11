#ifndef RUNPA_JO_FORTRESS_H
#define RUNPA_JO_FORTRESS_H

#include "types.h"
#include "jo.h"
#include "field_scene.h"
#include "item_ids.h"

#define PRIMARY_ID 24
#define DERIVED_ID 25

#include "staged_actor.h"
#include "object_runtime.h"

typedef struct SceneActor
{
	u8	pad0[8];
	s32 x;
	s32 y;
	s32 z;
} SceneActor;

typedef struct DirectionalSceneActor
{
	u8	pad0[6];
	u16 dir;
	s32 x;
	s32 y;
	s32 z;
} DirectionalSceneActor;
extern s32 gRunpaJoRandomPick;
extern u32 gRunpaJoPairTableA[];
extern u32 gRunpaJoPairTableB[];
extern u32 gRunpaJoPairTableC[];

static __inline__ void SetSceneValue(s16 *field, s32 value)
{

	*field = value;
}

#include "object_runtime.h"

#endif
