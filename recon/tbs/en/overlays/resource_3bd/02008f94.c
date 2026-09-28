/* NONMATCHING: resource_3bd 0x02008f94, SceneEffect_UpdateStateMachine, from
 * FIELD/ARUTAMIRA_DOU/EXTENDED_PRESENTATION.C (2026-09-28).
 * Reads the signed byte at 0x02001002 in EWRAM, which the main image does not
 * name; its table at image offset 0x3f6c and its callback
 * ArutamiraDou_UpdateScalePulse resolve. Remaining: a name for that byte. */
#include "ARUTAMIRA.H"

void SceneEffect_UpdateStateMachine(void)
{
    u16 *p = Data_0200bf6c;
    s32 flag = 1;
    s32 state = *(s16 *)p;

    if (state == 0) {
        s32 t = p[4] + 16;
        p[4] = t;
        if ((u16)t > 0xbff) {
            p[0] = p[0] + 1;
            p[1] = state;
        }
    } else if (state == 1) {
        if ((s16)p[1] == 30) {
            p[0] = p[0] + 1;
        }
    } else if (state == 2) {
        s32 t = p[4] + 0xfff8;
        p[4] = t;
        if ((u16)t <= 0x2ff) {
            p[0] = p[0] + 1;
        }
    } else if (state == 3) {
        s32 v = Data_02001002;
        s32 r = Math_Divide(v << 16, 5);
        if ((unsigned int)(((p[3] - r) << 16) + 0xc2ff0000) <= 0x5fe0000) {
            u8 *o;
            s32 nv = r + 0x4000;
            p[3] = nv;
            {
                s32 z = 0;
                s32 k = 0x63;
                p[0] = k;
                p[4] = z;
            }
            o = (u8 *)Engine_ActorGet(v + 11);
            *(s32 *)(o + 0x6c) = (s32)&Data_02008ee1;
        }
    } else if (state == 0x63) {
        flag = 0;
    }
    if (flag != 0) {
        u16 *q2;
        Data_0200bf6c[3] += Data_0200bf6c[4];
        SceneActor_PlaceFiveActorsInRow(Data_0200bf6c[3]);
        q2 = Data_0200bf6c;
        {
            s32 t2 = q2[5] + q2[4];
            q2[5] = t2;
            if ((u16)t2 > 0x3000) {
                s32 z2 = 0;
                q2[5] = z2;
                Audio_PlayCue(0x87);
            }
        }
    }
    {
        u16 *q = Data_0200bf6c;
        q[1]++;
    }
}
