#ifndef ALCHEMY_ABI_INVENTORY_EQUIP
#define ALCHEMY_ABI_INVENTORY_EQUIP

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08078bf0(s32 owner);
void Func_08077428(s32 owner);

#define Sys_Do Func_08078bf0
#define Sys_Do2 Func_08077428

#endif
