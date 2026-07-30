#include "types.h"

/*
 * Owner 0x080a3ef0 (444 bytes, prologue at 0x080a3ef0 through the single
 * epilogue at 0x080a4086; the five trailing words 0x080a4098..0x080a40ab are
 * this owner's literal pool and the ten words at 0x080a3f44 are its switch
 * jump table).  The frame is closed: nothing live leaves the range.
 *
 * Behaviour: show the character detail panel for `target`, previewing the
 * item held in `source`'s equipment slot.  When the previewed item's category
 * makes it relevant to the target character and the two characters differ,
 * the target's record is snapshotted, the item is resolved against the target
 * (Func_08077028), the panel is drawn, and the record is restored.
 */

void Func_08002df0(void *);
void *Func_08004938(s32);
void Func_080072f0(void *, const void *, s32, const void *);
void *Func_08077008(u16);
void *Func_08077018(u16);
s32 Func_08077028(u16, u16);
void Func_080a112c(s32, u16, s32, s32);
s32 Func_080a40ac(u16);

/* Size of one character record as copied by this owner (166 << 1). */
#define CHARACTER_RECORD_SIZE_080A3EF0 332

/* Transfer descriptor word the copy helper is always handed here. */
#define TRANSFER_WORK_080A3EF0 ((const void *)0x03001388)

/*
 * Root game-state pointer.  The word at +36 is loaded once in the prologue
 * and handed to Func_080a112c at every call site (that parameter is unused
 * there, but the load is a real observable read and is preserved).
 */
#define GAME_ROOT_080A3EF0 (*(u8 **)0x03001f2c)

/* Equipment word: low 9 bits select the item, bit 9 marks it as equipped. */
#define ITEM_ID_MASK_080A3EF0 0x01ff
#define ITEM_EQUIPPED_BIT_080A3EF0 0x0200

/* Panel mode bits assembled in the caller-saved accumulator (r10). */
#define MODE_NO_AUX_WINDOW_080A3EF0 0x0100
#define MODE_SELF_ITEM_080A3EF0 0x0002
#define MODE_SELF_CLASS_080A3EF0 0x0004

/*
 * Snapshot the target's record, draw the panel with the previewed item, then
 * restore the record.  `flag_bit` is the mode bit set when the target already
 * has a matching entry; the item-category path additionally clears the
 * "equipped" bit of the previewed word before the lookup, the class path does
 * not.  (Uncertainty: the asymmetry is faithful to the assembly but the reason
 * for it is not evident from this owner alone.)
 */
static void PreviewOnCharacter_080a3ef0(s32 panel_owner, u16 target, u16 slot,
                                        u32 item_word, s32 mode, s32 flag_bit,
                                        s32 clear_equipped_bit)
{
    void *record = Func_08077008(target);
    void *snapshot = Func_08004938(CHARACTER_RECORD_SIZE_080A3EF0);

    Func_080072f0(snapshot, record, CHARACTER_RECORD_SIZE_080A3EF0,
                  TRANSFER_WORK_080A3EF0);

    if (Func_080a40ac(target) != 0) {
        s32 resolved;

        if (clear_equipped_bit)
            item_word &= ~(u32)ITEM_EQUIPPED_BIT_080A3EF0;

        resolved = Func_08077028(target, (u16)item_word);
        if (resolved != -1) {
            mode |= flag_bit;
            /*
             * The resolved index replaces the incoming slot argument here:
             * r2 still carries Func_08077028's return value at the call.
             */
            Func_080a112c(panel_owner, target, resolved, mode);
        } else {
            Func_080a112c(panel_owner, target, slot, mode);
        }
    } else {
        Func_080a112c(panel_owner, target, slot, mode);
    }

    Func_080072f0(record, snapshot, CHARACTER_RECORD_SIZE_080A3EF0,
                  TRANSFER_WORK_080A3EF0);
    Func_08002df0(snapshot);
}

/*
 * source        r0 - character whose equipment slot supplies the preview item
 * slot          r1 - equipment slot index within that character's record
 * suppress_aux  r2 - when exactly 1, the panel is drawn without its auxiliary
 *                    window (mode bit 0x100)
 * target        r3 - character the panel is drawn for
 */
void Func_080a3ef0(u16 source, u16 slot, s32 suppress_aux, u16 target)
{
    u8 *source_record = Func_08077008(source);
    u32 item_word = *(u16 *)(source_record + 216 + slot * 2);
    const u8 *metadata;
    u32 category;
    s32 panel_owner = *(s32 *)(GAME_ROOT_080A3EF0 + 36);
    s32 mode = 0;

    if (suppress_aux == 1)
        mode = MODE_NO_AUX_WINDOW_080A3EF0;

    metadata = (const u8 *)Func_08077018((u16)(item_word & ITEM_ID_MASK_080A3EF0));
    category = metadata[2];

    /* Categories above 9 have no panel preview at all. */
    if (category > 9)
        return;

    if (category == 0) {
        /* Table entry 0 enters the shared publish tail with mode untouched. */
        Func_080a112c(panel_owner, target, slot, mode);
        return;
    }

    if (category == 6) {
        /* Class-changing item: relevance is a property of the target. */
        if (target == source) {
            mode |= MODE_SELF_CLASS_080A3EF0;
            Func_080a112c(panel_owner, target, slot, mode);
            return;
        }

        PreviewOnCharacter_080a3ef0(panel_owner, target, slot, item_word, mode,
                                    MODE_SELF_CLASS_080A3EF0, 0);
        return;
    }

    /* Categories 1-5 and 7-9 share the ordinary equipment preview. */
    if (source == target) {
        mode |= MODE_SELF_ITEM_080A3EF0;
        Func_080a112c(panel_owner, target, slot, mode);
        return;
    }

    PreviewOnCharacter_080a3ef0(panel_owner, target, slot, item_word, mode,
                                MODE_SELF_ITEM_080A3EF0, 1);
}
