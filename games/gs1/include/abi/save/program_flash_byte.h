#ifndef ALCHEMY_ABI_SAVE_PROGRAM_FLASH_BYTE
#define ALCHEMY_ABI_SAVE_PROGRAM_FLASH_BYTE

/* Binding layer — not production source. Address / far-call ABI only. */

extern s32 Func_080072f0(s32 mode, u8 *destination, u8 value, s32 status);

#define Sys_SetMode Func_080072f0

#endif
