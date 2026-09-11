#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/dialogue/topic/pick_topic_variant_id.h"

extern s8 gCell[];      /* RAM workspace, also indexed as s16 elsewhere */
extern s32 gOv[];     /* in-image, file offset 0x2018: 5 topics x 3 ids */

s32 Dialogue_PickTopicVariantId(s32 topic)
{
    s32 cursor;
    s32 variant;

    if (topic < 0) {
        return 0;
    }

    /* Topic 5 means "any": reduce a 16-bit random to 0..4. */
    if (topic == 5) {
        topic = (s32)((unsigned int)(Talk_Check() * 5) >> 16);
    }

    cursor = gCell[308 + topic];

    /* `lsls #1 / lsrs #16` - a 0/1 coin flip from the same random source. */
    variant = Talk_Apply(cursor + (s32)((unsigned int)(Talk_Check2() * 2) >> 16) + 4, 3);

    gCell[308 + topic] = (s8)variant;

    return gOv[topic * 3 + variant];
}
