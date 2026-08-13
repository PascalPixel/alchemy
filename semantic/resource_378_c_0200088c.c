#include "types.h"

/*
 * resource_378 scripted scene owner at 0x0200088c (4,080 bytes).
 *
 * Boundary evidence: `push {r5,lr}` at 0x0200088c; the only return is
 * `pop {r5} / pop {r0} / bx r0` at 0x0200186c-0x02001870.  The
 * alignment word at 0x02001872 and the two literal-pool words at
 * 0x02001874-0x0200187b belong to this owner; the next prologue is
 * 0x0200187c.  The control-flow walk also accounts for the embedded
 * pools at 0x02000b48-0x02000b5f, 0x02000f3c-0x02000f43, and
 * 0x02001600-0x02001607, all skipped by branches.
 *
 * This is a long, straight-line cutscene script.  Calls retain their
 * observed register arguments where the literal arithmetic establishes
 * them; old-style declarations are intentional because these shared
 * scene helpers are called with several historical signatures.  The
 * three scene-state tests preserve their source branch arms.  Loop
 * back-edges are represented by one source statement per distinct BL
 * site, matching the machine-site accounting used by the checkers.
 *
 * Call accounting: 489 resolved BL sites (the apparent BL at 0x020017ca,
 * named Func_020017ca by the raw target walk, is an intra-owner branch
 * target in the literal-pool path, not a callee).
 */

extern u8 *Data_03001ebc;
#define SCENE_REQUEST (*(u32 *)(Data_03001ebc + 0x1c0))
#define SCENE_SETUP_WORD (*(u32 *)(Data_03001ebc + 0x1c8))
#define SKIP_BEATS (*(u16 *)(Data_03001ebc + 0x1d8))

void Func_02003070();
void Func_02003264();
void Func_020034d8();
void Func_020035f6();
void Func_02003672();
void Func_02003680();
void Func_0200369c();
void Func_020036ba();
void Func_020036c8();
void Func_020036d2();
void Func_0200384c();
void Func_0200386c();
void Func_0200387a();
void Func_020038f8();
void Func_02003916();
void Func_02003924();
s32 Func_02003e66();
void Func_02003e80();
void Func_02003ef8();
void Func_02003f10();
void Func_02003f3c();
void Func_02003f3e();
void Func_02003f48();
void Func_02003f52();
void Func_02003f56();
void Func_02003f5c();
void Func_02003f66();
void Func_02003f6c();
void Func_02003f86();
void Func_02003fc6();
void Func_02003fce();
void Func_02003fd0();
void Func_02003fd6();
void Func_02003fda();
void Func_02003fdc();
void Func_02003fde();
void Func_02003fe2();
void Func_02003fe4();
void Func_02003fee();
void Func_02003ff8();
void Func_02003ffe();
void Func_02004002();
void Func_0200400c();
void Func_02004014();
void Func_0200401e();
void Func_02004022();
void Func_02004024();
void Func_02004028();
void Func_0200402a();
void Func_0200402e();
void Func_02004030();
void Func_02004032();
void Func_0200404a();
void Func_02004064();
void Func_02004068();
void Func_0200406a();
void Func_02004072();
void Func_02004074();
void Func_02004078();
s32 Func_0200407a();
void Func_0200407e();
void Func_0200408a();
void Func_0200408e();
void Func_02004090();
void Func_0200409c();
void Func_0200409e();
void Func_020040b0();
void Func_020040ba();
void Func_020040c8();
void Func_020040ca();
void Func_020040e2();
void Func_020040ec();
void Func_02004106();
void Func_02004114();
void Func_02004118();
void Func_0200411a();
s32 Func_0200411c();
void Func_0200414a();
void Func_0200414c();
void Func_0200415a();
void Func_02004162();
void Func_02004178();
void Func_0200417a();
void Func_02004184();
void Func_0200418e();
void Func_020041ae();
void Func_020041bc();
void Func_020041be();
void Func_020041ce();
void Func_020041d4();
void Func_020041de();
void Func_020041ec();
void Func_020041f8();
void Func_02004200();
void Func_02004208();
void Func_02004210();
void Func_02004218();
void Func_02004224();
void Func_02004228();
void Func_02004232();
void Func_02004238();
void Func_02004240();
void Func_02004242();
void Func_0200425a();
void Func_0200426a();
void Func_02004270();
void Func_02004278();
void Func_02004286();
void Func_02004298();
void Func_0200429e();
void Func_020042a0();
void Func_020042a8();
void Func_020042b2();
void Func_020042b6();
void Func_020042bc();
void Func_020042c4();
void Func_020042c6();
void Func_020042cc();
void Func_020042d2();
void Func_020042e4();
void Func_020042e8();
void Func_020042f2();
void Func_02004304();
void Func_0200430a();
void Func_0200430c();
void Func_02004314();
void Func_02004324();
void Func_02004330();
void Func_0200433c();
void Func_0200434a();
void Func_0200434c();
void Func_02004350();
void Func_0200435c();
void Func_0200436a();
void Func_02004378();
void Func_0200438e();
void Func_020043b6();
void Func_020043bc();
void Func_020043c6();
void Func_020043d4();
void Func_020043d6();
void Func_020043e2();
void Func_020043f0();
void Func_020043fe();
void Func_02004404();
void Func_02004406();
void Func_02004416();
void Func_02004420();
void Func_0200442a();
void Func_02004434();
void Func_02004438();
void Func_0200443e();
void Func_02004442();
void Func_02004446();
void Func_02004454();
void Func_0200446a();
void Func_0200446c();
void Func_0200446e();
void Func_02004478();
void Func_0200447c();
void Func_02004484();
void Func_0200448a();
void Func_02004490();
void Func_0200449a();
void Func_0200449c();
void Func_020044a8();
void Func_020044aa();
void Func_020044ac();
void Func_020044b2();
void Func_020044b6();
void Func_020044ba();
void Func_020044c0();
void Func_020044c2();
void Func_020044c6();
void Func_020044c8();
void Func_020044d2();
void Func_020044d4();
void Func_020044da();
void Func_020044f2();
void Func_02004506();
void Func_02004510();
void Func_02004516();
void Func_0200451e();
void Func_02004524();
void Func_0200453c();
void Func_0200454a();
void Func_02004550();
void Func_02004558();
void Func_02004562();
void Func_02004566();
void Func_02004568();
void Func_0200456c();
void Func_02004572();
void Func_0200457a();
void Func_02004582();
void Func_02004594();
void Func_0200459e();
void Func_020045a2();
void Func_020045ae();
void Func_020045e4();
void Func_020045f2();
void Func_020045fe();
void Func_02004600();
void Func_02004610();
void Func_0200461e();
void Func_0200462c();
void Func_0200462e();
void Func_02004638();
void Func_02004646();
void Func_0200464a();
void Func_02004656();
void Func_0200466c();
void Func_02004676();
void Func_0200468c();
void Func_02004692();
void Func_02004698();
void Func_0200469c();
void Func_020046a6();
void Func_020046ac();
void Func_020046b0();
void Func_020046ba();
void Func_020046c4();
void Func_020046ca();
void Func_020046ce();
void Func_020046d0();
void Func_020046d6();
void Func_020046d8();
void Func_020046da();
void Func_020046de();
void Func_020046f6();
void Func_020046fe();
void Func_02004700();
void Func_0200470a();
void Func_02004710();
void Func_02004714();
void Func_02004720();
void Func_0200472c();
void Func_02004734();
void Func_0200473c();
void Func_02004744();
void Func_0200474c();
void Func_02004754();
void Func_02004756();
void Func_0200475a();
void Func_02004760();
void Func_02004780();
void Func_02004798();
void Func_020047a6();
void Func_020047aa();
void Func_020047b4();
void Func_020047c2();
void Func_020047c4();
void Func_020047ce();
void Func_020047d0();
void Func_020047d8();
void Func_020047e2();
void Func_020047ea();
void Func_020047ec();
void Func_020047f4();
void Func_020047f6();
void Func_020047fa();
void Func_020047fc();
void Func_02004804();
void Func_0200480a();
void Func_0200480c();
void Func_02004814();
void Func_0200481c();
void Func_0200482c();
void Func_0200483a();
void Func_02004844();
void Func_02004848();
void Func_02004854();
void Func_02004882();
void Func_02004884();
void Func_0200488c();
void Func_0200489a();
void Func_020048b4();
void Func_020048bc();
void Func_020048c6();
void Func_020048c8();
void Func_020048d4();
void Func_020048d6();
void Func_020048dc();
void Func_020048e4();
void Func_020048e8();
void Func_020048f0();
void Func_020048f2();
void Func_020048f6();
void Func_02004902();
void Func_0200490a();
void Func_02004912();
void Func_0200492c();
void Func_02004934();
void Func_02004942();
void Func_0200494c();
void Func_02004950();
void Func_02004956();
void Func_0200495e();
void Func_02004960();
void Func_02004964();
void Func_0200496a();
void Func_02004976();
void Func_0200498c();
void Func_0200499a();
void Func_020049a0();
void Func_020049a8();
void Func_020049b8();
void Func_020049bc();
void Func_020049c6();
void Func_020049d0();
void Func_020049d4();
void Func_020049d8();
void Func_020049e0();
void Func_020049e4();
void Func_020049e8();
void Func_020049ee();
void Func_020049f0();
void Func_020049f2();
void Func_020049f8();
void Func_02004a02();
void Func_02004a0c();
void Func_02004a0e();
void Func_02004a16();
void Func_02004a1a();
void Func_02004a34();
void Func_02004a40();
void Func_02004a4e();
void Func_02004a5a();
void Func_02004a5c();
void Func_02004a6a();
void Func_02004a72();
void Func_02004a86();
void Func_02004a9e();
void Func_02004aac();
void Func_02004aba();
void Func_02004abc();
void Func_02004ac8();
void Func_02004ae4();
void Func_02004ae6();
void Func_02004ae8();
void Func_02004af2();
void Func_02004afe();
void Func_02004b0a();
void Func_02004b16();
void Func_02004b18();
void Func_02004b22();
void Func_02004b26();
void Func_02004b34();
void Func_02004b42();
void Func_02004b50();
void Func_02004b54();
void Func_02004b5c();
void Func_02004b66();
void Func_02004b70();
void Func_02004b74();
void Func_02004b7a();
void Func_02004b84();
void Func_02004b8e();
void Func_02004b92();
void Func_02004bb6();
void Func_02004bbc();
void Func_02004bc2();
void Func_02004bc4();
void Func_02004bcc();
void Func_02004bce();
void Func_02004bd2();
void Func_02004bd6();
void Func_02004be0();
void Func_02004bea();
void Func_02004bf0();
void Func_02004bf4();
void Func_02004c0a();
void Func_02004c0c();
void Func_02004c14();
void Func_02004c16();
void Func_02004c1a();
void Func_02004c26();
void Func_02004c2a();
void Func_02004c30();
void Func_02004c38();
void Func_02004c3a();
void Func_02004c3c();
void Func_02004c44();
void Func_02004c46();
void Func_02004c4e();
void Func_02004c54();
void Func_02004c58();
void Func_02004c5e();
void Func_02004c72();
void Func_02004c92();
void Func_02004c94();
void Func_02004c9a();
void Func_02004c9c();
void Func_02004ca0();
void Func_02004ca4();
void Func_02004cb2();
void Func_02004cb6();
void Func_02004cba();
void Func_02004cc8();
void Func_02004ccc();
void Func_02004ce0();
void Func_02004d02();
void Func_02004d10();
void Func_02004d1a();
void Func_02004d1e();
void Func_02004d24();
void Func_02004d32();
void Func_02004d3c();
void Func_02004d48();
void Func_02004d4a();
void Func_02004d4c();
void Func_02004d54();
void Func_02004d58();
void Func_02004d5c();
void Func_02004d68();
void Func_02004d6c();
void Func_02004d76();
void Func_02004d84();
void Func_02004d8a();
void Func_02004d8e();
void Func_02004d92();
void Func_02004d94();
void Func_02004d98();
void Func_02004da2();
void Func_02004db0();
void Func_02004dc4();
void Func_02004dd4();
void Func_02004de8();
void Func_02004df0();
void Func_02004df2();
s32 Func_02004df4();
void Func_02004e0a();
void Func_02004e10();
void Func_02004e12();
void Func_02004e1e();
void Func_02004e22();
void Func_02004e26();
void Func_02004e28();
void Func_02004e32();
void Func_02004e34();
void Func_02004e40();
void Func_02004e44();
void Func_02004e5e();
u8 * Func_02004e62();
u8 * Func_02004e72();
void Func_02004e76();
void Func_02004e7a();
void Func_02004e94();
void Func_02004ea0();
void Func_02004eb8();
void Func_02004efe();
void Func_02004f04();
void Func_02004f12();
void Func_02004f20();
void Func_02004f3e();
void Func_02004f4a();

#define RunEnsembleCutscene Func_0200088c

void RunEnsembleCutscene(void)
{
    Func_02003e80(); /* 0x088e */
    if (Func_02003e66(0x201) != 0) {
        Func_02003f3c(0, 8, 0); /* 0x08a2 */
        Func_02003f3e(1, 0, 0); /* 0x08ac */
        Func_02003f48(0xb, 0, 0); /* 0x08b6 */
        Func_02003f52(0xc, 0, 0); /* 0x08c0 */
        Func_02003f5c(9, 0, 0); /* 0x08ca */
        Func_02003f66(0xa, 0, 0); /* 0x08d4 */
        Func_02003fd0(0xc00000, -1, 0xa00000, 1); /* 0x08e6 */
        Func_02003fdc(); /* 0x08ea */
        SCENE_REQUEST = 0x100; /* 0x08ee-0x08fa */
        SCENE_SETUP_WORD = 0x40; /* 0x08fc-0x0902 */
        Func_0200401e(); /* 0x0904 */
        Func_0200402a(); /* 0x0908 */
        Func_02003ef8(0x78); /* 0x090e */
    }

    Func_02004024(0x10002, 0); /* 0x091a */
    Func_02004032(1); /* 0x0920 */
    Func_02003f10(1); /* 0x0926 */
    Func_02004022(0xc00000, -1, 0xa00000, 1); /* 0x0938 */
    Func_0200402e(); /* 0x093c */
    SCENE_REQUEST = 0x209; /* 0x0940-0x094c */
    Func_02004068(); /* 0x094e */
    Func_02004074(); /* 0x0952 */
    Func_02003264(); /* 0x0956 */
    Func_0200406a(0x10000, 0); /* 0x0960 */
    Func_02004078(0x3c); /* 0x0966 */
    Func_02003f56(0x64); /* 0x096c */
    Func_02003fd6(0, 1); /* 0x0974 */
    Func_02003fde(1, 1); /* 0x097c */
    Func_02003f6c(0x1e); /* 0x0982 */
    Func_0200401e(0, 0xc, 0); /* 0x098c */
    Func_02004028(1, 0xc, 0); /* 0x0996 */
    Func_02003f86(0x14); /* 0x099c */
    Func_02003fc6(8, 1); /* 0x09a4 */
    Func_02003fce(0xc, 1); /* 0x09ac */
    Func_02003fd0(0, 0xcccc, 0x6666); /* 0x09b6 */
    Func_02003fda(1, 0xcccc, 0x6666); /* 0x09c0 */
    Func_02003fe4(0xb, 0xcccc, 0x6666); /* 0x09ca */
    Func_02003fee(0xc, 0xcccc, 0x6666); /* 0x09d4 */
    Func_02003ff8(9, 0xcccc, 0x6666); /* 0x09de */
    Func_02004002(0xa, 0xcccc, 0x6666); /* 0x09e8 */
    Func_0200400c(8, 0xcccc, 0x6666); /* 0x09f2 */
    Func_02003fe2(0x1e); /* 0x09f8 */
    Func_0200408a(0xc, 2); /* 0x0a00 */
    Func_020040b0(0x1138); /* 0x0a06 */
    Func_020040c8(0xc, 0); /* 0x0a0e */
    Func_02003ffe(0xa); /* 0x0a14 */
    Func_0200407e(0, 3); /* 0x0a1c */
    Func_0200408e(1, 3); /* 0x0a24 */
    Func_02004014(0x1e); /* 0x0a2a */
    Func_0200409c(0xc, 3); /* 0x0a32 */
    Func_02004022(0x14); /* 0x0a38 */
    Func_020040ca(0xb, 2); /* 0x0a40 */
    Func_02004030(0x14); /* 0x0a46 */
    Func_020040e2(0, 0xb, 0); /* 0x0a50 */
    Func_020040ec(1, 0xb, 0); /* 0x0a5a */
    Func_0200404a(0x14); /* 0x0a60 */
    Func_0200411a(0xb, 0); /* 0x0a68 */
    if (Func_0200407a(0, 0) == 0) {
        Func_02004064(0x14); /* 0x0a7a */
        Func_020040ec(0xb, 3); /* 0x0a82 */
        Func_02004072(0x14); /* 0x0a88 */
        Func_0200414a(0xb, 0); /* 0x0a90 */
        SKIP_BEATS++;
    } else {
        Func_02004090(0x14); /* 0x0aa6 */
        Func_02004118(0xb, 4); /* 0x0aae */
        Func_0200409e(0x14); /* 0x0ab4 */
        Func_02004184(0xb, 0); /* 0x0aca */
        SKIP_BEATS++;
    }
    Func_020040ba(0x14); /* 0x0ad0 */
    Func_02004162(9, 1); /* 0x0ad8 */
    Func_020040c8(0x14); /* 0x0ade */
    Func_0200417a(0, 9, 0); /* 0x0ae8 */
    Func_02004184(1, 9, 0); /* 0x0af2 */
    Func_0200418e(0xb, 9, 0); /* 0x0afc */
    Func_020040ec(0x14); /* 0x0b02 */
    Func_020041bc(9, 0); /* 0x0b0a */
    if (Func_0200411c(0, 0) == 0) {
        Func_02004106(0x14); /* 0x0b1c */
        Func_0200418e(9, 3); /* 0x0b24 */
        Func_02004114(0x14); /* 0x0b2a */
        Func_020041ec(9, 0); /* 0x0b32 */
    } else {
        Func_0200414c(0x14); /* 0x0b62 */
        Func_020041d4(9, 4); /* 0x0b6a */
        SKIP_BEATS++;
        Func_0200415a(0x14); /* 0x0b70 */
        SKIP_BEATS++;
        Func_02004242(9, 0); /* 0x0b88 */
    }
    Func_02004178(0x14); /* 0x0b8e */
    Func_020041f8(0, 3); /* 0x0b96 */
    Func_02004200(1, 3); /* 0x0b9e */
    Func_02004208(0xb, 3); /* 0x0ba6 */
    Func_02004210(0xc, 3); /* 0x0bae */
    Func_02004218(9, 3); /* 0x0bb6 */
    Func_02004228(0xa, 3); /* 0x0bbe */
    Func_020041ae(0x1e); /* 0x0bc4 */
    Func_020042a0(0xc, 0x101, 0); /* 0x0bce */
    Func_020041be(0x3c); /* 0x0bd4 */
    Func_02004270(0xc, 8, 0); /* 0x0bde */
    Func_020041ce(0x14); /* 0x0be4 */
    Func_02004238(0xc, 0xe0, 0x78); /* 0x0bee */
    Func_020041de(0xa); /* 0x0bf4 */
    Func_020042b6(0xc, 0); /* 0x0bfc */
    Func_020041ec(0x14); /* 0x0c02 */
    Func_0200429e(0, 8, 0); /* 0x0c0c */
    Func_020042a8(1, 8, 0); /* 0x0c16 */
    Func_020042b2(0xb, 8, 0); /* 0x0c20 */
    Func_020042bc(9, 8, 0); /* 0x0c2a */
    Func_020042c6(0xa, 8, 0); /* 0x0c34 */
    Func_02004224(0xa); /* 0x0c3a */
    Func_020042cc(0xb, 1); /* 0x0c42 */
    Func_02004232(0xa); /* 0x0c48 */
    Func_0200430a(0xb, 0); /* 0x0c50 */
    Func_02004240(0x1e); /* 0x0c56 */
    Func_020042e8(8, 3); /* 0x0c5e */
    Func_0200433c(8, 0x100, 0); /* 0x0c6a */
    Func_0200425a(0x3c); /* 0x0c70 */
    Func_0200430c(8, 0xc, 0); /* 0x0c7a */
    Func_0200426a(0x14); /* 0x0c80 */
    Func_020042f2(8, 3); /* 0x0c88 */
    Func_02004278(0xa); /* 0x0c8e */
    Func_02004350(8, 0); /* 0x0c96 */
    Func_02004286(0x14); /* 0x0c9c */
    Func_0200436a(0xc, 0x5000, 0); /* 0x0ca8 */
    Func_02004298(0x3c); /* 0x0cae */
    Func_0200434a(0xc, 8, 0); /* 0x0cb8 */
    Func_020042a8(0x14); /* 0x0cbe */
    Func_02004330(0xc, 3); /* 0x0cc6 */
    Func_020042b6(0xa); /* 0x0ccc */
    Func_0200438e(0xc, 0); /* 0x0cd4 */
    Func_020042c4(0xa); /* 0x0cda */
    Func_0200434c(8, 3); /* 0x0ce2 */
    Func_020042d2(0x14); /* 0x0ce8 */
    Func_020043b6(8, 0x3000, 0); /* 0x0cf4 */
    Func_020042e4(0xa); /* 0x0cfa */
    Func_020043bc(8, 0); /* 0x0d02 */
    Func_020042f2(0x14); /* 0x0d08 */
    Func_020043d6(0xc, 0x5000, 0); /* 0x0d14 */
    Func_02004304(0x3c); /* 0x0d1a */
    Func_020043b6(0xc, 8, 0); /* 0x0d24 */
    Func_02004314(0x32); /* 0x0d2a */
    Func_02004406(0xc, 0x101, 0); /* 0x0d34 */
    Func_02004324(0x28); /* 0x0d3a */
    Func_02004416(0, 0x101, 0); /* 0x0d44 */
    Func_02004420(1, 0x101, 0); /* 0x0d4e */
    Func_0200442a(0xb, 0x101, 0); /* 0x0d58 */
    Func_02004434(9, 0x101, 0); /* 0x0d62 */
    Func_0200443e(0xa, 0x101, 0); /* 0x0d6c */
    Func_0200435c(0x3c); /* 0x0d72 */
    Func_02004404(8, 1); /* 0x0d7a */
    Func_0200436a(0xa); /* 0x0d80 */
    Func_02004442(8, 0); /* 0x0d88 */
    Func_02004378(0xa); /* 0x0d8e */
    Func_0200446c(0, 0x100, 0); /* 0x0d9a */
    Func_02004478(1, 0x100, 0); /* 0x0da6 */
    Func_02004484(0xb, 0x100, 0); /* 0x0db2 */
    Func_02004490(0xc, 0x100, 0); /* 0x0dbe */
    Func_0200449c(9, 0x100, 0); /* 0x0dca */
    Func_020044a8(0xa, 0x100, 0); /* 0x0dd6 */
    Func_020043c6(0x3c); /* 0x0ddc */
    Func_0200446e(1, 2); /* 0x0de4 */
    Func_020043d4(0xa); /* 0x0dea */
    Func_020044ac(1, 0); /* 0x0df2 */
    Func_020043e2(0x14); /* 0x0df8 */
    Func_0200446a(8, 3); /* 0x0e00 */
    Func_020043f0(0xa); /* 0x0e06 */
    Func_020044c8(8, 0); /* 0x0e0e */
    Func_020043fe(0xa); /* 0x0e14 */
    Func_020044f2(0xc, 0x102, 0); /* 0x0e20 */
    Func_020044aa(0, 1); /* 0x0e28 */
    Func_020044b2(1, 1); /* 0x0e30 */
    Func_020044ba(0xb, 1); /* 0x0e38 */
    Func_020044c2(9, 1); /* 0x0e40 */
    Func_020044d2(0xa, 1); /* 0x0e48 */
    Func_02004438(0xa); /* 0x0e4e */
    Func_020044c0(8, 4); /* 0x0e56 */
    Func_02004446(0xa); /* 0x0e5c */
    Func_0200451e(8, 0); /* 0x0e64 */
    Func_02004454(0x1e); /* 0x0e6a */
    Func_02004506(0xc, 0, 0); /* 0x0e74 */
    Func_02004510(0xb, 0, 0); /* 0x0e7e */
    Func_0200446e(0x14); /* 0x0e84 */
    Func_02004516(0, 2); /* 0x0e8c */
    Func_0200447c(0x3c); /* 0x0e92 */
    Func_02004524(0xc, 2); /* 0x0e9a */
    Func_0200448a(0x14); /* 0x0ea0 */
    Func_0200453c(0xc, 8, 0); /* 0x0eaa */
    Func_0200449a(0x14); /* 0x0eb0 */
    Func_02004572(0xc, 0); /* 0x0eb8 */
    Func_020044a8(0x1e); /* 0x0ebe */
    Func_02004550(0xb, 2); /* 0x0ec6 */
    Func_020044b6(0x14); /* 0x0ecc */
    Func_02004568(8, 0, 0); /* 0x0ed6 */
    Func_020044c6(0x14); /* 0x0edc */
    Func_0200459e(0xb, 0); /* 0x0ee4 */
    Func_020044d4(0x14); /* 0x0eea */
    Func_020044da(0x28); /* 0x0ef0 */
    Func_02004582(8, 2); /* 0x0ef8 */
    Func_02004562(8, 0); /* 0x0f00 */
    Func_020045ae(8, 0x100); /* 0x0f0c */
    Func_020034d8(); /* 0x0f12 */
    Func_0200464a(0xc4); /* 0x0f18 */
    Func_02003672(8, 0x1200); /* 0x0f20 */
    Func_02004510(0x20); /* 0x0f26 */
    Func_02003680(8, 0x1200); /* 0x0f2e */
    Func_0200451e(0x10); /* 0x0f34 */
    Func_0200369c(8, 0x1200); /* 0x0f4a */
    Func_0200453c(8); /* 0x0f52 */
    Func_0200454a(8); /* 0x0f60 */
    Func_020036ba(8, 0x1200); /* 0x0f68 */
    Func_02004558(0x20); /* 0x0f6e */
    Func_020036c8(8, 0x1200); /* 0x0f76 */
    Func_02004566(0x60); /* 0x0f7c */
    Func_0200456c(0x20); /* 0x0f82 */
    Func_0200462c(8, 0); /* 0x0f8a */
    Func_0200457a(0x1e); /* 0x0f90 */
    Func_020035f6(); /* 0x0f94 */
    Func_020045fe(8, 1); /* 0x0f9c */
    Func_0200462e(8, 2); /* 0x0fa4 */
    Func_02004594(0x14); /* 0x0faa */
    Func_0200466c(8, 0); /* 0x0fb2 */
    Func_020045a2(0x14); /* 0x0fb8 */
    Func_0200469c(0xc, 0x102); /* 0x0fc2 */
    Func_020046a6(0, 0x102); /* 0x0fcc */
    Func_020046b0(1, 0x102); /* 0x0fd6 */
    Func_020046ba(0xb, 0x102); /* 0x0fe0 */
    Func_020046c4(9, 0x102); /* 0x0fea */
    Func_020046ce(0xa, 0x102); /* 0x0ff4 */
    Func_020045e4(0x3c); /* 0x0ffa */
    Func_0200468c(0xa, 1); /* 0x1002 */
    Func_020045f2(0x14); /* 0x1008 */
    Func_020046ca(0xa, 0); /* 0x1010 */
    Func_02004600(0x14); /* 0x1016 */
    Func_020046ba(0, 1, 0); /* 0x1020 */
    Func_02004610(0x3c); /* 0x1026 */
    Func_02004698(8, 4); /* 0x102e */
    Func_0200461e(0x14); /* 0x1034 */
    Func_020046d0(0, 8, 0); /* 0x103e */
    Func_020046da(1, 8, 0); /* 0x1048 */
    Func_02004638(0x14); /* 0x104e */
    Func_02004710(8, 0); /* 0x1056 */
    Func_02004646(0x1e); /* 0x105c */
    Func_02004700(0, 1, 0); /* 0x1066 */
    Func_02004656(0x14); /* 0x106c */
    Func_020046d6(0, 3); /* 0x1074 */
    Func_020046de(1, 3); /* 0x107c */
    Func_02004720(9, 0xa, 0); /* 0x1086 */
    Func_02004676(0x14); /* 0x108c */
    Func_020046f6(9, 3); /* 0x1094 */
    Func_020046fe(0xa, 3); /* 0x109c */
    Func_02004714(0xa); /* 0x10a2 */
    Func_02004692(0x1e); /* 0x10a8 */
    Func_0200474c(0, 0xb, 0); /* 0x10b2 */
    Func_02004756(1, 0xc, 0); /* 0x10bc */
    Func_020046ac(0x14); /* 0x10c2 */
    Func_0200472c(0, 3); /* 0x10ca */
    Func_02004734(0xb, 3); /* 0x10d2 */
    Func_0200473c(1, 3); /* 0x10da */
    Func_02004744(0xc, 3); /* 0x10e2 */
    Func_0200475a(0xc); /* 0x10e8 */
    Func_020046d8(0x3c); /* 0x10ee */
    Func_02004780(8, 2); /* 0x10f6 */
    Func_02004760(8, 0); /* 0x10fe */
    Func_020047aa(8, 0x100); /* 0x1108 */
    Func_020036d2(); /* 0x110c */
    Func_02004844(0xc4); /* 0x1112 */
    Func_0200386c(8, 0x1200); /* 0x111a */
    Func_0200470a(0x20); /* 0x1120 */
    Func_0200387a(8, 0x1200); /* 0x1128 */
    Func_020047c4(0, 8, 0); /* 0x1132 */
    Func_020047ce(1, 8, 0); /* 0x113c */
    Func_020047d8(0xb, 8, 0); /* 0x1146 */
    Func_020047e2(0xc, 8, 0); /* 0x1150 */
    Func_020047ec(9, 8, 0); /* 0x115a */
    Func_020047f6(0xa, 8, 0); /* 0x1164 */
    Func_02004754(0x10); /* 0x116a */
    Func_020047f4(0, 1); /* 0x1172 */
    Func_020047fc(1, 1); /* 0x117a */
    Func_02004804(0xb, 1); /* 0x1182 */
    Func_0200480c(0xc, 1); /* 0x118a */
    Func_02004814(9, 1); /* 0x1192 */
    Func_0200481c(0xa, 1); /* 0x119a */
    Func_020038f8(8, 0x1200); /* 0x11a6 */
    Func_02004798(8); /* 0x11ae */
    Func_020047a6(8); /* 0x11bc */
    Func_02003916(8, 0x1200); /* 0x11c4 */
    Func_020047b4(0x20); /* 0x11ca */
    Func_02003924(8, 0x1200); /* 0x11d2 */
    Func_020047c2(0x80); /* 0x11d8 */
    Func_02004882(8, 0); /* 0x11e0 */
    Func_020047d0(0x1e); /* 0x11e6 */
    Func_0200384c(); /* 0x11ea */
    Func_02004854(8, 1); /* 0x11f2 */
    Func_02004884(8, 2); /* 0x11fa */
    Func_020047ea(0x1e); /* 0x1200 */
    Func_020048dc(0xc, 0x105, 0); /* 0x120a */
    Func_020047fa(0x14); /* 0x1210 */
    Func_020048b4(9, 0xa, 0); /* 0x121a */
    Func_0200480a(0x28); /* 0x1220 */
    Func_020048bc(9, 8, 0); /* 0x122a */
    Func_020048c6(0xa, 8, 0); /* 0x1234 */
    Func_020048f6(0xc, 0); /* 0x123c */
    Func_0200482c(0x14); /* 0x1242 */
    Func_020048d4(0xb, 2); /* 0x124a */
    Func_0200483a(0x14); /* 0x1250 */
    Func_02004912(0xb, 0); /* 0x1258 */
    Func_02004848(0x14); /* 0x125e */
    Func_020048e8(0, 1); /* 0x1266 */
    Func_020048f0(1, 1); /* 0x126e */
    Func_02004912(0, 1, 0); /* 0x1278 */
    Func_02004902(9, 1); /* 0x1280 */
    Func_0200490a(0xa, 1); /* 0x1288 */
    Func_0200492c(9, 0xa, 0); /* 0x1292 */
    Func_02004976(0xc, 0x102); /* 0x129c */
    Func_0200488c(0x3c); /* 0x12a2 */
    Func_02004964(8, 0); /* 0x12aa */
    Func_0200489a(0x14); /* 0x12b0 */
    Func_0200494c(0, 8, 0); /* 0x12ba */
    Func_02004956(1, 8, 0); /* 0x12c4 */
    Func_02004960(9, 8, 0); /* 0x12ce */
    Func_0200496a(0xa, 8, 0); /* 0x12d8 */
    Func_020048c8(0x14); /* 0x12de */
    Func_020049a0(1, 0); /* 0x12e6 */
    Func_020048d6(0x14); /* 0x12ec */
    Func_0200495e(8, 3); /* 0x12f4 */
    Func_020048e4(0x14); /* 0x12fa */
    Func_020049bc(8, 0); /* 0x1302 */
    Func_020048f2(0x14); /* 0x1308 */
    Func_020049e4(0, 0x101, 0); /* 0x1312 */
    Func_020049ee(1, 0x101, 0); /* 0x131c */
    Func_020049f8(0xb, 0x101, 0); /* 0x1326 */
    Func_02004a02(0xc, 0x101, 0); /* 0x1330 */
    Func_02004a0c(9, 0x101, 0); /* 0x133a */
    Func_02004a16(0xa, 0x101, 0); /* 0x1344 */
    Func_02004934(0x3c); /* 0x134a */
    Func_020049bc(8, 4); /* 0x1352 */
    Func_02004942(0x14); /* 0x1358 */
    Func_02004a1a(8, 0); /* 0x1360 */
    Func_02004950(0x14); /* 0x1366 */
    Func_020049d0(0, 3); /* 0x136e */
    Func_020049d8(1, 3); /* 0x1376 */
    Func_020049e0(0xb, 3); /* 0x137e */
    Func_020049e8(0xc, 3); /* 0x1386 */
    Func_020049f0(9, 3); /* 0x138e */
    Func_020049f8(0xa, 3); /* 0x1396 */
    Func_02004a0e(0xa); /* 0x139c */
    Func_0200498c(0x14); /* 0x13a2 */
    Func_02004a34(0xc, 1); /* 0x13aa */
    Func_0200499a(0x14); /* 0x13b0 */
    Func_02004a72(0xc, 0); /* 0x13b8 */
    Func_020049a8(0x14); /* 0x13be */
    Func_02004a5a(8, 0xc, 0); /* 0x13c8 */
    Func_020049b8(0x14); /* 0x13ce */
    Func_02004a40(8, 3); /* 0x13d6 */
    Func_020049c6(0x14); /* 0x13dc */
    Func_02004a9e(8, 0); /* 0x13e4 */
    Func_020049d4(0x32); /* 0x13ea */
    Func_02004a86(8, 0, 0); /* 0x13f4 */
    Func_020049e4(0x14); /* 0x13fa */
    Func_02004abc(8, 0); /* 0x1402 */
    Func_020049f2(0x1e); /* 0x1408 */
    Func_02004ae6(0, 0x102, 0); /* 0x1414 */
    Func_02004af2(1, 0x102, 0); /* 0x1420 */
    Func_02004afe(0xb, 0x102, 0); /* 0x142c */
    Func_02004b0a(0xc, 0x102, 0); /* 0x1438 */
    Func_02004b16(9, 0x102, 0); /* 0x1444 */
    Func_02004b22(0xa, 0x102, 0); /* 0x1450 */
    Func_02004a40(0x50); /* 0x1456 */
    Func_02004ae8(0xb, 1); /* 0x145e */
    Func_02004a4e(0x14); /* 0x1464 */
    Func_02004b26(0xb, 0); /* 0x146c */
    Func_02004a5c(0x14); /* 0x1472 */
    Func_02004ae4(8, 4); /* 0x147a */
    Func_02004a6a(0x14); /* 0x1480 */
    Func_02004b5c(0, 0x101, 0); /* 0x148a */
    Func_02004b66(1, 0x101, 0); /* 0x1494 */
    Func_02004b70(0xb, 0x101, 0); /* 0x149e */
    Func_02004b7a(0xc, 0x101, 0); /* 0x14a8 */
    Func_02004b84(9, 0x101, 0); /* 0x14b2 */
    Func_02004b8e(0xa, 0x101, 0); /* 0x14bc */
    Func_02004aac(0x50); /* 0x14c2 */
    Func_02004b54(8, 1); /* 0x14ca */
    Func_02004aba(0x14); /* 0x14d0 */
    Func_02004b92(8, 0); /* 0x14d8 */
    Func_02004ac8(0x14); /* 0x14de */
    Func_02004bc2(0, 0x102); /* 0x14e8 */
    Func_02004bcc(1, 0x102); /* 0x14f2 */
    Func_02004bd6(0xb, 0x102); /* 0x14fc */
    Func_02004be0(0xc, 0x102); /* 0x1506 */
    Func_02004bea(9, 0x102); /* 0x1510 */
    Func_02004bf4(0xa, 0x102); /* 0x151a */
    Func_02004b0a(0x3c); /* 0x1520 */
    Func_02004b92(0xc, 4); /* 0x1528 */
    Func_02004b18(0x14); /* 0x152e */
    Func_02004bf0(0xc, 0); /* 0x1536 */
    Func_02004b26(0x14); /* 0x153c */
    Func_02004bce(8, 1); /* 0x1544 */
    Func_02004b34(0x14); /* 0x154a */
    Func_02004bbc(8, 4); /* 0x1552 */
    Func_02004b42(0x14); /* 0x1558 */
    Func_02004c1a(8, 0); /* 0x1560 */
    Func_02004b50(0x28); /* 0x1566 */
    Func_02004c0a(0, 1, 0); /* 0x1570 */
    Func_02004c14(9, 0xa, 0); /* 0x157a */
    Func_02004c16(0xc, 0, 0); /* 0x1584 */
    Func_02004b74(0x3c); /* 0x158a */
    Func_02004c26(0, 8, 0); /* 0x1594 */
    Func_02004c30(1, 8, 0); /* 0x159e */
    Func_02004c3a(0xb, 8, 0); /* 0x15a8 */
    Func_02004c44(0xc, 8, 0); /* 0x15b2 */
    Func_02004c4e(9, 8, 0); /* 0x15bc */
    Func_02004c58(0xa, 8, 0); /* 0x15c6 */
    Func_02004bb6(0x14); /* 0x15cc */
    Func_02004c5e(0xb, 1); /* 0x15d4 */
    Func_02004bc4(0x14); /* 0x15da */
    Func_02004c9c(0xb, 0); /* 0x15e2 */
    Func_02004bd2(0x3c); /* 0x15e8 */
    Func_02004c3c(8, 0xc8, 0x88); /* 0x15f2 */
    Func_02004c9a(0, 0, 0); /* 0x1608 */
    Func_02004ca4(0xc, 0, 0); /* 0x1612 */
    Func_02004cb6(8, 1, 0); /* 0x161c */
    Func_02004c0c(0x28); /* 0x1622 */
    Func_02004c94(8, 3); /* 0x162a */
    Func_02004c1a(0x32); /* 0x1630 */
    Func_02004ccc(8, 0, 0); /* 0x163a */
    Func_02004c2a(0x28); /* 0x1640 */
    Func_02004cb2(8, 3); /* 0x1648 */
    Func_02004c38(0x32); /* 0x164e */
    Func_02004ce0(8, 1); /* 0x1656 */
    Func_02004c46(0x14); /* 0x165c */
    Func_02004d1e(8, 0); /* 0x1664 */
    Func_02004c54(0x1e); /* 0x166a */
    Func_02004d48(0, 0x102, 0); /* 0x1676 */
    Func_02004d54(1, 0x102, 0); /* 0x1682 */
    Func_02004c72(0x3c); /* 0x1688 */
    Func_02004d6c(0xb, 0x102); /* 0x1692 */
    Func_02004d24(0xb, 1); /* 0x169a */
    Func_02004d5c(0xb, 0); /* 0x16a2 */
    Func_02004c92(0x1e); /* 0x16a8 */
    Func_02004d1a(8, 3); /* 0x16b0 */
    Func_02004ca0(0x1e); /* 0x16b6 */
    Func_02004d94(0xc, 0x102, 0); /* 0x16c2 */
    Func_02004d4c(0xc, 1); /* 0x16ca */
    Func_02004cba(0x14); /* 0x16d0 */
    Func_02004d92(0xc, 0); /* 0x16d8 */
    Func_02004cc8(0x1e); /* 0x16de */
    Func_02004d32(8, 0xa8, 0x78); /* 0x16e8 */
    Func_02004d84(0, 8, 0); /* 0x16f2 */
    Func_02004d8e(1, 8, 0); /* 0x16fc */
    Func_02004d98(0xc, 8, 0); /* 0x1706 */
    Func_02004dd4(8, 0xd000, 0); /* 0x1712 */
    Func_02004d02(0x14); /* 0x1718 */
    Func_02004d8a(8, 4); /* 0x1720 */
    Func_02004d10(0x14); /* 0x1726 */
    Func_02004de8(8, 0); /* 0x172e */
    Func_02004d1e(0x1e); /* 0x1734 */
    Func_02004e12(0, 0x102, 0); /* 0x1740 */
    Func_02004e1e(1, 0x102, 0); /* 0x174c */
    Func_02004d3c(0x3c); /* 0x1752 */
    Func_02004dc4(8, 3); /* 0x175a */
    Func_02004d4a(0xa); /* 0x1760 */
    Func_02004e22(8, 0); /* 0x1768 */
    Func_02004d58(0xa); /* 0x176e */
    Func_02004e0a(8, 0, 0); /* 0x1778 */
    Func_02004d68(0x14); /* 0x177e */
    Func_02004e40(8, 0); /* 0x1786 */
    Func_02004d76(0x14); /* 0x178c */
    Func_02004e28(0xc, 0, 0); /* 0x1796 */
    Func_02004e7a(1, 0x102); /* 0x17a0 */
    Func_02004e32(1, 2); /* 0x17a8 */
    Func_02004e44(1, 0, 0); /* 0x17b2 */
    Func_02004da2(0x14); /* 0x17b8 */
    Func_02004e7a(1, 0); /* 0x17c0 */
    Func_02004db0(0x1e); /* 0x17c6 */
    Func_02004e76(0x1162); /* 0x17cc */
    Func_02004e5e(8, 1); /* 0x17d4 */
    Func_02004dc4(0x14); /* 0x17da */
    Func_02004e94(8, 0); /* 0x17e2 */
    if (Func_02004df4(0, 0) == 0) {
        Func_02003070(); /* 0x17f2 */
        Func_02004f04(0, 0); /* 0x17fa */
        Func_02004f12(0x78); /* 0x1800 */
        Func_02004df0(0x78); /* 0x1806 */
        Func_02004f3e(0x56); /* 0x180c */
        Func_02004f4a(); /* 0x1810 */
        Func_02004df2(0x9f0); /* 0x1818 */
        Func_02004f20(0x1e); /* 0x181e */
    } else {
        u8 *actor;

        Func_02004e10(0x1e); /* 0x1826 */
        Func_02004eb8(0xc, 1); /* 0x182e */
        Func_02004ea0(0xc, 4); /* 0x1836 */
        Func_02004e26(0x14); /* 0x183c */
        Func_02004efe(0xc, 0); /* 0x1844 */
        Func_02004e34(0x14); /* 0x184a */
        actor = Func_02004e62(0xa); /* 0x1850 */
        actor[0x23] &= (u8)~2;
        actor = Func_02004e72(0xa); /* 0x1860 */
        *(u32 *)(actor + 0x6c) = 0x02008849;
    }
    Func_02004e62(); /* 0x1868 */
}
