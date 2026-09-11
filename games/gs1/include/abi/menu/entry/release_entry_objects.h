#ifndef ALCHEMY_ABI_MENU_ENTRY_RELEASE_ENTRY_OBJECTS
#define ALCHEMY_ABI_MENU_ENTRY_RELEASE_ENTRY_OBJECTS

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08077158(void *);
void Func_08009038(void *);

#define Menu_Check Func_08077158
#define Menu_Do Func_08009038
#define gIw Data_03001f2c
#define gRom Data_080a19a1

#endif
