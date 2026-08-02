typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

extern u8 *Func_0808a080(s32 actor);
extern u8 *Func_08000140(s32 group, s32 index);
extern void Func_08015250(s32 selector);
extern s32 Func_080001c8(s32 slot, s32 count, u8 *buffer);
extern void Func_08000150(s32 group);
extern void Func_08009060(s32 handle);

/* Clear and republish one active actor's presentation resource. */
void Func_020020c4(s32 actorId, s32 selector)
{
    u8 *actor = Func_0808a080(actorId);

    if (actor != 0 && actor[84] == 1) {
        u8 *owner = *(u8 **)(actor + 80);
        u8 *buffer = Func_08000140(17, 1544) + 1024;
        u32 zero = 0;
        volatile u32 *dma3 = (volatile u32 *)0x040000d4;
        s32 tile;

        dma3[0] = (u32)&zero;
        dma3[1] = (u32)buffer;
        dma3[2] = 0x85000020;
        Func_08015250(selector);
        tile = Func_080001c8(owner[28], 128, buffer);
        Func_08000150(17);
        actor[92] = 1;
        Func_08009060(*(s32 *)(owner + 40));
        *(s32 *)(owner + 40) = 0;
        owner[39] = 0;
        owner[5] &= (u8)~0x20;
        *(u16 *)(owner + 8) =
            (u16)((*(u16 *)(owner + 8) & 0xfc00) | (tile & 0x03ff));
        owner[37] = 0;
        owner[38] = 0;
    }
}
