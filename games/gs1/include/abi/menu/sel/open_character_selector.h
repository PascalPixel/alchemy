#ifndef ALCHEMY_ABI_MENU_SEL_OPEN_CHARACTER_SELECTOR
#define ALCHEMY_ABI_MENU_SEL_OPEN_CHARACTER_SELECTOR

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08002dd8(s32);
void Func_08015278(s32);
void Func_08015408(s32, s32, s32, s32);
s32 Func_08077158(const u16 *);
void Func_080a1090(s32);
void Func_080a8034(s32, s32, s32, s32);
s32 Func_080a7440(void);

#define Menu_Check Func_08077158
#define Menu_Check2 Func_080a7440
#define Menu_Do Func_080a1090
#define Menu_Do2 Func_08015278
#define Menu_Do3 Func_08002dd8
#define Menu_SetMode Func_08015408
#define Menu_SetMode2 Func_080a8034
#define gIw Data_03001e68

#endif
