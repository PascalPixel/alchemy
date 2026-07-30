#include "types.h"

/*
 * サウンドドライバ作業領域の初期化モジュール。
 *
 * このファイルは二つの実行範囲をまとめて再構築する:
 *   Func_08015fb8 (0x08015fb8, 72バイト実行部) — VRAM タイル転送ヘルパ
 *   Func_08016018 (0x08016018, 172バイト実行部) — 作業領域の確保と初期化
 *
 * 併記の理由: 元のコードは Func_08016018 から Func_08015fb8 へ第三引数を
 * r9 で渡す。r9 は呼び出し規約外のレジスタなので C では表現できないが、
 * Func_08015fb8 の呼び出し元はツリー全体でこの関数だけ (三箇所) であり、
 * その受け渡しはこのモジュール内部で完結している。したがって明示的な
 * 引数として綴るのが忠実な再構築になる。
 */

struct DmaChannel_08016018 {
    const void *source;
    void *destination;
    u32 control;
};

/*
 * 0x03000164 は IWRAM に再配置された ARM ヘルパの入口アドレス。元のコードは
 * r3 に定数 0x03000164 を積んで `bl Func_080072f0` (call-via-r3 スランク)
 * に入る。すなわち関数ポインタ呼び出しであり、r0/r1/r2 が引数、四番目の
 * 引数はスタック経由で渡される。
 *
 * 未確定: このヘルパが第四引数のポインタをどう使うかは未解析。少なくとも
 * 書き込みはしない (下記 Func_08016018 の後始末がその語から r9 を復元する)。
 */
typedef void (*IwramHelper_08015fb8)(void *destination, u32 length,
                                     u32 control, const void *context);

void Func_08015ef4(void);
void *Func_080048f4(s32 resource, u32 size);
void Func_080041d8(const void *source, s32 size);
void Func_08017464(s32 initialize);

/*
 * タイル 32 バイト単位。source_tile の後半 16 バイトを destination_tile の
 * 前半へ転送してから、再配置ヘルパを呼ぶ。
 *
 * 元のコードでは context は r9 で渡される。呼び出し元は下の Func_08016018
 * だけなので、ここでは通常の引数として綴る。
 */
void Func_08015fb8(u32 source_tile, u32 destination_tile, const void *context)
{
    struct DmaChannel_08016018 *dma3 =
        (struct DmaChannel_08016018 *)0x040000d4;
    IwramHelper_08015fb8 helper = (IwramHelper_08015fb8)0x03000164;
    u32 source_offset = (source_tile & 0x3ff) << 5;
    u32 destination_offset = (destination_tile & 0x3ff) << 5;

    dma3->source = (const void *)(0x06000010 + source_offset);
    dma3->destination = (void *)(0x06000000 + destination_offset);
    dma3->control = 0x80000008;

    /*
     * 未確定: 20 と 0x80000008 の意味。0x80000008 は DMA 制御語と同じ形を
     * しているが、ここでは第三引数としてヘルパに渡されるだけである。
     */
    helper((void *)(0x0600000c + destination_offset), 20, 0x80000008, context);
}

void Func_08016018(s32 initialize)
{
    struct DmaChannel_08016018 *dma3 =
        (struct DmaChannel_08016018 *)0x040000d4;
    /*
     * frame[0] は DMA の固定元アドレスに使う一語。
     *
     * frame[1] は元のコードでは r9 の退避スロットそのもので、そのアドレス
     * (caller_sp+4 相当) が三回の Func_08015fb8 呼び出しに context として
     * 渡される。したがってヘルパが読む値は「この関数の呼び出し元が持って
     * いた r9」であり、C では書けない。ヘルパ側が書き込まないことは
     * 元の後始末が同じ語から r9 を復元することで確かめられる。
     * 未確定: ヘルパがその語を読むのか、単に一時領域の起点として使うのか。
     */
    u32 frame[2];
    u8 *work;
    s32 index;

    frame[0] = 0;
    work = Func_080048f4(15, 0x12fc);

    /* 確保した 0x12fc バイトを 0 で埋める (32ビット、元アドレス固定)。 */
    dma3->source = &frame[0];
    dma3->destination = work;
    dma3->control = 0x850004bf;

    work[0xea3] = 1;
    *(u16 *)(work + 0x12b6) = 99;
    work[0xea5] = 1;
    work[0xea7] = 15;

    /* 先頭 0x500 バイトを 0xf000f000 で埋める。 */
    frame[0] = 0xf000f000;
    dma3->source = &frame[0];
    dma3->destination = work;
    dma3->control = 0x85000140;

    Func_08015ef4();
    Func_080041d8((const void *)0x080160fd, 144 << 3);
    Func_08017464(initialize);

    Func_08015fb8(0xf013, 128, &frame[1]);
    Func_08015fb8(0xf014, 129, &frame[1]);
    Func_08015fb8(0xf015, 130, &frame[1]);

    for (index = 0; index < 3; index++)
        work[0xda2 - index] = 4;
}
