#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* runtime/reserved_no_op_f.c */
/* runtime/reserved_no_op_f.c */
/* runtime/reserved_no_op_f.c */
/* runtime/dispatch/reserved_no_op_f.c */
void RuntimeDispatch_ReservedNoOp03008(void)
{
}

/* runtime/set_main_state_19.c */
void Runtime_SetMainState19(void)
{
    *(s32 *)ADDR_03001B00 = 0x13;
}

/* input/update_key_repeat_and_direction.c */
#define REG32(address) (*(volatile u32 *)(address))

void Input_UpdateKeyRepeatAndDirection(void)
{
    s32 cnt = REG32(ADDR_03001B00);
    u32 flags;
    u32 sel = 0;
    u32 n;
    u32 mask;
    volatile u32 *active;

    if (cnt <= 0) {
        REG32(ADDR_03001B04) = REG32(ADDR_03001AE8);
        flags = REG32(ADDR_03001B04);
        if (cnt == 0)
            REG32(ADDR_03001B00) = 6;
        else
            REG32(ADDR_03001B00) = 19;
    } else {
        REG32(ADDR_03001B04) = 0;
        flags = REG32(ADDR_03001B04);
    }

    if (flags != 0) {
        n = 0;
        if ((flags & 0x40) != 0)
            n = 1;
        if ((flags & 0x80) != 0)
            n++;
        if ((flags & 0x20) != 0)
            n++;
        if ((flags & 0x10) != 0)
            n++;

        active = (volatile u32 *)ADDR_03001AFC;
        *active = flags;
        switch (n) {
        default:
            REG32(ADDR_03001D04) = 0x30;
            mask = 0xff0f;
            *active &= mask;
            break;
        case 0:
            REG32(ADDR_03001D04) = 0x30;
            break;
        case 1:
            REG32(ADDR_03001D04) = flags & 0xf0;
            break;
        case 2:
            if ((REG32(ADDR_03001D04)& *active) == 0)
                REG32(ADDR_03001D04) = 0x30;
            *active &= REG32(ADDR_03001D04) ^ 0xffff;
            break;
        case 3:
            if ((REG32(ADDR_03001D04)& 0x30) != 0)
                sel = 0x30;
            if ((REG32(ADDR_03001D04)& 0xc0) != 0)
                sel = 0xc0;
            mask = 0xffff ^ sel;
            REG32(ADDR_03001D04) = flags & mask;
            *active &= mask;
            break;
        }
    } else {
        REG32(ADDR_03001AFC) = flags;
    }

    REG32(ADDR_03001C94) =
        (REG32(ADDR_03001AE8) ^ REG32(ADDR_03001CF4)) & REG32(ADDR_03001AE8);
    REG32(ADDR_03001CF4) = REG32(ADDR_03001AE8);
}

/* input/init_key_irq.c */
void Input_InitKeyIrq(void)
{
    u32 keyInterruptMask;
    volatile u16 *keyControl;
    s32 enabled;

    if (*(volatile u16 *)0x02002000 == 0) {
        keyInterruptMask = 0xC3FF;
        *(keyControl = (volatile u16 *)0x04000132) = keyInterruptMask;
        *(volatile s8 *)ADDR_03001CB8 = (enabled = 1);
    }
}

/* display/blend/update_transition.c */
extern u8 gIwUpdateTransition;
extern volatile u8 gIw2UpdateTransition;
extern u8 gIw3UpdateTransition;
extern volatile u8 gIw4UpdateTransition;
extern u8 gIw5UpdateTransition;
extern u16 gIw6UpdateTransition;

void BlendTransition_Update(void)
{
    if (gIw4UpdateTransition != 0) {
        {
            volatile u16 *blend_control;
            u32 control;

            if (gIw5UpdateTransition != 0) {
                control = gIw6UpdateTransition | 0x80;
                blend_control = (volatile u16 *)0x04000050;
            } else {
                control = gIw6UpdateTransition | 0xc0;
                blend_control = (volatile u16 *)0x04000050;
            }
            *blend_control = control;
        }
        {
            u8 *remaining = &gIwUpdateTransition;
            s32 delta;
            s32 level;
            s32 step;

            (*remaining)--;
            level = gIw2UpdateTransition;
            delta = gIw3UpdateTransition - gIw2UpdateTransition;
            step = *remaining;
            level += FixedPoint_Ratio(delta *step, gIw4UpdateTransition);
            *(volatile u16 *)0x04000054 = level;
            if (*remaining == 0)
                gIw4UpdateTransition = 0;
        }
    }
}

/* display/blend/blend_set_darken_target16.c */
extern u8 gIwSetDarkenTarget16;
extern u16 gIw2SetDarkenTarget16;
extern volatile u8 gIw3SetDarkenTarget16;
extern u8 gIw4SetDarkenTarget16;
extern volatile u8 gIw5SetDarkenTarget16;
extern u8 gIw6SetDarkenTarget16;

void Blend_SetDarkenTarget16(s32 duration)
{
    gIwSetDarkenTarget16 = 0;
    gIw2SetDarkenTarget16 = 0x3e;
    gIw4SetDarkenTarget16 = gIw3SetDarkenTarget16;
    gIw3SetDarkenTarget16 = 0x10;
    gIw5SetDarkenTarget16 = duration;
    gIw6SetDarkenTarget16 = gIw5SetDarkenTarget16;
}

/* display/blend/blend_set_darken_target0.c */
extern u8 gIwSetDarkenTarget0;
extern u16 gIw2SetDarkenTarget0;
extern volatile u8 gIw3SetDarkenTarget0;
extern u8 gIw4SetDarkenTarget0;
extern volatile u8 gIw5SetDarkenTarget0;
extern u8 gIw6SetDarkenTarget0;

void Blend_SetDarkenTarget0(s32 duration)
{
    gIwSetDarkenTarget0 = 0;
    gIw2SetDarkenTarget0 = 0x3e;
    gIw4SetDarkenTarget0 = gIw3SetDarkenTarget0;
    gIw3SetDarkenTarget0 = 0;
    gIw5SetDarkenTarget0 = duration;
    gIw6SetDarkenTarget0 = gIw5SetDarkenTarget0;
}

/* display/blend/blend_set_brighten_target16.c */
extern u8 gIwSetBrightenTarget16;
extern u16 gIw2SetBrightenTarget16;
extern volatile u8 gIw3SetBrightenTarget16;
extern u8 gIw4SetBrightenTarget16;
extern volatile u8 gIw5SetBrightenTarget16;
extern u8 gIw6SetBrightenTarget16;

void Blend_SetBrightenTarget16(s32 duration)
{
    gIwSetBrightenTarget16 = 1;
    gIw2SetBrightenTarget16 = 0x3e;
    gIw4SetBrightenTarget16 = gIw3SetBrightenTarget16;
    gIw3SetBrightenTarget16 = 0x10;
    gIw5SetBrightenTarget16 = duration;
    gIw6SetBrightenTarget16 = gIw5SetBrightenTarget16;
}

/* display/blend/blend_set_brighten_target0.c */
extern u8 gIwSetBrightenTarget0;
extern u16 gIw2SetBrightenTarget0;
extern volatile u8 gIw3SetBrightenTarget0;
extern u8 gIw4SetBrightenTarget0;
extern volatile u8 gIw5SetBrightenTarget0;
extern u8 gIw6SetBrightenTarget0;

void Blend_SetBrightenTarget0(s32 duration)
{
    gIwSetBrightenTarget0 = 1;
    gIw2SetBrightenTarget0 = 0x3e;
    gIw4SetBrightenTarget0 = gIw3SetBrightenTarget0;
    gIw3SetBrightenTarget0 = 0;
    gIw5SetBrightenTarget0 = duration;
    gIw6SetBrightenTarget0 = gIw5SetBrightenTarget0;
}

/* display/blend/configure_transition.c */
extern u8 gIwConfigureTransition;
extern u16 gIw2ConfigureTransition;
extern volatile u8 gIw3ConfigureTransition;
extern u8 gIw4ConfigureTransition;
extern volatile u8 gIw5ConfigureTransition;
extern u8 gIw6ConfigureTransition;

void Blend_ConfigureTransition(s8 mode, s32 coefficient, u32 start, s32 target, s32 duration)
{
    gIwConfigureTransition = mode;
    gIw2ConfigureTransition = coefficient & 0x3f;
    if (start > 0x10U) {
        gIw4ConfigureTransition = gIw3ConfigureTransition;
    } else {
        gIw4ConfigureTransition = start;
    }
    gIw3ConfigureTransition = target;
    gIw6ConfigureTransition = (gIw5ConfigureTransition = duration);
}

/* display/blend/wait_for_transition.c */
void WaitFrames(s32);

void Blend_WaitForTransition(void)
{
    if (*(u8 *)ADDR_03001C98 != 0) {
        do {
            WaitFrames(1);
        } while (*(u8 *)ADDR_03001C98 != 0);
    }
}

/* graphics/reset_frame_state.c */
/* graphics/reset_frame_state.c */
/*
 * Sys_SetMode names a bx rN veneer slot, so this is an indirect call
 * through the register loaded just before it -- the relocated routine at
 * 0x03000164. Its argument count is not established.
 */

void Graphics_ResetFrameState(void)
{
    *(s8 *)ADDR_03001D00 = 0;
    Sys_SetMode(ADDR_03001400, 0x400, ADDR_03001D00, 0x03000164);
}

/* graphics/affine/build_matrix.c */
/* graphics/affine/build_matrix.c */
typedef s32 (*SignedDivide)(s32 numerator, s32 denominator);

struct Effect {
    unsigned x : 16;
    unsigned y : 16;
    unsigned angle : 16;
    unsigned unused : 16;
};

union AffineMatrix {
    s16 coefficients[4];
    u32 rows[2];
};

s32 FixedPoint_Ratio(s32 numerator, s32 denominator);

extern u8 gIw;
extern union AffineMatrix gIw2[];

s32 AffineMatrix_BuildForEffect(struct Effect *source)
{
    union AffineMatrix *matrix;
    s16 *coefficient;
    s32 x_scale;
    s32 y_scale;
    s32 angle;
    u8 index;

    index = gIw;
    x_scale = (s16)source->x;
    y_scale = (s16)source->y;
    angle = source->angle;
    if (index > 31)
        return 0;

    matrix = &gIw2[index];
    coefficient = matrix->coefficients;
    if ((x_scale == y_scale || -x_scale == y_scale) && angle == 0) {
        SignedDivide divide;
        s32 reciprocal;
        s32 x_reciprocal;

        divide = (SignedDivide)0x03000380;
        reciprocal = divide(0x10000, y_scale);
        x_reciprocal = reciprocal;
        if (-x_scale == y_scale)
            x_reciprocal = -reciprocal;

        matrix->rows[0] = (u16)x_reciprocal;
        matrix->rows[1] = (u32)reciprocal << 16;
    } else {
        s32 sine;
        s32 cosine;

        sine = FixedSin(angle);
        cosine = FixedCos(angle);
        *coefficient = FixedPoint_Ratio(cosine, x_scale);
        coefficient++;
        *coefficient = FixedPoint_Ratio(sine, x_scale);
        coefficient++;
        *coefficient = FixedPoint_Ratio(-sine, y_scale);
        coefficient++;
        *coefficient = FixedPoint_Ratio(cosine, y_scale);
    }

    gIw = index + 1;
    return index;
}

/* runtime/slot/push_entry.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Runtime_PushSlotEntry(s32 *slot_entry, s32 slot)
{
    s32 *previous_head;
    s32 slot_offset;
    s32 clamped_slot;

    clamped_slot = slot;
    if (clamped_slot > 0xFF) {
        clamped_slot = 0xFF;
    }
    if (clamped_slot < 0) {
        clamped_slot = 0;
    }
    slot_offset = clamped_slot * 4;
    previous_head = FIELD_AT_OFFSET(slot_offset, s32 **, ADDR_03001400);
    FIELD_AT_OFFSET(slot_offset, s32 **, ADDR_03001400) = slot_entry;
    *slot_entry = previous_head;
}

/* resource/table/get_longest_free_block_run.c */
/* resource/table/get_longest_free_block_run.c */
extern u8 ResourceBlockOwners[];

/* Keep the 128-block scan bound live independently from the run counter. */
#define SCAN_BLOCKS(seed) (((u32)(seed) | ~(u32)(seed)) + 129)

s32 ResourceTable_GetLongestFreeBlockRun(void)
{
    u8 *marker = ResourceBlockOwners;
    s32 current_run = 0;
    s32 remaining = SCAN_BLOCKS(marker);
    s32 longest_run = 0;

    remaining <<= 2;
    do {
        if (*marker++ != 0xff) {
            current_run = 0;
        } else {
            current_run++;
            if (longest_run < current_run) {
                longest_run = current_run;
            }
        }
        remaining--;
    } while (remaining != 0);
    return longest_run;
}

/* resource/table/clear_entry_references.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))


s32 Resource_ClearSlotReferences(s32 resource_id)
{
    s32 cleared_count = 0;
    s32 remaining;
    u8 *marker;
    u8 empty_marker;
    if ((u32)resource_id > 0x5F) {
        return -1;
    }
    marker = ResourceBlockOwners;
    empty_marker = 0xFF;
    remaining = 0x200;
    do {
        if (*marker == resource_id) {
            *marker = empty_marker;
            cleared_count++;
        }
        remaining--;
        marker++;
    } while (remaining != 0);
    if (cleared_count != 0) {
        return -1;
    }
    return 0;
}

/* resource/table/reset_entry.c */
struct ResourceTableEntry {
    u16 value;
    u16 flags;
};

extern struct ResourceTableEntry ResourceTableEntries[];
s32 Resource_ClearSlotReferences(s32 resource_id);

s32 Resource_ResetEntry(u32 resource_index)
{
    struct ResourceTableEntry *resource_entry = &ResourceTableEntries[resource_index];

    if (resource_index > 95) {
        return -1;
    }
    if (resource_entry->flags != 0xffff) {
        Resource_ClearSlotReferences(resource_index);
        resource_entry->flags |= 0xffff;
        resource_entry->value = 0;
    }
    return 0;
}

/* resource/table/activate_entry.c */

s32 Resource_ActivateEntry(u32 resource_index)
{
    u16 *resource_entry = (u16 *)(ResourceTableEntries + resource_index * 4);

    if (resource_index > 95)
        return -1;
    if (*resource_entry > 16) {
        s32 value;

        Resource_ClearSlotReferences(resource_index);
        value = 1;
        *resource_entry = value;
    }
    return 0;
}
