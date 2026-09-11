#ifndef ALCHEMY_ABI_MENU_ENTRY_CREATE_WINDOW_AND_ENTRY_OBJECTS
#define ALCHEMY_ABI_MENU_ENTRY_CREATE_WINDOW_AND_ENTRY_OBJECTS

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080a8604(s32, s32, s32);
s32 Func_080a9cf8(s32 resource);

#define Menu_Check Func_080a9cf8
#define Menu_Place Func_080a8604
#define Menu_Run Func_080150d8
#define Menu_Run2 Func_080a10d0
#define Menu_Run3 Func_080a33d4

#endif
