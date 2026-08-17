#include "types.h"
#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

extern u8 *Data_03001ebc;

void Func_02004afc(void);
s32 Func_02004ae4(s32);
void Func_02004b6e(s32, s32, s32);
void Func_02004b14(s32);
void Func_02004c10(s32, s32, s32);
void *Func_02004b56(s32);
void Func_02004cda(s32);
void *Func_02004b68();
void Func_02004bc6(s32, s32);
void Func_02004b4c(s32);
void Func_02004c1a(s32);
void Func_02004c22(s32);
void Func_0200486a(s32, s32);
void Func_02004c48(s32, s32);
void Func_020048c8(void);
void Func_02004b7a();
void Func_02004be6(s32, s32, s32);
void Func_02004b8c(s32);
void Func_02004c88(s32, s32, s32);
void Func_02004d46(s32);
void *Func_02004bd4(s32);
void *Func_02004bde(s32);
void Func_02004c3c(s32, s32);
void Func_02004bc2(s32);
s32 Func_02004c9a(s32, s32);
s32 Func_02004bf2();
void Func_02004c7e(s32, s32);
void Func_02004be4(s32);
void Func_02004cc4(s32, s32);
void Func_02004902(s32, s32);
void Func_02004c00(s32);
void Func_02004ca8(s32, s32);
void Func_02004c0e(s32);
void Func_02004966(void);
void Func_02004c18(s32);
void Func_02004d00();
void Func_02004d1a(s32, s32);
void Func_02004cd2(s32, s32);
void Func_02004c50(s32);
void Func_02004d54(s32, s32, s32);
void Func_02004d2a(s32);
s32 Func_02004d3a(s32, s32);
s32 Func_02004c92(s32, s32);
void Func_02004d78(s32, s32, s32);
void Func_02004990(s32, s32);
void Func_02004c8e(s32);
void Func_02004d36(s32, s32);
void Func_02004c9c(s32);
void Func_020049f4();
void Func_02004ca6(s32);
void Func_02004d86(s32, s32);
void Func_02004cd6(s32);
void Func_02004d66(s32, s32);
void Func_02004ce4(s32);
void Func_02004dc4(s32, s32);
void Func_02004cf2(s32);
void Func_02004d82(s32, s32);
void Func_02004de0(s32, s32);
void Func_02004db0(s32, s32);
void Func_02004d16(s32);
void Func_02004df6(s32, s32);
void Func_02004d24(s32);
void Func_02004e20(s32, s32, s32);
void Func_02004d7a(s32, s32, s32);
void Func_02004da6(s32, s32, s32);
void Func_02004d4c(s32);
void Func_02004e48(s32, s32, s32);
void Func_02004e42();
void Func_02004e66(s32, s32, s32);
void Func_02004dda(s32, s32, s32);
s32 Func_02004e52(s32, s32);
s32 Func_02004daa(s32, s32);
void Func_02004e5c(s32);
void Func_02004e74();
void Func_02004d94(s32);
void Func_02004db2(s32);
void Func_02004dc0(s32);
void Func_02004edc(s32, s32);
void Func_02004efc(s32, s32);
void Func_02004f1e(s32, s32);
void Func_02004f34(s32);
void Func_02004f50();
void Func_02004f14(void);
void Func_02004b64(void);
void Func_02004f3a(s32, s32);
void Func_02004ee8(s32, s32);
void Func_02004eb8(s32, s32);
void Func_02004e1e(s32);
void Func_02004efe(s32, s32);
void Func_02004ec6(s32, s32);
void Func_02004f38(s32, s32);
void Func_02004e3e();
void Func_02004f28(s32, s32, s32);
void Func_02004ed6(s32, s32);
void Func_02004e6a(void);

/*
 * The declarations above are the relocation boundary.  The aliases below
 * keep those raw witnesses local while giving the scene body a cautious
 * vocabulary.  Only the scene entry, its workspace, and the actor-record
 * reads have a demonstrated role here; the remaining calls stay explicitly
 * unknown rather than acquiring guessed engine or story names.
 */
#define Resource39eSceneDriver       Func_0200071c
#define SceneWorkspace               Data_03001ebc
#define InitializeResource39eScene   Func_02004afc
#define ReadSceneSetupResult         Func_02004ae4
#define GetSceneActorRecord          Func_02004b56
#define GetAlternateSceneActor       Func_02004bd4
#define GetAlternateSceneActorState  Func_02004bde
#define unknown_scene_step_0486a     Func_0200486a
#define unknown_scene_step_048c8     Func_020048c8
#define unknown_scene_step_04902     Func_02004902
#define unknown_scene_step_04966     Func_02004966
#define unknown_scene_step_04990     Func_02004990
#define unknown_scene_step_049f4     Func_020049f4
#define unknown_scene_step_04b14     Func_02004b14
#define unknown_scene_step_04b4c     Func_02004b4c
#define unknown_scene_step_04b64     Func_02004b64
#define unknown_scene_step_04b68     Func_02004b68
#define unknown_scene_step_04b6e     Func_02004b6e
#define unknown_scene_step_04b7a     Func_02004b7a
#define unknown_scene_step_04b8c     Func_02004b8c
#define unknown_scene_step_04bc2     Func_02004bc2
#define unknown_scene_step_04bc6     Func_02004bc6
#define unknown_scene_step_04be4     Func_02004be4
#define unknown_scene_step_04be6     Func_02004be6
#define unknown_scene_step_04bf2     Func_02004bf2
#define unknown_scene_step_04c00     Func_02004c00
#define unknown_scene_step_04c0e     Func_02004c0e
#define unknown_scene_step_04c10     Func_02004c10
#define unknown_scene_step_04c18     Func_02004c18
#define unknown_scene_step_04c1a     Func_02004c1a
#define unknown_scene_step_04c22     Func_02004c22
#define unknown_scene_step_04c3c     Func_02004c3c
#define unknown_scene_step_04c48     Func_02004c48
#define unknown_scene_step_04c50     Func_02004c50
#define unknown_scene_step_04c7e     Func_02004c7e
#define unknown_scene_step_04c88     Func_02004c88
#define unknown_scene_step_04c8e     Func_02004c8e
#define unknown_scene_step_04c92     Func_02004c92
#define unknown_scene_step_04c9a     Func_02004c9a
#define unknown_scene_step_04c9c     Func_02004c9c
#define unknown_scene_step_04ca6     Func_02004ca6
#define unknown_scene_step_04ca8     Func_02004ca8
#define unknown_scene_step_04cc4     Func_02004cc4
#define unknown_scene_step_04cd2     Func_02004cd2
#define unknown_scene_step_04cd6     Func_02004cd6
#define unknown_scene_step_04cda     Func_02004cda
#define unknown_scene_step_04ce4     Func_02004ce4
#define unknown_scene_step_04cf2     Func_02004cf2
#define unknown_scene_step_04d00     Func_02004d00
#define unknown_scene_step_04d16     Func_02004d16
#define unknown_scene_step_04d1a     Func_02004d1a
#define unknown_scene_step_04d24     Func_02004d24
#define unknown_scene_step_04d2a     Func_02004d2a
#define unknown_scene_step_04d36     Func_02004d36
#define unknown_scene_step_04d3a     Func_02004d3a
#define unknown_scene_step_04d46     Func_02004d46
#define unknown_scene_step_04d4c     Func_02004d4c
#define unknown_scene_step_04d54     Func_02004d54
#define unknown_scene_step_04d66     Func_02004d66
#define unknown_scene_step_04d78     Func_02004d78
#define unknown_scene_step_04d7a     Func_02004d7a
#define unknown_scene_step_04d82     Func_02004d82
#define unknown_scene_step_04d86     Func_02004d86
#define unknown_scene_step_04d94     Func_02004d94
#define unknown_scene_step_04da6     Func_02004da6
#define unknown_scene_step_04daa     Func_02004daa
#define unknown_scene_step_04db0     Func_02004db0
#define unknown_scene_step_04db2     Func_02004db2
#define unknown_scene_step_04dc0     Func_02004dc0
#define unknown_scene_step_04dc4     Func_02004dc4
#define unknown_scene_step_04dda     Func_02004dda
#define unknown_scene_step_04de0     Func_02004de0
#define unknown_scene_step_04df6     Func_02004df6
#define unknown_scene_step_04e1e     Func_02004e1e
#define unknown_scene_step_04e20     Func_02004e20
#define unknown_scene_step_04e3e     Func_02004e3e
#define unknown_scene_step_04e42     Func_02004e42
#define unknown_scene_step_04e48     Func_02004e48
#define unknown_scene_step_04e52     Func_02004e52
#define unknown_scene_step_04e5c     Func_02004e5c
#define unknown_scene_step_04e66     Func_02004e66
#define unknown_scene_step_04e6a     Func_02004e6a
#define unknown_scene_step_04e74     Func_02004e74
#define unknown_scene_step_04eb8     Func_02004eb8
#define unknown_scene_step_04ec6     Func_02004ec6
#define unknown_scene_step_04ed6     Func_02004ed6
#define unknown_scene_step_04edc     Func_02004edc
#define unknown_scene_step_04ee8     Func_02004ee8
#define unknown_scene_step_04efc     Func_02004efc
#define unknown_scene_step_04efe     Func_02004efe
#define unknown_scene_step_04f14     Func_02004f14
#define unknown_scene_step_04f1e     Func_02004f1e
#define unknown_scene_step_04f28     Func_02004f28
#define unknown_scene_step_04f34     Func_02004f34
#define unknown_scene_step_04f38     Func_02004f38
#define unknown_scene_step_04f3a     Func_02004f3a
#define unknown_scene_step_04f50     Func_02004f50

void Resource39eSceneDriver(void)
{
    s32 r;

    InitializeResource39eScene();
    r = ReadSceneSetupResult(0x300);
    if (r != 0) {
        unknown_scene_step_04b6e(0, 0xA8, 0x1F8);
        unknown_scene_step_04b14(5);
        unknown_scene_step_04c10(0, 0xC000, 20);
        M2C_FIELD(GetSceneActorRecord(8), u8, 0x5B) = 0;
        unknown_scene_step_04cda(0x98);
        M2C_FIELD(unknown_scene_step_04b68(8), s32, 0x28) = 0x80000;
        unknown_scene_step_04bc6(8, 1);
        unknown_scene_step_04b4c(30);
        unknown_scene_step_04c1a(0x17AC);
    } else {
        unknown_scene_step_04c22(0x179F);
        unknown_scene_step_0486a(0, 8);
        unknown_scene_step_04b68(30);
        unknown_scene_step_04c48(8, 0);
        unknown_scene_step_048c8();
        unknown_scene_step_04b7a(20);
        unknown_scene_step_04be6(0, 0xA8, 0x1F8);
        unknown_scene_step_04b8c(5);
        unknown_scene_step_04c88(0, 0xC000, 20);
        unknown_scene_step_04d46(0x98);
        M2C_FIELD(GetAlternateSceneActor(8), u8, 0x5B) = r;
        M2C_FIELD(GetAlternateSceneActorState(8), s32, 0x28) = 0x80000;
        unknown_scene_step_04c3c(8, 1);
        unknown_scene_step_04bc2(30);
        unknown_scene_step_04c9a(8, 0);
        if (unknown_scene_step_04bf2(0, 0) == 1) {
            unknown_scene_step_04c7e(8, 2);
            unknown_scene_step_04be4(20);
            unknown_scene_step_04cc4(8, 0);
            unknown_scene_step_04bf2(20);
            unknown_scene_step_04902(8, 0);
            unknown_scene_step_04c00(30);
            unknown_scene_step_04ca8(0, 2);
            unknown_scene_step_04c0e(50);
            unknown_scene_step_04966();
            unknown_scene_step_04c18(30);
            unknown_scene_step_04ca8(8, 3);
            unknown_scene_step_04d00(8, 0);
        } else {
            u16 *q = (u16 *)(SceneWorkspace + 0x1D8);
            q[0] = q[0] + 2;
            unknown_scene_step_04d1a(8, 0);
        }
        unknown_scene_step_04cd2(8, 3);
        unknown_scene_step_04c50(30);
        unknown_scene_step_04d54(8, 0x100, 0x3C);
        unknown_scene_step_04d2a(0x17A4);
        unknown_scene_step_04d3a(8, 0);
        if (unknown_scene_step_04c92(0, 0) == 1) {
            unknown_scene_step_04d78(8, 0x105, 0x3C);
            unknown_scene_step_04990(8, 0);
            unknown_scene_step_04c8e(30);
            unknown_scene_step_04d36(0, 2);
            unknown_scene_step_04c9c(50);
            unknown_scene_step_049f4();
            unknown_scene_step_04ca6(30);
            unknown_scene_step_04d86(8, 0);
            {
                u16 *q = (u16 *)(SceneWorkspace + 0x1D8);
                q[0] = q[0] + 1;
            }
        } else {
            u16 *q = (u16 *)(SceneWorkspace + 0x1D8);
            q[0] = q[0] + 1;
            unknown_scene_step_04cd6(20);
            unknown_scene_step_04d66(8, 3);
            unknown_scene_step_04ce4(20);
            unknown_scene_step_04dc4(8, 0);
        }
        unknown_scene_step_04cf2(20);
        unknown_scene_step_04d82(8, 4);
        unknown_scene_step_04d00(20);
        unknown_scene_step_04de0(8, 0);
        unknown_scene_step_04db0(0, 2);
        unknown_scene_step_04d16(20);
        unknown_scene_step_04df6(8, 0);
        unknown_scene_step_04d24(20);
        unknown_scene_step_04e20(8, 0xC000, 20);
        unknown_scene_step_04d7a(8, 0x4CCC, 0x2666);
        unknown_scene_step_04da6(8, 0xA8, 0x1D0);
        unknown_scene_step_04d4c(0x3C);
        unknown_scene_step_04e48(8, 0x4000, 0x28);
        unknown_scene_step_04e42(8, 0, 10);
        unknown_scene_step_04e66(0, 0x102, 0x3C);
        unknown_scene_step_04dda(8, 0xA8, 0x1D8);
    }
    unknown_scene_step_04e52(8, 0);
    if (unknown_scene_step_04daa(0, 0) == 1) {
        unknown_scene_step_04e5c(0x17AB);
        unknown_scene_step_04e74(8, 0);
        unknown_scene_step_04d94(0x300);
    } else {
        unknown_scene_step_04e74(0x17AD);
        unknown_scene_step_04db2(30);
        unknown_scene_step_04e42(8, 3);
        unknown_scene_step_04dc0(20);
        unknown_scene_step_04edc(0x8000, 0x1000);
        unknown_scene_step_04efc(8, 1);
        unknown_scene_step_04f1e(0x10000, 0);
        unknown_scene_step_04f1e(0x10003, 1);
        unknown_scene_step_04f34(30);
        unknown_scene_step_04f50();
        unknown_scene_step_04f14();
        unknown_scene_step_04b64();
        unknown_scene_step_04f3a(0x10000, 0);
        unknown_scene_step_04f50(30);
        unknown_scene_step_04ee8(8, 0);
        unknown_scene_step_04eb8(8, 2);
        unknown_scene_step_04e1e(20);
        unknown_scene_step_04efe(8, 0);
        unknown_scene_step_04ec6(0, 1);
        unknown_scene_step_04f38(0, 0x102);
        unknown_scene_step_04e3e(0x3C);
        unknown_scene_step_04f28(8, 0, 10);
        unknown_scene_step_04e3e(0x891);
    }
    unknown_scene_step_04ed6(8, 5);
    unknown_scene_step_04e6a();
}
