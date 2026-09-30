#include "TYPES.H"
#include "DMA.H"

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
    s16 frame_index;
    u8 draw_kind;
    u8 padding05[3];
    s32 padding08;
    s32 animation_table;
    s32 current_animation;
    u8 playback_state;
    u8 reset_timer;
};

struct AnimationSetupState {
    u8 padding00[34];
    u8 adjust_x;
    u8 adjust_y;
    u8 selected_animation;
    u8 padding37[2];
    u8 count;
    struct AnimationObject *entries[4];
};

extern struct AnimationMetadata *Resource_GetMetadataRecordFar(s32 id);
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern u8 Func_0800a418[];

/* The strided tile copy runs from a heap copy of itself. */
extern u8 Tile_CopyStridedCodeSize[];

/* The 16 by 8 oval, in colour 1, that objects draw beneath them. */
extern const u8 Object_ShadowTiles[];
void *Runtime_AllocateBlock(s32 slot, s32 size);
void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void PaletteDma_LoadBlock(void);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);

void Animation_SetWorkEntry(void *work, s32 no)
{
    s32 hi;
    void *info;
    s32 value;

    hi = 0x80 & no;
    if (FIELD_AT_OFFSET(work, s32, 0x0c) != 0) {
        info = Resource_GetMetadataRecordFar((s32)FIELD_AT_OFFSET(work, s16, 0));
        if (no < (s32)FIELD_AT_OFFSET(info, u8, 5)) {
            value = *(s32 *)((u8 *)FIELD_AT_OFFSET(work, s32, 0x0c) + (no * 4));
            FIELD_AT_OFFSET(work, u8, 4) = (u8)FIELD_AT_OFFSET(info, u8, 4);
            FIELD_AT_OFFSET(work, s32, 0x10) = value;
            FIELD_AT_OFFSET(work, s8, 0x15) = 0x10;
            if (hi == 0) {
                FIELD_AT_OFFSET(work, s8, 0x14) = hi;
                FIELD_AT_OFFSET(work, s16, 2) = (s16)hi;
            }
        }
    }
}

s32 AnimationObjects_SelectAnimation(struct AnimationSetupState *state, s32 flags)
{
    s32 high_bit;
    s32 index;

    high_bit = flags & 0x80;
    flags &= 0x7f;

    if (state->selected_animation != flags) {
        index = 0;
        goto test_entry;
entry_loop:
        {
            struct AnimationObject *entry = state->entries[index];
            struct AnimationMetadata *metadata;
            s32 selected_animation;

            if (entry == 0)
                goto next_entry;
            if (entry->animation_table == 0)
                goto next_entry;

            metadata = Resource_GetMetadataRecordFar(entry->id);
            if (flags >= metadata->animation_count)
                goto next_entry;

            selected_animation = ((s32 *)entry->animation_table)[flags];
            entry->draw_kind = metadata->draw_kind;
            entry->current_animation = selected_animation;
            entry->reset_timer = 0x10;
            if (high_bit == 0) {
                entry->playback_state = 0;
                entry->frame_index = 0;
            }
            if (index == 0) {
                state->adjust_y = metadata->adjust_y;
                state->adjust_x = metadata->adjust_x;
            }
            goto next_entry;
        }
next_entry:
        index++;
test_entry:
        if (index < state->count)
            goto entry_loop;
        state->selected_animation = (u8)flags;
    }
    return 0;
}

s32 AnimationObjects_SetHalfword02OnActive(u8 *grp, s32 val)
{
    u8 n = grp[0x27];
    void **p;
    s32 cnt;

    if (n != 0) {
        /* 有効な登録項目だけへ下位16bitを4bit左へずらして設定する。 */
        val = (s32)((u32)val << 4);
        p = (void **)(grp + 0x28);
        cnt = n;
        do {
            u8 *obj = *p++;
            if (obj != 0 && *(s32 *)(obj + 0xc) != 0)
                *(s16 *)(obj + 2) = val;
            cnt--;
        } while (cnt != 0);
    }
    return 0;
}

void AnimationObjects_SetField15OnActive(u8 *grp, s32 val)
{
    u8 n = grp[0x27];
    if (n != 0) {
        /* 登録順を保ったまま有効な項目へ値を配る。 */
        void **p = (void **)(grp + 0x28);
        s32 cnt = n;
        do {
            u8 *obj = *p++;
            if (obj != 0 && *(s32 *)(obj + 0xc) != 0)
                obj[0x15] = val;
            cnt--;
        } while (cnt != 0);
    }
}

/* Allocates and clears the object and state blocks (from the heap in mode
   3), loads the object graphics and copies the strided tile copy routine
   into heap block 53. */
void ObjectSystem_Configure(s32 mode)
{
    void *objects;
    void *states;
    void *table;
    u32 size;
    volatile u32 zero;

    if (mode == 3) {
        objects = Runtime_AllocateBlock(4, 0xe00);
        states = Runtime_AllocateBlock(3, 0x600);
    } else {
        objects = Runtime_AllocateHeapBlock(4, 0xe00);
        states = Runtime_AllocateHeapBlock(3, 0x600);
    }
    PaletteDma_LoadBlock();
    zero = 0;
    Dma_Set((const void *)&zero, objects, 0x85000380, (volatile u32 *)0x040000d4);
    zero = 0;
    Dma_Set((const void *)&zero, states, 0x85000180, (volatile u32 *)0x040000d4);
    VramBlock_LoadCached(93, 128, Object_ShadowTiles);
    size = (u32)Tile_CopyStridedCodeSize;
    table = Runtime_AllocateHeapBlock(53, size);
    Dma_Set((const void *)Func_0800a418, table, 0x84000000 | (size >> 2),
            (volatile u32 *)0x040000d4);
}
