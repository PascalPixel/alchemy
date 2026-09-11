#ifndef ALCHEMY_ABI_MENU_MISC_RUN_WORKSPACE_SELECTION_LOOP
#define ALCHEMY_ABI_MENU_MISC_RUN_WORKSPACE_SELECTION_LOOP

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080770c0(s32);
void Func_0801d980(void);
void Func_0801d9bc(void);
void Func_080216e8(s32, s32, s32);
void Func_080216b4(void *);
void Func_08016418(void *, s32);
s32 Func_080022fc(s32, s32);
void Func_080a1038(void *);
void Func_080b0030(void *, s32, s32, s32);
void Func_080030f8(s32);
void Func_080f9010(s32);

#define Menu_Apply Func_080022fc
#define Menu_Apply2 Func_08016418
#define Menu_Check Func_080770c0
#define Menu_Do Func_080030f8
#define Menu_Do2 Func_080a1038
#define Menu_Do3 Func_080216b4
#define Menu_Do4 Func_080f9010
#define Menu_Place Func_080216e8
#define Menu_Run Func_0801d9d4
#define Menu_Run2 Func_08021620
#define Menu_Run3 Func_0801d980
#define Menu_Run4 Func_0801d9bc
#define Menu_RunWorkspaceSelectionLoop Func_0801db70
#define Menu_SetMode Func_080b0030
#define gIw Data_03001ea0
#define gIw2 Data_03001f54
#define gRom Data_080367dc

#endif
