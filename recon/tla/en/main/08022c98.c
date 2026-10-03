#include "TYPES.H"
#include "METADATA_LOOKUP.H"

struct AnimationObject {
    s16 id;
    u8 reserved_02[2];
    u8 draw_kind;
    u8 field_05;
    u8 reserved_06;
    u8 frame_codec;
    s32 frames;
    s32 animation;
    u32 current;
    u8 state;
    u8 reserved_15;
    u8 marker;
};

extern struct AnimationObject *gAnimationObjects[];

s32 Animation_LookupValueByKey(s32 key);


/* Takes the first free object of the 64 animation objects, one whose draw
 * kind is zero, and sets it up from the metadata of id: its frames, looked up
 * by id when the metadata gives none, its animation table and first entry,
 * draw kind and frame codec. Returns the object, or NULL when the metadata
 * has zero width or no object is free. */

struct AnimationObject *AnimationObject_Allocate(s32 id)
{
    struct AnimationMetadata *metadata;
    struct AnimationObject *entry;
    struct AnimationObject *found;
    struct AnimationObject *object;
    s32 i;
    s32 frames;
    s32 animation;
    /* FAKEMATCH: the zero stored into state and field_05 is a halfword field of
       a struct, so GCC takes it from the literal pool ahead of the frames
       load and keeps it in r8, as the ROM does; a plain zero local loads
       it after the frames. */
    struct { u16 v; } zero;

    found = NULL;
    metadata = Resource_GetMetadataRecordFar(id);
    entry = gAnimationObjects[0];
    object = NULL;

    if (metadata->width != 0) {
        for (i = 0; i <= 63; i++, entry++) {
            if (entry->draw_kind == 0) {
                found = entry;
                break;
            }
        }
        if (found != NULL) {
            zero.v = 0;
            frames = metadata->frames;
            object = found;
            object->id = (s16)id;
            if (frames == 0)
                frames = Animation_LookupValueByKey(id);
            animation = metadata->animation;
            object->animation = animation;
            object->frames = frames;
            object->frame_codec = metadata->frame_codec;
            object->marker = 0xff;
            object->current = *(u32 *)animation;
            object->state = zero.v;
            object->draw_kind = metadata->draw_kind;
            object->field_05 = zero.v;
        }
    }
    return object;
}
