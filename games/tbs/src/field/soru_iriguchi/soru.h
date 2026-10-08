#ifndef ALCHEMY_SORU_H
#define ALCHEMY_SORU_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

enum {
    ACTOR_SUKURETA = ACTOR_FIRST_PLACED
};

enum SanctumEntranceFlag {
    FLAG_SOL_SANCTUM_ENTERED = 0x802,
    FLAG_SEARCHING_FOR_HIDDEN_PASSAGE = 0x804
};

#define GATE_ID 2081
#define RECORD_A32(rec) (*(s32 *)((rec) + 8))
#define RECORD_B32(rec) (*(s32 *)((rec) + 16))
#define RECORD_A16(rec) (*(s16 *)((rec) + 10))
#define RECORD_B16(rec) (*(s16 *)((rec) + 18))

extern s32 SoruIriguchi_CueTimer;

void Battle_ResetEffectCounter(void);

void Scene_RunActorFormation(s32 a0);
void Scene_UpdateCueTimer(s32 a0, s32 a1, s32 a2);

#endif
