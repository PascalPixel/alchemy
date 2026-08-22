#include "types.h"

/* 所持品欄の再描画。窓を開き直し、選択中の品目に応じて見出しを差し替え、
   所持枠を左上から順に並べる。枠は5個目と10個目で折り返す。
   枠番号は0xd8からのu16列で、0が終端。 */

extern u8 Value_00000c90;
extern u8 Value_00000c8f;

u8 *Func_08077008(s32 actor);
void Func_08015060(s32 window);
s32 Func_08077038(s32 actor, s32 item);
void Func_08015120(s32 kosuu, s32 style);
void Func_08015080(s32 message, s32 window, s32 x, s32 y);
u8 *Func_080152d0(u16 no, s32 kind, s32 window, s32 x, s32 y);

void Func_080b1470(s32 window, s32 actor, s32 item)
{
    u8 *state;
    s32 x;
    s32 y;
    s32 i;
    s32 slot;
    s32 a0;
    s32 a1;
    s32 a2;
    s32 a3;
    u8 *entry;

    state = Func_08077008(actor);
    x = 8;
    y = 8;
    if (window != 0) {
        Func_08015060(window);
        slot = Func_08077038(actor, item);
        /* 参照は枠位置を「バイト差」として先に組み、状態先頭を基底に残す。
           足し込む順を変えると二レジスタ番地形が崩れる。 */
        if (slot != -1) {
            a0 = slot * 2 + 216;
            Func_08015120((*(u16 *)(state + a0) >> 11) + 1, 5);
            Func_08015080((s32)&Value_00000c90, window, 0, 0);
        } else {
            Func_08015080((s32)&Value_00000c8f, window, 0, 0);
        }
        i = 0;
        a1 = 216;
        if (*(u16 *)(state + a1) != 0) {
            for (;;) {
                a2 = i * 2 + 216;
                entry = Func_080152d0(*(u16 *)(state + a2), 27,
                                      window, x, y);
                entry[15] = 252;
                x += 16;
                if (i == 4) {
                    x = 8;
                    y += 16;
                }
                if (i == 9) {
                    x = 8;
                    y += 16;
                }
                i++;
                if (i > 14)
                    break;
                a3 = i * 2 + 216;
                if (*(u16 *)(state + a3) == 0)
                    break;
            }
        }
    }
}
