#ifndef ALCHEMY_TOPIC_H
#define ALCHEMY_TOPIC_H

#include "TYPES.H"
#include "FIELD_EVENT.H"



/* The overlay's scene tables and topic, tile and heading tables. */
extern u8 TorebiIzumi_SceneTableA[];
extern u8 TorebiIzumi_SceneTableB[];
extern const s32 TorebiIzumi_TopicIds[];        /* 5 topics x 3 ids */
extern const u8 TorebiIzumi_ActorTileX[];       /* four X tile coordinates */
extern const u8 TorebiIzumi_ActorTileZ[];       /* four Z tile coordinates */
extern const u16 TorebiIzumi_ActorHeadings[];   /* four headings */

/* A 16.16 position or velocity. */
struct SpringPoint {
    s32 x;
    s32 y;
    s32 z;
};

/* The spring ride's work, just past the overlay's image: the body the
 * spring throws, chased toward the fountain's middle. */
struct SpringRide {
    u8 unknown_00[2];
    /* Counts the frames since the launch; -1 while no ride runs. */
    s16 hold;
    /* Where the body is, then where it was one, two and three frames ago. */
    struct SpringPoint trail[4];
    /* The body's position on the ground, for its shadow. */
    struct SpringPoint shadow;
    struct SpringPoint velocity;
    /* The frames the body keeps chasing; a hit costs a hundred. */
    s32 frames;
};

/* One of the four fountain actors the body can strike. */
struct SpringActor {
    s32 x;
    s32 y;
    s32 z;
    /* The sliders' direction flag, the circlers' angle. */
    s16 heading;
    /* The sliders' rest at a turn, the circlers' lap counter. */
    s16 hold;
    /* Speeds up with each hit, 0 to 2. */
    s16 mode;
    /* Frames left of the reaction to a hit. */
    s16 react;
    /* Frames until the actor can be hit again. */
    s16 cool;
    u8 unknown_16[2];
};

extern struct SpringRide TorebiIzumi_Ride;
extern struct SpringActor TorebiIzumi_ActorRecords[];
/* The frame each actor shows in each mode: the sliders', then the circlers'. */
extern const u8 TorebiIzumi_SliderFrames[];
extern const u8 TorebiIzumi_CircleFrames[];
void TorebiIzumi_PlaceActor(s32 id, s32 *pos, s32 dir, s32 palette, s32 value);
void FieldScene_RunSecondaryScript(void);

void TorebiIzumi_OfferLuckyWheels(s32 wheel);

#endif
