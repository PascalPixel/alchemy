#ifndef ALCHEMY_ABI_EVENT_PREPARE_TWO_OBJECTS_AND_APPLY
#define ALCHEMY_ABI_EVENT_PREPARE_TWO_OBJECTS_AND_APPLY

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08077150(void *event);
s32 Func_08015428(void *first, void *second);

#define Sys_Apply Func_08015428
#define Sys_Check Func_08077150

#endif
