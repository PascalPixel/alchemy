#include "types.h"

/*
 * 0x02000474 に置かれた保留リクエスト語を処理して消去する。
 *
 * 語の構成: 上位 4 ビット (0xf000) がモード、下位 12 ビット (0x0fff) が
 * ペイロード。ペイロードはさらに 0x7ff で切り出してイベント番号にする。
 */

/* 旗の番号を取る。src/080955b0.c の呼び出しは同じ式をポインタ型で綴るが、
 * そこでも実体は整数の旗番号である (下の註記を参照)。 */
void Func_080770c8(s32 flag);
s32 Func_080022ec(s32 numerator, s32 denominator);  /* 商 */
s32 Func_080022fc(s32 numerator, s32 denominator);  /* 剰余 */
void *Func_0808d394(s32 index);
void Func_080030f8(u32 frames);

/*
 * src/080955b0.c は第三引数を `void *entries` と綴っているが、この呼び出し元は
 * そこに `(event - 300) % 20` という小さな整数を渡す。第二引数は
 * `(event - 300) / 20` なので、080955b0 側の `entries + page * 20 + 48` は
 * 分解した値を組み直して `event - 252` を作っているにすぎない。つまり
 * ポインタではなく整数の旗番号であり、+ 演算の生成コードが同一なため
 * バイト一致の側では型の違いが表に出ていない。
 * 意味としてはこちらの綴りが正しい。
 */
void Func_080955b0(s32 slot, s32 page, s32 entry);

void Func_08095778(s32 acknowledge)
{
    u16 *request = (u16 *)0x02000474;
    s16 *status = (s16 *)0x02000476;
    u32 mode = *request & 0xf000;
    u32 payload = *request & 0x0fff;

    if (acknowledge == 0) {
        if (mode == 0) {
            u32 event = payload & 0x7ff;

            /* 符号なし比較。300 <= event <= 380 の範囲だけを扱う。 */
            if (event - 300 > 80)
                return;
            if (*status > 0 && *status != 999)
                return;
            Func_080770c8((s32)event - 172);
            *request = 0;
            return;
        }

        if (mode != 0x1000)
            return;
        if (*status == 1)
            Func_080770c8((s32)payload);
        *request = 0;
        return;
    }

    if (mode == 0) {
        u32 event = payload & 0x7ff;

        if (event - 300 <= 80 && *status > 0) {
            s32 offset = (s32)event - 300;
            s32 page = Func_080022ec(offset, 20);
            s32 entry = Func_080022fc(offset, 20);
            s32 slot;

            for (slot = 8; slot <= 65; slot++) {
                s16 *record = Func_0808d394(slot);

                if (record == 0)
                    continue;
                if (record[1] - 48 != offset)
                    continue;
                Func_080030f8(40);
                Func_080955b0(slot, page, entry);
                break;
            }
        }
    }

    *request = 0;
}
