#ifndef ALCHEMY_VINASU_H
#define ALCHEMY_VINASU_H

#include "FIELD_SCENE.H"
#include "TYPES.H"

enum {
    ACTOR_FIRST_OF_PAIR = 20,
    ACTOR_SECOND_OF_PAIR = 19,
    FLAG_PAIR_SLIDING = 0x234,
    FLAG_PAIR_GROWING = 0x235
};

void VinasuChojo_ShowMessage(s32 speaker);
void VinasuChojo_FaceActor(s32 actor, s32 facing);
void VinasuChojo_FlashScreen(void);
void VinasuChojo_UpdateBeamActors(void);

#endif
