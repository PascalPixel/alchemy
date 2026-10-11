#include "unit/system/scheduler/numtext.h"
#include "types.h"
#include "system.h"

const u8 Text_HexDigits[] = "0123456789ABCDEF";

// 10 進表示の桁
const s32 Text_PowersOfTen[9] =
{
	1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10,
};
