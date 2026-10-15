#ifndef ALCHEMY_SOL_ENTRANCE_H
#define ALCHEMY_SOL_ENTRANCE_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

enum
{
	ACTOR_SUKURETA = ACTOR_FIRST_PLACED
};

enum SanctumEntranceFlag
{
	FLAG_SOL_SANCTUM_ENTERED = 0x802,
	FLAG_SEARCHING_FOR_HIDDEN_PASSAGE = 0x804
};

#define GATE_ID 2081
#define RECORD_A32(rec) (*(s32 *)((rec) + 8))
#define RECORD_B32(rec) (*(s32 *)((rec) + 16))
#define RECORD_A16(rec) (*(s16 *)((rec) + 10))
#define RECORD_B16(rec) (*(s16 *)((rec) + 18))

extern s32 SoruIriguchi_CueTimer;

#endif
