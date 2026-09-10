#include "types.h"

extern u8 Value_00001a9e[];
extern u8 Value_00001aa2[];

/* Two early long branches share the scene-skip tail. Four polling loops
 * wait on signed actor fields; calls bind at loader runtime addresses. */
#define FieldScene_RunStagedGroupSequence Func_02000590

void Func_02003bf4();
s32 Func_02003bd4();
void Func_02003c04();
void Func_02003c74();
void Func_02003d7c();
void Func_02003cec();
void Func_02003bc4();
void Func_02003bec();
void Func_02003c9c();
void Func_02003d0c();
void Func_02003ce4();
void Func_02003c2c();
void Func_02003c34();
u8 *Func_02003c1c();
void Func_02003d14();
void Func_02003c3c();
void Func_02003b14();
void Func_02003cbc();
void Func_02003cd4();
void Func_02003c7c();
void Func_02003d04();
void Func_02003cfc();
void Func_02003c84();
void Func_02003c5c();
void Func_02003c8c();
void Func_02001e6c();
void Func_02003c94();
void Func_02003c4c();
void Func_02001ed8();
void Func_02003c44();
void Func_02001ea4();
void Func_02003b24();
void Func_02003cf4();
void Func_02003c24();
void Func_02003d54();
void Func_02003bdc();
void Func_02003bfc();
void Func_02003d24();
void Func_02003d34();
void Func_02003d3c();

extern const u32 SceneAction_GroupMotion[];
extern const u32 SceneAction_GroupOffsetMotion[];

/* Verified scene siblings use these call forms for independently evaluated
 * large constants in repeated actor operations. */
static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

#define Func_02003c74(a0, a1, a2) Call3(Func_02003c74, a0, a1, a2)
#define Func_02003c2c(a0, a1, a2) Call3(Func_02003c2c, a0, a1, a2)
#define Func_02003ce4(a0, a1, a2) Call3(Func_02003ce4, a0, a1, a2)
#define Func_02003cfc(a0, a1, a2) Call3(Func_02003cfc, a0, a1, a2)
#define Func_02003d04(a0, a1) Call2(Func_02003d04, a0, a1)
#define Func_02003d0c(a0, a1) Call2(Func_02003d0c, a0, a1)
#define Func_02003c5c(a0, a1, a2) Call3(Func_02003c5c, a0, a1, a2)

void FieldScene_RunStagedGroupSequence(void)
{
    s32 messageId;
    const u32 *actionDescriptor;
    s32 idleState;
    s32 actionEnabled;
    u8 *actor20PairedWait;
    u8 *actor21PairedWait;
    u8 *actor21ClosingWait;
    u8 *actor14ClosingWait;
    u8 *actor14FinalFacing;
    u8 *actor20FinalFacing;
    u8 *actor21FinalFacing;
    u8 *playerFinalFacing;
    u8 *actor21OpeningWait;
    u8 *actor20BeforeFirstMove;
    u8 *actor20AfterFirstMove;
    u8 *actor20BeforeSecondMove;
    u8 *actor20AfterSecondMove;
    u8 *sceneWorkspace;

    sceneWorkspace = *(u8 **)0x03001ebc;
    Func_02003bf4();
    if (Func_02003bd4(2320) == 0) {
        goto skip_scene;
    }
    if (Func_02003bd4(2321) != 0) {
        goto skip_scene;
    }
    Func_02003c04(33605960);
    Func_02003c74(20, 16515072, 17825792);
    Func_02003c74(27, 18612224, 17301504);
    Func_02003c74(28, 18612224, 18350080);
    Func_02003c74(29, 19660800, 17301504);
    Func_02003c74(30, 19660800, 18350080);
    Func_02003c74(32, 20709376, 17301504);
    Func_02003c74(31, 20709376, 18350080);
    Func_02003c74(33, 21757952, 17301504);
    Func_02003c74(34, 21757952, 18350080);
    Func_02003c74(21, 23855104, 17825792);
    Func_02003d7c(17);
    Func_02003cec(20);
    Func_02003bc4(6801, 1, 0);
    Func_02003d7c(9);
    Func_02003bec(10);
    Func_02003c9c(0, 2);
    if (*(s16 *)(sceneWorkspace + 364) == 9) {
        Func_02003d0c(157286, 19660);
        Func_02003ce4(0, 57344, 20);
    } else {
        Func_02003d0c(78643, 9830);
        Func_02003ce4(0, 0, 20);
    }
    Func_02003c2c(20, 72089, 36044);
    Func_02003c2c(27, 65536, 32768);
    Func_02003c2c(28, 65536, 32768);
    Func_02003c2c(29, 58982, 29491);
    Func_02003c2c(30, 58982, 29491);
    Func_02003c2c(32, 52428, 26214);
    Func_02003c2c(31, 52428, 26214);
    Func_02003c2c(33, 45875, 22937);
    Func_02003c2c(34, 45875, 22937);
    Func_02003c2c(21, 39321, 19660);
    actionDescriptor = SceneAction_GroupMotion;
    Func_02003c34(20, actionDescriptor);
    Func_02003c34(27, actionDescriptor);
    Func_02003c34(28, actionDescriptor);
    Func_02003c34(29, actionDescriptor);
    Func_02003c34(30, actionDescriptor);
    Func_02003c34(32, actionDescriptor);
    Func_02003c34(31, actionDescriptor);
    Func_02003c34(33, actionDescriptor);
    Func_02003c34(34, actionDescriptor);
    actor21OpeningWait = Func_02003c1c(21);
    {
        s32 value = 0;
        *(u16 *)(actor21OpeningWait + 100) = value;
    }
    Func_02003c34(21, actionDescriptor);
    Func_02003d14(12189696, -1, 17825792, 1);
    Func_02003c3c(20);
    Func_02003ce4(20, 0, 0);
    do {
        Func_02003b14(1);
    } while (*(s16 *)((u8 *)actor21OpeningWait + 100) == 0);
    Func_02003bec(40);
    Func_02003c9c(27, 2);
    Func_02003ce4(27, 20480, 20);
    Func_02003cbc(6802);
    Func_02003cd4(27, 0, 10);
    Func_02003c9c(28, 2);
    Func_02003ce4(28, 45056, 10);
    Func_02003c7c(28, 3);
    Func_02003cd4(28, 0, 10);
    Func_02003d04(32, 258);
    Func_02003bec(40);
    Func_02003cd4(32, 0, 10);
    Func_02003cfc(31, 256, 40);
    Func_02003ce4(31, 45056, 10);
    Func_02003cd4(31, 0, 10);
    Func_02003ce4(31, 32768, 10);
    Func_02003c9c(31, 2);
    Func_02003c7c(31, 4);
    Func_02003cd4(31, 0, 10);
    Func_02003ce4(31, 45056, 0);
    Func_02003ce4(32, 20480, 20);
    Func_02003c7c(31, 3);
    Func_02003c84(32, 3);
    Func_02003c9c(20, 2);
    Func_02003d04(20, 258);
    Func_02003bec(40);
    Func_02003cd4(20, 0, 10);
    Func_02003c9c(20, 2);
    actor20BeforeFirstMove = Func_02003c1c(20);
    {
        s32 flags = actor20BeforeFirstMove[90] & 0xfe;
        idleState = 0;
        actor20BeforeFirstMove[90] = flags;
    }
    Func_02003c5c(20, 172, 264);
    Func_02003bec(1);
    actor20AfterFirstMove = Func_02003c1c(20);
    actionEnabled = 1;
    {
        s32 flags = actor20AfterFirstMove[90];
        flags |= actionEnabled;
        actor20AfterFirstMove[90] = flags;
    }
    Func_02003ce4(27, 32768, 0);
    Func_02003ce4(28, 32768, 0);
    Func_02003ce4(32, 32768, 0);
    Func_02003ce4(31, 32768, 20);
    Func_02003c84(20, 3);
    Func_02003bec(20);
    actor20BeforeSecondMove = Func_02003c1c(20);
    *(u8 *)((u8 *)(actor20BeforeSecondMove) + 90) &= 0xfe;
    Func_02003c5c(20, 172, 272);
    Func_02003bec(1);
    actor20AfterSecondMove = Func_02003c1c(20);
    actionEnabled |= actor20AfterSecondMove[90];
    actor20AfterSecondMove[90] = actionEnabled;
    Func_02003c5c(20, 180, 272);
    Func_02003ce4(20, 0, 0);
    Func_02003cd4(20, 0, 10);
    Func_02003cfc(34, 261, 0);
    Func_02003c9c(34, 1);
    Func_02003c84(34, 3);
    Func_02003cd4(34, 0, 10);
    Func_02003c9c(33, 1);
    Func_02003cd4(33, 0, 10);
    Func_02003c7c(33, 4);
    Func_02003cd4(33, 0, 10);
    Func_02003c9c(21, 2);
    Func_02003cfc(21, 258, 0);
    Func_02003cd4(21, 0, 10);
    Func_02003c8c(20, 2, 20);
    Func_02003c8c(20, 4, 40);
    Func_02003c9c(20, 2);
    Func_02003cd4(20, 0, 10);
    Func_02003c2c(21, 104857, 52428);
    Func_02003c5c(21, 265, 282);
    Func_02003c5c(21, 251, 284);
    Func_02003c5c(21, 246, 296);
    Func_02003ce4(21, 49152, 0);
    Func_02001e6c();
    Func_02003bec(40);
    Func_02003c2c(21, 104857, 52428);
    Func_02003c5c(21, 228, 296);
    Func_02003ce4(21, 49152, 40);
    Func_02003c5c(21, 212, 296);
    Func_02003ce4(21, 49152, 40);
    Func_02003c5c(21, 192, 296);
    Func_02003ce4(21, 49152, 40);
    Func_02003c94(21, 2);
    Func_02003cfc(21, 256, 60);
    Func_02003ce4(20, 12288, 0);
    Func_02003c5c(21, 184, 286);
    Func_02003ce4(21, 45056, 10);
    Func_02003d04(21, 258);
    Func_02003bec(40);
    Func_02003c84(21, 4);
    Func_02003cfc(20, 257, 40);
    Func_02003c84(20, 3);
    Func_02003c9c(20, 2);
    Func_02003ce4(20, 0, 0);
    Func_02003ce4(21, 0, 60);
    Func_02003ce4(20, 12288, 0);
    Func_02003ce4(21, 45056, 10);
    Func_02003c84(20, 3);
    Func_02003c84(21, 3);
    Func_02003ce4(21, 0, 0);
    Func_02003c2c(20, 104857, 52428);
    Func_02003c4c(20, 33603504);
    Func_02003c5c(20, 228, 296);
    Func_02003ce4(20, 49152, 40);
    Func_02003c5c(20, 212, 296);
    Func_02003ce4(20, 49152, 40);
    Func_02003c5c(20, 192, 296);
    Func_02003ce4(20, 49152, 40);
    Func_02003ce4(20, 45056, 0);
    Func_02003ce4(21, 12288, 10);
    Func_02003d04(20, 258);
    Func_02003bec(60);
    Func_02003c84(20, 4);
    messageId = (s32)Value_00001a9e;
    Func_02003cbc(messageId);
    Func_02003cd4(20, 0, 40);
    Func_02001ed8();
    Func_02003c5c(20, 178, 272);
    Func_02003ce4(20, 0, 0);
    Func_02003bec(240);
    Func_02003c44(27);
    Func_02003b14(1);
    Func_02003ce4(27, 32768, 10);
    Func_02003cfc(27, 257, 60);
    Func_02003cd4(27, 0, 10);
    Func_02001ea4(27);
    Func_02003bec(80);
    Func_02003c44(28);
    Func_02003b14(1);
    Func_02003ce4(28, 53248, 20);
    Func_02003c9c(28, 2);
    Func_02003cd4(28, 0, 10);
    Func_02001ea4(28);
    Func_02003bec(160);
    Func_02003c44(32);
    Func_02003b14(1);
    Func_02003ce4(32, 20480, 10);
    Func_02003cfc(32, 257, 60);
    Func_02003cd4(32, 0, 10);
    Func_02001ea4(32);
    Func_02003bec(80);
    Func_02003c44(30);
    Func_02003b14(1);
    Func_02003ce4(30, 45056, 10);
    Func_02003c9c(30, 1);
    Func_02003cbc(messageId + 6);
    Func_02003cd4(30, 0, 10);
    Func_02003b24(33595157);
    Func_02003c44(20);
    Func_02003c44(21);
    Func_02003b14(1);
    actor20PairedWait = Func_02003c1c(20);
    *(u16 *)(actor20PairedWait + 100) = idleState;
    actor21PairedWait = Func_02003c1c(21);
    *(u16 *)(actor21PairedWait + 100) = idleState;
    Func_02003c2c(20, 52428, 26214);
    Func_02003c2c(21, 52428, 26214);
    Func_02003c34(20, 33603636);
    Func_02003c34(21, 33603788);
    Func_02003c44(29);
    Func_02003b14(1);
    Func_02003ce4(29, 20480, 10);
    messageId += 5;
    Func_02003c9c(29, 2);
    Func_02003cbc(messageId);
    Func_02003cd4(29, 0, 20);
    Func_02001ea4(29);
    Func_02001ea4(30);
    do {
        Func_02003b14(1);
    } while (*(s16 *)((u8 *)Func_02003c1c(20) + 100) == 0 ||
             *(s16 *)((u8 *)Func_02003c1c(21) + 100) != 1);
    Func_02003c34(20, 33603940);
    Func_02003c34(21, 33604012);
    Func_02003c44(31);
    Func_02003b14(1);
    Func_02003ce4(31, 0x5000, 10);
    Func_02003c9c(31, 1);
    Func_02003c84(31, 4);
    messageId = (s32)Value_00001aa2;
    Func_02003cbc(messageId);
    Func_02003cd4(31, 0, 10);
    Func_02001ea4(31);
    Func_02003c44(34);
    Func_02003c44(33);
    Func_02003b14(1);
    Func_02003cfc(34, 261, 40);
    Func_02003cfc(33, 261, 60);
    Func_02003ce4(34, 45056, 10);
    Func_02003ce4(33, 20480, 10);
    messageId += 3;
    Func_02003c84(34, 4);
    Func_02003cbc(messageId);
    Func_02003cd4(34, 0, 10);
    Func_02003c9c(33, 1);
    Func_02003c7c(33, 4);
    Func_02003cd4(33, 0, 10);
    Func_02003cfc(34, 258, 60);
    Func_02003cfc(20, 259, 0);
    Func_02003c9c(20, 2);
    Func_02003cbc(6834);
    Func_02003cd4(20, 0, 10);
    Func_02003c44(27);
    Func_02003c44(28);
    Func_02003c44(29);
    Func_02003c44(30);
    Func_02003c44(32);
    Func_02003c44(31);
    Func_02003c44(33);
    Func_02003c44(34);
    Func_02003c44(20);
    Func_02003c44(21);
    Func_02003b14(1);
    Func_02003c8c(27, 2, 0);
    Func_02003c8c(28, 2, 0);
    Func_02003c8c(29, 2, 0);
    Func_02003c8c(30, 2, 0);
    Func_02003c8c(32, 2, 0);
    Func_02003c8c(31, 2, 0);
    Func_02003c8c(33, 2, 0);
    Func_02003c8c(34, 2, 0);
    Func_02003c8c(21, 2, 40);
    Func_02003ce4(27, 32768, 0);
    Func_02003ce4(28, 32768, 0);
    Func_02003ce4(29, 32768, 0);
    Func_02003ce4(30, 32768, 0);
    Func_02003ce4(32, 32768, 0);
    Func_02003ce4(31, 32768, 0);
    Func_02003ce4(33, 32768, 0);
    Func_02003ce4(34, 32768, 40);
    Func_02003c8c(21, 4, 40);
    Func_02003cd4(21, 0, 10);
    Func_02003c9c(20, 1);
    Func_02003cd4(20, 0, 10);
    Func_02003c84(21, 3);
    Func_02003cd4(21, 0, 10);
    Func_02003c84(20, 3);
    Func_02003cd4(20, 0, 10);
    Func_02003cfc(27, 258, 40);
    Func_02003c94(27, 1);
    Func_02003cd4(27, 0, 10);
    Func_02003cfc(28, 258, 40);
    Func_02003cd4(28, 0, 10);
    Func_02003c84(21, 4);
    Func_02003bec(40);
    Func_02003c84(21, 3);
    Func_02003cd4(21, 0, 20);
    Func_02003c84(20, 3);
    Func_02003cd4(20, 0, 10);
    Func_02003ce4(27, 20480, 0);
    Func_02003ce4(28, 45056, 4);
    Func_02003ce4(29, 20480, 0);
    Func_02003ce4(30, 45056, 4);
    Func_02003ce4(32, 20480, 0);
    Func_02003ce4(31, 45056, 4);
    Func_02003ce4(33, 20480, 0);
    Func_02003ce4(34, 45056, 4);
    Func_02003c7c(27, 3);
    Func_02003c84(28, 3);
    Func_02003c7c(29, 3);
    Func_02003c84(30, 3);
    Func_02003c7c(32, 3);
    Func_02003c84(31, 3);
    Func_02003c7c(33, 3);
    Func_02003c84(34, 3);
    Func_02003c8c(20, 2, 40);
    Func_02003cd4(20, 0, 10);
    Func_02003ce4(27, 32768, 0);
    Func_02003ce4(28, 32768, 4);
    Func_02003ce4(29, 32768, 0);
    Func_02003ce4(30, 32768, 4);
    Func_02003ce4(32, 32768, 0);
    Func_02003ce4(31, 32768, 4);
    Func_02003ce4(33, 32768, 0);
    Func_02003ce4(34, 32768, 4);
    Func_02003c2c(20, 72089, 36044);
    Func_02003c2c(27, 68812, 34406);
    Func_02003c2c(28, 68812, 34406);
    Func_02003c2c(29, 65536, 32768);
    Func_02003c2c(30, 65536, 32768);
    Func_02003c2c(32, 0xf333, 0x7999);
    Func_02003c2c(31, 0xf333, 0x7999);
    Func_02003c2c(33, 58982, 29491);
    Func_02003c2c(34, 58982, 29491);
    Func_02003c2c(21, 55705, 27852);
    Func_02003cf4(27, 1);
    Func_02003cf4(28, 1);
    Func_02003cf4(29, 1);
    Func_02003cf4(30, 1);
    Func_02003cf4(32, 1);
    Func_02003cf4(31, 1);
    Func_02003cf4(33, 1);
    Func_02003cf4(34, 1);
    Func_02003cf4(20, 1);
    Func_02003cf4(21, 1);
    Func_02003c44(27);
    Func_02003c44(28);
    Func_02003c44(29);
    Func_02003c44(30);
    Func_02003c44(32);
    Func_02003c44(31);
    Func_02003c44(33);
    Func_02003c44(34);
    Func_02003c44(20);
    Func_02003c44(21);
    Func_02003b14(1);
    actionDescriptor = SceneAction_GroupOffsetMotion;
    Func_02003c34(20, actionDescriptor);
    Func_02003c34(27, actionDescriptor);
    Func_02003c34(28, actionDescriptor);
    Func_02003c34(29, actionDescriptor);
    Func_02003c34(30, actionDescriptor);
    Func_02003c34(32, actionDescriptor);
    Func_02003c34(31, actionDescriptor);
    Func_02003c34(33, actionDescriptor);
    Func_02003c34(34, actionDescriptor);
    actor21ClosingWait = Func_02003c1c(21);
    {
        u16 *state = (u16 *)(actor21ClosingWait + 100);
        s32 value = 0;
        *state = value;
    }
    Func_02003c34(21, actionDescriptor);
    do {
        Func_02003b14(1);
    } while (*(s16 *)((u8 *)Func_02003c1c(21) + 100) != 1);
    Func_02003bec(80);
    Func_02003c74(14, 22282240, 17956864);
    Func_02003b14(1);
    Func_02003c2c(14, 65536, 32768);
    Func_02003c5c(14, 224, 274);
    Func_02003ce4(14, 0, 40);
    Func_02003ce4(14, 32768, 40);
    Func_02003ce4(14, 49152, 40);
    Func_02003ce4(14, 20480, 40);
    Func_02003cfc(14, 257, 60);
    Func_02003cd4(14, 0, 10);
    Func_02003ce4(14, 0, 40);
    Func_02003ce4(14, 49152, 40);
    Func_02003ce4(14, 32768, 40);
    Func_02003d04(14, 258);
    Func_02003c8c(14, 4, 40);
    Func_02003cd4(14, 0, 20);
    Func_02003c9c(14, 2);
    Func_02003cd4(14, 0, 10);
    Func_02003c8c(14, 4, 40);
    Func_02003c2c(14, 78643, 39321);
    actor14ClosingWait = Func_02003c1c(14);
    {
        u16 *state = (u16 *)(actor14ClosingWait + 100);
        s32 value = 0;
        *state = value;
    }
    Func_02003c34(14, SceneAction_GroupOffsetMotion);
    do {
        Func_02003b14(1);
    } while (*(s16 *)((u8 *)Func_02003c1c(14) + 100) != 1);
    Func_02003c74(14, 23527424, 20578304);
    actor14FinalFacing = Func_02003c1c(14);
    {
        s32 value = 53248;
        *(u16 *)(actor14FinalFacing + 6) = value;
    }
    Func_02003c74(20, 29818880, 28442624);
    actor20FinalFacing = Func_02003c1c(20);
    {
        s32 value = 53248;
        *(u16 *)(actor20FinalFacing + 6) = value;
    }
    Func_02003c74(21, 30408704, 27262976);
    actor21FinalFacing = Func_02003c1c(21);
    {
        s32 value = 20480;
        *(u16 *)(actor21FinalFacing + 6) = value;
    }
    Func_02003c24(27);
    Func_02003c24(28);
    Func_02003c24(29);
    Func_02003c24(30);
    Func_02003c24(31);
    Func_02003c24(32);
    Func_02003c24(33);
    Func_02003c24(34);
    Func_02003d7c(17);
    if (*(s16 *)(sceneWorkspace + 364) == 9) {
        Func_02003c5c(0, 224, 458);
        playerFinalFacing = Func_02003c1c(0);
        {
            s32 value = 49152;
            *(u16 *)(playerFinalFacing + 6) = value;
        }
    } else {
        Func_02003c5c(0, 40, 248);
        playerFinalFacing = Func_02003c1c(0);
        {
            s32 value = 16384;
            *(u16 *)(playerFinalFacing + 6) = value;
        }
    }
    Func_02003d54();
    Func_02003bdc(0x911);
    goto finish;
skip_scene:
    Func_02003d7c(123);
    Func_02003d24(*(s16 *)(sceneWorkspace + 364));
    Func_02003d34();
    Func_02003d3c();
finish:
    Func_02003bfc();
}
