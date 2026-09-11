#ifndef ALCHEMY_ABI_MENU_DRAW_DRAW_FLAG_BIT_TABLE
#define ALCHEMY_ABI_MENU_DRAW_DRAW_FLAG_BIT_TABLE

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08016478(void);
void Func_0801e940(s32 text, s32 window, s32 x, s32 y);
void Func_08029274(s32 value, s32 width, s32 buf);
s32 Func_080770c0(s32 flag);

#define Menu_Check Func_080770c0
#define Menu_Place Func_08029274
#define Menu_Run Func_08016478
#define Menu_SetMode Func_0801e940

#endif
