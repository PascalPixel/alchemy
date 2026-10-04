/* 2026-10-04 (second-wave finisher 1): alchemy diff 415 -> 293, plain C,
 * no device. The transfer block has its own sprite pointer (te) and the
 * third chain its own node (p), so loop one keeps node r7 / e r6 as the
 * ROM does; slot = 241 is set first in the cursor arm and cnt++ follows
 * node = node->next; the FAKEMATCH step temporary is gone (293 without it).
 * Left: global allocation gives fp to cursor_entry and puts cnt at sp+8;
 * the ROM gives cnt fp and spills cursor_entry, which frees fp for the
 * hoisted -13 in chain three and the tile base in the cursor block. Pinning
 * cnt to fp scores 246, so the rest is mostly that cascade plus local
 * choices (sel copied to r7, p in r7). cnt as u16/s16/u32/register and
 * declaration swaps do not move it.
 * Then 293 -> 265, still plain: p declared after cnt and te->shape stored
 * after te->size (found by a devsearch run whose only device, sel pinned to
 * r0, scores worse than leaving it out).
 * 2026-10-04: current EN score 3560 (162 register-only, 6 operand,
 * 21 reordered, 7 inserted, 6 deleted), down from the fresh 5250 baseline.
 * A five-minute, two-worker ordinary-source search found 3710; removed an
 * unnecessary path temporary without changing that score. Replaced raw work
 * offsets with SELECT.H's existing SelectionScreen owner and named fields,
 * improving to 3560. Complete matching remains unproved; no source credit. */
/* 2026-10-03 ordinary owner/API attempt: consume the shared affine input
 * and canonical resource/allocator contracts. Earlier measurements are
 * prior checkpoints; fresh complete-object comparison is pending. */
#include "AFFINE.H"
#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "SELECT.H"
/* 2026-10-03 owner cleanup: SELECT.H now supplies the real doubly-linked
 * node and sprite record. This remains a draft: its object is 1372 bytes,
 * four short of the complete 1376-byte extent, and prior ordering/register
 * differences remain. The typed first member lets GCC keep the projection
 * coordinates across cursor stores; the old s32 unknown prefix reloaded them.
 */
/* DRAFT checkpoint 2026-09-26: its complete owner now ends at 0801aeec,
 * before the separately registered MenuSelection_DrawSideMarker.
 * Explicit --size 1376: 1376/1376 bytes, 415 differing halfwords and 259
 * aligned edits; branch topology and 24-byte frame agree. The first broad
 * divergence keeps cnt on the stack and cursor_entry in fp, where the
 * reference keeps cnt in fp and cursor_entry at sp+8. No new variant tried.
 * 2026-09-29 alchemy permute (seed 1, 3 jobs, 10 minutes): 24,941
 * candidates; the best scored 4140 against 5805 (174 register-only, 8
 * operand, 27 reordered, 8 inserted, 6 deleted) after 18 rewrites (swap
 * commutative operands, reorder independent statements, change loop form,
 * introduce a temporary), none of them kept. Register choice (174
 * register-only) is still most of the difference.
 */
#include "TYPES.H"

/* 選択メニューの毎フレーム描画更新。
 *
 * gResQueueWork が指す選択状態ブロックから四組の表示要素を組み立て、
 * それぞれを Runtime_PushSlotEntry でランタイムの表示スロット列に積む。
 *
 *   1. +0x348 の節点鎖   … 各項目の横移動を進め、選択中の項目だけ
 *                          +0x2d8 のカーソル節点に目標座標を配布する。
 *   2. +0x30c の転送節点 … 倍率が目標に達していなければアフィン行列を
 *                          作り直し、達していればアフィンを解除する。
 *   3. +0x34c の節点鎖   … 縦横両方向の移動と倍率を進める。
 *   4. +0x2d8 のカーソル … 資源を読み直し、目標へ半分ずつ寄せて描画する。
 *
 * 2 と 3 の間で MenuSelection_DrawSideMarker を引数 0 と 1 で二度呼ぶ。この呼び出しが
 * 何を描くかはここからは分からない。最後に +0x3a2 の u16 を一つ進める。
 *
 * 判明していない点:
 *   - 選択状態ブロックは SELECT.H に共有した。描画の試作は未一致。
 *   - 節点 +0x00 と +0x1c..+0x21 の役割。
 *   - BattleMotion_ProjectScaledPositionFar は項目番号から座標を引く照会。既存の
 *     games/THE BROKEN SEAL/src/effects/position/apply_step_and_y_offset.c は同じ
 *     入口を戻り値なしで宣言しているが、この所有者は戻り値を -1 と
 *     比較して失敗を判定する。名前は未確定。
 *   - Data_08036740 は 16 個の符号付き揺れ量表、Menu_AnimatedCursorTiles は
 *     0x100 バイト単位の画素ブロック列。どちらも役割名は未確定。
 *   - 節点 +0x22 は二役ある。2 と 3 では AffineMatrix_BuildForEffect の
 *     x/y 倍率として渡すので scale と呼ぶが、1 と 4 では単に 0 以外かどうか
 *     を見る表示可否の印として使う。同じ語が両方に使われている。
 */

/* 表示スロット一件。先頭語はランタイムが繋ぎ替える次要素へのリンク。
   残りは GBA のオブジェクト属性と同じ並びで、+4 が縦位置、+5 が属性 0 の
   上位バイト、+6 が属性 1、+8 が属性 2 に相当する。 */
/* 効果位置。既存の games/THE BROKEN SEAL/src/battle/effects/radial_camera/update.c と
   同じ三語の並び。BattleMotion_ProjectScaledPositionFar は三語すべてを書き、ここでは x と y だけを読む。 */
struct EffectPosition {
    s32 x;
    s32 y;
    s32 z;
};

/* 選択鎖の節点。カーソル (+0x2d8) と転送枠 (+0x30c) も同じ並びを使う。 */
extern struct SelectionScreen *gResQueueWork;
extern volatile u32 gFrameTick;
extern s8 Data_08036740[];
extern u8 Menu_AnimatedCursorTiles[];

extern s32 BattleMotion_ProjectScaledPositionFar(s32 no, struct EffectPosition *pos);
extern s32 GameFlag_IsSet(s32 flag);
extern void MenuSelection_DrawSideMarker(u8 *state, s32 index);
extern struct SelectionNode *NodeChain_GetNodeAtCount(u8 *state);

void MenuSelection_DrawFrame(void)
{
    struct SelectionScreen *state = gResQueueWork;
    /* +0x300 はカーソル節点 (+0x2d8) 自身の表示スロットに当たる。 */
    struct SelectionSprite *cursor_entry = &state->records[14].sprite;
    struct SelectionNode *cursor = &state->records[14];
    register struct SelectionNode *transfer = &state->records[15];
    struct SelectionNode *node;
    struct SelectionSprite *e;
    struct SelectionSprite *te;
    s32 cnt = 0;
    struct SelectionNode *p;
    s32 slot;
    s32 tmp;
    struct EffectPosition pos;
    u32 y;
    /* 一列目: 項目の横移動と、選択中項目からのカーソル座標配布。 */

    node = state->head;
    while (node != 0) {
        e = &node->sprite;
        e->affine = 0;
        e->param = 0;
        e->x = node->x;
        y = (u16)node->y;
        slot = 240;
        e->y = y;
        if (state->vertical_scroll != 0) {
            /* 画面全体の縦送りが有効な間は移動を止め、そのまま描く。 */
            e->y = state->vertical_scroll + y;
        } else if (node->x != node->target_x) {
            if (node->dx > 0) {
                if (node->x + node->dx > node->target_x) {
                    node->x = node->target_x;
                } else {
                    node->x = node->x + node->dx;
                }
            } else {
                if (node->x + node->dx < node->target_x) {
                    node->x = node->target_x;
                } else {
                    node->x = node->x + node->dx;
                }
            }
            e->x = node->x;
        } else if (cnt == state->cursor_index) {
            slot = 241;
            if (cursor->kind != 0) {
                if (BattleMotion_ProjectScaledPositionFar(node->base, &pos) != -1) {
                    cursor->target_x = pos.x;
                    /* FAKEMATCH: a scheduling barrier the native order needs. */
                    __asm__("");
                    cursor->target_y = pos.y;
                    if (cursor->scale == 0) {
                        cursor->x = pos.x;
                        cursor->y = pos.y;
                        cursor->scale = 1;
                    }
                }
            }
        }
        if (node->scale != 0) {
            if (GameFlag_IsSet(0x103) != 0) {
                if (state->records[14].kind == 1) {
                    e->mode = 1;
                } else {
                    e->mode = 0;
                }
                if (node->kind == 1) {
                    e->mode = 1;
                }
            }
            Runtime_PushSlotEntry((s32 *)e, slot);
        }
        node = node->next;
        cnt++;
    }
    /* 二列目: 転送枠。選択中の節点に合わせて拡大縮小しながら描く。 */
    if (transfer->kind != 0) {
        struct SelectionNode *sel = NodeChain_GetNodeAtCount((u8 *)state);
        te = &transfer->sprite;
        te->mode = 0;
        te->affine = 0;
        te->param = 0;
        te->mosaic = 0;
        te->colors = 1;
        te->size = 2;
        te->shape = 0;
        te->priority = 0;
        te->tile = transfer->tile;
        te->x = sel->x - 4;
        /* FAKEMATCH: a scheduling barrier the native order needs. */
        __asm__("");
        te->y = (Data_08036740[(gFrameTick >> 1) & 15] >> 1) + sel->y - 4;
        if (transfer->scale != transfer->scale_end) {
            state->matrix[0] = transfer->scale;
            state->matrix[1] = transfer->scale;
            state->matrix[2] = 0;
            te->param = AffineMatrix_BuildForEffect((struct AffineTransform *)state->matrix);
            te->affine = 3;
            te->x = te->x + 0xfff0;
            te->y = te->y + 0xf0;
            transfer->scale = transfer->scale + transfer->scale_step;
        }
        if (GameFlag_IsSet(0x103) != 0) {
            te->mode = 1;
        }
        Runtime_PushSlotEntry((s32 *)te, 248);
    }
    MenuSelection_DrawSideMarker((u8 *)state, 0);
    MenuSelection_DrawSideMarker((u8 *)state, 1);
    /* 三列目: 縦横の移動と倍率を同時に進める節点鎖。 */
    p = state->path;
    while (p != 0) {
        e = &p->sprite;
        if (p->x != p->target_x) {
            p->x = p->x + p->dx;
        }
        if (p->y != p->target_y) {
            p->y = p->y + p->dy;
        }
        e->x = p->x;
        e->y = p->y;
        if (p->scale != p->scale_end) {
            p->scale = p->scale + p->scale_step;
            state->matrix[0] = p->scale;
            state->matrix[1] = p->scale;
            state->matrix[2] = 0;
            e->param = AffineMatrix_BuildForEffect((struct AffineTransform *)state->matrix);
            e->affine = 3;
            e->x = e->x + 0xfff8;
            e->y = e->y + 0xf8;
        } else {
            e->affine = 0;
            e->param = 0;
            /* FAKEMATCH: keeps transfer's lifetime as the native allocates it. */
            __asm__("" : "+r"(transfer));
        }
        if (GameFlag_IsSet(0x103) != 0) {
            if (state->records[14].kind == 1) {
                e->mode = 1;
            } else {
                e->mode = 0;
            }
            if (p->kind == 1) {
                e->mode = 1;
            }
        }
        p = p->next;
        Runtime_PushSlotEntry((s32 *)e, 240);
    }
    /* 四列目: カーソル本体。目標との差を毎フレーム半分ずつ詰める。 */
    if (cursor->kind != 0) {
        cursor_entry->tile = VramBlock_LoadCached(cursor->slot, 0x100, &Menu_AnimatedCursorTiles[((gFrameTick >> 2) & 15) << 8]);
        if (cursor->target_x != cursor->x) {
            tmp = (cursor->target_x - cursor->x) >> 1;
            if (tmp != 0) {
                cursor->x = cursor->x + tmp;
            } else {
                cursor->x = cursor->target_x;
            }
        }
        if (cursor->target_y != cursor->y) {
            if ((tmp = (cursor->target_y - cursor->y) >> 1) != 0) {
                cursor->y = cursor->y + tmp;
            } else {
                cursor->y = cursor->target_y;
            }
        }
        cursor_entry->y = cursor->y + Data_08036740[(gFrameTick >> 2) & 15] - 32;
        cursor_entry->x = cursor->x - 4;
        if (GameFlag_IsSet(0x103) != 0) {
            if (state->records[14].kind == 1) {
                cursor_entry->mode = 1;
            } else {
                cursor_entry->mode = 0;
            }
        }
        Runtime_PushSlotEntry((s32 *)cursor_entry, 248);
    }
    state->status += 1;
}
