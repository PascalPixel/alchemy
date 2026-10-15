#include "types.h"
#include "resource.h"
#include "animspr.h"
#include "metadata_lookup.h"
#include "ram_buffer.h"

#define ANIMATION_OBJECT_COUNT 64

s32 Animation_LookupValueByKey(s32 key);

void VramBlock_Reset93(void)
{
    Resource_ResetEntry(93);
}

extern const u8 Data_0802e89c[];

void VramBlock_Load93(void)
{
    const u8 *data = Data_0802e89c;
    s32 size = 128;

    VramBlock_LoadCached(93, size, data);
}

struct AnimationEntry *AnimationObject_Allocate(s32 id)
{
    struct AnimationMetadata *metadata;
    struct AnimationEntry *entry;
    struct AnimationEntry *found;
    struct AnimationEntry *object;
    s32 i;
    void **frames;
    u8 **animation;

    found = NULL;
    metadata = Resource_GetMetadataRecordFar(id);
    entry = Ram_HeapSlots->unknown_0c[0];
    object = NULL;

    if (metadata->width != 0) {
        for (i = 0; i < ANIMATION_OBJECT_COUNT; i++, entry++) {
            if (entry->kind == 0) {
                found = entry;
                break;
            }
        }
        if (found != NULL) {
            /* FAKEMATCH: instruction order only: read as a word, the frames load does not wait on the anim_id store, so the zero for pos and param loads first as native; the typed read swaps the two (differ 2). */
            frames = (void **)*(s32 *)&metadata->frames;
            object = found;
            object->anim_id = id;
            if (frames == 0)
                frames = (void **)Animation_LookupValueByKey(id);

            animation = metadata->animation;
            object->scripts = animation;
            object->frames = frames;
            object->mode = metadata->frame_codec;
            object->frame = 0xff;
            object->script = animation[0];
            object->pos = 0;
            object->kind = metadata->draw_kind;
            object->param = 0;
        }
    }
    return object;
}
