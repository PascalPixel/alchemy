#include "types.h"

/* 0x02004c08 が指すフラッシュ機種表。 */
struct FlashChip08006e24 {
    u8 unknown_00[4];
    s32 blockSize;
    u8 sectorShift;
    u8 unknown_09[7];
    u16 wait;
};

/* RAM 常駐の書き込み手続き。0x08006f6c との差が複写長になる。 */
void Func_08006f48(void);
s32 Func_08006f6c(u8 *, u8 *);
s32 Func_08006d50(u16);
void Func_08006ac0(void *);
s32 Func_08006dec(u8 *, u8 *);

/* セクタ消去つき書き込み。RAM へ手続きを複写し、消去と検査が通るまで
   最大 81 回やり直してから 1 バイトずつ書く。 */
u16 Func_08006e24(u16 sector, u8 *source)
{
    u8 ramCode[96];
    u16 status;
    u16 remaining;
    u16 *sourceCode;
    u16 *destinationCode;
    u16 *counter;
    u8 attempt;
    u8 index;
    u8 rounds;
    struct FlashChip08006e24 *chip;
    volatile u16 *waitcnt;
    u16 setting;
    u8 *destination;

    if (sector > 15) {
        return 0x80FF;
    }

    destination = (u8 *)((sector << (*(struct FlashChip08006e24 **)0x02004C08)->sectorShift)
                         + 0x0E000000);

    sourceCode = (u16 *)((s32)Func_08006f48 ^ 1);
    destinationCode = (u16 *)ramCode;
    remaining = (u16)((s32)Func_08006f6c - (s32)Func_08006f48);
    while (remaining != 0) {
        *destinationCode = *sourceCode;
        sourceCode++;
        destinationCode++;
        remaining = remaining - 2;
    }

    attempt = 0;
    for (;;) {
        status = Func_08006d50(sector);
        if (status == 0) {
            status = Func_08006f6c(destination, ramCode + 1);
            if (status == 0) {
                break;
            }
        }
        attempt = attempt + 1;
        if (attempt == 81) {
            return status;
        }
    }

    rounds = 1;
    if (attempt != 0) {
        rounds = 6;
    }
    index = 1;
    while (index <= rounds) {
        Func_08006d50(sector);
        index = index + 1;
    }

    Func_08006ac0(ramCode);

    waitcnt = (volatile u16 *)0x04000204;
    setting = *waitcnt & 0xFFFC;
    chip = *(struct FlashChip08006e24 **)0x02004C08;
    *waitcnt = setting | chip->wait;

    *(u16 *)0x02004C0C = chip->blockSize;
    counter = (u16 *)0x02004C0C;
    while (*counter != 0) {
        status = Func_08006dec(source, destination);
        if (status != 0) {
            break;
        }
        *counter = *counter - 1;
        source++;
        destination++;
    }

    *(volatile u16 *)0x04000204 =
        (*(volatile u16 *)0x04000204 & 0xFFFC) | 3;

    return status;
}
