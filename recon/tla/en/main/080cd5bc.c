#include "TYPES.H"
#include "SYSTEM.H"
extern u8 gOverlayArea[];
extern char MsgPonderEllipsis;

extern const u8 ObjectMotion_LinkedActionScript[];
extern const u8 ObjectMotion_StepAngleScript[];

s32 BattleAction_RunDescriptor(s32 arg0)
{
    EffectDescriptorWorkView *desc;
    EffectDescriptorWorkView *desc2;
    s32 kind;
    s32 ret;
    EffectDescriptorWorkView *work;

    desc = (EffectDescriptorWorkView *)BattleFx_FindDescriptor(2, arg0);
    ret = -1;
    work = *(EffectDescriptorWorkView **)&gEventWork;
    if ((desc != 0) && (desc->result != 0)) {
        if (desc->flags & 0x200) {
            work->limit = 0;
        }
        if (desc->result < 0x10000) {
            Battle_Reset();
            Event_SetValue1d8(desc->result);
            BattleEv_RunWait(-1, 0);
            ret = 0;
            BattleFx_FinishAction();
        } else {
            ((void (*)(s32))desc->result)(arg0);
            goto block_17;
        }
    } else {
        desc2 = (EffectDescriptorWorkView *)BattleFx_FindDescriptor(1, arg0);
        if (desc2 != 0) {
            kind = desc2->flags & 0x30;
            switch (kind) {
            case 0: Audio_PlayCue(0x7B); break;
            case 32: Audio_PlayCue(0x80); ObjectEffect_BeginContextEffect26(); break;
            case 48: Audio_PlayCue(0x81); ObjectEffect_BeginContextEffect25(); break;
            }
            work->queued_result = (s16)desc2->result;
block_17:
            ret = 0;
        }
    }
    return ret;
}
