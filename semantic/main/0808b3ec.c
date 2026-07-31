#include "types.h"
#include "motion_object.h"

/*
 * Core-drive row 0x0808b3ec, 608 bytes. Called from four sites (two inside
 * asm/0808b674.s, the neighbour whose own 0x0808b7b8 fragment is already
 * registered as part of semantic/main/0808b674.c) plus one published Thumb
 * pointer -- a real, heavily-used entry (bun tools/main_xref.ts 0808b3ec).
 *
 * Iterates a caller-supplied array of 24-byte "publish request" records
 * (record->id == -1 terminates), resolving each to a slot in the object
 * table at Data_03001ebc (the same base Func_0808ba1c indexes -- see
 * src/0808ba1c.c, exact): small ids (0-7) map to fixed slots, larger ones
 * (8-0x2705) get sequentially assigned slots from a shared counter capped
 * at 65. For each record, looks up (or creates) a struct MotionObject at
 * that slot and republishes the record's display fields onto it.
 *
 * struct MotionObject identification: Func_0808ba1c's returned object here
 * is typed struct MotionObject* (include/motion_object.h, pre-existing,
 * exact) on the strength of six exact field-position hits during this
 * trace (x@8, y@12, z@16, motion_flags@85, and two more corroborating
 * writes at the same byte positions the header already names) -- every
 * other offset this function touches (6, 20, 28, 29, 35, 36, 80, 84, 89,
 * 100, 102 on this object or the one linked through its own +80 field)
 * falls cleanly inside the header's existing unknown_* padding ranges with
 * zero contradictions. Posted to chat (2026-07-31) for jupiter/Ivan to
 * reuse; not promoted into motion_object.h itself since a header-only
 * change there stages no real byte movement and trips the commit hook's
 * progress-report requirement for nothing (see this session's mars log).
 *
 * The +80 field is confirmed to hold a pointer to a SECOND MotionObject
 * (its own +24/+28/+29/+84 fields get touched the same way as the primary
 * object's), i.e. this row treats MotionObject as self-referentially
 * linked through an as-yet-unnamed field inside the header's existing
 * unknown_4c[0x09] byte range (76-84) -- transcribed via raw offset
 * arithmetic here rather than by editing the shared struct.
 *
 * The dedup registry at the very front of Data_03001ebc (four pointer
 * slots, byte offsets 0/4/8/12, distinct from Func_0808ba1c's own
 * *4+0x14-indexed array) and the fixed mode flag at byte offset 0x19e are
 * both read directly by offset; neither has an established name.
 *
 * Register provenance note: the asm reuses r8 across the "randomize a
 * linked object's field" block and the later "mode==3, feed the linked
 * object's field24 through the IWRAM multiply" block without reloading it
 * in between when the first block's guard (object's own +84 byte) is
 * false. Both possible sources for r8 resolve to the same value
 * (object's own +80 field), so this draft reloads it explicitly at the
 * second use point rather than modeling genuine register staleness --
 * flagging the transcription choice rather than asserting it as proven.
 *
 * 0x03000118 (fixed-point multiply, IWRAM-relocated, `mov ip,pc / bx r3`)
 * is the same opaque helper 0x0800c62c calls -- not inlined here either,
 * consistent with that row's park note.
 */

struct PublishRequest_0808b3ec {
    s16 id;          /* 0x00: selects a fixed slot (0-7) or joins the
                       * dynamic-allocation range (8-0x2705) */
    s16 effect_kind;  /* 0x02 */
    u32 param_08;       /* 0x04: passed to Func_08093a6c as `kind` */
    u32 param_0c;         /* 0x08: passed to Func_080090c8/080090f0 */
    u32 param_10;           /* 0x0c: passed to Func_080090c8/080090f0 */
    u32 param_14;             /* 0x10: passed to Func_080090c8/080090f0 */
    u16 copy_value;              /* 0x14: copied onto the object's own
                                   * +6 halfword */
    u8 unknown_16;
    u8 flags;                     /* 0x17: bit 0 gates the swap-with-
                                    * previous-slot logic below */
};

extern u8 *Data_03001ebc;

extern s32 Func_0808d428(s32 kind);
extern s32 Func_0808b398(s32 id);
extern void *Func_0808ba1c(u32 index);
extern void *Func_080090c8(s32 category, u32 a, u32 b, u32 c);
extern void Func_080090f0(void *object, u32 a, u32 b, u32 c);
extern s32 Func_080770c0(s32 mode);
extern void Func_08093a6c(s8 *object, s32 kind);
extern void Func_08009080(void *object, s32 flag);
extern u32 Func_08004458(void);
extern s32 Func_08002304(u32 seed, s32 range);
extern s32 Func_08003f3c(u32 index);
extern s32 Func_080091a8(s32 unused, s32 x, s32 z);
extern void Func_08009228(void *object, s32 arg1);

/* IWRAM-relocated fixed-point multiply, opaque -- see 0x0800c62c's own
 * park note for the same helper. */
extern s32 Func_03000118(s32 value, s32 multiplier);

void Func_0808b3ec(struct PublishRequest_0808b3ec *record, s32 counter)
{
    u32 *dedup = (u32 *)Data_03001ebc;
    s32 i;

    /* Find-or-claim this record pointer in the 4-slot dedup registry;
       gives up silently past slot 3. Result isn't used again -- pure
       side effect. */
    if (dedup[0] != (u32)record) {
        if (dedup[0] == 0) {
            dedup[0] = (u32)record;
        } else {
            for (i = 1; i <= 3; i++) {
                if (dedup[i] == (u32)record) {
                    break;
                }
                if (dedup[i] == 0) {
                    dedup[i] = (u32)record;
                    break;
                }
            }
        }
    }

    while (record->id != -1 && counter <= 65) {
        s32 localSlot;
        s32 category;
        struct MotionObject *object;

        if (record->id <= 7) {
            localSlot = record->id;
        } else if (record->id <= 0x2705) {
            localSlot = counter;
            counter++;
        }
        /* else: localSlot keeps whatever value it held from a previous
           iteration, transcribed as-is rather than guessed at */

        if (Func_0808d428(record->effect_kind) == 0) {
            goto next_record;
        }

        if ((u32)(record->effect_kind - 48) <= 79 &&
            *(s16 *)(Data_03001ebc + 0x19e) != 3 &&
            Func_0808d428(record->effect_kind + 80) == 0) {
            goto next_record;
        }

        category = Func_0808b398(record->id);
        object = Func_0808ba1c(localSlot);

        if (object != 0) {
            /* slot already occupied: refresh it with the new record's
               params, unless the mode flag suppresses that */
            if (Func_080770c0(0x109) == 0) {
                Func_080090f0(object, record->param_0c, record->param_10,
                              record->param_14);
            }
        } else {
            /* slot empty: create a new object for this record */
            object = Func_080090c8(category, record->param_0c,
                                    record->param_10, record->param_14);

            if (record->flags & 1) {
                struct MotionObject *prevObject =
                    Func_0808ba1c(localSlot - 1);

                if (*((u8 *)prevObject + 84) == 1 &&
                    *((u8 *)object + 84) == 1) {
                    struct MotionObject *linkedA =
                        *(struct MotionObject **)((u8 *)prevObject + 80);
                    struct MotionObject *linkedB;
                    u8 savedId;

                    *((u8 *)linkedA + 29) |= 1;
                    savedId = *((u8 *)linkedA + 28);

                    linkedB = *(struct MotionObject **)((u8 *)object + 80);
                    *((u8 *)linkedB + 29) |= 1;
                    Func_08003f3c(*((u8 *)linkedB + 28));
                    *((u8 *)linkedB + 28) = savedId;
                }
            }

            if (Func_080770c0(33) != 0 && (u32)(category - 18) <= 1) {
                Func_08009228(object, 226);
            }
        }

        if (object != 0) {
            Func_08009080(object, 1);

            if (*((u8 *)object + 84) == 1) {
                struct MotionObject *linked =
                    *(struct MotionObject **)((u8 *)object + 80);

                if (linked != 0) {
                    u32 seed = Func_08004458();

                    *((u8 *)linked + 36) = (u8)Func_08002304(seed, 30);
                }
            }

            *(u16 *)((u8 *)object + 6) = record->copy_value;
            *((u8 *)object + 89) = 1;
            Func_08093a6c((s8 *)object, record->param_08);
            Func_08009080(object, 1);

            if (*(s16 *)(Data_03001ebc + 0x19e) == 3) {
                object->motion_flags &= 0xfe;
                if (Func_080770c0(33) == 0) {
                    struct MotionObject *linked =
                        *(struct MotionObject **)((u8 *)object + 80);
                    s32 *field24 = (s32 *)((u8 *)linked + 24);

                    *field24 = Func_03000118(*field24, 0xc000);
                }
            } else {
                s32 heightAdjust = Func_080091a8(0, object->x, object->z);

                *(s32 *)((u8 *)object + 20) = heightAdjust;
                object->y += heightAdjust;
            }

            *((u8 *)object + 35) = 1;
        }

        /* write the resolved object back into the same table slot
           Func_0808ba1c reads from (index*4 + 0x14) */
        *(struct MotionObject **)(Data_03001ebc + localSlot * 4 + 0x14) =
            object;

    next_record:
        record = (struct PublishRequest_0808b3ec *)((u8 *)record + 24);
    }
}
