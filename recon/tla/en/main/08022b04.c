/*
 * Draft: Animation_ApplyChildArgument, ☀️'s AnimationObjects_SelectAnimation
 * with ⚓️'s object layout; it goes after Animation_SetWorkEntry in ⚓️'s
 * SYSTEM/RESOURCE/RESOURCE3.C. Score 2: in the first entry's offset copy the
 * listing reloads the entry count between loading adjust_y and storing it;
 * this C reloads it after the adjust_x load. Barriers after either offset
 * store change nothing. It matches with load scheduling (-mtune=arm9tdmi).
 */
#include "TYPES.H"
#include "ANIMSPR.H"
#include "METADATA_LOOKUP.H"

/* ⚓️'s animation object keeps its offsets, last animation and entry count
   0x0c earlier than ☀️'s, and its entries at the same 0x28. */
struct AnimationGroup {
    u8 unknown_00[0x16];
    s8 offset_x;
    s8 offset_y;
    u8 last_no;
    u8 unknown_19[2];
    u8 count;
    u8 unknown_1c[0x0c];
    struct AnimationEntry *entries[4];
};

s32 Animation_ApplyChildArgument(struct AnimationGroup *state, s32 flags)
{
    s32 high_bit;
    s32 index;

    high_bit = flags & 0x80;
    flags &= 0x7f;

    if (state->last_no != flags) {
        index = 0;
        goto test_entry;
entry_loop:
        {
            struct AnimationEntry *entry = state->entries[index];
            struct AnimationMetadata *metadata;
            s32 selected_animation;

            if (entry == 0)
                goto next_entry;
            if (entry->scripts == 0)
                goto next_entry;

            metadata = Resource_GetMetadataRecordFar(entry->anim_id);
            if (flags >= metadata->animation_count)
                goto next_entry;

            selected_animation = (s32)entry->scripts[flags];
            entry->kind = metadata->draw_kind;
            entry->script = (u8 *)selected_animation;
            /* FAKEMATCH: keeps the step's constant after both stores. */
            __asm__("");
            entry->step = 0x10;
            if (high_bit == 0) {
                entry->pos = 0;
                entry->timer = 0;
            }
            if (index == 0) {
                state->offset_y = metadata->adjust_y;
                state->offset_x = metadata->adjust_x;
            }
            goto next_entry;
        }
next_entry:
        index++;
test_entry:
        if (index < state->count)
            goto entry_loop;
        state->last_no = (u8)flags;
    }
    return 0;
}
