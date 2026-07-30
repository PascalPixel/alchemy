#include "types.h"

/* 効果音チャンネル管理。番号表を線形探索し、既存なら次の空きビットを */
/* 剰余で回して確保、無ければ表末尾に新規登録する。 */

struct Snd_080c1df4 {
    u8 pad00[0x10];
    u16 no[6];      /* 0x10 */
    u32 use[6];     /* 0x1c */
    s8 ch[6];       /* 0x34 */
    u8 pad3a[6];
    u8 num;         /* 0x40 */
};

extern struct Snd_080c1df4 *Data_03001e74;

s32 Func_080022fc(s32, s32);

s32 Func_080c1df4(s32 no)
{
    struct Snd_080c1df4 *w;
    s32 num;
    s32 i;
    s32 retry;
    s32 ch;

    w = Data_03001e74;
    num = w->num;
    for (i = 0; i < num; i++) {
        if (w->no[i] == no)
            break;
    }
    if (i != num) {
        retry = 0;
        if (w->ch[i] < 0) {
            w->ch[i] = 1;
            w->use[i] = 3;
            return 0x8001;
        }
        for (; retry <= 31; retry++) {
            ch = Func_080022fc(w->ch[i] + 1, 9);
            w->ch[i] = ch;
            if ((w->use[i] & (1 << (s8)ch)) == 0)
                break;
        }
        w->use[i] |= 1 << w->ch[i];
        return w->ch[i];
    }
    if (num <= 4) {
        w->ch[num] = -1;
        w->no[num] = no;
        w->use[num] = 0;
        w->num = num + 1;
        return 9;
    }
    return -1;
}
