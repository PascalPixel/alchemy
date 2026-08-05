#include "types.h"

/*
 * 戦闘導入の演出振り分け。selector で四通りの導入を選び、前二つは自前で
 * 毎フレームのスクロール転送を積む。
 *
 * ordinary-blockers.json は「二つの live な r2 は意図的な呼び出し先残渣」と
 * していたが、今回の再確認では二件とも成立しなかった。
 *
 * (1) Func_080c1470 は入口が `ldr r3,[pc,..] / ldr r2,[r3,#0]` で、r2 を
 *     読む前に上書きする。引数は r0 一本だけで、Func_0800387c の残渣を
 *     消費していない。
 *
 * (2) 0x03000164 の再配置ヘルパ (call-via-r3 スランク経由) は、この像の
 *     三箇所すべてで r2 を設定せずに呼ばれる — ここの二箇所と
 *     Func_08015fb8 の一箇所である。三箇所とも直前の値の残りであり、
 *     どれも意図して積まれていない。したがってヘルパの引数は
 *     r0 (宛先) と r1 (長さ) の二本と読むのが最も素直で、r2 は残渣では
 *     なく単に未使用である。
 *     未確定: ヘルパ本体は IWRAM に再配置されるため像から読めない。
 *     この二引数解釈は三つの呼び出し側の一致に基づく推定である。
 *
 * その帰結として src/080c16d0.c の第三引数 `mode` は、どの呼び出し側も
 * 設定しない引数である。ここでは原典と同じく引数一本で呼び、宣言を
 * 原型なしにして「渡していない」ことを明示する。
 */

typedef struct Point_080c1798 {
    s32 x;
    s32 y;
} Point_080c1798;

/* 0x02002090 の転送待ち行列。Func_0800387c が積むものと同じ形。 */
typedef struct DmaRequest_080c1798 {
    void *source;
    void *destination;
    u32 control;
} DmaRequest_080c1798;

typedef struct DmaQueue_080c1798 {
    u16 count;
    u16 unknown_02;
    DmaRequest_080c1798 entries[32];
} DmaQueue_080c1798;

/* Func_080c9020 / Func_080c9030 が受け取る作業域 (84バイト)。 */
typedef struct Work_080c1798 {
    s32 flags;
    s32 unknown_04;
    s32 primary_id;
    s32 secondary_id;
    s32 field_10;
    s32 count;
    s32 mode;
    s32 unknown_1c;
    u8 padding_20[4];
    s16 members[24];
} Work_080c1798;

/* 0x03000164 に再配置された ARM ヘルパ。VRAM の消去に使う。 */
typedef void (*VramFill_080c1798)(void *destination, u32 length);

void Func_080030f8(u32 frames);
void Func_080c0774(s32 mode, s32 unused, s32 fade);
void Func_0800387c(u32 destination, u32 source);
void Func_080c1470(s32 scene);
void Func_080c1724(void *source, void *destination, s32 scale, s32 count);
void Func_080b845c(s32 focus, Point_080c1798 *position);
void Func_080c9038(s32 scene);
void Func_080c9040(void);
void Func_080c9020(Work_080c1798 *work);
void Func_080c9030(Work_080c1798 *work);

/* 原型なし: この呼び出し側は引数を一本しか渡さない (上の註記を参照)。 */
s32 Func_080c16d0();

#define IME_080C1798 (*(volatile u16 *)0x04000208)

/*
 * 注視点から BG2 のアフィン原点を求め、その二語を 0x04000028 へ送る転送を
 * 待ち行列に積んで一フレーム待つ。二つの導入ループが共有する本体。
 */
static void SubmitScroll_080c1798(s32 focus)
{
    u8 *render = *(u8 **)0x03001eec;
    DmaQueue_080c1798 *queue = (DmaQueue_080c1798 *)0x02002090;
    Point_080c1798 position;
    u16 saved_ime;

    Func_080b845c(focus, &position);
    *(s32 *)(render + 0x13c4) = (64 - position.x) << 8;
    *(s32 *)(render + 0x13c8) = (64 - position.y) << 8;

    saved_ime = IME_080C1798;
    /* 元のコードは IME のアドレスそのものを書く。下位ビットが 0 なので禁止。 */
    IME_080C1798 = (u16)0x04000208;
    if (queue->count <= 31) {
        DmaRequest_080c1798 *entry = &queue->entries[queue->count];

        entry->source = render + 0x13c4;
        queue->count++;
        entry->destination = (void *)0x04000028;
        entry->control = 0x84000002;
    }
    IME_080C1798 = saved_ime;

    *(s32 *)(render + 0x13cc) = 1;
    Func_080030f8(1);
}

void Func_080c1798(s32 focus, s32 scene, s32 selector, s32 mode)
{
    u8 *work = *(u8 **)0x03001e74;

    Func_080030f8(1);
    Func_080c0774(1, *(u16 *)(work + 0x648), 0);
    ((VramFill_080c1798)0x03000164)((void *)0x06004000, 0x4000);

    Func_0800387c(0x04000000, 0x3741);
    Func_0800387c(0x0400000c, 0x784);
    Func_0800387c(0x04000050, 0x3f44);
    Func_080030f8(1);

    *(volatile u16 *)0x04000040 = 240;    /* WIN0H */
    *(volatile u16 *)0x04000044 = 0x1088; /* WIN0V */
    *(volatile u16 *)0x04000048 = 63;     /* WININ */
    *(volatile u16 *)0x0400004a = 17;     /* WINOUT */

    if (selector == 0) {
        s32 *fade = (s32 *)(work + 0x644);
        s32 step = 0;
        u32 index;

        Func_0800387c(0x04000052, 0x100e);
        Func_080c1470(scene);

        for (index = 0; index <= 44; index++) {
            if (index <= 24) {
                *fade = 0x10000 - step;
                Func_080c1724(work + 0x544, (void *)0x050000c0,
                              0x10000 - step, 128);
            }
            SubmitScroll_080c1798(focus);
            step += 0x444;
        }

        Func_080c16d0(scene);
    } else if (selector == 1) {
        u32 index;

        Func_080c9038(scene);
        for (index = 39; index >= 0; index--)
            SubmitScroll_080c1798(focus);
        Func_080c9040();
    } else if (selector == 2) {
        Work_080c1798 request;

        request.flags = scene;
        request.primary_id = focus;
        request.secondary_id = focus;
        request.field_10 = 1;
        request.count = 1;
        request.mode = mode;
        request.unknown_1c = 0;
        request.members[0] = (s16)focus;
        Func_080c9020(&request);
    } else {
        Work_080c1798 request;

        request.flags = scene;
        request.primary_id = focus;
        request.secondary_id = focus;
        request.field_10 = 1;
        request.count = 1;
        request.mode = 0;
        request.unknown_1c = 0;
        request.members[0] = (s16)focus;
        Func_080c9030(&request);
    }
}
