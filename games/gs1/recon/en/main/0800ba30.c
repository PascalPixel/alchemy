#include "types.h"

struct AnimationMetadata {
    u8 width;
    u8 height;
    u16 scale;
    u8 draw_kind;
    u8 animation_count;
    s8 adjust_x;
    s8 adjust_y;
    u8 frame_codec;
    u8 padding0b;
    s32 frames;
    s32 animation;
};

struct AnimationObject {
    s16 id;
    s16 field02;
    u8 draw_kind;
    u8 padding05[3];
    s32 padding08;
    s32 table;
    s32 current;
    u8 field20;
    u8 marker;
};

struct AnimationSetupState {
    u8 padding00[34];
    u8 field34;
    u8 field35;
    u8 field36;
    u8 padding37[2];
    u8 count;
    struct AnimationObject *entries[4];
};

extern struct AnimationMetadata *Func_08185000(s32 id);

s32 Func_0800ba30(struct AnimationSetupState *state, s32 flags)
{
    s32 high_bit;
    s32 index;

    high_bit = flags & 0x80;
    flags &= 0x7f;

    if (state->field36 != flags) {
        for (index = 0; index < state->count; index++) {
            struct AnimationObject *entry = state->entries[index];
            struct AnimationMetadata *metadata;

            if (entry == 0)
                continue;
            if (entry->table == 0)
                continue;

            metadata = Func_08185000(entry->id);
            if (flags >= metadata->animation_count)
                continue;

            entry->draw_kind = metadata->draw_kind;
            entry->current = ((s32 *)entry->table)[flags];
            entry->marker = 0x10;
            if (high_bit == 0) {
                entry->field20 = 0;
                entry->field02 = 0;
            }
            if (index == 0) {
                state->field35 = metadata->adjust_y;
                state->field34 = metadata->adjust_x;
            }
        }
        state->field36 = (u8)flags;
    }
    return 0;
}
