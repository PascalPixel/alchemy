/*
 * resource_3c4 @ 0x020019e4 (44 bytes: 28 code + alignment + three pool words).
 *
 * Pool words: 0x0200b3b8 is a data block (spelled as a symbol), 0x00000203 is
 * a selector, and 0x02008fe9 is an odd address — a Thumb code pointer —
 * installed as the +108 handler of slot 9's descriptor.
 *
 * This overlay is linked at 0x02008000, not at the 0x02000000 base the
 * inventory prints, so 0x02008fe9 is file offset 0xfe8 plus the Thumb bit:
 * the handler is Func_02000fe8.  (The same +0x8000 relation resolves the jump
 * table of Func_02000e20 and the handler pool word of Func_02001a10.)
 *
 * `pop {r0} ; bx r0` return: void.
 */
typedef signed int s32;
typedef unsigned char u8;

extern u8 Data_0200b3b8[];

struct Record_02000fe8;

/* The installed handler; see the base note above. */
s32 Func_02000fe8(struct Record_02000fe8 *record);

typedef s32 (*Handler_020019e4)(struct Record_02000fe8 *record);

typedef struct Slot_020019e4 {
    u8 pad[108];
    Handler_020019e4 handler;   /* +108 */
} Slot_020019e4;

void Func_02004b08(s32 slot, void *table);
void Func_02004ace(s32 selector);
Slot_020019e4 *Func_02004b04(s32 slot);

void Func_020019e4(void)
{
    Slot_020019e4 *slot;

    Func_02004b08(8, Data_0200b3b8);
    Func_02004ace(0x203);
    slot = Func_02004b04(9);
    slot->handler = Func_02000fe8;
}
