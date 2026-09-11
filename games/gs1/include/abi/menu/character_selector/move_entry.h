#ifndef ALCHEMY_ABI_MENU_CHARACTER_SELECTOR_MOVE_ENTRY
#define ALCHEMY_ABI_MENU_CHARACTER_SELECTOR_MOVE_ENTRY

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08077150(s32 character_id);
s32 Func_08077158(const u16 *character_ids);
s32 Func_08077168(s32 character_id);

#define Menu_Check Func_08077168
#define Menu_Check2 Func_08077150
#define Menu_Check3 Func_08077158

#endif
