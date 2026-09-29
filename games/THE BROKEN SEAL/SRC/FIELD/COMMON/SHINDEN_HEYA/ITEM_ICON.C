#include "TYPES.H"


s32 Engine_ObjectCreate();
void ObjectDispatch_Initialize();
s32 Runtime_AllocateHeapBlock();
void WaitFrames();
void Runtime_ReleaseHeapBlock();
s32 VramBlock_LoadCached();
void Ui_PrepareTransferForItem();
/* The motion scripts that pop the icon in and delete it a second later. */
extern const s32 ShindenHeya_ItemIconGrowScript[];
extern const s32 ShindenHeya_ItemIconEndScript[];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* NONMATCHING: 172 of 172 bytes, 12 halfword edits (2026-09-24). Same shape
 * as 380:02004260. Remaining: the -33 mask is folded to 0xdf (a word mask
 * variable grows the function), the sprite[28] load is scheduled late, and
 * the loop zero is not shared with the counter in r5. */
struct Spr5 { u8 pad[5]; u8 lo:5; u8 bit5:1; u8 hi:2; };

/* Shrine room: raises an item icon object, loads the item's icon into its sprite and clears its motion flags while it rises for sixty frames. */
void ShindenHeya_RaiseItemIcon(s32 item)
{
    u8 *obj;
    u8 *spr;
    s32 buf;
    s32 zero;
    s32 z;
    u8 *flag;
    u32 i;

    obj = Value1(Engine_ObjectCreate, 22);
    zero = 0;
    if (obj != 0) {
        Call2(ObjectDispatch_Initialize, (s32)obj, (s32)ShindenHeya_ItemIconGrowScript);
        spr = *(u8 **)(obj + 80);
        spr[38] = zero;
        spr[39] = zero;
        ((struct Spr5 *)spr)->bit5 = 0;
        spr[9] &= 15;
        *(s32 *)(obj + 40) = 0x20000;
        *(s32 *)(obj + 72) = 0x4000;
        buf = Value2(Runtime_AllocateHeapBlock, 17, 0x608);
        Ui_PrepareTransferForItem(item);
        VramBlock_LoadCached(spr[28], 128, buf + 0x400);
        Runtime_ReleaseHeapBlock(17);
        /* FAKEMATCH: the stored zero is a variable set after the flag
         * address, so it copies the counter's zero after that address. */
        for (i = 0, flag = obj + 85, z = 0; i <= 59; i++) {
            if ((u32)(*(s32 *)(obj + 40) + 255) <= 0x1fe)
                *flag = z;
            WaitFrames(1);
        }
        ObjectDispatch_Initialize((s32)obj, (s32)ShindenHeya_ItemIconEndScript);
    }
}
