#ifndef ALCHEMY_COUNTER_RUNTIME_H
#define ALCHEMY_COUNTER_RUNTIME_H

#include "types.h"

/* 各カウンタは独立して減算され、隣接する符号付き状態を持つものがある。 */
struct CounterRuntime {
    u8 unknown_000[0x42];
    u8 value_42;
    u8 unknown_043[0xef];
    u8 counter_132;
    s8 state_133;
    u8 counter_134;
    s8 state_135;
    u8 counter_136;
    s8 state_137;
    u8 counter_138;
    u8 counter_139;
    u8 counter_13a;
    u8 counter_13b;
    u8 counter_13c;
    u8 counter_13d;
};

/* 呼出し時のr0をそのまま渡すため、引数型は確定まで省略する。 */
struct CounterRuntime *Func_08077008();
s32 Func_080bf208(s32 object_id, s32 count, s32 bias);

#endif
