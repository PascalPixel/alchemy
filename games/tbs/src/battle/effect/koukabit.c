#include "types.h"

//--------------------------------------------------------------------------
// 合成ルーチンの幅に足すオペランド
// ARM のデータ処理命令の即値（回転 4 ビット + 8 ビット）で 1 << n を表す。
//--------------------------------------------------------------------------

#define IMM_BIT(n) ((n) < 8 ? 1 << (n) : (12 - ((n) - 8) / 2) << 8 | 1 << ((n) & 1))

#define IMM_BITS                                                        \
	IMM_BIT(0), IMM_BIT(1), IMM_BIT(2), IMM_BIT(3),                     \
	IMM_BIT(4), IMM_BIT(5), IMM_BIT(6), IMM_BIT(7),                     \
	IMM_BIT(8), IMM_BIT(9), IMM_BIT(10), IMM_BIT(11),                   \
	IMM_BIT(12), IMM_BIT(13), IMM_BIT(14), IMM_BIT(15)

// 同じ表が二つ並ぶ。最初のものはどこからも参照されない。
const u16 SentouKouka_IroBitOperands[] = { IMM_BITS };
const u16 SentouKouka_BitOperands[] = { IMM_BITS };
