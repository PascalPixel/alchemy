#ifndef ALCHEMY_SORU_H
#define ALCHEMY_SORU_H

/* The sanctum entrance (resource_37f): what its files share. */

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/* Sukureta answers Gerald's "What's wrong, Kraden?" as placed actor 8. */
enum {
    ACTOR_SUKURETA = ACTOR_FIRST_PLACED
};

enum SanctumEntranceFlag {
    /* Set after the party is asked whether this is its first visit. */
    FLAG_SOL_SANCTUM_ENTERED = 0x802,
    /* Set once the party decides to help Sukureta look for the hidden passage. */
    FLAG_SEARCHING_FOR_HIDDEN_PASSAGE = 0x804
};

#define GATE_ID 2081
#define RECORD_A32(rec) (*(s32 *)((rec) + 8))
#define RECORD_B32(rec) (*(s32 *)((rec) + 16))
#define RECORD_A16(rec) (*(s16 *)((rec) + 10))
#define RECORD_B16(rec) (*(s16 *)((rec) + 18))

/* The cue timer, the overlay's work past its image; ENTRY_STATE.C owns it. */
extern s32 SoruIriguchi_CueTimer;
extern u8 *gWork;

void Battle_ResetEffectCounter(void);

void Scene_RunActorFormation(s32 a0);
void Scene_UpdateCueTimer(s32 a0, s32 a1, s32 a2);

#endif
