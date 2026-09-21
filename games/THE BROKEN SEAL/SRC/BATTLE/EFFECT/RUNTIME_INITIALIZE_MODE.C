#include "TYPES.H"

s32 Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);
void Func_08002dd8(s32);
#define Runtime_ReleaseHeapBlock Func_08002dd8
void Func_080e823c(s32 *);
void Func_080d2d98(s32 *);
void Func_080eb754(s32 *);
void Func_080dc968(s32 *);
#define BattleEffect_RunStagedParticles Func_080dc968
void Func_080d6970(s32 *);
void Func_080ec100(s32 *);
#define FunctionHead_080ec100 Func_080ec100
void Func_080d2458(s32 arg0);
#define BattleFx_RenderAnimationMode0 Func_080d2458
void Func_080d1714(s32 *);
void Func_080ea0d8(s32 *);
void Func_080d765c(s32 *);
#define FunctionHead_080d765c Func_080d765c
void Func_080e7320(s32 arg0);
#define BattleFx_InitializeDefaultMode Func_080e7320
void Func_080e15e8(s32 *);
#define FunctionHead_080e15e8 Func_080e15e8

void BattleFx_InitializeMode(s32 *arg0)
{
    Runtime_AllocateHeapBlock(41, 0x302);
    Runtime_AllocateHeapBlock(39, 0x782c);
    Runtime_AllocateHeapBlock(40, 0x4000);

    if (*arg0 == 0) {
        BattleFx_InitializeDefaultMode(arg0);
    } else {
        switch (*arg0) {
        case 1:
            Func_080e823c(arg0);
            break;
        case 2:
            Func_080d2d98(arg0);
            break;
        case 3:
            Func_080eb754(arg0);
            break;
        case 4:
            BattleEffect_RunStagedParticles(arg0);
            break;
        case 5:
            Func_080d6970(arg0);
            break;
        case 6:
            FunctionHead_080ec100(arg0);
            break;
        case 7:
            BattleFx_RenderAnimationMode0(arg0);
            break;
        case 8:
            Func_080d1714(arg0);
            break;
        case 9:
            Func_080ea0d8(arg0);
            break;
        case 10:
            FunctionHead_080d765c(arg0);
            break;
        case 11:
            BattleFx_InitializeDefaultMode(arg0);
            break;
        case 12:
            FunctionHead_080e15e8(arg0);
            break;
        }
    }

    Runtime_ReleaseHeapBlock(40);
    Runtime_ReleaseHeapBlock(39);
    Runtime_ReleaseHeapBlock(41);
}
