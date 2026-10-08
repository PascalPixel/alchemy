#ifndef ALCHEMY_TOPIC_H
#define ALCHEMY_TOPIC_H

#include "types.h"
#include "field_event.h"

extern u8 TorebiIzumi_SceneTableA[];
extern u8 TorebiIzumi_SceneTableB[];
extern const s32 TorebiIzumi_TopicIds[];
extern const u8 TorebiIzumi_ActorTileX[];
extern const u8 TorebiIzumi_ActorTileZ[];
extern const u16 TorebiIzumi_ActorHeadings[];

struct SpringPoint {
    s32 x;
    s32 y;
    s32 z;
};

struct SpringRide {
    u8 unknown_00[2];
    s16 hold;
    struct SpringPoint trail[4];
    struct SpringPoint shadow;
    struct SpringPoint velocity;
    s32 frames;
};

struct SpringActor {
    s32 x;
    s32 y;
    s32 z;
    s16 heading;
    s16 hold;
    s16 mode;
    s16 react;
    s16 cool;
    u8 unknown_16[2];
};

extern struct SpringRide TorebiIzumi_Ride;
extern struct SpringActor TorebiIzumi_ActorRecords[];
extern const u8 TorebiIzumi_SliderFrames[];
extern const u8 TorebiIzumi_CircleFrames[];
void TorebiIzumi_PlaceActor(s32 id, s32 *pos, s32 dir, s32 palette, s32 value);
void FieldScene_RunSecondaryScript(void);

void TorebiIzumi_OfferLuckyWheels(s32 wheel);

#endif
