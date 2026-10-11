#ifndef GUARD_FLASH_DATA_H
#define GUARD_FLASH_DATA_H

#include "types.h"

// セクタの大きさと数
struct FlashSector
{
	s32 size;
	u8	shift;
	u16 count;
	u16 top;
};

// フラッシュ型番ごとの設定 (44 バイト)。flashset.s の flash_setup と同じ並び
struct FlashSetupInfo
{
	u32 handlers[5]; // 書き込み, 全消去, セクタ消去, 書き込み待ち, 待ち時間の表
	u32 rom_size;
	struct FlashSector sector;
	u16 wait[2]; // REG_WAITCNT の下位 2 ビット
	u16 id;
};

extern struct FlashSetupInfo Flash_ChipAtmel;
extern struct FlashSetupInfo Flash_ChipAtmelLayout;

typedef u16 (*FlashWaitProc)(s32 mode, volatile u8 *address, u8 expected);

#endif
