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

s32 CountRemainingErasedFlashBytes(u8 *ptr);
s32 RunFlashEraseVerifier(u8 *ptr, s32 (*verify)(u8 *));

s32 _call_via_r3(s32, u8 *, u8, s32);

u16 EraseFlashChip(void)
{
    u8 savedCode[64];
    u16 result;
    s32 *status;

    REG_WAITCNT =
        (REG_WAITCNT & 0xFFFC) |
        gFlash->wait[0];

    *(volatile u8 *)0x0E005555 = 0xAA;
    *(volatile u8 *)0x0E002AAA = 0x55;
    *(volatile u8 *)0x0E005555 = 0x80;
    *(volatile u8 *)0x0E005555 = 0xAA;
    *(volatile u8 *)0x0E002AAA = 0x55;
    *(volatile u8 *)0x0E005555 = 0x10;

    CopyFlashReadRoutineToRam(savedCode);

    status = (s32 *)&Flash_Handler3;
    result = _call_via_r3(3, (u8 *)0x0E000000, 255, *status);

    REG_WAITCNT =
        (REG_WAITCNT & 0xFFFC) | 3;

    return result;
}

u16 EraseFlashSector(u16 sector)
{
    u8 savedCode[64];
    u16 result;
    u16 setting;
    s32 *status;
    const struct FlashType *chip;
    volatile u16 *waitcnt;
    volatile u8 *command;
    volatile u8 *unlock;
    volatile u8 *target;

    if (sector > 15) {
        return 0x80FF;
    }

    waitcnt = (volatile u16 *)0x04000204;
    setting = *waitcnt & 0xFFFC;
    chip = gFlash;
    *waitcnt = setting | chip->wait[0];

    target = (volatile u8 *)((sector << chip->sector.shift) + 0x0E000000);

    command = (volatile u8 *)0x0E005555;
    *command = 0xAA;
    unlock = (volatile u8 *)0x0E002AAA;
    *unlock = 0x55;
    *command = 0x80;
    *command = 0xAA;
    *unlock = 0x55;
    *target = 0x30;

    CopyFlashReadRoutineToRam(savedCode);

    status = (s32 *)&Flash_Handler3;
    result = _call_via_r3(2, (u8 *)target, 255, *status);

    *waitcnt = (*waitcnt & 0xFFFC) | 3;

    return result;
}

u16 ProgramFlashByte(u8 *source, u8 *destination)
{
    FlashWaitProc *wait;

    *(volatile u8 *)0x0e005555 = 0xaa;
    *(volatile u8 *)0x0e002aaa = 0x55;
    *(volatile u8 *)0x0e005555 = 0xa0;
    *destination = *source;
    wait = &Flash_Handler3;
    return (*wait)(1, destination, *source);
}

extern u16 gFlashNumRemainingBytes;

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

s32 CountRemainingErasedFlashBytes(u8 *ptr)
{
    s32 remaining;

    for (remaining = gFlash->sector.size; remaining != 0; remaining--) {
        if (*ptr++ != 0xff)
            break;
    }
    return remaining;
}

s32 RunFlashEraseVerifier(u8 *ptr, s32 (*verify)(u8 *))
{
    if (verify(ptr) != 0)
        return 0x8004;
    return 0;
}

u16 ProgramFlashSector(u16 sector, u8 *source)
{
    u8 savedCode[64];
    u16 result;
    u8 *destination;

    if ((u32)sector > 15)
        return 0x80FF;

    result = EraseFlashSector(sector);
    if (result != 0)
        return result;

    CopyFlashReadRoutineToRam(savedCode);
    REG_WAITCNT =
        (REG_WAITCNT & 0xFFFC) |
        gFlash->wait[0];
    gFlashNumRemainingBytes =
        gFlash->sector.size;
    destination = (u8 *)(
        (sector << gFlash->sector.shift) +
        0x0E000000);

    while (gFlashNumRemainingBytes != 0) {
        result = ProgramFlashByte(source, destination);
        if (result != 0)
            break;
        gFlashNumRemainingBytes -= 1;
        source++;
        destination++;
    }

    REG_WAITCNT =
        (REG_WAITCNT & 0xFFFC) | 3;

    return result;
}

u16 EraseAtmelFlashChip(void)
{
    u8 savedCode[64];
    u16 result;
    volatile u8 *command;
    volatile u8 *unlock;

    CopyFlashReadRoutineToRam(savedCode);

    REG_WAITCNT =
        (REG_WAITCNT & 0xFFFC) |
        Flash_ChipAtmelLayout.type.wait[0];

    command = (volatile u8 *)0x0E005555;
    *command = 0xAA;
    unlock = (volatile u8 *)0x0E002AAA;
    *unlock = 0x55;
    *command = 0x80;
    *command = 0xAA;
    *unlock = 0x55;
    *command = 0x10;

    result = Flash_Handler3(3, (u8 *)0x0E000000, 255);

    REG_WAITCNT =
        (REG_WAITCNT & 0xFFFC) | 3;

    return result;
}

u16 EraseAtmelFlashSector(u16 sector)
{
    struct FlashSetupInfo *info = &Flash_ChipAtmelLayout;
    u8 *cursor;
    u16 saved;
    s32 count;
    u16 result;

    cursor = (u8 *)(0x0E000000 + (sector << info->type.sector.shift));
    saved = REG_IME;
    REG_IME = 0;
    *(volatile u8 *)0x0E005555 = 0xAA;
    *(volatile u8 *)0x0E002AAA = 0x55;
    *(volatile u8 *)0x0E005555 = 0xA0;
    count = info->type.sector.size;
    while (count != 0) {
        *cursor = 0xFF;
        cursor++;
        count--;
    }
    cursor--;
    REG_IME = saved;
    result = Flash_Handler3(1, cursor, 0xFF);
    if (result != 0) {
        result = (result & 0xFF00) | 2;
    }
    return result;
}

u16 EraseAtmelFlashBlock(u16 sector)
{
    u8 savedCode[64];
    u16 result;
    u16 current;
    u16 retries;
    u16 count;

    if ((u32)sector > 15)
        return 0x80ff;

    CopyFlashReadRoutineToRam(savedCode);
    REG_WAITCNT =
        (REG_WAITCNT & 0xfffc) |
        Flash_ChipAtmelLayout.type.wait[0];

    current = sector << 4;
    current = (u16)(current << 1);
    count = 0;
next_sector:
    retries = 2;
    goto attempt;
retry_failed:
    retries -= 1;
    if (retries == 0)
        goto advance;
attempt:
    result = EraseAtmelFlashSector(current);
    if (result != 0)
        goto retry_failed;
advance:
    current += 1;
    if (result != 0)
        goto done;
    count += 1;
    if ((u32)count <= 31)
        goto next_sector;

done:
    REG_WAITCNT =
        (REG_WAITCNT & 0xfffc) | 3;
    return result;
}

u16 ProgramAtmelFlashSector(u32 slot, const u8 *source)
{
    const u8 *input;
    volatile u8 *destination;
    u32 saved_ime;
    s32 remaining;

    input = source;
    slot = (u16)slot;
    destination = (volatile u8 *)(
        0x0e000000 + (slot << Flash_ChipAtmelLayout.type.sector.shift));
    saved_ime = REG_IME;
    REG_IME = 0;
    *(volatile u8 *)0x0e005555 = 0xaa;
    *(volatile u8 *)0x0e002aaa = 0x55;
    *(volatile u8 *)0x0e005555 = 0xa0;

    remaining = Flash_ChipAtmelLayout.type.sector.size;
    if (remaining != 0) {
        do {
            *destination++ = *input++;
            remaining--;
        } while (remaining != 0);
    }

    destination--;
    input--;
    REG_IME = saved_ime;
    return Flash_Handler3(1, destination, *input);
}

u16 ProgramAtmelFlashBlock(u16 sector, u8 *source)
{
    u8 savedCode[64];
    u16 result;
    u16 current;
    u16 retries;

    if ((u32)sector > 15)
        return 0x80ff;

    CopyFlashReadRoutineToRam(savedCode);
    REG_WAITCNT =
        (REG_WAITCNT & 0xfffc) |
        Flash_ChipAtmelLayout.type.wait[0];

    current = sector << 5;
    gFlashNumRemainingBytes = Flash_ChipAtmel.type.sector.size;

    while (gFlashNumRemainingBytes != 0) {
        retries = 2;
        goto attempt;
        do {
retry_failed:
            retries -= 1;
            if (retries == 0)
                break;
attempt:
            result = ProgramAtmelFlashSector(current, source);
        } while (result != 0);

        if (result != 0)
            break;

        gFlashNumRemainingBytes -= Flash_ChipAtmelLayout.type.sector.size;
        source += Flash_ChipAtmelLayout.type.sector.size;
        current += 1;
    }

    REG_WAITCNT =
        (REG_WAITCNT & 0xfffc) | 3;
    return result;
}
