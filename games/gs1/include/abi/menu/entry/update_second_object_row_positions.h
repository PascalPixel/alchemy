#ifndef ALCHEMY_ABI_MENU_ENTRY_UPDATE_SECOND_OBJECT_ROW_POSITIONS
#define ALCHEMY_ABI_MENU_ENTRY_UPDATE_SECOND_OBJECT_ROW_POSITIONS

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08009008(void *object, s32 *position, s32 *scale, s32 flags);

#define Menu_SetMode Func_08009008
#define gIw Data_03001f2c

#endif
