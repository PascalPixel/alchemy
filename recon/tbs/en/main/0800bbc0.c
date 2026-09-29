/*
 * main:0800bbc0 AnimationObject_Allocate - draft; the range links as
 * disassembly (recon/tbs/raw/0800bbc0.s).
 *
 * Remaining: the reference loads the zero it stores into state and field_05
 * as a word from the literal pool (ldr r3, =0) before the frames load and
 * keeps it in r8. Written as zero = 0 below, GCC also takes the zero from
 * the pool into r8 but as a halfword (ldrh) scheduled after the frames load,
 * a two-halfword difference; u8, s16 and s32 spellings give the same ldrh.
 * The unit matched only while the zero was the address of a link-time
 * symbol at 0.
 */
#include "TYPES.H"

struct AnimationMetadata {
    u8 width;
    u8 height;
    u16 scale;
    u8 draw_kind;
    u8 animation_count;
    s8 adjust_x;
    s8 adjust_y;
    u8 reserved_08[2];
    u8 frame_codec;
    u8 reserved_0b;
    s32 frames;
    s32 animation;
};

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

struct AnimationMetadata *Resource_GetMetadataRecordFar(s32 id);
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
    s32 zero;

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
            zero = 0;
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
            object->state = zero;
            object->draw_kind = metadata->draw_kind;
            object->field_05 = zero;
        }
    }
    return object;
}
