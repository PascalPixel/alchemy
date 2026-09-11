#ifndef ALCHEMY_ABI_MENU_ENTRY_UPDATE_ENTRY_OBJECT_TRANSFORMS
#define ALCHEMY_ABI_MENU_ENTRY_UPDATE_ENTRY_OBJECT_TRANSFORMS

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08077148(void);
void Func_08009008(void *, s32 *, s32 *, s32);

#define Menu_Check Func_08077148
#define Menu_SetMode Func_08009008
#define gIw Data_03001f2c

#endif
