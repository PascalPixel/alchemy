#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunFourActorStagingSequence:
 * 52 ordered calls, actor 9/10/11/12 staging, placement publication,
 * workspace transitions, camera motion, and the terminal scene close. */

#define FieldScene_RunFourActorStagingSequence Func_02000564

void *Func_02001736();
void *Func_0200178a();
void Func_020017a0();
s32 Func_020017c4();
void Func_020017c8();
void Func_020017ca();
void Func_020017e2();
void Func_02001806();
void Func_0200180c();
void Func_0200180e();
void *Func_02001810();
void Func_02001838();
void Func_0200183a();
void Func_0200185a();
void Func_02001874();
void Func_020018a0();
void Func_020018ac();
void Func_020018b4();
void Func_020018b8();
void Func_020018be();
void Func_020018c4();
void Func_020018c8();
void *Func_020018ca();
void Func_020018cc();
void Func_020018d0();
void *Func_020018dc();
void *Func_020018e8();
void Func_02001904();
void Func_02001906();
void Func_0200190c();
void Func_02001910();
void Func_02001912();
void Func_02001914();
void Func_0200191a();
void Func_0200191c();
void Func_0200193c();
void Func_02001948();
void Func_02001954();
void Func_02001976();
void Func_02001978();
void Func_0200198a();
void Func_02001990();
void Func_0200199a();
void Func_020019aa();
void Func_020019d4();
void Func_02001a2c();
void Func_02001a42();
void Func_02001a4e();

extern s32 Data_02009930[2];
extern s32 Data_02009938[2];
extern u8 *Data_03001e70;
extern u8 *Data_03001ebc;

void Func_02000564(void)
{
    void *p1;
    void *p12;
    void *p3;
    void *p32;
    void *p33;
    void *p34;
    s32 *placement;
    s32 scene_state;

    placement = *(s32 **)Data_03001e70;
    p1 = Func_0200178a();
    Func_020017a0(33593076);
    p3 = Func_02001736(1);
    Func_0200180e(0, 15);
    scene_state = Func_020017c4(0);
    Func_0200178a(scene_state, 0);
    Func_020017e2(8, 33592220);
    *(s32 *)(Data_03001ebc + 448) = 515;
    Func_020018a0();
    Func_020018b4();
    Func_020017ca(20);
    Data_02009938[0] = placement[0];
    Data_02009938[1] = placement[1];
    Func_0200183a(9, 5242880, 13762560);
    p12 = Func_02001810(9);
    *(u8 *)((u8 *)(p12) + 85) = 0;
    Data_02009930[0] = 5242880;
    Data_02009930[1] = 0;
    Func_02001838(9, 33592084);
    Func_02001806(20);
    Func_0200190c(29);
    Func_0200180c(2288);
    Func_0200185a(8);
    Func_020017c8(1);
    Func_020018cc(8, 256, 0);
    Func_020018c8(8, 45056, 0);
    Func_020018be(7742);
    Func_020018d0(8, 0, 10);
    Func_020018ac(10, 5242880, 13762560);
    Func_020018b8(11, 5242880, 13762560);
    Func_020018c4(12, 5242880, 13762560);
    Func_0200190c(10, 3);
    Func_02001914(11, 3);
    Func_0200191c(12, 3);
    Func_02001904(10, 3);
    Func_0200190c(11, 3);
    Func_02001914(12, 3);
    p32 = Func_020018ca(10);
    *(s32 *)(p32 + 28) = 32768;
    *(s32 *)(p32 + 24) = 32768;
    *(s32 *)(p32 + 108) = 33587365;
    p33 = Func_020018dc(11);
    *(s32 *)(p33 + 28) = 32768;
    *(s32 *)(p33 + 24) = 32768;
    *(s32 *)(p33 + 108) = 33587365;
    p34 = Func_020018e8(12);
    *(s32 *)(p34 + 28) = 32768;
    *(s32 *)(p34 + 24) = 32768;
    *(s32 *)(p34 + 108) = 33587365;
    Func_02001874(1);
    Func_02001906(10, 34078, 17039);
    Func_02001910(11, 29491, 14745);
    Func_0200191a(12, 39321, 19660);
    Func_0200193c(10, 128, 345);
    Func_02001948(11, 136, 330);
    Func_02001954(12, 156, 340);
    Func_02001912(60);
    Func_0200198a(8, 2);
    Func_02001976(8, 164, 344);
    Func_02001990(8, 4, 10);
    Func_0200199a(8, 6, 40);
    Func_020019aa(8, 3);
    Func_020019d4(8, 0, 20);
    *(s32 *)(Data_03001ebc + 448) = 514;
    Func_02001a42();
    Func_02001a4e();
    Func_02001a2c(11);
    Func_02001978();
}
