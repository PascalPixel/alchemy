#include "../../../include/system/flash.h"
#include "../../../include/system/flash_data.h"
#include "../../../include/system/io_reg.h"
#include "types.h"

typedef u8 (*FlashByteRead)(s32 address);

s32 ReadFlashId(void)
{
    struct {
        u8 buffer[64];
        u16 delay;
    } locals;
    FlashByteRead readByte;
    s32 result;

    CopyFlashReadRoutineToRam(locals.buffer);
    readByte = (FlashByteRead)(locals.buffer + 1);

    *(volatile u8 *)0x0e005555 = 0xaa;
    *(volatile u8 *)0x0e002aaa = 0x55;
    *(volatile u8 *)0x0e005555 = 0x90;

    {
        volatile u16 *delay = &locals.delay;

        *delay = 20000;
        while (*delay != 0)
            *delay = *delay - 1;
    }

    result = (u16)readByte(0x0e000001) << 8;
    result |= readByte(0x0e000000);

    *(volatile u8 *)0x0e005555 = 0xaa;
    *(volatile u8 *)0x0e002aaa = 0x55;
    *(volatile u8 *)0x0e005555 = 0xf0;

    {
        volatile u16 *delay = &locals.delay;

        *delay = 20000;
        while (*delay != 0)
            *delay = *delay - 1;
    }
    return result;
}

extern const struct FlashSetupInfo *const Flash_Chips[];
extern FlashWaitProc Flash_Handler3;
extern u32 Flash_Handler0;
extern u32 Flash_Handler1;
extern u32 gEraseFlashSector;
extern u16 *Flash_Handler4;

s32 IdentifyFlash(void)
{
    u16 missing;
    u16 id;
    const struct FlashSetupInfo *const *chip;

    REG_WAITCNT = (REG_WAITCNT & ~3) | 3;
    id = ReadFlashId();
    chip = Flash_Chips;
    missing = 1;
    while (((u8 *)&(*chip)->type.id)[0] != 0) {
        if (id == (*chip)->type.id) {
            missing = 0;
            break;
        }
        chip++;
    }
    Flash_Handler0 = (*chip)->handlers[0];
    Flash_Handler1 = (*chip)->handlers[1];
    gEraseFlashSector = (*chip)->handlers[2];
    Flash_Handler3 = (*chip)->wait;
    Flash_Handler4 = (*chip)->max_time;
    gFlash = &(*chip)->type;
    return missing;
}

extern volatile u16 gFlashTimerCount;
extern volatile u8 gFlashTimeoutFlag;
extern volatile u8 gFlashTimerNum;
extern u16 *volatile gFlashTimerReg;

void FlashTimerIntr(void)
{
    if (gFlashTimerCount != 0) {
        s32 v = gFlashTimerCount;

        v -= 1;
        gFlashTimerCount = v;
        if ((v << 16) == 0) {
            gFlashTimeoutFlag = 1;
        }
    }
}

s32 SetFlashTimerIntr(u8 timer_index, void (**callback)(void))
{
    if (timer_index > 3)
        return 1;
    gFlashTimerNum = timer_index;
    gFlashTimerReg = (u16 *)(0x04000100 + gFlashTimerNum * 4);
    *callback = FlashTimerIntr;
    return 0;
}

extern volatile u16 gFlashSavedIme;

void StartFlashTimer(u8 index)
{
    u16 *limit = (u16 *)((u8 *)Flash_Handler4 + index * 6);
    u16 *timer;

    gFlashSavedIme = REG_IME;
    REG_IME = 0;
    REG_IE |= 8 << gFlashTimerNum;
    REG_IME = 1;
    gFlashTimeoutFlag = 0;
    gFlashTimerCount = *limit;
    limit++;
    timer = gFlashTimerReg;
    *timer = *limit;
    timer++;
    gFlashTimerReg = timer;
    *timer = limit[1];
    timer--;
    gFlashTimerReg = timer;
}

void StopFlashTimer(void)
{
    u16 *volatile *cursor = &gFlashTimerReg;
    u16 *record = *cursor;

    *record = 0;
    record++;
    *cursor = record;
    *record = 0;
    record--;
    *cursor = record;

    REG_IME = 0;
    REG_IE &= ~(u16)(8 << gFlashTimerNum);
    REG_IME = gFlashSavedIme;
}

u8 ReadFlashByte(u8 *value)
{
    return *value;
}

void CopyFlashReadRoutineToRam(void *destination)
{
    u16 *to = destination;
    u16 *from;
    u16 count;

    gFlashReadRoutine = (u8 (*)(u8 *))((s32)to + 1);
    from = (u16 *)ReadFlashByte;
    from = (u16 *)((s32)from ^ 1);
    count = ((s32)CopyFlashReadRoutineToRam - (s32)ReadFlashByte) >> 1;
    while (count != 0) {
        *to++ = *from++;
        count--;
    }
}

void StartFlashTimer(u8 index);

s32 WaitForFlashWrite(u8 timing_index, s32 address, u8 expected)
{
    u16 result = 0;

    StartFlashTimer(timing_index);
    while ((u8)gFlashReadRoutine((u8 *)address) != expected) {
        if (gFlashTimeoutFlag) {
            if ((u8)gFlashReadRoutine((u8 *)address) == expected)
                break;
            if (gFlash->id == 0x1cc2)
                *(volatile u8 *)0x0e005555 = 0xf0;
            result = timing_index | 0xc000;
            break;
        }
    }
    StopFlashTimer();
    return result;
}

void ReadFlashCore(const u8 *src, u8 *dest, u32 size)
{
    u32 remaining = size - 1;

    if (size != 0) {
        do {
            *dest++ = *src++;
        } while (remaining-- != 0);
    }
}

struct FlashChipType {
    u32 handlers[5];
    u32 size;
    u16 sector_size;
    u16 unknown_1a;
    u8 sector_shift;
};

typedef void (*FlashReadCore)(u32 source, u8 *destination, s32 size);

extern const struct FlashChipType Flash_ChipUnknown;

void ReadFlash(u16 sector, u32 offset, u8 *destination, s32 size)
{
    u16 code[64];
    u16 *from;
    u16 *to;
    u16 count;
    FlashReadCore core;

    REG_WAITCNT = (REG_WAITCNT & 0xfffc) | 3;
    from = (u16 *)ReadFlashCore;
    from = (u16 *)((u32)from ^ 1);
    to = code;
    count = ((u32)ReadFlash - (u32)ReadFlashCore) / 2;
    while (count != 0) {
        *to++ = *from++;
        count--;
    }
    core = (FlashReadCore)((u8 *)code + 1);
    core((u32)((u8 *)0x0e000000 + (sector << Flash_ChipUnknown.sector_shift) + offset), destination, size);
}

u8 *VerifyFlashCore(u8 *left, u8 *right, u16 size)
{
    u16 index = size - 1;

    while (index != 0xFFFF) {
        u8 rightByte = *right;
        u8 leftByte = *left;

        left++;
        right++;
        if (rightByte != leftByte) {
            return right - 1;
        }
        index--;
    }
    return 0;
}

typedef s32 (*FlashVerifyCore)(s32 source, u8 *flash, u32 size);

s32 Flash_VerifySector(u16 sector, s32 source)
{
    u16 code[128];
    u16 *from;
    u16 *to;
    u16 count;
    FlashVerifyCore core;

    REG_WAITCNT = (REG_WAITCNT & 0xfffc) | 3;
    from = (u16 *)VerifyFlashCore;
    from = (u16 *)((u32)from ^ 1);
    to = code;
    count = ((u32)Flash_VerifySector - (u32)VerifyFlashCore) / 2;
    while (count != 0) {
        *to++ = *from++;
        count--;
    }
    core = (FlashVerifyCore)((u8 *)code + 1);
    return core(source, (u8 *)0x0e000000 + (sector << Flash_ChipUnknown.sector_shift), Flash_ChipUnknown.sector_size);
}
