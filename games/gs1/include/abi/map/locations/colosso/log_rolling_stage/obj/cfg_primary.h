#ifndef ALCHEMY_ABI_MAP_LOCATIONS_COLOSSO_LOG_ROLLING_STAGE_OBJ_CFG_PRIMARY
#define ALCHEMY_ABI_MAP_LOCATIONS_COLOSSO_LOG_ROLLING_STAGE_OBJ_CFG_PRIMARY

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02004bae(PrimaryStageObject *, s32, s32, s32);
void Func_02004bc4(PrimaryStageObject *, s32, s32, s32);
void Func_02004c0e(s32, s32, s32, s32, s32, s32);
void Func_02004c20(s32, s32, s32, s32, s32, s32);
void Func_02004c72(s32);

#define Map_Do Func_02004c72
#define Map_Run Func_02004c94
#define Map_Run2 Func_02004ca2
#define Map_Run3 Func_02004cbc
#define Map_SetMode Func_02004bae
#define Map_SetMode2 Func_02004bc4
#define Map_SetRect Func_02004c0e
#define Map_SetRect2 Func_02004c20

#endif
