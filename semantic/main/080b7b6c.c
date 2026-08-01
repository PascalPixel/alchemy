/*
 * Correctness fix, veneer audit (mars, 2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320 -- so a `bl` into that
 * range is an indirect call through the named register, not a call to a
 * function at the branch target.  Resolved with tools/veneer_resolve.ts.
 *
 * UNCERTAINTY, and it is deliberate.  What 0x03000164 DOES is not
 * established.  semantic/main/080c1ffc.c calls it a resident two-argument
 * owner initializer; across the tree it is reached with two arguments at
 * some sites and three at others, and where a third is passed it is almost
 * always zero.  It also sits four bytes -- one ARM instruction -- from the
 * fill at 0x03000168, the way the sin/cos pair at 0x0800231c/0x08002322
 * does.  That is suggestive of two entry points into one routine and it is
 * NOT asserted here: the evidence is recorded so the exact lane can settle
 * it, and the type below says only what this call site proves.
 */
#include "layout_guard.h"
#include "types.h"

/*
 * Core-drive row 0x080b7b6c, 612 bytes (kind nonstandard_thumb_call_module;
 * body read confirms an ordinary push/pop frame -- the label comes from the
 * three `mov ip, pc ; bx r3` sequences, which are the established ordinary
 * indirect calls through the pooled 0x03000118 IWRAM fixed-point multiply,
 * spelled here exactly as semantic/main/0800447c.c spells them).
 *
 * Takes a 255-terminated array of s16 resource ids (entries equal to 254
 * are skipped without processing) plus a flag, in three phases:
 *
 * Phase 1: for slots 0..13, if Func_080b770c(list, slot) reports zero,
 * calls Func_080b7b30 with the slot id, biased by +120 for slots 8..13
 * (the second half of a split 14-entry table). Then a one-time init guard:
 * if the flag byte at 0x03001a10 (the same cell semantic/main/080030f8.c
 * maintains) is still clear, calls Func_080030f8(1).
 *
 * Phase 2: walks the id list. For each id (until 255, skipping 254),
 * fetches the id's registry slot (Func_080b7dd0) and its runtime object
 * (slot->object_00, an Object_0800c150 -- the offsets 0x50/0x54/0x20
 * written here are that file's render_data_50/render_kind_54/
 * frame_count_20, layout-guarded there), announces the pairing to
 * Func_080b78e4, and if the object's render kind is still 0 installs its
 * render resources -- the same two modes semantic/main/0800c150.c
 * documents:
 *  - resource 0x1dc or 0x1e3 (of the id's low 12 bits): BUNDLE mode
 *    (render_kind 2). Reserves the next four-pointer bundle in
 *    Data_03001e68's multi_objects_08 array (without advancing
 *    multi_object_count_18, unlike the cousin), clears its 16 bytes
 *    through the 0x03000164 IWRAM routine via the Func_080072f0 thunk
 *    (the asm leaves r2 uninitialized at that call; 0 is passed here),
 *    then inserts up to two renders: Func_08009030(resource) and
 *    Func_08009030(resource + 0x2001), each rescaled through the
 *    0x03000118 multiply against the slot's scale word.
 *  - anything else: SINGLE mode (render_kind 1). One render owns the
 *    object directly; its primary animation record gets flag/palette
 *    bytes, and up to three extra animation channels (ids at slot +6,
 *    +8, +10) are attached via Func_08009048, the third id remapped to
 *    0x1ff when the render's mode byte reads 32.
 * Either way finishes with Func_080b7aac(id).
 *
 * Transcribed faithfully, bugs included: in bundle mode the render's
 * state byte at +0x26 is stored through the Func_08009030 result WITHOUT
 * a null check (both times); when the allocator returns 0 the reference
 * stores to address 0x26, and this draft reproduces that store rather
 * than guarding it.
 *
 * Phase 3: if the flag argument is set, re-walks the same list (same
 * 255/254 rules) and calls Func_080b7aac(id) for every id whose registry
 * slot exists and already holds an object.
 *
 * Complete owner: asm/080b7b6c.s in full -- 0x080b7b6c through the
 * epilogue `bx r0` at 0x080b7dbe plus the mid-function pool at +0xcc
 * (0x00000fff, 0x03001a10, 0x03001e68) and the tail pool at +0x254
 * (0x03000164, 0x03000118, 0x00002001, 0x000001ff). Span 612 bytes,
 * matching the census row exactly. Continues my own park note
 * work/claude/notes/core-080b7b6c.md; second half now fully read.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_080b7b6c)(s32 a, s32 b);

typedef void (*Resident_03000164)(void *destination, u32 size, u32 value);

typedef struct RenderManager_080b7b6c {
    u8 padding_00[8];
    void *multi_objects_08[4];
    u32 multi_object_count_18;
} RenderManager_080b7b6c;

/* The registry slot Func_080b7dd0 resolves an id to. */
typedef struct Slot_080b7b6c {
    u8 *object_00;
    u16 resource_04;
    u16 anim_a_06;
    u16 anim_b_08;
    u16 anim_c_0a;
    u8 padding_0c[8];
    u32 palette_14;
    s32 scale_18;
    u8 padding_1c[4];
    u8 *anim_slot_20;
    u8 *anim_slot_24;
} Slot_080b7b6c;

LAYOUT_OFFSET_GUARD(Slot080b7b6c_Palette, Slot_080b7b6c, palette_14, 0x14);
LAYOUT_OFFSET_GUARD(Slot080b7b6c_Scale, Slot_080b7b6c, scale_18, 0x18);
LAYOUT_OFFSET_GUARD(Slot080b7b6c_AnimSlot, Slot_080b7b6c, anim_slot_20, 0x20);

/* The render record Func_08009030 returns. */
typedef struct Render_080b7b6c {
    u8 padding_00[0x18];
    s32 scale_18;
    u8 padding_1c[4];
    u8 mode_20;
    u8 padding_21[5];
    u8 state_26;
    u8 padding_27;
    u8 *animation_28;
} Render_080b7b6c;

LAYOUT_OFFSET_GUARD(Render080b7b6c_Scale, Render_080b7b6c, scale_18, 0x18);
LAYOUT_OFFSET_GUARD(Render080b7b6c_Mode, Render_080b7b6c, mode_20, 0x20);
LAYOUT_OFFSET_GUARD(Render080b7b6c_State, Render_080b7b6c, state_26, 0x26);
LAYOUT_OFFSET_GUARD(
    Render080b7b6c_Animation, Render_080b7b6c, animation_28, 0x28);

extern RenderManager_080b7b6c *Data_03001e68;

s32 Func_080b770c(s16 *list, s32 slot);
void Func_080b7b30(s32 slot);
void Func_080030f8(s32 mode);
Slot_080b7b6c *Func_080b7dd0(s32 id);
void Func_080b78e4(s32 id, Slot_080b7b6c *slot);
Render_080b7b6c *Func_08009030(s32 resource);
u8 *Func_08185000(s32 resource);
u8 *Func_08009048(Render_080b7b6c *render, s32 animation);
void Func_08009070(u8 *animation, s32 mode);
void Func_080b7aac(s32 id);

void Func_080b7b6c(s16 *list, s32 announce)
{
    Multiply_080b7b6c multiply = (Multiply_080b7b6c)0x03000118;
    s32 slot_index;
    s32 id;
    s32 index;

    for (slot_index = 0; slot_index <= 13; slot_index++) {
        if (Func_080b770c(list, slot_index) == 0) {
            Func_080b7b30(
                slot_index > 7 ? slot_index + 120 : slot_index);
        }
    }
    if (*(volatile u8 *)0x03001a10 == 0) {
        Func_080030f8(1);
    }

    id = list[0];
    index = 0;
    if (id != 255) {
        s16 *cursor = list;

        for (;;) {
            if (id != 254) {
                Slot_080b7b6c *slot = Func_080b7dd0(id);

                if (slot == 0) {
                    goto advance;
                }
                Func_080b78e4(id, slot);
                {
                    u8 *object = slot->object_00;

                    if (object == 0) {
                        goto advance;
                    }
                    {
                        u8 *kind_cell = object + 0x54;
                        s32 saved_kind = *kind_cell;

                        if (saved_kind != 0) {
                            goto advance;
                        }
                        if ((slot->resource_04 & 0xfff) == 0x1dc ||
                            (slot->resource_04 & 0xfff) == 0x1e3) {
                            RenderManager_080b7b6c *manager = Data_03001e68;
                            void **bundle_base = (void **)(
                                (u8 *)manager +
                                manager->multi_object_count_18 * 4);
                            void **insertion = (void **)(
                                (u8 *)bundle_base + 8);
                            u16 resource = slot->resource_04;
                            Render_080b7b6c *render;

                            *kind_cell = 2;
                            *(void ***)(object + 0x50) = insertion;
                            ((Resident_03000164)0x03000164)(
                                insertion, 16, 0);
                            render = Func_08009030(resource);
                            if (render != 0) {
                                render->scale_18 = multiply(
                                    render->scale_18, slot->scale_18);
                                *(s16 *)(object + 0x20) =
                                    Func_08185000(resource)[9] >> 1;
                                *insertion = render;
                                insertion = (void **)(
                                    (u8 *)bundle_base + 12);
                            }
                            /* Reference stores through a null render
                             * unguarded; reproduced faithfully. */
                            ((Render_080b7b6c *)render)->state_26 =
                                saved_kind;
                            render = Func_08009030(resource + 0x2001);
                            if (render != 0) {
                                render->scale_18 = multiply(
                                    render->scale_18, slot->scale_18);
                                *insertion = render;
                            }
                            ((Render_080b7b6c *)render)->state_26 =
                                saved_kind;
                        } else {
                            Render_080b7b6c *render =
                                Func_08009030(slot->resource_04);

                            if (render != 0) {
                                u8 *animation;
                                s32 channel;

                                *kind_cell = 1;
                                *(Render_080b7b6c **)(object + 0x50) =
                                    render;
                                render->scale_18 = multiply(
                                    render->scale_18, slot->scale_18);
                                animation = render->animation_28;
                                animation[6] = 1;
                                animation[5] = (u8)slot->palette_14;
                                channel = slot->anim_a_06;
                                if (channel != 0) {
                                    animation = Func_08009048(
                                        render, channel);
                                    animation[6] = 1;
                                }
                                channel = slot->anim_b_08;
                                if (channel != 0) {
                                    animation = Func_08009048(
                                        render, channel);
                                    slot->anim_slot_20 = animation;
                                    Func_08009070(animation, 0);
                                    animation[6] = 3;
                                }
                                channel = slot->anim_c_0a;
                                if (channel != 0) {
                                    if (render->mode_20 == 32) {
                                        channel = 0x1ff;
                                    }
                                    animation = Func_08009048(
                                        render, channel);
                                    slot->anim_slot_24 = animation;
                                    animation[6] = saved_kind;
                                    render->state_26 = saved_kind;
                                }
                            }
                        }
                        Func_080b7aac(id);
                    }
                }
            }
advance:
            cursor += 1;
            index += 1;
            if (index > 13) {
                break;
            }
            id = *cursor;
            if (id == 255) {
                break;
            }
        }
    }

    if (announce != 0) {
        id = list[0];
        index = 0;
        if (id != 255) {
            do {
                if (list[index] != 254) {
                    Slot_080b7b6c *slot = Func_080b7dd0(id);

                    if (slot != 0 && slot->object_00 != 0) {
                        Func_080b7aac(id);
                    }
                }
                index += 1;
                if (index > 13) {
                    break;
                }
                id = list[index];
            } while (id != 255);
        }
    }
}
