/* Draft: Script_ApplyOperandSet, Script_ApplyOperandAdd and
   Script_ApplyOperandCompare (0x08026278, 168 bytes). Each calls its callback as
   mov lr, r3 and a lone bl suffix (0xf800), which stock GCC 2.96 does not emit:
   4 bytes differ at +0x24 of each. Links as recon/tla/raw/08026278.s until the source or
   compiler that produces that call is known. */
#include "SCRIPT_OPERANDS.H"

typedef void (*OperandFunc)(struct ScriptOperands *, s32, s32);
extern OperandFunc Data_0802f2dc[];

s32 Script_ApplyOperandSet(struct ScriptOperands *work)
{
    s16 index = (s16)work->cursor;
    u8 *entry = (u8 *)(work->script_address + index * 4 + 4);
    OperandFunc callback = Data_0802f2dc[*(s32 *)entry];

    if (callback != 0)
        callback(work, 0, *(s32 *)(entry + 4));
    work->cursor += 3;
    return 1;
}

s32 Script_ApplyOperandAdd(struct ScriptOperands *work)
{
    s16 index = (s16)work->cursor;
    u8 *entry = (u8 *)(work->script_address + index * 4 + 4);
    OperandFunc callback = Data_0802f2dc[*(s32 *)entry];

    if (callback != 0)
        callback(work, 1, *(s32 *)(entry + 4));
    work->cursor += 3;
    return 1;
}

s32 Script_ApplyOperandCompare(struct ScriptOperands *work)
{
    s16 index = (s16)work->cursor;
    u8 *entry = (u8 *)(work->script_address + index * 4 + 4);
    OperandFunc callback = Data_0802f2dc[*(s32 *)entry];

    if (callback != 0)
        callback(work, 2, *(s32 *)(entry + 4));
    work->cursor += 3;
    return 1;
}
