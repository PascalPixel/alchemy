#include "layout_guard.h"
#include "types.h"

/*
 * Special-tile interaction handler.
 *
 * Called with the tile kind reported by Func_0808ce74 for the tile the
 * selected party member is facing.  Kinds 0xf2..0xf7 are the fixed
 * "scenery" tiles and only print a two-line message.  Every other kind is
 * looked up as a class-3 map descriptor through Func_0808d48c; the
 * descriptor's 32-bit command word then selects one of five behaviours
 * (unlock, plain message, container, shop/item grant, generic item grant).
 */

struct MapEntry_0808d9a4 {
    /* Descriptor word: low 9 bits are the script class, bits 4..8 also
     * double as the message-slot index used by the lock/unlock messages. */
    s32 descriptor;
    /* Halfword at +4 holds the entry kind byte plus flag bits; only bit
     * 0x400 is tested here. */
    u16 kind_flags;
    /* Signed progress-flag id, -1 when the entry has no flag. */
    s16 owner_flag;
    /* Command word: bits 20..31 select the behaviour, bits 16..19 carry a
     * sub-selector, and the low halfword is a message or item id. */
    u32 command;
};

LAYOUT_OFFSET_GUARD(
    MapEntry0808d9a4_KindFlags,
    struct MapEntry_0808d9a4,
    kind_flags,
    4);
LAYOUT_OFFSET_GUARD(
    MapEntry0808d9a4_OwnerFlag,
    struct MapEntry_0808d9a4,
    owner_flag,
    6);
LAYOUT_OFFSET_GUARD(
    MapEntry0808d9a4_Command,
    struct MapEntry_0808d9a4,
    command,
    8);

/* Base of the map/party work block at 0x02000240.  The original code
 * reaches 0x02000434 both through this base (+0x1f4) and through a direct
 * literal; both forms are written as Data_02000434 below. */
extern u8 Data_02000240[];
/* Id of the currently selected party object. */
extern s32 Data_02000434;
/* Six-entry message-slot table for tile kinds 0xf2..0xf7. */
extern const u8 Data_0809e680[];
/* Scene runtime pointer. */
extern u8 *Data_03001ebc;

void Func_080030f8(s32 frames);

/*
 * The 0x0808da64 site is `__call_via_r3` (0x080072f0 is index 3 of the
 * 0x080072e4 bank), so the call is INDIRECT through the command word --
 * this file's own header already read it that way, and the ROM agrees.
 * It is now written as what it is rather than as a fourth argument.
 *
 * What the branch's guard actually tests: `(command & 0x0f000000) != 0` is
 * true exactly when the word's high byte carries a GBA memory-region nibble
 * (0x02 EWRAM, 0x03 IWRAM, 0x08 ROM). The three sibling branches all compare
 * `command & 0xfff00000` against small constants (0x00400000, 0x00500000,
 * 0x00200000). So bits 24..27 are the tag that says "this word is an address,
 * not a packed id", and the field is a tagged union -- the same offset is read
 * as `ldrh` for a message id in the untagged branches.
 *
 * ARITY: one argument. Only r0 is deliberately set at 0x0808da60. r1 holds
 * 500 solely because `movs r1,#250; lsls r1,r1,#1` materialised the
 * 0x02000240 + 0x1f4 address two instructions earlier, and r2 is caller-saved
 * scratch left over from Func_0808d428. Counting live intermediates as
 * arguments is exactly the batch-3 error; they are not reproduced.
 */
typedef void (*MapEntryHandler)(s32 selectedObject);
void Func_08009080(void *object, s32 mode);
void Func_080090d0(void *object);
void Func_08015040(s32 message, s32 mode);
void Func_08015120(s32 value, s32 slot);
void Func_08015128(s32 mode);
void Func_08015138(void);
s32 Func_08077030(s32 item);
void Func_08077230(s32 item);
s32 Func_080770c0(s32 flag);
void Func_080770c8(s32 flag);
void Func_080770d0(s32 flag);
s32 Func_0808b05c(s32 kind, s32 item);
void Func_0808b320(s32 kind, s32 item);
void Func_0808c2dc(s32 request, s32 mode);
s32 Func_0808d428(s32 flag);
struct MapEntry_0808d9a4 *Func_0808d48c(s32 descriptor_kind, s32 kind);
void Func_0808ec50(s32 tile_kind);
void Func_0808ec8c(s32 tile_kind);
void Func_0808ece0(s32 tile_kind);
void Func_0808ed1c(s32 tile_kind);
void *Func_0808ed4c(s32 tile_kind);
void Func_0808ed78(s32 tile_kind);
void *Func_0808ef70(s32 object_id, s32 resource);
void Func_0808f0c8(void *object);
void Func_0808f0d8(void *object);
void Func_0809163c(s32 effect);
void Func_08091660(void);
void Func_080916b0(void);
void Func_08091750(void);
void Func_0809202c(void);
void Func_080f9010(s32 sound);

s32 Func_0808d9a4(s32 tile_kind)
{
    struct MapEntry_0808d9a4 *entry;
    s32 scenery_index;
    s32 message_slot;
    s32 flag;
    s32 command;
    s32 selected;
    s32 slot;
    void *object;
    void *handle;

    selected = Data_02000434;
    scenery_index = tile_kind - 242;

    if ((u32)scenery_index <= 5) {
        /* Fixed scenery tiles: a name line followed by a description. */
        s32 index = Data_0809e680[scenery_index];

        Func_08091660();
        Func_08015040(0x0928 + index, 1);
        Func_08015040(0x0948 + index, 1);
        return 0;
    }

    entry = Func_0808d48c(3, tile_kind);
    if (entry == 0) {
        /* No descriptor: generic "nothing happens" pair. */
        Func_08015040(0x092d, 1);
        Func_08015040(0x094d, 1);
        Func_080770d0(0x142);
        return 0;
    }

    message_slot = (entry->descriptor >> 4) & 31;
    flag = entry->owner_flag;

    if ((entry->kind_flags & 0x400) == 0 && message_slot != 0) {
        /* Locked object: announce it and latch the "locked" state flag. */
        Func_08091660();
        Func_08015040(0x0928 + message_slot, 1);
        Func_080770c8(0x142);
    } else {
        Func_080770d0(0x142);
    }

    command = (s32)entry->command;

    if ((command & 0x0f000000) != 0) {
        /* Key/unlock class.  The descriptor's flag gates the state edit. */
        if (Func_0808d428(flag) != 0) {
            /* Indirect call: the command word is the handler address.
             * See the MapEntryHandler note above for the tag test and for
             * why r1 and r2 are leftovers rather than arguments. */
            ((MapEntryHandler)command)(Data_02000434);
        }
        if (Func_080770c0(0x142) != 0)
            Func_08015040(0x0928 + message_slot, 1);
        Func_080770d0(0x142);
        return 0;
    }

    if (((u32)command & 0xfff00000) == 0x00400000) {
        /* Plain message tile: the entry supplies its own text id once the
         * gating flag is set, otherwise a fallback line is used. */
        if (Func_0808d428(flag) != 0)
            Func_08015040((u16)entry->command, 1);
        else
            Func_08015040(0x0976, 1);
        Func_080770d0(0x142);
        return 0;
    }

    Func_080916b0();

    if (Func_0808d428(flag) == 0) {
        /* Gating flag not set: the object is still sealed. */
        Func_08015040(0x0948 + message_slot, 1);
        Func_08091750();
        Func_0809202c();
        Func_080770d0(0x142);
        return 0;
    }

    /*
     * Sub-selector 1 in bits 16..19 restricts the action to party slots
     * 0..7; outside that range the tile only reports that it is empty.
     */
    if ((command & 0x000f0000) == 0x00010000 && selected <= 7) {
        Func_08015040(0x096f, 1);
        Func_08091750();
        Func_0809202c();
        Func_080770d0(0x142);
        return 0;
    }

    if ((entry->descriptor & 0x1ff) == 19) {
        Func_0808ece0(tile_kind);
        /* The callback may rewrite the entry, so reload the command. */
        command = (s32)entry->command;
    }

    switch ((u32)command & 0xfff00000) {
    case 0x00c00000:
        /* Container that spawns a persistent object. */
        if ((entry->descriptor & 0x1ff) == 19)
            Func_0808ed1c(tile_kind);
        handle = Func_0808ed4c(tile_kind);
        Func_0808f0d8(handle);
        Func_080f9010(0x53);
        Func_08015120((u16)entry->command, 5);
        Func_08015040(0x0970, 3);
        Func_0808c2dc(0x3e7, 0);
        Func_08015128(1);
        Func_080f9010(126);
        Func_08015040(0x0971, 1);
        Func_08015138();
        Func_08009080(handle, 2);
        Func_080f9010(246);
        Func_0809163c(30);
        Func_08015040(0x0972, 1);
        Func_0808ed78(tile_kind);
        if (flag != -1)
            Func_080770c8(flag);
        break;

    case 0x00500000:
        /* Shop/summon style entry: publishes a runtime id and marks the
         * work block busy before and after the transaction. */
        {
            u8 *runtime = Data_03001ebc;

            if ((entry->descriptor & 0x1ff) == 19)
                Func_0808ec8c(tile_kind);
            if (flag != -1) {
                flag |= 0x1000;
                *(u16 *)(Data_02000240 + 0x234) = (u16)flag;
            }
            *(s16 *)(runtime + 0x17c) =
                (s16)Func_0808b05c(99, (u16)entry->command);
            Data_02000240[0x22b] = 2;
            Func_0808b320(99, (u16)entry->command);
            Func_080f9010(*(s16 *)(Data_02000240 + 0x1ee));
            Func_08015040(0x0973, 1);
        }
        break;

    case 0x00200000:
        /* Fixed-effect tile: run the canned object with no resource. */
        object = Func_0808ef70(Data_02000434, 0);
        Func_080030f8(30);
        if ((entry->descriptor & 0x1ff) == 19)
            Func_0808ed1c(tile_kind);
        Func_0808f0d8(object);
        Func_080f9010(0x53);
        Func_08015120((u16)entry->command, 5);
        Func_08015040(0x0969, 3);
        Func_08077230((u16)entry->command);
        if (flag != -1)
            Func_080770c8(flag);
        Func_080090d0(object);
        break;

    default:
        /* Item grant.  The low 12 bits of the command select the resource
         * used for the presentation object. */
        object = Func_0808ef70(Data_02000434, command & 0xfff);
        Func_080030f8(30);
        slot = Func_08077030((u16)entry->command);
        if (slot == -1) {
            /* Nobody can carry it: report the refusal and drop the item. */
            Func_08015120((s32)(entry->command & 0xfff), 2);
            Func_08015040(0x0968, 1);
            Func_08015040(0x096c, 1);
            Func_0808f0c8(object);
            if ((entry->descriptor & 0x1ff) == 19)
                Func_0808ec50(tile_kind);
            break;
        }
        if ((entry->descriptor & 0x1ff) == 19)
            Func_0808ed1c(tile_kind);
        Func_0808f0d8(object);
        Func_080f9010(0x53);
        /* Note the asymmetry with the refusal path above: the accepted
         * path masks the full low halfword, the refusal path only 12 bits. */
        Func_08015120((s32)(entry->command & 0xffff), 2);
        if (slot == Data_02000434) {
            Func_08015040(0x096a, 3);
        } else {
            Func_08015120(slot, 1);
            Func_08015040(0x096b, 3);
        }
        if (flag != -1)
            Func_080770c8(flag);
        Func_080090d0(object);
        break;
    }

    Func_08091750();
    Func_0809202c();
    Func_080770d0(0x142);
    return 0;
}
