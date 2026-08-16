#include "types.h"

/* 送信パケットの組み立て。通信ワークは0x02002240に常駐する。
   先頭バイトに+11の識別値、次バイトに+2と+3の排他和を置き、
   本体24バイトはDMA3で引数のバッファから流し込む。
   検査値は本体を含む14ハーフワードの総和の1の補数。総和は32bitで
   取り、格納時だけ16bitへ落とす。
   +0が立っていればタイマ3を止め、+8も立っていれば割り込み有効で
   再開する。 */
void Func_080060e8(s32 source)
{
    u8 *state = (u8 *)0x02002240;
    u8 *packet;
    u16 *hw;
    s32 checksum;
    s32 index;
    s32 value;
    s32 val3;
    s32 val2;

    packet = *(u8 **)(state + 40);
    packet[0] = state[11];
    val3 = state[3];
    val2 = state[2];
    checksum = 0;
    packet[1] = val2 ^ val3;
    *(u16 *)(packet + 2) = checksum;
    packet += 4;
    {
        u32 *dma = (u32 *)0x040000d4;

        dma[0] = source;
        dma[1] = (u32)packet;
        dma[2] = 0x84000006;
    }
    /* 検査値の欄は0のまま総和に加える。 */
    hw = *(u16 **)(state + 40);
    index = 0;
    do {
        value = *hw;
        index++;
        hw++;
        checksum += value;
    } while ((u32)index <= 13);
    *(u16 *)(*(u8 **)(state + 40) + 2) = ~checksum;
    if (state[0] != 0) {
        /* タイマ3を止める。 */
        u16 *timer = (u16 *)0x0400010e;
        s32 stop = 0;

        *timer = stop;
    }
    *(s32 *)(state + 20) = -1;
    if (state[0] != 0 && state[8] != 0) {
        /* 0xC0はハーフワードの定数として溜まり、ldrで読み出される。 */
        *(u16 *)0x0400010e = 0xC0;
    }
}
