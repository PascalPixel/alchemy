#include "types.h"
#include "scene.h"

typedef struct {
    s32 flags;
    u8 pad4[4];
    s32 val8;
    u8 padA[0x170 - 0xC];
    s16 h170;
    u8 padB[0x19C - 0x172];
    s16 h19c;
} Obj;

struct EffectDescriptor *Battle_Run(s32, s32);
void Battle_Reset(void);

void Audio_PlayCue(s32);

s32 BattleAction_RunDescriptor(s32 arg0)
{
    Obj *desc;
    Obj *desc2;
    s32 kind;
    s32 ret;
    Obj *work;

    desc = (Obj *)Battle_Run(2, arg0);
    ret = -1;
    work = *(Obj **)0x03001ebc;
    if ((desc != 0) && (desc->val8 != 0)) {
        if (desc->flags & 0x200) {
            work->h19c = 0;
        }
        if (desc->val8 < 0x10000) {
            Battle_Reset();
            Battle_Do(desc->val8);
            Battle_Apply(-1, 0);
            ret = 0;
            Battle_unk2_4();
        } else {
            ((void (*)(s32))desc->val8)(arg0);
            goto block_17;
        }
    } else {
        desc2 = (Obj *)Battle_Run(1, arg0);
        if (desc2 != 0) {
            kind = desc2->flags & 0x30;
            switch (kind) {
            case 0: Audio_PlayCue(0x7B); break;
            case 32: Audio_PlayCue(0x80); Battle_unk3_4(); break;
            case 48: Audio_PlayCue(0x81); Battle_unk4_4(); break;
            }
            work->h170 = (s16)desc2->val8;
block_17:
            ret = 0;
        }
    }
    return ret;
}
