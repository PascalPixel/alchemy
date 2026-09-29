#include "ARUTAMIRA.H"

void ArutamiraDou_UpdateScalePulse();

/* Five actors turn in a ring: the wheel speeds up, runs, slows, and stops
   when the actor the scene state's third byte chooses comes round; that
   actor then pulses. A cue plays at every 0x3000 of travel.
   FAKEMATCH: forced temporaries; the block-local zero and state, the
   reloaded work pointers and the stored-sum temporaries keep the game's
   register and pool order, which the plain statements do not. */
void ArutamiraDou_SpinActorWheel(void)
{
    u16 *p = ArutamiraDou_ClearTarget;
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
        s32 v = ((s8 *)gSceneState)[2];
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
            *(s32 *)(o + 0x6c) = (s32)ArutamiraDou_UpdateScalePulse;
        }
    } else if (state == 0x63) {
        flag = 0;
    }
    if (flag != 0) {
        u16 *q2;
        ArutamiraDou_ClearTarget[3] += ArutamiraDou_ClearTarget[4];
        SceneActor_PlaceFiveActorsInRow(ArutamiraDou_ClearTarget[3]);
        q2 = ArutamiraDou_ClearTarget;
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
        u16 *q = ArutamiraDou_ClearTarget;
        q[1]++;
    }
}
