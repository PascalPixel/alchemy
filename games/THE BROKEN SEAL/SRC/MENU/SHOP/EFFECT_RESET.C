#include "TYPES.H"
#include "SYSTEM.H"
extern u8 Data_03001f2c[];

/* shop/effect/reset.c */
void EffectSlot_UpdateFar(s32);
void ObjectGroup_SetChildValueUnlessFifteenFar(s32, u32);

void Shop_ResetEffects(void)
{
    s32 p;
    s32 cnt;
    s32 work;
    s8 no;
    s32 offset;

    work = *(s32 *)((u32)&Data_03001f2c);
    p = work + 0x3B0;
    cnt = 0x17;
    do {
        cnt -= 1;
        EffectSlot_UpdateFar(p);
        p += 0x48;
    } while (cnt >= 0);
    no = *(s8 *)(work + 0x3AB);
    if (no != -1) {
        ObjectGroup_SetChildValueUnlessFifteenFar(*(s32 *)(work + (offset = (no * 4) + 0x114)), (Random16() * 7) >> 16);
    }
}
