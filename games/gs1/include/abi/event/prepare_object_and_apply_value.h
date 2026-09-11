#ifndef ALCHEMY_ABI_EVENT_PREPARE_OBJECT_AND_APPLY_VALUE
#define ALCHEMY_ABI_EVENT_PREPARE_OBJECT_AND_APPLY_VALUE

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08015420(s32 event_id, s32 value);
s32 Func_08077150(s32 event_id);

#define Sys_Apply Func_08015420
#define Sys_Check Func_08077150

#endif
