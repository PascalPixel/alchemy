#include "../../../include/system/flash.h"
#include "../../../include/system/flash_data.h"
#include "../../../include/system/io_reg.h"

extern u16 gFlashNumRemainingBytes;

void CountRemainingErasedFlashBytes(void);
s32 RunFlashEraseVerifier(u8 *, u8 *);

u16 ProgramFlashSectorWithEraseRetry(u16 sector, u8 *source)
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
    const struct FlashType *chip;
    volatile u16 *waitcnt;
    u16 setting;
    u8 *destination;

    if (sector > 15) {
        return 0x80FF;
    }

    destination = (u8 *)((sector << gFlash->sector.shift)
                         + 0x0E000000);

    sourceCode = (u16 *)((s32)CountRemainingErasedFlashBytes ^ 1);
    destinationCode = (u16 *)ramCode;
    remaining = (u16)((s32)RunFlashEraseVerifier -
                      (s32)CountRemainingErasedFlashBytes);
    while (remaining != 0) {
        *destinationCode = *sourceCode;
        sourceCode++;
        destinationCode++;
        remaining = remaining - 2;
    }

    attempt = 0;
    for (;;) {
        status = EraseFlashSector(sector);
        if (status == 0) {
            status = RunFlashEraseVerifier(destination, ramCode + 1);
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
        EraseFlashSector(sector);
        index = index + 1;
    }

    CopyFlashReadRoutineToRam(ramCode);

    waitcnt = &REG_WAITCNT;
    setting = *waitcnt & 0xFFFC;
    chip = gFlash;
    *waitcnt = setting | chip->wait[0];

    gFlashNumRemainingBytes = chip->sector.size;
    counter = &gFlashNumRemainingBytes;
    while (*counter != 0) {
        status = ProgramFlashByte(source, destination);
        if (status != 0) {
            break;
        }
        *counter = *counter - 1;
        source++;
        destination++;
    }

    REG_WAITCNT =
        (REG_WAITCNT & 0xFFFC) | 3;

    return status;
}
