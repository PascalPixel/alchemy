#include "types.h"

#define Menu_UpdateEntryObjectTransforms Func_080a19a0

extern u8 *Data_03001f2c;

s32 Func_08077148(void);
void Func_08009008(void *, s32 *, s32 *, s32);

void Menu_UpdateEntryObjectTransforms(void)
{
    u8 *p;
    s32 pos[2];
    s32 trans[4];
    s32 *pp;
    volatile s32 *tp;
    s16 *hp;
    s32 i;
    s32 cnt;

    p = Data_03001f2c;
    cnt = (u16)Func_08077148();
    i = 0;
    if (i < cnt) {
        pp = pos;
        tp = trans;
        hp = (s16 *)(p + 308);
        p += 276;
        do {
            void *obj;
            s32 top;

            top = 0x01e20000 - (hp[8] << 16);
            obj = *(void **)p;
            if (obj != 0) {
                *((s8 *)obj + 9) &= -13;
                pos[0] = *(s32 *)(p + 64);
                pp[1] = *(s32 *)(p + 64);
                tp[1] = top;
                tp[0] = hp[0] << 16;
                tp[2] = (hp[8] << 16) + top;
                tp[3] = 0;
                Func_08009008(obj, (s32 *)tp, pp, 0x4000);
            }
            i++;
            hp++;
            p += 4;
        } while (i < cnt);
    }
}
