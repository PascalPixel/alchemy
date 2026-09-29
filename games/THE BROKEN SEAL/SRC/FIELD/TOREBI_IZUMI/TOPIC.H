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

/* The spring ride's work, just past the overlay's image (SPRING_RIDE.C). */
extern u8 TorebiIzumi_Ride[];
extern u8 TorebiIzumi_ActorRecords[];           /* 4 x 24 bytes */
void FieldScene_RunSecondaryScript(void);

void TorebiIzumi_OfferLuckyWheels(s32 wheel);
s32 Engine_MathRemainder(s32 dividend, s32 divisor);

#endif
