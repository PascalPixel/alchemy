#include "scene_effect_sequence.h"

/*
 * Scene step dispatcher for resource_3a0.  The 228-byte owner includes its
 * seven-entry jump table, an alignment halfword and its literal pool.  Each
 * call site is spelled with its own import name.
 */

extern u8 *Data_03001ebc;

extern u8 Data_02009778[];
extern u8 Data_0200978e[];
extern u8 Data_020097a4[];
extern u8 Data_020097ba[];
extern u8 Data_020097d0[];

void Func_02002214();
void Func_02002348();
void Func_02002212();
void Func_0200235a();
void Func_02002366();
void Func_02002372();
void Func_0200223c();
void Func_02002384();
void Func_0200224e();
void Func_02002396();
void Func_02002260();
void Func_020023a8();
void Func_02002272();
void Func_02001cf0();
void Func_020022d4();

/*
 * The scene table is reached through the IWRAM pointer Data_03001ebc, not
 * through the resident workspace, and the selector is its halfword [182],
 * guarded to the range 1..7.  The pointer is loaded before the first call and
 * held across all of them, so it is a function-top local.  Cases 2 and 3 set
 * the two shared arguments and jump into the middle of case 6 to share its
 * final call; the goto and the two locals are what reproduce that.
 */
void SceneEffect_DispatchStep(void)
{
    s16 *scene = (s16 *)Data_03001ebc;
    u8 *shared0;
    s32 shared1;

    Func_02002214();

    switch (scene[182]) {
    case 1:
        Func_02002348(158);
        Func_02002212(Data_02009778, 81, 18);
        break;
    case 2:
        Func_0200235a(158);
        shared0 = Data_0200978e;
        shared1 = 83;
        goto shared;
    case 3:
        Func_02002366(158);
        shared0 = Data_0200978e;
        shared1 = 86;
        goto shared;
    case 4:
        Func_02002372(158);
        Func_0200223c(Data_020097a4, 84, 24);
        break;
    case 5:
        Func_02002384(158);
        Func_0200224e(Data_020097a4, 72, 7);
        break;
    case 6:
        Func_02002396(188);
        shared0 = Data_020097ba;
        shared1 = 69;
    shared:
        Func_02002260(shared0, shared1, 11);
        break;
    case 7:
        Func_020023a8(158);
        Func_02002272(Data_020097d0, 83, 7);
        break;
    default:
        break;
    }

    Func_02001cf0(scene[182]);
    Func_020022d4();
}
