#include "types.h"

/*
 * 対戦通信で行動表を交換するモジュール。三つの実行範囲をまとめて再構築する:
 *
 *   Func_080b9554 (0x080b9554, 172バイト実行部) — 送信
 *   Func_080b9604 (0x080b9604, 280バイト実行部) — 受信
 *   Func_080b9724 (0x080b9724 + 0x080b9890, 344 + 36バイト) — 交換の本体
 *
 * 併記の理由: 元のコードは Func_080b9724 が自分のローカル領域の終端
 * (sp+20) を r9 に載せ、二つの下請けがそこから負のオフセットで
 * count/result/records/size/header を読む。r9 は呼び出し規約外なので
 * そのままでは C にならないが、実体は「呼び出し元のローカル構造体への
 * ポインタ」でしかない。下請けの呼び出し元はツリー全体でこの関数だけ
 * なので、明示的な構造体ポインタ引数として綴るのが忠実な再構築になる。
 * Func_080b9604 が途中で r9 を r9-16 に張り替えるのも、同じ構造体の
 * result 欄を指しているだけである。
 */

struct BattleDecision_080b9724 {
    s16 owner;
    u16 pending_owner;
    u16 weight;
    s16 state;
    u16 action;
    s16 target;
    u8 unknown_0c[4];
};

/* 40バイト確保されるが、通信に載るのは先頭 20 バイトだけ。 */
struct ExchangeHeader_080b9724 {
    s32 count;
    s32 identity;
    s32 timer;
    u8 unknown_0c[28];
};

/*
 * Func_080b9724 のローカル領域そのもの。元のコードでは r9 がこの構造体の
 * 直後 (sp+20) を指し、下請けが r9[-20]..r9[-4] として各欄を読む。
 */
struct Exchange_080b9724 {
    s32 count;                              /* r9 - 20 */
    s32 result;                             /* r9 - 16 */
    struct BattleDecision_080b9724 *records; /* r9 - 12 */
    s32 size;                               /* r9 -  8 */
    struct ExchangeHeader_080b9724 *header;  /* r9 -  4 */
};

u32 Func_080022f4(s32 numerator, s32 denominator);
void *Func_08004970(s32 size);
s32 Func_08002df0(void *buffer);
s32 Func_080771a0(void);
void Func_080030f8(u32 frames);

/* 通信層。 */
s32 Func_080063bc(const void *buffer, s32 size);  /* 送信要求 */
s32 Func_08006408(void *buffer);                  /* 受信要求 */
s32 Func_080064f4(void);                          /* 転送中なら非0 */
void Func_0800651c(void);
void Func_08006358(void);

#define LINK_STATUS_080B9724   (*(volatile u16 *)0x03001f64)
#define LINK_RECEIVED_080B9724 (*(volatile u16 *)0x02002238)

/* 20バイト単位に切り上げる。元のコードは count*16+19 を 20 で割ってから
 * 20 倍する。 */
static s32 RoundedSize_080b9724(s32 count)
{
    return (s32)Func_080022f4(count * 16 + 19, 20) * 20;
}

/*
 * 転送が終わるまで待つ。戻り値 0 で正常終了、-1 で打ち切り。
 * frames/idle は呼び出し側で共有される打ち切り条件。
 */
static s32 WaitForTransfer_080b9724(s32 *frames, s32 *idle)
{
    while (Func_080064f4() != 0) {
        Func_080030f8(1);
        (*frames)--;
        if (*frames < 0)
            return -1;
        if ((LINK_STATUS_080B9724 & 3) == 3)
            *idle = 0;
        else if (++(*idle) > 24)
            return -1;
    }
    return 0;
}

/* 送信: ヘッダ 20 バイトに続けて行動表を送る。 */
s32 Func_080b9554(struct Exchange_080b9724 *exchange)
{
    s32 frames = 300;
    s32 idle = 0;

    if (Func_080063bc(exchange->header, 20) == -1)
        return -1;
    if (WaitForTransfer_080b9724(&frames, &idle) < 0)
        return -1;

    if (exchange->size != 0) {
        if (Func_080063bc(exchange->records, exchange->size) == -1)
            return -1;
        if (WaitForTransfer_080b9724(&frames, &idle) < 0)
            return -1;
    }
    return 0;
}

/* 受信: ヘッダ 20 バイトを受けてから、告げられた件数だけ行動表を受ける。 */
s32 Func_080b9604(struct Exchange_080b9724 *exchange)
{
    s32 frames = 300;
    s32 idle = 0;
    s32 expected;

    if (Func_08006408(exchange->header) == -1)
        return -1;

    while (Func_080064f4() != 0) {
        if (LINK_RECEIVED_080B9724 > 20)
            return -1;
        Func_080030f8(1);
        if (--frames < 0)
            return -1;
        if ((LINK_STATUS_080B9724 & 3) == 3)
            idle = 0;
        else if (++idle > 24)
            return -1;
    }
    if (LINK_RECEIVED_080B9724 != 20)
        return -1;

    exchange->result = exchange->header->count;
    if (exchange->header->count == 0)
        return 0;

    if (Func_08006408(&exchange->records[exchange->count]) == -1)
        return -1;

    expected = RoundedSize_080b9724(exchange->result);
    while (Func_080064f4() != 0) {
        if ((s32)LINK_RECEIVED_080B9724 > expected)
            return -1;
        Func_080030f8(1);
        if (--frames < 0)
            return -1;
        if ((LINK_STATUS_080B9724 & 3) == 3)
            idle = 0;
        else if (++idle > 24)
            return -1;
        expected = RoundedSize_080b9724(exchange->result);
    }

    if ((s32)LINK_RECEIVED_080B9724 != RoundedSize_080b9724(exchange->result))
        return -1;
    return 0;
}

s32 Func_080b9724(struct BattleDecision_080b9724 *records, s32 count)
{
    u8 *battle = *(u8 **)0x03001e74;
    struct Exchange_080b9724 exchange;
    s32 index;

    exchange.count = count;
    exchange.result = 0;
    exchange.records = records;
    exchange.size = RoundedSize_080b9724(count);
    exchange.header = Func_08004970(40);

    for (index = 0; index < count; index++) {
        struct BattleDecision_080b9724 *record = &records[index];

        record->pending_owner = battle[72 + record->owner];
        if (battle[80] != 0) {
            record->weight |= 1;
        } else if ((record->weight & 1) != 0) {
            record->weight++;
        }
    }

    if (battle[82] != 0)
        goto failed;

    if (battle[80] == 0) {
        u16 interrupt_master;

        exchange.header->count = count;
        exchange.header->identity = Func_080771a0();

        interrupt_master = *(volatile u16 *)0x04000208;
        *(volatile u16 *)0x04000208 = 0;
        exchange.header->timer = *(volatile u32 *)0x03001cb4;
        *(u32 *)0x020023a8 = exchange.header->timer;
        *(volatile u16 *)0x04000208 = interrupt_master;

        if (Func_080b9554(&exchange) < 0)
            goto failed;
        if (Func_080b9604(&exchange) < 0)
            goto failed;
        exchange.result = exchange.header->count;
    } else {
        if (Func_080b9604(&exchange) < 0)
            goto failed;
        exchange.result = exchange.header->count;
        exchange.header->count = count;
        if (Func_080b9554(&exchange) < 0)
            goto failed;
        if (Func_080771a0() != exchange.header->identity)
            goto failed;
        *(u32 *)0x020023a8 = exchange.header->timer;
    }

    for (index = 0; index < exchange.result; index++) {
        struct BattleDecision_080b9724 *record = &records[count + index];

        record->owner = (s16)record->pending_owner;
        record->target ^= 0x80;
    }

    Func_08002df0(exchange.header);
    return exchange.result;

failed:
    Func_0800651c();
    Func_08006358();
    Func_08002df0(exchange.header);
    return -1;
}
