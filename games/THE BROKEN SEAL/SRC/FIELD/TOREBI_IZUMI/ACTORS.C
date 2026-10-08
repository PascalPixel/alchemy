#define OWN_gGameState
#include "TOPIC.H"
#include "TYPES.H"

extern s8 gGameState[];

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

    if (topic == 5) {
        topic = (s32)((unsigned int)(Engine_RandomNext() * 5) >> 16);
    }

    cursor = gGameState[308 + topic];

    variant = (cursor + (s32)((unsigned int)(Engine_RandomNext() * 2) >> 16) + 4) % 3;

    gGameState[308 + topic] = (s8)variant;

    return TorebiIzumi_TopicIds[topic * 3 + variant];
}

void TorebiIzumi_PlaceActor(s32 id, s32 *pos, s32 dir, s32 palette, s32 value)
{
    u8 *actor = (u8 *)Object_GetById(id);
    u8 *sprite;
    u32 n;

    if (actor != 0) {
        struct Half zero;

        ((struct FieldActor *)actor)->x.fixed = *pos++;
        ((struct FieldActor *)actor)->y.fixed = *pos++;
        ((struct FieldActor *)actor)->z.fixed = *pos;
        ((struct FieldActor *)actor)->facing = dir;
        zero.v = 0;
        ((struct FieldActor *)actor)->motion_flags = zero.v;
        ((struct FieldActor *)actor)->sprite->flags = zero.v;
        ObjectDispatch_ApplyValueToChildren(actor, value);
    }
    sprite = (u8 *)((struct FieldActor *)actor)->sprite;
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

void TorebiIzumi_SetActorActive(s32 arg0, s32 arg1)
{
    struct FieldActor *entry = Object_GetById(arg0);

    if (entry != 0) {
        u8 *field = &entry->active;

        *field = arg1;
    }
}
