#include "types.h"

/*
 * resource_3a8 の 0x02003768、252 バイト。随伴者 8 と 9 の毎フレーム処理。
 *
 * 0x02003768 の `push {r5, r6, lr}` から 0x0200383c の単一エピローグまで。
 * 続く整列半語と八語のリテラルプールは本体に含めない。
 *
 * 呼び先は全て本オーバレイのベニア枠 (0x02007056..0x020075a6)。
 * 呼び出し地点ごとに別枠が割り当たるため、同じ処理でも別名で宣言する。
 * 本体像の番地を直接書くと BL が届かない。
 *
 * 二本の腕はほぼ同型。物語フラグ・整数 x・0x03001e40 の第 0 ビット
 * (フレーム交替) で門を作り、随伴者の 16.16 座標を次の処理へ渡す。
 * 腕の内側の一回きりの分岐は合図 0xe6 を鳴らして自分のフラグを立てる。
 * 随伴者 8 には導入の場合分けがあり、狭い x 窓かつ z 閾値超えのときは
 * 主人公の向きビットを写すだけでその場を終える。
 *
 * 注意: プロローグは r5, r6, lr しか退避しないが r4 を作業に使う。
 * 原作側の ABI の不整合であり、C では表現しない。
 *
 * コンパイラ経路: -fno-cse-pool-immediate と -fthumb-orr-dead-input-reuse。
 * 前者を欠くとフラグ番号のプール定数が r5/r6 に溜まり複写が二回増える。
 * 後者を欠くと最後の orr の入力順が逆になる。
 * 向きビットの ~0x0c は (s8) 経由で読む。u8 のままだと 8 ビットに
 * 詰められて `movs #0xf3` 一命令になり、参照の `movs #13 / negs` に
 * ならず全体が二バイトずれる。
 */

struct SceneHandle {
    u8 unknown_00[9];
    u8 flags09;                     /* 0x09、第 2-3 ビットが向き */
};

struct SceneEntity {
    u8 unknown_00[8];
    s32 x;                          /* 0x08、16.16 */
    s32 y;                          /* 0x0c、16.16 */
    s32 z;                          /* 0x10、16.16 */
    u8 unknown_14[0x3c];
    struct SceneHandle *handle;     /* 0x50 */
};

/* 実体取得のベニア三枠。 */
extern struct SceneEntity *Func_0200738a(s32);
extern struct SceneEntity *Func_02007392(s32);
extern struct SceneEntity *Func_020073b0(s32);

/* 物語フラグ読みの四枠。 */
extern s32 Func_02007386(s32);
extern s32 Func_020073a4(s32);
extern s32 Func_020073cc(s32);
extern s32 Func_020073ec(s32);

/* 物語フラグ立ての二枠。 */
extern void Func_020073c4(s32);
extern void Func_0200740c(s32);

/* 用途不明の呼び先、二枠。 */
extern void Func_02007540(s32);
extern void Func_02007588(s32);

/* 効果音の二枠。 */
extern void Func_0200755e(s32);
extern void Func_020075a6(s32);

/* 16.16 座標を受け取る同オーバレイの処理、二枠。 */
extern void Func_02007056(s32, s32, s32);
extern void Func_0200709e(s32, s32, s32);

#define FRAME_BITS (*(volatile u32 *)0x03001e40)

void Func_02003768(void)
{
    struct SceneEntity *follower8;
    struct SceneEntity *follower9;
    struct SceneEntity *hero;
    s32 tileX;

    follower8 = Func_0200738a(8);
    follower9 = Func_02007392(9);

    tileX = (s16)(follower8->x >> 16);

    /* `adds r3,#-381 / cmp #12 / bhi` — 符号なしの窓判定。 */
    if ((u32)(tileX - 381) <= 12 && (s16)(follower8->z >> 16) > 0x309) {
        hero = Func_020073b0(0);
        follower8->handle->flags09 =
            (u8)((hero->handle->flags09 & 0x0c) |
                 ((s8)follower8->handle->flags09 & ~0x0c));
    } else if (Func_02007386(0x302) == 0 &&
               (s16)(follower8->x >> 16) <= 245 &&
               (FRAME_BITS & 1) == 0) {
        if (Func_020073a4(0x202) == 0) {
            Func_02007540(-1);
            Func_0200755e(0xe6);
            Func_020073c4(0x202);
        }
        Func_02007056(follower8->x, follower8->y, follower8->z);
    }

    if (Func_020073cc(0x303) == 0 &&
        (s16)(follower9->x >> 16) <= 0x2c5 &&
        (FRAME_BITS & 1) == 0) {
        if (Func_020073ec(0x203) == 0) {
            Func_02007588(-1);
            Func_020075a6(0xe6);
            Func_0200740c(0x203);
        }
        Func_0200709e(follower9->x, follower9->y, follower9->z);
    }
}
