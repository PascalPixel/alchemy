#ifndef GUARD_FLASH_DATA_H
#define GUARD_FLASH_DATA_H

#include "types.h"

typedef u16 (*FlashWaitProc)(s32 mode, volatile u8 *address, u8 expected);

// セクタの大きさと数
struct FlashSector
{
	s32 size;
	u8	shift;
	u16 count;
	u16 top;
};

// フラッシュの種類 (gFlash が指す 24 バイト)
struct FlashType
{
	u32 romSize;
	struct FlashSector sector;
	u16 wait[2]; // REG_WAITCNT の下位 2 ビット
	u16 id;
};

// フラッシュ型番ごとの設定 (44 バイト)。flashset.s の flash_setup と同じ並び
struct FlashSetupInfo
{
	u32 handlers[3]; // 書き込み, 全消去, セクタ消去
	FlashWaitProc wait; // 書き込み待ち
	u16 *maxTime; // 待ち時間の表
	struct FlashType type;
};

extern const struct FlashType *gFlash;
extern struct FlashSetupInfo Flash_ChipAtmel;
extern struct FlashSetupInfo Flash_ChipAtmelLayout;

#endif
