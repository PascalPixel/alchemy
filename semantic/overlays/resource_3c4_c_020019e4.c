/*
 * resource_3c4 @ 0x020019e4 (44 bytes: 28 code + alignment + three pool words).
 *
 * Pool words: 0x0200b3b8 is a RAM data block (spelled as a symbol),
 * 0x00000203 is a selector, and 0x02008fe9 is an odd address — a Thumb code
 * pointer — stored into the +108 field of slot 9's descriptor.  It lies well
 * outside this overlay image, so it is a resident routine and is spelled as a
 * pointer literal rather than given a local symbol.
 *
 * `pop {r0} ; bx r0` return: void.
 */
typedef signed int s32;
typedef unsigned char u8;

extern u8 Data_0200b3b8[];

typedef void (*Handler_020019e4)(void);

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
    slot->handler = (Handler_020019e4)0x02008fe9;
}
