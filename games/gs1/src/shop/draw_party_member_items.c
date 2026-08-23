#include "shop.h"
#include "battle_runtime.h"

/* 所持品欄の再描画。窓を開き直し、選択中の品目に応じて見出しを差し替え、
   所持枠を左上から順に並べる。枠は5個目と10個目で折り返す。
   枠番号は0xd8からのu16列で、0が終端。 */

extern u8 Value_00000c90;
extern u8 Value_00000c8f;

void Func_08015060(s32 window);
s32 Func_08077038(s32 unit_id, s32 item_id);
void Func_08015120(s32 kosuu, s32 style);
void Func_08015080(s32 message, s32 window, s32 x, s32 y);
u8 *Func_080152d0(u16 no, s32 kind, s32 window, s32 x, s32 y);

void Shop_DrawPartyMemberItems(s32 window, s32 unit_id, s32 item_id)
{
    u8 *unit;
    s32 x;
    s32 y;
    s32 item_index;
    s32 slot;
    s32 selected_offset;
    s32 first_offset;
    s32 item_offset;
    s32 next_offset;
    u8 *icon;

    unit = (u8 *)BattleUnit_Get(unit_id);
    x = 8;
    y = 8;
    if (window != 0) {
        Func_08015060(window);
        slot = Func_08077038(unit_id, item_id);
        /* 参照は枠位置を「バイト差」として先に組み、状態先頭を基底に残す。
           足し込む順を変えると二レジスタ番地形が崩れる。 */
        if (slot != -1) {
            selected_offset = slot * 2 + 216;
            Func_08015120((*(u16 *)(unit + selected_offset) >> 11) + 1, 5);
            Func_08015080((s32)&Value_00000c90, window, 0, 0);
        } else {
            Func_08015080((s32)&Value_00000c8f, window, 0, 0);
        }
        item_index = 0;
        first_offset = 216;
        if (*(u16 *)(unit + first_offset) != 0) {
            for (;;) {
                item_offset = item_index * 2 + 216;
                icon = Func_080152d0(*(u16 *)(unit + item_offset), 27,
                                     window, x, y);
                icon[15] = 252;
                x += 16;
                if (item_index == 4) {
                    x = 8;
                    y += 16;
                }
                if (item_index == 9) {
                    x = 8;
                    y += 16;
                }
                item_index++;
                if (item_index > 14)
                    break;
                next_offset = item_index * 2 + 216;
                if (*(u16 *)(unit + next_offset) == 0)
                    break;
            }
        }
    }
}
