#include "TYPES.H"

void Engine_ObjectSetScript();


extern u8 Data_00000003[];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

struct Rise {
    u8 pad00[12];
    s32 y;
    u8 pad10[8];
    s32 scaleX;
    s32 scaleY;
    u8 pad20[28];
    s32 baseY;
    u8 pad40[36];
    s16 speed;
    u16 timer;
};

void ShindenHeya_UpdateRisingIcon(struct Rise *obj)
{
    s32 y;
    s32 scale;

    obj->y += obj->speed << 12;
    obj->baseY = obj->y;
    scale = 0;
    /* FAKEMATCH: the mask 3 is a HImode pool constant through the linker's
     * address-3 symbol, which places the literal pool after the dispatch. */
    switch (((s16)obj->timer >> 2) & (u16)(u32)Data_00000003) {
    case 0:
        scale = 0x10000;
        break;
    case 1:
    case 3:
        scale = 0xcccc;
        break;
    case 2:
        scale = 0x9999;
        break;
    }
    obj->scaleX = scale;
    obj->scaleY = scale;
    y = obj->timer - 1;
    obj->timer = y;
    if ((y << 16) <= 0)
        Call2(Engine_ObjectSetScript, (s32)obj, 0x200c18c);
}
