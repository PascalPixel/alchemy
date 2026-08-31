#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunSevenActorEnsemble:
 * Seven actor setup and motion sequences, two scene-workspace state writes,
 * the closing map-state publication, and the final selector branch. */

#define FieldScene_RunSevenActorEnsemble Func_02000e78

void Func_02001926();
void Func_0200192c();
void Func_02001932();
void Func_02001938();
void Func_0200193e();
void Func_02001944();
void Func_0200194a();
void Func_02002050();
void Func_02002084();
void Func_02002090();
void Func_020020ba();
s32 Func_020020be();
void Func_020020f6();
void Func_02002108();
void Func_02002112();
void Func_0200213c();
void Func_02002142();
void Func_02002148();
void Func_0200214e();
void Func_02002154();
void Func_0200215a();
void Func_0200215e();
void Func_02002160();
void Func_0200216c();
s32 Func_02002176();
void Func_0200217a();
void Func_02002188();
void Func_02002196();
void Func_020021a4();
void Func_020021b2();
void Func_020021c0();
void Func_020021c2();
void Func_020021ca();
void Func_020021d2();
void Func_020021d8();
void Func_020021da();
void Func_020021de();
void Func_020021e2();
void Func_020021ea();
void Func_020021f2();
void Func_0200222a();
void Func_02002230();
void Func_02002248();
void Func_02002256();
void Func_02002270();
void Func_02002278();
void Func_0200227a();
void Func_0200228c();
void Func_020022a0();
void Func_020022b2();
void Func_0200231c();
void Func_02002324();
void Func_02002328();
void Func_0200232c();

extern u8 *Data_03001ebc;
extern u16 Data_02000240[];

void Func_02000e78(void)
{
    s32 ensemble;
    s32 selector;

    Func_02002090();
    Func_02002108(0, 15);
    ensemble = Func_020020be(0);
    Func_02002084(ensemble, 0);
    Func_020020ba(33593196);
    Func_02002050(1);
    Func_02001926(9);
    Func_0200192c(10);
    Func_02001932(11);
    Func_02001938(12);
    Func_0200193e(13);
    Func_02001944(14);
    Func_0200194a(15);
    Func_02002112(8, 33592220);
    *(s32 *)(Data_03001ebc + 448) = 515;
    Func_020021ca();
    Func_020021de();
    Func_020020f6(400);
    Func_0200213c(9);
    Func_02002142(10);
    Func_02002148(11);
    Func_0200214e(12);
    Func_02002154(13);
    Func_0200215a(14);
    Func_02002160(15);
    Func_0200215e(9, 196608, 98304);
    Func_0200216c(10, 196608, 98304);
    Func_0200217a(11, 196608, 98304);
    Func_02002188(12, 196608, 98304);
    Func_02002196(13, 196608, 98304);
    Func_020021a4(14, 196608, 98304);
    Func_020021b2(15, 196608, 98304);
    Func_020021c2(9, 33592400);
    Func_020021ca(10, 33592448);
    Func_020021d2(11, 33592496);
    Func_020021da(12, 33592544);
    Func_020021e2(13, 33592592);
    Func_020021ea(14, 33592640);
    Func_020021f2(15, 33592688);
    Func_020021c0(40);
    Func_02002230(8, 3);
    Func_0200227a(8, 258);
    Func_020021d8(120);
    Func_02002248(8, 1);
    Func_0200228c(8, 256, 60);
    Func_0200222a(8, 65536, 32768);
    Func_02002256(8, 164, 344);
    Func_02002270(8, 4, 10);
    Func_0200227a(8, 6, 20);
    Func_020022a0(7908);
    Func_020022b2(8, 0, 20);
    *(s32 *)(Data_03001ebc + 448) = 514;
    Func_0200231c();
    Func_02002328();
    Data_02000240[226] = 111;
    Data_02000240[227] = 2;
    selector = Func_02002176();
    if (selector == 11) {
        Func_02002324(15);
    } else {
        Func_0200232c(14);
    }
    Func_02002278();
}
