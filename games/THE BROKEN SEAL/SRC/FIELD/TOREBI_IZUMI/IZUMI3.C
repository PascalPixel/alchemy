#include "TOPIC.H"
#include "TYPES.H"

/* The saved game as bytes: the signed topic cursors lie at 308. */
extern s8 gCell[];

void ObjectDispatch_ApplyValueToChildren();

struct Half {
    u16 v;
};

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

/* The zero is a one-halfword struct; the child-byte OR goes through a word temporary with 255 held in a variable, so it stays ldrb/orrs. */
void TorebiIzumi_PlaceActor(s32 id, s32 *pos, s32 dir, s32 palette, s32 value)
{
    u8 *actor = (u8 *)Engine_ActorGet(id);
    u8 *sprite;
    u32 n;

    if (actor != 0) {
        struct Half zero;

        *(s32 *)(actor + 8) = *pos++;
        *(s32 *)(actor + 12) = *pos++;
        *(s32 *)(actor + 16) = *pos;
        *(u16 *)(actor + 6) = dir;
        zero.v = 0;
        actor[85] = zero.v;
        (*(u8 **)(actor + 80))[38] = zero.v;
        ObjectDispatch_ApplyValueToChildren(actor, value);
    }
    sprite = *(u8 **)(actor + 80);
    n = sprite[39];
    if (n != 0) {
        u32 mask = 255;
        u8 **list = (u8 **)(sprite + 40);
        u32 left = n;

        do {
            u8 *cell = *list++;

            if (cell[5] != palette) {
                cell[5] = palette;
                {
                    u32 v = cell[22];

                    v |= mask;
                    cell[22] = v;
                }
            }
        } while (--left != 0);
    }
}

void OverlayObject_SetField54(s32 arg0, s32 arg1)
{
    u8 *entry = (u8 *)Engine_ActorGet(arg0);

    if (entry != 0) {
        u8 *field = entry + 0x54;

        *field = arg1;
    }
}
