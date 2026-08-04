#include "types.h"

/*
 * 隊列の行進を描く演出。実行部は二範囲 (0x080d5c48 の 80 バイトと
 * 0x080d5cb8 の 392 バイト) で、その間の 32 バイトは埋め込みリテラル
 * プールである。末尾 20 バイトもプール。
 *
 * ordinary-blockers.json が挙げていた「二つの Func_08007300 呼び出し」は
 * call-via-r7 スランク、すなわち関数ポインタ呼び出しである。対象は
 * 0x03001f08 (= 0x03001ef0 + 24) に置かれた実行時レンダラで、引数は
 * 確立済みの六引数レンダラ ABI (面, タイル, x, y, 幅, 高さ) に一致する。
 * 元のコードはポインタを r7 に読み込んだまま二回使い回す。
 *
 * 同じ註記が「登録した後始末が call-via-r3 に依存する」とも述べているが、
 * それは Func_080041d8 / Func_08004278 に登録される 0x080cd260 側の
 * 事情であって、この所有者からは通常の登録・解除呼び出しである。
 */

typedef void (*Renderer_080d5c48)(void *surface, const void *tiles,
                                  s32 x, s32 y, s32 width, s32 height);

struct Point_080d5c48 {
    s32 x;
    s32 y;
};

/* work + 0x7080 から 28 バイト刻みで並ぶ行進者の作業欄。 */
struct Marcher_080d5c48 {
    u8 unknown_00[24];
    s32 phase;          /* 24: 開始位相。乱数 & 63 で初期化される。 */
};

/* work + 0x7828 に控えられる場面記述子。 */
struct Parade_080d5c48 {
    s32 unknown_00;
    s32 layout;         /* 4: 1 なら横位置を 112 だけ寄せる */
    u8 unknown_08[12];
    s32 count;          /* 20 */
    u8 unknown_18[12];
    s16 ids[1];         /* 36 */
};

void Func_080cd594(s32 mode);
void Func_080e0524(s32 resource, void *work, s32 arg2, s32 arg3);
void Func_080ed408(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080041d8(const void *entry, s32 size);
u32 Func_08004458(void);
void Func_080b50e8(s32 arg0);
void Func_080f9010(s32 sound);
void Func_080e396c(s32 id, struct Point_080d5c48 *position);
s32 Func_080022ec(s32 numerator, s32 denominator);  /* 商 */
s32 Func_080022fc(s32 numerator, s32 denominator);  /* 剰余 */
void Func_080030f8(u32 frames);
void Func_08004278(const void *entry);
void Func_08002dd8(s32 resource);
void Func_080cdbc0(void);

void Func_080d5c48(struct Parade_080d5c48 *parade)
{
    void *surface = *(void **)0x03001ef0;
    u8 *work = *(u8 **)0x03001eec;
    Renderer_080d5c48 draw;
    struct Parade_080d5c48 **scene =
        (struct Parade_080d5c48 **)(work + 0x7828);
    s32 frame;

    *scene = parade;
    Func_080cd594(1);
    *(volatile u16 *)0x04000020 = 0x100;
    *(volatile u16 *)0x04000050 = 0;
    Func_080e0524(0x7a, work, 1, 1);
    if ((*scene)->layout == 1)
        *(volatile u32 *)0x04000028 = 0xffff9000;

    Func_080ed408(46, 7, 7, 3, 1);
    *(s32 *)(work + 0x7780) = 1;
    *(s32 *)(work + 0x7784) = 0;

    /* 実行時レンダラ。元のコードはこの値を r7 に保持したまま二度使う。 */
    draw = *(Renderer_080d5c48 *)0x03001f08;

    Func_080041d8((const void *)0x080cd261, 144 << 3);

    if ((*scene)->count != 0) {
        struct Marcher_080d5c48 *marcher =
            (struct Marcher_080d5c48 *)(work + 0x7080);
        s32 index = 0;

        do {
            marcher[index].phase = (s32)(Func_08004458() & 63);
            index++;
        } while (index != (*scene)->count);
    }

    frame = 0;
    /* 元の入口判定そのまま。count は非負なので実際には常に成立する。 */
    if (((*scene)->count << 5) != -32) {
        do {
            if (frame == 32)
                Func_080b50e8(0);

            if ((*scene)->count != 0) {
                struct Marcher_080d5c48 *marcher =
                    (struct Marcher_080d5c48 *)(work + 0x7080);
                s32 index = 0;

                do {
                    s32 origin = index * 16;

                    if (frame == origin)
                        Func_080f9010(143);

                    if (frame >= origin && frame < origin + 72) {
                        struct Point_080d5c48 position;
                        s32 top;
                        s32 step;

                        Func_080e396c((*scene)->ids[index], &position);
                        if ((*scene)->layout == 1)
                            position.x -= 112;

                        top = position.y;
                        position.y = top - 16;
                        draw(surface, work + 0x6c0,
                             position.x - 8, top - 20, 16, 20);

                        /* frame >= origin は上で確定済み。元の再判定を残す。 */
                        step = Func_080022fc(
                            Func_080022ec(frame - origin + marcher->phase, 6),
                            9);
                        draw(surface, work + step * 192,
                             position.x - 8, position.y - 16, 16, 12);
                    }

                    marcher++;
                    index++;
                } while (index != (*scene)->count);
            }

            *(s32 *)(work + 0x7824) = 1;
            Func_080030f8(1);
            frame++;
        } while (frame != ((*scene)->count << 5) + 32);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(46);
    Func_080cdbc0();
}
