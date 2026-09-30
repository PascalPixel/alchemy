#include "TYPES.H"

extern s32 KorimaMura_PendingPose;
extern s32 KorimaMura_LayoutFlag;

/* The script the region runs, laid out after the code. */
extern u8 KorimaMura_RegionScript[];

void Engine_ObjectSetScript();
void Engine_AudioPlayCue();

s32 KorimaMura_TriggerRegionScript(s32 a0)
{
    if (KorimaMura_LayoutFlag != 0) {
        if ((u32)(*(s32 *)(a0 + 8) - 0x3b0001) <= 0x51fffe && *(s32 *)(a0 + 16) > 0xd30000 && *(s32 *)(a0 + 16) <= 0x100ffff)
            goto hit;
        if ((u32)(*(s32 *)(a0 + 8) - 0x450001) <= 0x34fffe && *(s32 *)(a0 + 16) > 0xc20000 && *(s32 *)(a0 + 16) <= 0x114ffff)
            goto hit;
    } else {
        if ((u32)(*(s32 *)(a0 + 8) - 0x3b0001) <= 0x33fffe && *(s32 *)(a0 + 16) > 0xc20000 && *(s32 *)(a0 + 16) < 0xe60000)
            goto hit;
        if ((u32)(*(s32 *)(a0 + 8) - 0x6f0001) <= 0x1dfffe && *(s32 *)(a0 + 16) > 0xd80000 && *(s32 *)(a0 + 16) < 0xfa0000)
            goto hit;
        if ((u32)(*(s32 *)(a0 + 8) - 0x4e0001) <= 0x2bfffe && *(s32 *)(a0 + 16) > 0xf10000 && *(s32 *)(a0 + 16) <= 0x114ffff)
            goto hit;
    }
    return 0;
hit:
    Engine_AudioPlayCue(106);
    /* FAKEMATCH: the do/while loads the script address before a0. */
    do {
        Engine_ObjectSetScript(a0, KorimaMura_RegionScript);
    } while (0);
    KorimaMura_PendingPose = 1;
    return 0;
}
