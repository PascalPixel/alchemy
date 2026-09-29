#include "TOPIC.H"

/* The saved game as bytes: the signed topic cursors lie at 308. */
extern s8 gCell[];

s32 SceneDialogue_PickTopicVariantId(s32 topic)
{
    s32 cursor;
    s32 variant;

    if (topic < 0) {
        return 0;
    }

    /* Topic 5 means "any": reduce a 16-bit random to 0..4. */
    if (topic == 5) {
        topic = (s32)((unsigned int)(Random_Next() * 5) >> 16);
    }

    cursor = gCell[308 + topic];

    /* `lsls #1 / lsrs #16` - a 0/1 coin flip from the same random source. */
    variant = Engine_MathRemainder(cursor + (s32)((unsigned int)(Random_Next() * 2) >> 16) + 4, 3);

    gCell[308 + topic] = (s8)variant;

    return TorebiIzumi_TopicIds[topic * 3 + variant];
}
