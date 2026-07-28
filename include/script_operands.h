#ifndef ALCHEMY_SCRIPT_OPERANDS_H
#define ALCHEMY_SCRIPT_OPERANDS_H

#include "types.h"

/* スクリプト演算ワーク。+0x57は比較結果の格納先。 */
struct ScriptOperands {
    u32 script_address;      /* 0x00 */
    s16 signed_halfword;     /* 0x04 */
    u16 unsigned_halfword;   /* 0x06 */
    u32 word_08;
    u32 word_0c;
    u32 word_10;
    u32 word_14;
    u32 word_18;
    u32 word_1c;
    u16 halfword_20;
    u8 padding_22[2];
    u32 word_24;
    u32 word_28;
    u32 word_2c;
    u32 word_30;
    u32 word_34;
    u32 word_38;
    u32 word_3c;
    u32 word_40;
    u32 word_44;
    u32 word_48;
    u32 word_4c;
    u32 address_50;
    u8 byte_54;
    u8 byte_55;
    u8 byte_56;
    u8 comparison_result;
    u8 byte_58;
    u8 byte_59;
    u8 byte_5a;
    u8 byte_5b;
};

#endif
