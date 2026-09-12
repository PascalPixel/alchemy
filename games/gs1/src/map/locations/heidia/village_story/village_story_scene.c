#include "types.h"
void Func_0200646e_sequence();

#if defined(GS1_EDITION_JA)
#define SCENE_STEP_VALUE_run_scene_58_sequence 0x1725
#elif defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#define SCENE_STEP_VALUE_run_scene_58_sequence 0x157e
#else
#define SCENE_STEP_VALUE_run_scene_58_sequence 0x159c
#endif
#if defined(GS1_EDITION_DE)
#define Scene58Work Data_03001ecc
#else
#define Scene58Work Data_03001ebc
#endif

extern u8 *Scene58Work;
extern u8 Value_0000003a;

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

/*
 * resource_39d: scene-58 renderer setup.  The owner runs from 0x02000b24 to
 * its interworking epilogue at 0x02001ad4 and includes the 22-byte literal and
 * alignment tail that ends immediately before the next prologue, 4044 bytes in
 * all.  The declarations below are old-style because the image exposes no
 * prototype for these calls; call sites vary in arity.
 */

extern void Func_02003208();
extern void Func_080000c0();
extern s32 Func_080091e0();
extern void Func_0808a010();
extern void Func_0808a018();
extern void Func_0808a020();
extern s32 Func_0808a070();
extern u8 *Func_0808a080();
extern void Func_0808a090();
extern void Func_0808a0c0();
extern void Func_0808a0c8();
extern void Func_0808a0d0();
extern void Func_0808a0e8();
extern void Func_0808a0f0();
extern void Func_0808a100();
extern s32 Func_0808a110();
extern void Func_0808a130();
extern void Func_0808a138();
extern void Func_0808a150();
extern void Func_0808a170();
extern void Func_0808a178();
extern void Func_0808a180();
extern void Func_0808a188();
extern void Func_0808a1b8();
extern void Func_0808a1e0();
extern void Func_0808a1e8();
extern void Func_0808a1f0();
extern void Func_0808a208();
extern void Func_0808a210();
extern void Func_0808a218();
extern u8 *Func_0808a228();
extern void Func_0808a250();
extern void Func_0808a260();
extern void Func_080f9010();

/*
 * Loader-relocated ROM calls: each name spells the pre-relocation call word
 * the image holds, not a runtime address.
 */
void Func_020041d2_sequence();
void Func_02004200();
s32 Func_0200421e();
void Func_0200423a();
void Func_02004244();
void Func_0200424e();
s32 Func_02004256();
void Func_02004258();
s32 Func_0200426a();
s32 Func_0200427e();
void Func_0200429c();
void Func_020042b0();
void Func_020042be();
void Func_020042c4();
void Func_020042ca();
void Func_020042d6();
void Func_020042e2();
void Func_020042f0();
void Func_020042f8();
void Func_020042fe();
void Func_0200430a();
void Func_0200431c();
void Func_0200432e();
void Func_02004354();
void Func_0200435c();
void Func_0200436c();
void Func_02004370();
void Func_02004376();
void Func_0200437e();
void Func_02004388();
void Func_02004390();
void Func_0200439e();
void Func_020043a2();
void Func_020043be();
void Func_020043d4();
void Func_020043d6();
void Func_020043e2();
void Func_020043e6();
void Func_020043f0();
void Func_020043fc();
void Func_02004400();
s32 Func_02004410();
void Func_0200441a();
void Func_0200441c();
void Func_0200442a();
void Func_0200442c();
void Func_02004438();
void Func_0200443e();
void Func_02004444();
void Func_02004446();
void Func_02004448();
s32 Func_02004456();
void Func_0200445e();
void Func_02004468();
void Func_0200446a();
void Func_02004478();
void Func_0200448c();
void Func_02004490();
void Func_020044c0();
void Func_020044cc();
void Func_020044d0();
void Func_020044d0_a();
void Func_020044d4();
s32 Func_020044d6();
void Func_020044e6();
void Func_020044ee();
void Func_02004512();
void Func_02004518();
void Func_0200451c();
void Func_02004522();
void Func_02004536();
void Func_02004540();
void Func_02004546();
void Func_02004556();
void Func_0200456e();
void Func_02004584();
void Func_0200459e();
void Func_020045a2();
void Func_020045a8();
void Func_020045b6();
void Func_020045ba();
void Func_020045bc();
void Func_020045c0();
void Func_020045cc();
void Func_020045d0();
void Func_020045e0();
void Func_020045ee();
void Func_020045f2();
void Func_020045f8();
void Func_020045fa();
void Func_02004604();
void Func_0200460a();
void Func_02004610();
void Func_02004618();
void Func_02004628();
void Func_0200463a();
void Func_0200463e();
void Func_02004656();
void Func_02004670();
void Func_02004678();
void Func_02004682();
void Func_02004688();
void Func_02004698();
void Func_020046a0();
void Func_020046c0();
void Func_020046c6();
void Func_020046d4();
void Func_020046e2();
void Func_020046ee();
void Func_020046f2();
void Func_020046f6();
void Func_020046fa();
void Func_02004706();
void Func_02004712();
void Func_0200471e();
void Func_0200472a();
void Func_02004736();
void Func_02004746();
void Func_02004748();
void Func_0200474a();
void Func_02004750();
void Func_02004764();
void Func_02004768();
void Func_02004782();
void Func_02004784();
void Func_02004794();
void Func_02004798();
void Func_0200479a();
void Func_0200479c();
void Func_020047a8();
void Func_020047ac();
void Func_020047b2();
void Func_020047c0();
void Func_020047ca();
void Func_020047cc();
void Func_020047d0();
void Func_020047e4();
void Func_020047ea();
void Func_0200480a();
void Func_0200480c();
void Func_02004822();
void Func_02004824();
void Func_02004832();
void Func_02004836();
void Func_0200483a();
void Func_02004842();
void Func_0200484a();
void Func_0200484c();
s32 Func_0200484e();
void Func_0200485a();
void Func_0200485c();
void Func_02004868();
void Func_02004872();
void Func_02004894();
void Func_0200489e();
void Func_020048b6();
void Func_020048ce();
void Func_020048d8();
void Func_020048f4();
void Func_02004900();
void Func_0200490a();
void Func_0200491c();
void Func_02004922();
void Func_0200492a();
void Func_0200492e();
void Func_02004936();
void Func_02004938();
void Func_0200493a();
void Func_02004942();
void Func_02004946();
void Func_0200494e();
void Func_02004950();
void Func_0200495a();
void Func_02004974();
void Func_02004986();
void Func_02004988();
void Func_0200499a();
void Func_020049a0();
void Func_020049a6();
void Func_020049ae();
void Func_020049b8();
void Func_020049bc();
void Func_020049cc();
void Func_020049ce();
void Func_020049e8();
void Func_020049ec();
void Func_020049f2();
void Func_020049f4();
void Func_020049fc();
void Func_02004a00();
void Func_02004a0c();
void Func_02004a10();
void Func_02004a22();
void Func_02004a32();
void Func_02004a36();
void Func_02004a44();
void Func_02004a52();
void Func_02004a56();
void Func_02004a5e();
void Func_02004a64();
void Func_02004a72();
void Func_02004a80();
void Func_02004a8e();
void Func_02004a92();
void Func_02004a98();
void Func_02004aa0();
void Func_02004aa2();
void Func_02004aac();
s32 Func_02004ac6();
void Func_02004ad2();
void Func_02004adc();
void Func_02004ae6();
void Func_02004af0();
void Func_02004af4();
void Func_02004afe();
s32 Func_02004b02();
void Func_02004b0a();
void Func_02004b14();
void Func_02004b16();
void Func_02004b22();
void Func_02004b2c();
void Func_02004b42();
void Func_02004b44();
void Func_02004b4e();
void Func_02004b56();
void Func_02004b60();
void Func_02004b88();
void Func_02004b8e();
void Func_02004ba0();
void Func_02004ba8();
void Func_02004bb0();
void Func_02004bc2();
void Func_02004bc8();
void Func_02004bd4();
void Func_02004bda();
void Func_02004bea();
void Func_02004bf0();
void Func_02004c00();
void Func_02004c0a();
void Func_02004c10();
void Func_02004c42();
void Func_02004c44();
void Func_02004c52();
void Func_02004c54();
void Func_02004c5a();
void Func_02004c5c();
void Func_02004c62();
void Func_02004c68();
void Func_02004c74();
void Func_02004c76();
void Func_02004c82();
void Func_02004c9c();
void Func_02004c9e();
void Func_02004ca4();
void Func_02004caa();
void Func_02004cac();
void Func_02004cb4();
void Func_02004cbc();
void Func_02004cc6();
void Func_02004cce();
void Func_02004cd4();
void Func_02004ce4();
void Func_02004ce8();
void Func_02004cfc();
void Func_02004d14();
void Func_02004d16();
void Func_02004d22();
void Func_02004d2c();
void Func_02004d30();
void Func_02004d3c();
void Func_02004d3e();
void Func_02004d4e();
void Func_02004d56();
void Func_02004d60();
void Func_02004d66();
s32 Func_02004d66_a();
void Func_02004d78();
void Func_02004d7a();
s32 Func_02004d80();
void Func_02004d86();
void Func_02004d8a();
void Func_02004da0();
void Func_02004da2();
void Func_02004da8();
s32 Func_02004dbe();
void Func_02004dc0();
s32 Func_02004dd0();
void Func_02004ddc();
s32 Func_02004de0();
void Func_02004dec();
void Func_02004dec_a();
s32 Func_02004dec_b();
void Func_02004df8();
s32 Func_02004e02();
s32 Func_02004e0e();
void Func_02004e14();
void Func_02004e28();
void Func_02004e2c();
void Func_02004e8a();
void Func_02004e92();
void Func_02004eb6();
void Func_02004ec0();
void Func_02004ec2();
void Func_02004ed6();
void Func_02004ee2();
void Func_02004eec();
void Func_02004f04();
void Func_02004f0c();
void Func_02004f16();
void Func_02004f18();
void Func_02004f1a();
void Func_02004f20();
void Func_02004f24();
void Func_02004f30();
void Func_02004f3c();
void Func_02004f42();
void Func_02004f48();
void Func_02004f6e();
void Func_02004f82();
void Func_02004f8e();
void Func_02004f90();
void Func_02004f94();
void Func_02004f9c();
s32 Func_02004fa2();
void Func_02004fa4();
void Func_02004fbe();
void Func_02004fc0();
s32 Func_02004fc2();
void Func_02004fd0();
void Func_02004fd4();
void Func_02004fde();
void Func_02004fe0();
void Func_02004fec();
void Func_02004ff6();
void Func_02004ff8();
void Func_02005004();
void Func_02005010();
void Func_02005012();
void Func_0200501c();
void Func_02005022();
void Func_0200502a();
void Func_02005032();
void Func_02005036();
void Func_02005042();
void Func_02005050();
void Func_02005058();
s32 Func_02005066();
void Func_02005068();
void Func_0200508c();
void Func_0200508e();
void Func_02005098();
void Func_020050a6();
void Func_020050be();
void Func_020050c8();
void Func_020050ce();
void Func_020050e4();
s32 Func_020050e6();
void Func_02005106();
void Func_02005108();
void Func_0200511a();
s32 Func_0200511e();
void Func_02005120();
void Func_0200512c();
void Func_02005140();
s32 Func_02005140_a();

void Func_020045a8_a();
void Func_02004628_a();
void Func_020046a0_a();
void Func_02004712_a();
void Func_020047ca_a();
void Func_0200480a_a();
void Func_02004900_a();
void Func_0200499a_a();
void Func_020049f2_a();
void Func_02004a64_a();
void Func_02004a98_a();
void Func_02004aa2_a();
void Func_02004ae6_a();
void Func_02004b16_a();
void Func_02004b22_a();
void Func_02004c62_a();
void Func_02004c82_a();
void Func_02004ce4_a();
void Func_02004ddc_a();
void Func_02004f0c_a();
void Func_02004f20_a();
void Func_02004f48_a();
void Func_02004fde_a();
void Func_0200502a_a();
s32 Func_02005066_a();
s32 Func_02005066_b();
void Func_02005098_a();
void Func_0200515c();
void Func_02005164();
void Func_0200516e();
s32 Func_02005170();
void Func_02005188();
void Func_020051b0();
void Func_020051b2();
void Func_020051cc();
void Func_020051e0();
void Func_020051fa();
void Func_020051fe();
void Func_02005204();
void Func_02005208();
void Func_02005218();
void Func_02005228();
void Func_02005294();
void Func_0200529e();

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}



#include "types.h"

/* Ordinary C field access retained from the reviewed disassembly dataflow. */
extern u8 *Data_03001ebc;
extern u8 *Data_03001e70;

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))
#define SCENE_REQUEST(workspace) FIELD((workspace), s32 *, 0x1C0)
#define SCENE_SKIP_COUNT(workspace) FIELD((workspace), u16 *, 0x1D8)

/*
 * resource_39d owner 0x02001af0..0x02002ddb (0x12ec = 4844 bytes).
 *
 * Scene 59 setup sequence: imported service calls with constant arguments,
 * branch-selected paths, record-field updates, and workspace counters.
 * ROM services are reached through loader-relocated call words; each call
 * names the pre-relocation word the image holds, as the retained assembly
 * does with its per-site targets.
 */

extern void Func_02002f18();
extern void Func_02003060();
extern void Func_02003208();
extern void Func_02003324();
extern void Func_080000c0();
extern void Func_08009128();
extern void Func_08009180();
extern s32 Func_080091e0();
extern void Func_080770c8();
extern void Func_0808a010();
extern void Func_020036d8();
extern void Func_020036e0();
extern s32 Func_020036e8();
extern u8 *Func_020036f0();
extern void Func_020036f8();
extern void Func_02003710();
extern void Func_02003718();
extern void Func_02003720();
extern void Func_02003728();
extern void Func_02003730();
extern void Func_02003738();
extern void Func_02003740();
extern void Func_02003748();
extern void Func_02003750();
extern void Func_02003758();
extern void Func_02003768();
extern s32 Func_02003770();
extern void Func_02003778();
extern void Func_02003780();
extern void Func_02003788();
extern void Func_02003790();
extern void Func_02003798();
extern void Func_020037a0();
extern void Func_020037a8();
extern void Func_020037b0();
extern void Func_020037b8();
extern u8 *Func_020037c0();
extern void Func_020037f8();
extern void Func_02003800();
extern void Func_02003838();
extern void Func_02003840();

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */
void Func_020051ce();
void Func_020051d4();
void Func_020051de();
s32 Func_020051ec();
s32 Func_0200521c();
void Func_02005220();
void Func_02005238();
void Func_02005246();
void Func_02005254();
void Func_02005262();
void Func_02005266();
s32 Func_02005276();
void Func_0200529c();
void Func_020052a0();
void Func_020052aa();
void Func_020052b0();
void Func_020052ce();
void Func_020052da();
void Func_020052de();
void Func_020052e6();
void Func_020052ec();
void Func_020052f2();
void Func_0200531c();
void Func_02005330();
void Func_02005340();
void Func_0200534e();
void Func_02005352();
void Func_0200535e();
void Func_0200536c();
void Func_02005374();
void Func_02005388();
void Func_0200539c();
void Func_020053a0();
void Func_020053a2();
void Func_020053a6();
void Func_020053ba();
void Func_020053ba_a();
void Func_020053c6();
void Func_020053c6_a();
void Func_020053ce();
void Func_020053d2();
void Func_020053e0();
void Func_020053ea();
void Func_020053fa();
void Func_0200540a();
void Func_0200541e();
void Func_02005420();
void Func_0200542c();
void Func_02005432();
void Func_0200543e();
void Func_0200544a();
void Func_02005454();
void Func_0200545c();
s32 Func_02005470();
void Func_02005474();
void Func_02005476();
void Func_02005476_a();
void Func_0200547c();
void Func_02005482();
void Func_02005498();
void Func_020054aa();
void Func_020054be();
void Func_020054c0();
void Func_020054c6();
void Func_020054ce();
void Func_020054d6();
void Func_020054da();
void Func_020054e4();
void Func_020054ee();
void Func_020054f6();
void Func_020054fa();
void Func_020054fa_a();
void Func_02005506();
void Func_02005516();
void Func_0200552e();
void Func_02005540();
void Func_02005542();
void Func_0200554e();
void Func_02005558();
void Func_02005558_a();
void Func_0200555a();
void Func_02005566();
void Func_02005570();
void Func_02005572();
void Func_02005578();
void Func_0200557e();
void Func_02005588();
void Func_0200558a();
void Func_02005596();
void Func_02005598();
void Func_020055a2();
void Func_020055b0();
void Func_020055c2();
void Func_020055ca();
void Func_020055d2();
void Func_020055d6();
void Func_020055d8();
void Func_020055da();
void Func_020055e6();
s32 Func_020055f4();
void Func_020055fa();
void Func_020055fa_a();
void Func_020055fa_b();
void Func_02005612();
void Func_0200562a();
void Func_02005634();
void Func_02005642();
void Func_0200565e();
void Func_02005660();
void Func_02005662();
void Func_02005664();
s32 Func_02005674();
void Func_02005684();
void Func_02005698();
void Func_020056a2();
void Func_020056a8();
void Func_020056c0();
void Func_020056c4();
void Func_020056c4_a();
void Func_020056ea();
void Func_020056ea_a();
void Func_020056f4();
void Func_020056f8();
void Func_020056fa();
void Func_02005706();
void Func_02005708();
void Func_02005714();
void Func_02005718();
void Func_02005722();
void Func_02005726();
void Func_0200572c();
void Func_0200572e();
void Func_02005734();
void Func_02005742();
void Func_02005742_a();
void Func_02005750();
void Func_02005752();
void Func_0200575e();
void Func_02005766();
void Func_0200576a();
void Func_0200576c();
void Func_02005774();
void Func_0200577e();
void Func_02005786();
void Func_02005786_a();
void Func_02005792();
void Func_02005794();
void Func_02005794_a();
void Func_020057a2();
void Func_020057a4();
void Func_020057a8();
void Func_020057b0();
void Func_020057be();
void Func_020057c4();
void Func_020057d0();
void Func_020057d0_a();
s32 Func_020057d2();
void Func_020057e2();
void Func_020057e6();
void Func_020057f0();
void Func_020057f2();
void Func_020057f6();
void Func_020057fe();
void Func_0200580e();
void Func_02005818();
void Func_02005822();
void Func_02005828();
s32 Func_0200582c();
void Func_02005832();
void Func_02005840();
void Func_02005844();
void Func_02005844_a();
void Func_02005852();
void Func_02005858();
void Func_02005860();
void Func_02005860_a();
void Func_02005862();
void Func_02005866();
void Func_02005872();
void Func_02005880();
void Func_02005880_a();
void Func_02005882();
void Func_0200588e();
void Func_0200589c();
void Func_020058a4();
void Func_020058ac();
void Func_020058b4();
void Func_020058ba();
void Func_020058c8();
void Func_020058d4();
void Func_020058ec();
void Func_020058f6();
void Func_020058f6_a();
void Func_020058f8();
void Func_02005900();
void Func_02005902();
void Func_02005904();
void Func_02005912();
void Func_0200591c();
void Func_0200591c_a();
void Func_02005922();
void Func_02005924();
void Func_02005926();
void Func_0200592a();
void Func_0200592a_a();
void Func_0200593e();
void Func_02005948();
void Func_02005950();
void Func_0200596a();
void Func_02005976();
void Func_0200597c();
s32 Func_0200598a();
void Func_02005998();
void Func_020059a2();
void Func_020059a4();
void Func_020059aa();
void Func_020059b4();
void Func_020059ba();
void Func_020059c2();
s32 Func_020059c4();
void Func_020059d0();
void Func_020059e4();
void Func_020059fa();
void Func_02005a04();
void Func_02005a10();
void Func_02005a10_a();
void Func_02005a1c();
void Func_02005a24();
void Func_02005a2c();
void Func_02005a32();
void Func_02005a64();
void Func_02005a82();
void Func_02005a94();
void Func_02005a9c();
void Func_02005aa0();
void Func_02005aa4();
void Func_02005aaa();
void Func_02005aac();
void Func_02005aac_a();
void Func_02005abc();
void Func_02005aca();
void Func_02005aee();
void Func_02005b06();
void Func_02005b12();
void Func_02005b1e();
void Func_02005b2a();
void Func_02005b36();
void Func_02005b36_a();
void Func_02005b42();
void Func_02005b44();
void Func_02005b4e();
void Func_02005b50();
void Func_02005b58();
void Func_02005b58_a();
void Func_02005b84();
void Func_02005b90();
s32 Func_02005b94();
void Func_02005b9c();
void Func_02005ba4();
void Func_02005ba4_a();
void Func_02005ba8();
void Func_02005bb0();
void Func_02005bb4();
void Func_02005bbc();
void Func_02005bbe();
void Func_02005bcc();
void Func_02005bd0();
void Func_02005bd8();
void Func_02005be4();
void Func_02005bf0();
void Func_02005bf6();
void Func_02005bfc();
void Func_02005c16();
void Func_02005c20();
void Func_02005c32();
void Func_02005c40();
void Func_02005c44();
void Func_02005c46();
void Func_02005c50();
void Func_02005c5e();
void Func_02005c5e_a();
void Func_02005c72();
void Func_02005c74();
s32 Func_02005c7a();
void Func_02005c88();
s32 Func_02005c92();
void Func_02005c9c();
s32 Func_02005c9c_a();
void Func_02005c9e();
void Func_02005cb6();
void Func_02005cbe();
void Func_02005cc4();
void Func_02005cca();
void Func_02005ccc();
void Func_02005d00();
void Func_02005d16();
void Func_02005d1a();
void Func_02005d3a();
void Func_02005d58();
void Func_02005d5e();
void Func_02005d60();
void Func_02005d68();
void Func_02005d6c();
void Func_02005d74();
s32 Func_02005d76();
void Func_02005d76_a();
void Func_02005d82();
void Func_02005d84();
void Func_02005d88();
void Func_02005da0();
void Func_02005dac();
void Func_02005dae();
void Func_02005db6();
void Func_02005db8();
void Func_02005dc6();
void Func_02005dce();
void Func_02005dd2();
void Func_02005dda();
void Func_02005ddc();
void Func_02005de8();
void Func_02005df4();
s32 Func_02005df6();
void Func_02005e16();
void Func_02005e1a();
s32 Func_02005e28();
void Func_02005e28_a();
void Func_02005e30();
void Func_02005e40();
void Func_02005e5a();
void Func_02005e68();
void Func_02005e70();
void Func_02005e78();
void Func_02005e82();
void Func_02005e88();
void Func_02005e9a();
s32 Func_02005ea8();
void Func_02005eaa();
void Func_02005eb4();
void Func_02005ebe();
void Func_02005ec8();
void Func_02005eda();
void Func_02005ee2();
void Func_02005ef6();
void Func_02005f00();
void Func_02005f08();
void Func_02005f1e();
void Func_02005f22();
void Func_02005f22_a();
s32 Func_02005f28();
void Func_02005f2c();
void Func_02005f38();
void Func_02005f40();
void Func_02005f5c();
void Func_02005f74();
void Func_02005f7e();
void Func_02005f80();
void Func_02005f84();
void Func_02005f98();
void Func_02005f98_a();
void Func_02005fa4();
s32 Func_02005fa8();
void Func_02005fb8();
void Func_02005fbe();
void Func_02005fdc();
void Func_02005ff0();
void Func_02005ff0_a();
void Func_02006016();
void Func_02006020();
void Func_02006024();
void Func_02006040();
void Func_02006042();
void Func_02006052();
void Func_0200605a();
void Func_0200606a();
void Func_02006072();
void Func_0200607c();
void Func_0200607e();
void Func_02006086();
s32 Func_02006092();
void Func_02006094();
void Func_02006096();
void Func_020060ae();
void Func_020060ae_a();
void Func_020060be();
void Func_020060c2();
void Func_020060ca();
void Func_020060cc();
void Func_020060ce();
void Func_020060de();
s32 Func_020060e4();
void Func_020060f2();
void Func_02006100();
void Func_02006106();
void Func_02006106_a();
void Func_02006108();
void Func_02006108_a();
s32 Func_0200610e();
void Func_02006110();
void Func_0200611c();
void Func_0200611e();
void Func_02006136();
void Func_0200613c();
void Func_02006154();
s32 Func_02006156();
void Func_02006158();
void Func_0200615c();
void Func_0200616a();
void Func_02006178();
void Func_0200618e();
void Func_0200619a();
void Func_020061a6();
void Func_020061a8();
void Func_020061c2();
void Func_020061c6();
void Func_020061cc();
void Func_020061da();
void Func_0200620e();
void Func_0200621a();
void Func_02006222();
void Func_0200622e();
void Func_02006232();
void Func_02006238();
void Func_02006242();
void Func_02006246();
void Func_02006252();
void Func_02006256();
s32 Func_02006258();
void Func_02006262();
void Func_0200626e();
void Func_02006276();
void Func_02006286();
void Func_0200628c();
void Func_0200628e();
void Func_02006298();
void Func_0200629a();
void Func_020062ae();
void Func_020062b0();
void Func_020062ba();
void Func_020062c4();
void Func_020062ce();
void Func_020062ce_a();
void Func_020062ea();
void Func_020062fc();
void Func_020062fe();
void Func_02006314();
void Func_02006320();
void Func_02006324();
void Func_02006328();
void Func_02006330();
void Func_02006338();
void Func_02006340();
void Func_02006348();
void Func_0200634c();
void Func_0200634e();
void Func_02006350();
void Func_02006368();
void Func_02006376();
void Func_0200638c();
void Func_020063ae();
void Func_020063b2();
void Func_020063b6();
void Func_020063bc();
void Func_020063c6();
void Func_020063c6_a();
void Func_020063ce();
void Func_020063ce_a();
void Func_020063d6();
void Func_020063dc();
void Func_020063de();
void Func_020063e2();
void Func_020063e4();
void Func_020063ee();
void Func_020063f6();
void Func_02006406();
void Func_02006408();
void Func_02006418();
void Func_0200641a();
void Func_02006420();
void Func_02006426();
void Func_02006428();
void Func_02006430();
void Func_02006432();
void Func_0200643e();
void Func_0200643e_a();
void Func_0200644a();
void Func_02006450();
void Func_02006456();
void Func_0200645c();
void Func_02006468();
void Func_02006468_a();
void Func_0200646c();
void Func_0200646e();
void Func_02006472();
void Func_02006482();
void Func_02006484();
void Func_02006492();
void Func_02006494();
void Func_02006496();
void Func_0200649a();
void Func_020064a4();











static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}









#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define OverlayObject_PrepareSpawnedObject      Func_02000048
#define AcquireOverlayObject      Func_020036ca
#define RunOverlayObjectCommand0  Func_02003710
#define RunOverlayObjectCommand1  Func_02003728
#define RunOverlayObjectCommand14 Func_020037e8
#define CreateOverlayObject Func_02003722
#define SetOverlayObjectMode Func_0200376c
#define SetOverlayObjectSlot Func_02003844
#define SetOverlayObjectRecordField1 Func_02000030
#define OverlayObject_CreateConfiguredObject Func_020000a0
#define OverlayObject_ApplyVelocity Func_02000104

#include "overlay_object.h"
#include "create_configured_overlay_object.h"

struct OverlayObjectRecordFieldView {
    u8 unknown_00[9];
    u8 unknown_00_0 : 2;
    u8 field_01 : 2;
    u8 unknown_04_0 : 4;
};

union Slot {
    s32 w;
    u16 h[2];
    void *p;
};

void *Func_020036ca(s32, s32, s32, s32);
void Func_02003710(void *, s32);
void Func_02003728(void *, s32);
void Func_020037e8(void *, s32);









#include "types.h"

#define EffectDescriptorTable Data_0200b92c
#define GetPartyEffect Func_0200384e
#define SpawnEffect Func_020037f4
#define SetEffectVariant Func_020037fe
#define SetEffectDescriptor Func_02003818
#define SetEffectMode Func_0200397a
#define ScaleEffectDeltaFromAccumulated Func_020038a4
#define ScaleEffectDeltaFromOrigin Func_020038bc
#define ScaleEffectVerticalDelta Func_020038ca
#define SetEffectCallbackMode Func_02003918
#define SetEffectCallbackArgument Func_02003928
#define SceneEffect_SpawnConfigured Func_0200013c

#include "configured_effect_spawn.h"



#include "types.h"

#define SceneData_GetTableB938 Func_02000314
extern u8 Data_0200b938[];



#include "types.h"



#include "types.h"

#define SceneData_GetTableb9c8 Func_02000320
#define SceneData_GetTableB9d4AfterStateCheck Func_02000328
#define SceneData_GetTablebbe4 Func_02000354
#define SceneActor_SetMode55OnSevenRecords Func_02000ad0
#define FieldScene_ConfigureValue93Scene Func_02003438
/*
 * resource_39d owner at 0x02000ad0, 82 bytes.
 *
 * Sets one byte on seven records in a row. Each record is fetched by id through
 * its own per-site veneer -- seven calls, seven veneers, as every other owner in
 * this overlay does -- and the byte at +85 is written immediately after.
 *
 * The ids are 0 and then 14 through 19, and the first record takes 3 where the
 * rest take 4. Nothing in the span says what the byte means, so it keeps an
 * offset-derived name.
 */
struct Record {
    u8 pad00[85];
    u8 mode55;              /* +85, 0x55 */
};

extern u8 Data_0200b9c8[];
extern u8 Data_0200b9d4[];
extern u8 Data_0200bbe4[];

void Func_020039f6(s32);
struct Record *Func_020041c6();
struct Record *Func_020041d2();
struct Record *Func_020041de();
struct Record *Func_020041e8();
struct Record *Func_020041f2();
struct Record *Func_020041fc();
struct Record *Func_02004206();
void Func_02006c50(s32 value, s32 mode);
void Func_02006c60(s32 left, s32 right);
void Func_02006c7c(void);
void Func_02006c5a(s32 enabled);
void Func_02006c76(void);
void Func_02006c82(void);

/* Contiguous unnamed leaf-owner run for resource_39d. */

/* Configure and publish the scene's presentation layer. */











#include "types.h"

#define BattleRuntime_Reset_1(args...) Func_02003c48(args)
#define Scene_GetRecord_1(args...) Func_02003c6a(args)
#define Scene_GetRecord_2(args...) Func_02003c76(args)
#define Scene_GetRecord_3(args...) Func_02003c82(args)
#define Scene_GetRecord_4(args...) Func_02003c8e(args)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_02003d66, a0, a1, a2, a3)
#define Audio_PlayCue_1(args...) Func_02003e0c(args)
#define Audio_PlayCue_2(a0) Call1(Func_02003e22, a0)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02003dfa(args)
#define ObjectMotion_SetSpeedParameters_1(args...) Func_02003e06(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02003ce0(args)
#define RuntimeBlock_GetOffset1e0Pointer_1(args...) Func_02003dd4(args)
#define RuntimeBlock_GetOffset1e0Pointer_2(args...) Func_02003dd4(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_02003dca, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_2(a0, a1, a2, a3) Call4(Func_02003de2, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_02003dee(args)
#define BattleEffect_ApplyColorToSourceBuffer_1(a0, a1) Call2(Func_02003e28, a0, a1)
#define BattleRuntime_WaitIfModeZero_3(a0, a1) Call2(Func_02003e28_a, a0, a1)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_02003e3e(args)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_02003d24(args)
#define BattleRuntime_WaitIfModeZero_6(a0, a1) Call2(Func_02003e3c, a0, a1)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_02003e52(args)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_02003d38(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_02003d9e, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_02003dac, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(a0, a1, a2) Call3(Func_02003dba, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4(a0, a1, a2) Call3(Func_02003dc8, a0, a1, a2)
#define Object_SetModeById_1(args...) Func_02003de0(args)
#define Object_SetModeById_2(args...) Func_02003de8(args)
#define Object_SetModeById_3(args...) Func_02003df0(args)
#define BattleRuntime_WaitIfModeZero_9(args...) Func_02003d96(args)
#define BattleRuntime_WaitIfModeZero_10(a0, a1) Call2(Func_02003eb0, a0, a1)
#define BattleRuntime_WaitIfModeZero_11(args...) Func_02003ec6(args)
#define BattleRuntime_WaitIfModeZero_12(args...) Func_02003dac_a(args)
#define BattleRuntime_WaitIfModeZero_13(args...) Func_02003db2(args)
#define Scene_GetRecord_5(args...) Func_02003dd8(args)
#define Object_SetModeById_4(args...) Func_02003e26(args)
#define BattleRuntime_WaitIfModeZero_14(args...) Func_02003dcc(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02003e3c_a(args)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02003e98, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_1(args...) Func_02003e58(args)
#define BattleRuntime_WaitIfModeZero_15(args...) Func_02003dee_a(args)
#define Scene_GetRecord_6(args...) Func_02003e14(args)
#define Object_SetModeById_5(args...) Func_02003e62(args)
#define BattleRuntime_WaitIfModeZero_16(args...) Func_02003e08(args)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02003ecc, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_17(args...) Func_02003e1a(args)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02003ede, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_18(args...) Func_02003e2c(args)
#define ObjectMotion_ArmCallback_4(args...) Func_02003eee(args)
#define Scene_GetRecord_7(args...) Func_02003e5c(args)
#define Object_SetModeById_6(args...) Func_02003eaa(args)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Call3(Func_02003f0e, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_19(args...) Func_02003e5c_a(args)
#define Scene_GetRecord_8(args...) Func_02003e82(args)
#define Object_SetModeById_7(args...) Func_02003ed0(args)
#define BattleRuntime_WaitIfModeZero_20(args...) Func_02003e76(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_02003ee6(args)
#define BattleRuntime_WaitIfModeZero_21(args...) Func_02003e84(args)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02003eb6, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02003ec0, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_02003eca, a0, a1, a2)
#define Object_SetModeById_8(args...) Func_02003f0a(args)
#define Object_SetModeById_9(args...) Func_02003f12(args)
#define Object_SetModeById_10(args...) Func_02003f1a(args)
#define Scene_GetRecord_9(args...) Func_02003ee0(args)
#define ObjectMotion_ResetAndSetPosition_1(args...) Func_02003f02(args)
#define Scene_GetRecord_10(args...) Func_02003ef8(args)
#define ObjectMotion_ResetAndSetPosition_2(args...) Func_02003f1a_a(args)
#define Scene_GetRecord_11(args...) Func_02003f10(args)
#define ObjectMotion_ResetAndSetPosition_3(args...) Func_02003f32(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_02003f58(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_5(args...) Func_02003f6a(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_2(args...) Func_02003f68(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_6(args...) Func_02003f7a(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_3(args...) Func_02003f78(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_7(args...) Func_02003f8a(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02003f46(args)
#define Object_SetModeById_12(a0, a1) Call2(Func_02003dd8_a, a0, a1)
#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))
#define ACTOR_FIELD_0XA(record) (*(s16 *)((record) + 10))
#define ACTOR_FIELD_0X12(record) (*(s16 *)((record) + 18))
#define FieldScene_RunFourActorPresentation Func_0200056c
#define FieldScene_RunScene39d_020009fc Func_020009fc
#define FieldScene_RunScene39d_02002ddc Func_02002ddc
#define FieldScene_RunScene39d_02002eb8 Func_02002eb8

extern u8 Data_0200bc50[];

void Func_02003bec();
void Func_02003bf6();
void Func_02003c10();
void Func_02003c1c();
void Func_02003c28();
void Func_02003c34();
void Func_02003c38();
void Func_02003c3c();
void Func_02003c48();
u8 *Func_02003c6a();
u8 *Func_02003c76();
void Func_02003c7c();
u8 *Func_02003c82();
u8 *Func_02003c8e();
void Func_02003c90();
void Func_02003cb2();
void Func_02003ce0();
void Func_02003d24();
void Func_02003d38();
void Func_02003d66();
void Func_02003d7e();
void Func_02003d96();
void Func_02003d9e();
void Func_02003dac();
void Func_02003dac_a();
void Func_02003db2();
void Func_02003dba();
void Func_02003dba_a();
void Func_02003dc8();
void Func_02003dca();
void Func_02003dcc();
s32 Func_02003dd4();
void Func_02003de0();
void Func_02003de2();
void Func_02003de8();
void Func_02003dee();
void Func_02003dee_a();
void Func_02003df0();
void Func_02003dfa();
void Func_02003e02();
void Func_02003e06();
void Func_02003e08();
void Func_02003e0c();
u8 *Func_02003e14();
void Func_02003e1a();
void Func_02003e22();
void Func_02003e26();
void Func_02003e28();
void Func_02003e28_a();
void Func_02003e28_b();
void Func_02003e2c();
void Func_02003e3c();
void Func_02003e3c_a();
void Func_02003e3e();
void Func_02003e52();
void Func_02003e58();
u8 *Func_02003e5c();
void Func_02003e62();
void Func_02003e76();
u8 *Func_02003e82();
void Func_02003e84();
void Func_02003e98();
void Func_02003eaa();
void Func_02003eb0();
void Func_02003eb6();
void Func_02003ec0();
void Func_02003ec6();
void Func_02003eca();
void Func_02003ecc();
void Func_02003ed0();
void Func_02003ede();
u8 *Func_02003ee0();
void Func_02003ee6();
void Func_02003eee();
u8 *Func_02003ef8();
void Func_02003f02();
void Func_02003f0a();
void Func_02003f0e();
u8 *Func_02003f10();
void Func_02003f12();
void Func_02003f1a();
void Func_02003f1a_a();
void Func_02003f32();
void Func_02003f46();
void Func_02003f58();
void Func_02003f68();
void Func_02003f6a();
void Func_02003f78();
void Func_02003f7a();
void Func_02003f8a();
u8 *Func_02003dd8();
void Func_02003dd8_a();
void Func_02003e5c_a();
s32 Func_020012d8();
void Func_02004082();
void Func_0200409c();
void Func_020040b8();
void Func_020040d0();
void Func_020040d4();
void Func_020040e0();
s32 Func_020040f2();
s32 Func_02004112();
void Func_0200419a();
void Func_0200619e();
void Func_0200642c();
void Func_0200646e();
void Func_020064c4();
void Func_020064e6();
s32 Func_020064f2();
s32 Func_0200650c();
s32 Func_02006516();
void Func_02006524();
void Func_0200653e();
void Func_0200654e();
void Func_02006560();
void Func_0200657a();
void Func_020065e0();
s32 Func_020065ec();
void Func_020065fa();
void Func_020065fe();
void Func_02006606();
void Func_02006606_a();
void Func_02006612();
void Func_0200661a();
void Func_020060fc();
void Func_02006594();
void Func_020065c2();
void Func_020065ec_a();
void Func_020065f0();
void Func_02006668();
void Func_02006678();
void Func_02006680();
void Func_020066b6();
void Func_020066d4();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Newly named call sites: the mechanical pass left these spelled raw because
 * their calling form (a cast, or an odd argument count) did not match its
 * patterns, or because the engine function itself had no name yet. */

/* Status/phase word at 0x1c0 of the shared scene work record. */

/* Two s16 fields (offsets 0xa and 0x12) read together from an actor slot
 * record and forwarded as a pair of arguments. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */








/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}





/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02002eb8(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Configures four actor slots (0-3), advances the shared scene phase, then
 * drives handles 1-3 through a sequence of timed calls, some of which use
 * the two s16 fields read back from an actor slot record. */








#include "types.h"

#define MeasureFixedPointPositionDistance Func_02000890
typedef s32(*IwramIntegerSquareRoot)(s32);



#include "types.h"

#define GetActor Func_02003fd0
#define MeasurePositionDistance Func_02001196
#define FindNearestF2Actor Func_020008cc

typedef struct {
    u8 pad_to_kind[0x28];
    s16 *kind;
} ActorData;

typedef struct {
    u8 pad_to_data[0x50];
    ActorData *data;
} Actor;

Actor *Func_02003fd0(s32 actor_id);
s32 Func_02001196(u8 *a, u8 *b);



#include "types.h"

s32 Func_02006aca();
s32 Func_02006ad8();



void SetOverlayObjectRecordField1(struct OverlayObject *object, s32 value)
{
    struct OverlayObjectRecordFieldView *record;
    record = (struct OverlayObjectRecordFieldView *)object->record;
    record->field_01 = value;
}

void *OverlayObject_PrepareSpawnedObject(s32 first, s32 second, s32 third, s32 fourth)
{
    void *obj;
    void *rec;
    s32 mask;

    obj = AcquireOverlayObject(fourth, first, second, third);
    if (obj != NULL) {
        rec = FIELD_AT_OFFSET(obj, void *, 0x50);
        mask = -0xD;
        FIELD_AT_OFFSET(rec, u8, 9) = (u8)(mask & FIELD_AT_OFFSET(rec, u8, 9));
        FIELD_AT_OFFSET(obj, u8, 0x55) = 0;
        FIELD_AT_OFFSET(obj, u8, 0x59) = 8;
        RunOverlayObjectCommand0(obj, 0);
        RunOverlayObjectCommand14(obj, 0xE);
        RunOverlayObjectCommand1(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "create_configured_overlay_object_body.inc"
}

void OverlayObject_ApplyVelocity(union Slot *object)
{
    u16 *record;

    object[2].w += object[17].w;
    object[3].w += object[18].w;
    object[4].w += object[19].w;
    object[6].w += object[12].w;
    object[7].w += object[13].w;
    record = (u16 *)object[20].p;
    record[15] += object[25].h[0];
}

void SceneEffect_SpawnConfigured(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}

void *SceneData_GetTableB938(void)
{
    return Data_0200b938;
}

s32 Func_0200031c(void)
{
    return 0;
}

void *SceneData_GetTableb9c8(void)
{
    return Data_0200b9c8;
}

void *SceneData_GetTableB9d4AfterStateCheck(void)
{
    extern s16 Data_02000240[];

    if (Data_02000240[225] != 1) {
        Func_020039f6(0x253);
    }
    return Data_0200b9d4;
}

void *SceneData_GetTablebbe4(void)
{
    return Data_0200bbe4;
}

void FieldScene_RunFourActorPresentation(void)
{
    u32 i;
    u8 *record;

    BattleRuntime_Reset_1();
    Func_02003c3c(); /* main:08077268 */
    record = Scene_GetRecord_1(0); /* main:0808a080 */
    Func_02003c10(record, 0); /* main:080091e0 */
    record = Scene_GetRecord_2(1); /* main:0808a080 */
    Func_02003c1c(record, 0); /* main:080091e0 */
    record = Scene_GetRecord_3(2); /* main:0808a080 */
    Func_02003c28(record, 0); /* main:080091e0 */
    record = Scene_GetRecord_4(3); /* main:0808a080 */
    Func_02003c34(record, 0); /* main:080091e0 */
    ObjectMotion_PlaceWithinCameraBounds_1(0x1300000, -1, 0x780000, 0);
    Func_02003bec(1); /* main:080000c0 */
    Func_02003c38(); /* main:08009128 */
    Func_02003bf6(1); /* main:080000c0 */
    Audio_PlayCue_1(141);
    Call3(Func_02003c7c, 0x50000, 0x50000, 0x10000); /* main:080091f0 */
    Audio_PlayCue_2(0x121);
    Call3(Func_02003c90, -1, -1, 0xe666); /* main:080091f0 */
    SCENE_PHASE = 0x100;
    BattleRuntime_WaitIfModeZero_1(); /* main:0808a360 */
    ObjectMotion_SetSpeedParameters_1(); /* main:0808a370 */
    Func_02003cb2(); /* main:080091f8 */
    BattleRuntime_WaitIfModeZero_2(30);
    /* Clear the byte at offset 85 of the record RuntimeBlock_GetOffset1e0Pointer_1() returns. */
    *(u8 *)(RuntimeBlock_GetOffset1e0Pointer_2() + 85) = 0;
    ObjectMotion_SetSpeedLimitAndAcceleration_1(0xcccc, 0x1999); /* speed_limit, acceleration */
    ObjectMotion_PlaceWithinCameraBounds_2(0x2000000, -0x180000, 0xa00000, 1);
    Object_CommitPositionThenWaitIfModeZero_1();
    BattleEffect_ApplyColorToSourceBuffer_1(0x10000, 0);
    BattleRuntime_WaitIfModeZero_3(0x10005, 0); /* main:0808a330 */
    BattleRuntime_WaitIfModeZero_4(50); /* main:0808a348 */
    BattleRuntime_WaitIfModeZero_5(50);
    BattleRuntime_WaitIfModeZero_6(0x7fff, 0); /* main:0808a330 */
    BattleRuntime_WaitIfModeZero_7(30); /* main:0808a348 */
    BattleRuntime_WaitIfModeZero_8(30);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(0, 0x1f80000, 0xa80000);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(1, 0x2100000, 0x900000);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(2, 0x1e80000, 0x900000);
    ObjectMotion_SetHorizontalPositionWithTerrain_4(3, 0x2000000, 0x980000);
    Object_SetModeById_12(0, 19); /* object 0, action 19 */
    Object_SetModeById_1(1, 19);
    Object_SetModeById_2(2, 19);
    Object_SetModeById_3(3, 19);
    BattleRuntime_WaitIfModeZero_9(10);
    BattleRuntime_WaitIfModeZero_10(0x10000, 0); /* main:0808a330 */
    BattleRuntime_WaitIfModeZero_11(30); /* main:0808a348 */
    BattleRuntime_WaitIfModeZero_12(30);
    BattleRuntime_WaitIfModeZero_13(80);
    record = Scene_GetRecord_5(0);
    Func_02003d7e(record, 1); /* main:080091e0 */
    Object_SetModeById_4(0, 1);
    BattleRuntime_WaitIfModeZero_14(30);
    ObjectMotion_CallThenWaitForAnimationChange_1(0, 4);
    ObjectMotion_ArmCallback_1(0, 0xc000, 20);
    ObjectMotion_SetVariantCallback_1(0, 2);
    BattleRuntime_WaitIfModeZero_15(60);
    record = Scene_GetRecord_6(1); /* main:0808a080 */
    Func_02003dba_a(record, 1); /* main:080091e0 */
    Object_SetModeById_5(1, 1);
    BattleRuntime_WaitIfModeZero_16(20);
    ObjectMotion_ArmCallback_2(1, 0x2000, 0);
    BattleRuntime_WaitIfModeZero_17(20);
    ObjectMotion_ArmCallback_3(1, 0x6000, 0);
    BattleRuntime_WaitIfModeZero_18(20);
    ObjectMotion_ArmCallback_4(1, 0, 0);
    record = Scene_GetRecord_7(2);
    Func_02003e02(record, 1); /* main:080091e0 */
    Object_SetModeById_6(2, 1);
    ObjectMotion_ArmCallback_5(1, 0x6000, 0);
    BattleRuntime_WaitIfModeZero_19(40);
    record = Scene_GetRecord_8(3); /* main:0808a080 */
    Func_02003e28_b(record, 1); /* main:080091e0 */
    Object_SetModeById_7(3, 1);
    BattleRuntime_WaitIfModeZero_20(20);
    ObjectMotion_CallThenWaitForAnimationChange_2(3, 3);
    BattleRuntime_WaitIfModeZero_21(20);
    ObjectMotion_SetSpeedParameters_2(1, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_3(2, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_4(3, 0xcccc, 0x6666);
    Object_SetModeById_8(1, 2);
    Object_SetModeById_9(2, 2);
    Object_SetModeById_10(3, 2);
    record = Scene_GetRecord_9(0); /* main:0808a080 */
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition_1(1, ACTOR_FIELD_0XA(record), ACTOR_FIELD_0X12(record));
    }
    record = Scene_GetRecord_10(0); /* main:0808a080 */
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition_2(2, ACTOR_FIELD_0XA(record), ACTOR_FIELD_0X12(record));
    }
    record = Scene_GetRecord_11(0); /* main:0808a080 */
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition_3(3, ACTOR_FIELD_0XA(record), ACTOR_FIELD_0X12(record));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_1(3);
    ObjectMotion_SetHorizontalPositionWithTerrain_5(3, 0, 0);
    ObjectMotion_CommitCurrentPositionAndActivate_2(2);
    ObjectMotion_SetHorizontalPositionWithTerrain_6(2, 0, 0);
    ObjectMotion_CommitCurrentPositionAndActivate_3(1);
    ObjectMotion_SetHorizontalPositionWithTerrain_7(1, 0, 0);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

s32 MeasureFixedPointPositionDistance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((IwramIntegerSquareRoot)0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}

s32 FindNearestF2Actor(void)
{
    u8 *work = *(u8 **)0x03001ebc;
    Actor **actor_slot;
    Actor *origin;
    s32 nearest_actor = 0;
    s32 min_dist;
    u32 actor_id;

    min_dist = 640;
    origin = GetActor(0);
    actor_id = 8;
    actor_slot = (Actor **)(work + 0x34);
    do {
        Actor *actor = *actor_slot++;
        if (actor != 0) {
            if (*actor->data->kind == 0xf2) {
                s32 dist = MeasurePositionDistance(
                    (u8 *)origin + 8, (u8 *)actor + 8);
                if (dist < min_dist) {
                    min_dist = dist;
                    nearest_actor = actor_id;
                }
            }
        }
        actor_id++;
    } while (actor_id <= 65);
    return nearest_actor;
}

void FieldScene_RunScene39d_020009fc(void)
{
    u32 i;
    u8 *rec;
    u8 *rec8;
    s32 record;
    s32 base5_200bc50;

    rec = Value1(Func_020040f2, 0);
    Func_020040e0();
    record = Func_020012d8();
    base5_200bc50 = (s32)Data_0200bc50;
    *(s32 *)base5_200bc50 = record;
    if (record != 0) {
        Call1(Func_020040d4, 0x250);
        rec8 = Value1(Func_02004112, *(s32 *)base5_200bc50);
        rec8[85] = 0;
        rec[85] &= 254;
        *(s32 *)((s32)rec8 + 12) += -0x30000;
        *(s32 *)((s32)rec + 12) += -0x30000;
        *(s32 *)((s32)rec + 20) += -0x30000;
        Func_02004082(2);
        *(s32 *)((s32)rec8 + 12) += -0x20000;
        *(s32 *)((s32)rec + 12) += -0x20000;
        *(s32 *)((s32)rec + 20) += -0x20000;
        Func_0200409c(10);
        *(s32 *)((s32)rec8 + 12) += 0x20000;
        *(s32 *)((s32)rec + 12) += 0x20000;
        *(s32 *)((s32)rec + 20) += 0x20000;
        Func_020040b8(4);
        *(s32 *)((s32)rec8 + 12) += 0x20000;
        *(s32 *)((s32)rec + 12) += 0x20000;
        *(s32 *)((s32)rec + 20) += 0x20000;
        Func_020040d0(4);
        *(s32 *)((s32)rec8 + 12) += 0x10000;
        *(s32 *)((s32)rec + 12) += 0x10000;
        *(s32 *)((s32)rec + 20) += 0x10000;
    }
    Func_0200419a();
}

void SceneActor_SetMode55OnSevenRecords(void)
{
    Func_020041c6(0)->mode55 = 3;
    Func_020041d2(14)->mode55 = 4;
    Func_020041de(15)->mode55 = 4;
    Func_020041e8(16)->mode55 = 4;
    Func_020041f2(17)->mode55 = 4;
    Func_020041fc(18)->mode55 = 4;
    Func_02004206(19)->mode55 = 4;
}

void RunScene58Sequence(void)
{
    extern u8 Data_02000240[];
    void *temp_r0;
    void *temp_r0_10;
    void *temp_r0_11;
    void *temp_r0_12;
    void *temp_r0_13;
    void *temp_r0_2;
    void *temp_r0_3;
    void *temp_r0_4;
    void *temp_r0_5;
    void *temp_r0_6;
    void *temp_r0_7;
    void *temp_r0_8;
    void *temp_r0_9;
    void *temp_r2;
    void *temp_r2_2;
    void *temp_r2_3;
    void *temp_r2_4;
    s32 flag;
    s32 flag2;
    s32 bits;

    Call0(Func_02004200);
    temp_r0 = Value1(Func_0200421e, 0x11);
    FIELD(temp_r0, u8 *, 0x55) = (u8)(0xFA & FIELD(temp_r0, u8 *, 0x55));
    Call3(Func_0200423a, 0, 0x0000cccc, 0x00006666);
    Call3(Func_02004244, 1, 0x0000cccc, 0x00006666);
    Call3(Func_0200424e, 2, 0x0000cccc, 0x00006666);
    Call3(Func_02004258, 3, 0x0000cccc, 0x00006666);
    temp_r0_2 = Value1(Func_02004256, 0);
    if (temp_r0_2 != 0) {
        Call3(Func_0200429c, 1, FIELD(temp_r0_2, s32 *, 8), FIELD(temp_r0_2, s32 *, 0x10));
    }
    temp_r0_3 = Value1(Func_0200426a, 0);
    if (temp_r0_3 != 0) {
        Call3(Func_020042b0, 2, FIELD(temp_r0_3, s32 *, 8), FIELD(temp_r0_3, s32 *, 0x10));
    }
    temp_r0_4 = Value1(Func_0200427e, 0);
    if (temp_r0_4 != 0) {
        Call3(Func_020042c4, 3, FIELD(temp_r0_4, s32 *, 8), FIELD(temp_r0_4, s32 *, 0x10));
    }
    Call1(Func_020041d2_sequence, 1);
    Call3(Func_020042be, 0, 0x158, 0xE8);
    Call3(Func_020042ca, 1, 0x148, 0xE8);
    Call3(Func_020042d6, 2, 0x158, 0xF8);
    Call3(Func_020042e2, 3, 0x148, 0xF8);
    Call1(Func_020042f8, 0);
    Call3(Func_0200436c, 0, 0xA000, 0);
    Call1(Func_0200430a, 1);
    Call3(Func_0200437e, 1, 0xC000, 0);
    Call1(Func_0200431c, 2);
    Call3(Func_02004390, 2, 0xA000, 0);
    Call1(Func_0200432e, 3);
    Call3(Func_020043a2, 3, 0xC000, 0);
    Call1(Func_020042f0, 0x32);
    Call2(Func_02004370, 1, 2);
    Call1(Func_020042fe, 0x14);
    Call2(Func_020043e2, 0x18000, 0x3000);
    Call4(Func_020043fc, 0x01480000, 0x280000, 0xB00000, 1);
    FIELD(Value0(Func_02004410), s8 *, 0x55) = 0;
    Call3(Func_0200435c, 1, 0x18000, 0xC000);
    Call3(Func_02004388, 1, 0x148, 0xD8);
    Call0(Func_0200442c);
    Call3(Func_02004376, 1, 0x0000cccc, 0x00006666);
    Call1(Func_02004354, 0x3C);
    Call4(Func_02004446, 0x01580000, 0x180000, 0xE80000, 1);
    Call3(Func_0200442a, 1, 0x2000, 0x14);
    Call0(Func_0200445e);
    Call2(Func_020043e6, 1, 4);
    Call1(Func_0200441c, SCENE_STEP_VALUE_run_scene_58_sequence);
    Call3(Func_0200443e, 1, 0, 0x14);
    Call2(Func_02004468, 3, 0x102);
    Call1(Func_0200439e, 0x3C);
    Call3(Func_020043d4, 3, 0x18000, 0xC000);
    Call3(Func_02004400, 3, 0x148, 0xE8);
    Call1(Func_020043be, 0xA);
    Call3(Func_02004478, 3, 0, 0x14);
    Call2(Func_02004438, 3, 4);
    Call1(Func_020043d6, 0x14);
    Call3(Func_02004490, 3, 0, 0x1E);
    Call3(Func_0200446a, 2, 0, 0);
    Call1(Func_020043f0, 0x3C);
    Call3(Func_02004444, 2, 0x150, 0xF8);
    Call3(Func_020044c0, 2, 0xA000, 0);
    Call3(Func_020044cc, 0, 0x8000, 0);
    Call1(Func_0200441a, 0x1E);
    Call3(Func_020044d4, 2, 0, 0x14);
    Call3(Func_020044e6, 3, 0, 0x1E);
    Value2(Func_020044d6, 3, 0);
    if (Value2(Func_02004456, 0, 0) == 0) {
        Call1(Func_02004448, 0x14);
        Call3(Func_0200451c, 3, 0x100, 0x3C);
        Call3(Func_02004518, 1, 0x4000, 0xA);
        Call2(Func_020044d0, 1, 3);
        Call3(Func_02004522, 1, 0, 0x14);
        Call3(Func_02004536, 3, 0xC000, 0);
        Call2(Func_020044ee, 1, 4);
        Call1(Func_0200448c, 0x14);
        Call3(Func_02004546, 1, 0, 0x14);
        temp_r2 = Scene58Work;
        FIELD(temp_r2, u16 *, 0x1D8) = (u16)(FIELD(temp_r2, u16 *, 0x1D8) + 2);
    } else {
        temp_r2_2 = Scene58Work;
        FIELD(temp_r2_2, u16 *, 0x1D8) = (u16)(FIELD(temp_r2_2, u16 *, 0x1D8) + 2);
        Call1(Func_020044d0_a, 0x14);
        Call3(Func_020045a2, 3, 0x00000101, 0x3C);
        Call3(Func_0200459e, 1, 0x4000, 0xA);
        Call2(Func_02004556, 1, 4);
        Call3(Func_020045a8, 1, 0, 0x14);
        Call3(Func_020045bc, 3, 0xC000, 0);
        Call2(Func_02004584, 1, 2);
        Call1(Func_02004512, 0x14);
        Call3(Func_020045cc, 1, 0, 0x14);
    }
    Call3(Func_020045ee, 3, 0x00000101, 0x3C);
    Call3(Func_020045e0, 3, 0, 0x14);
    Call3(Func_020045ba, 0, 1, 0);
    Call1(Func_02004540, 0x3C);
    Call3(Func_02004604, 0, 0x8000, 0);
    Call3(Func_02004610, 1, 0x4000, 0x14);
    Call2(Func_020045c0, 0, 3);
    Call2(Func_020045d0, 1, 3);
    Call1(Func_0200456e, 0x14);
    Call3(Func_02004628, 1, 0, 0x14);
    Call2(Func_020045f8, 3, 2);
    Call3(Func_0200463a, 3, 0, 0x14);
    Call2(Func_020045f2, 0, 3);
    Call2(Func_020045fa, 1, 3);
    Call2(Func_0200460a, 2, 3);
    Call1(Func_020045a8_a, 0x14);
    Call2(Func_02004618, 3, 4);
    Call1(Func_020045b6, 0xA);
    Call3(Func_02004678, 3, 0, 0xA);
    Call2(Func_02004628_a, 3, 0x10);
    Call3(Func_02004682, 3, 0, 0x3C);
    Call1(Func_02004748, 0x11);
    Call3(Func_0200463e, 5, 0xD80000, 0xC80000);
    Call3(Func_020046a0, 5, 0, 0x14);
    Call3(Func_02004656, 5, 0x780000, 0xA00000);
    Call3(Func_020046c0, 5, 0, 0);
    Call2(Func_02004670, 3, 1);
    Call2(Func_02004688, 0, 1);
    Call2(Func_02004698, 1, 1);
    Call3(Func_020046e2, 0, 0, 0xA);
    Call3(Func_020046ee, 1, 0xE000, 0x14);
    Call3(Func_020046fa, 2, 0xA000, 0xA);
    Call3(Func_02004706, 3, 0x8000, 0x14);
    Call3(Func_02004712, 0, 0xA000, 5);
    Call3(Func_0200471e, 1, 0xC000, 0xA);
    Call3(Func_0200472a, 2, 0x8000, 5);
    Call3(Func_02004736, 0, 0x8000, 5);
    Call2(Func_020046f6, 0, 2);
    Call3(Func_0200474a, 1, 0x8000, 0xA);
    Call2(Func_02004712_a, 1, 2);
    Call1(Func_020046a0_a, 0x14);
    Call2(Func_02004784, 0x30000, 0x6000);
    Call4(Func_0200479c, 0x780000, 0xffe80000, 0xA80000, 1);
    Call0(Func_020047a8);
    Call1(Func_020046c6, 0x28);
    Call2(Func_02004746, 0x15, 1);
    Call1(Func_020046d4, 0x14);
    Call1(Func_0200484a, 0x3D);
    Call3(Func_02004794, 0x15, 0, 0x14);
    Call2(Func_02004764, 0x17, 1);
    Call1(Func_020046f2, 0x14);
    Call3(Func_020047ac, 0x17, 0, 0x14);
    Call3(Func_020047c0, 0x17, 0xC000, 0x3C);
    Call3(Func_020047ca, 0x17, 0, 0x14);
    Call3(Func_020047cc, 0x17, 0, 0x14);
    Call3(Func_02004782, 1, 0x01080000, 0x01200000);
    Call3(Func_020047e4, 1, 0, 0x14);
    Call3(Func_0200479a, 1, 0x01480000, 0xD80000);
    Call2(Func_020047b2, 0x17, 4);
    Call1(Func_02004750, 0x14);
    Call3(Func_0200480a, 0x17, 0, 0x14);
    Call2(Func_020047ca_a, 0x17, 3);
    Call1(Func_02004768, 0x14);
    Call3(Func_02004822, 0x17, 0, 0x14);
    Call3(Func_02004836, 0x17, 0xA000, 0xA);
    Call3(Func_02004842, 0x15, 0x2000, 0xA);
    Call2(Func_0200480a_a, 0x15, 1);
    Call1(Func_02004798, 0x14);
    Call3(Func_0200485c, 5, 0x8000, 0);
    Call3(Func_02004868, 0x14, 0x8000, 0x1E);
    Call3(Func_02004872, 0x15, 0, 0x1E);
    Call2(Func_0200483a, 5, 2);
    Call2(Func_02004832, 0x15, 4);
    Call1(Func_020047d0, 0x14);
    Call3(Func_02004894, 0x15, 0x2000, 0xA);
    Call2(Func_0200484c, 0x15, 3);
    Call1(Func_020047ea, 0x14);
    Call2(Func_0200485a, 0x17, 3);
    Call3(Func_02004824, 0x15, 0x0000cccc, 0x00006666);
    Value3(Func_0200484e, 0x15, 0x68, 0xA8);
    Call1(Func_0200480c, 0x14);
    Call3(Func_020048ce, 0x17, 0, 0x14);
    Call3(Func_020048d8, 0x14, 0, 0xA);
    Call3(Func_020048f4, 5, 0x102, 0);
    Call3(Func_02004900, 0x14, 0x102, 0x46);
    Call3(Func_0200489e, 0x16, 0x01080000, 0x01200000);
    Call3(Func_02004900_a, 0x16, 0, 0x14);
    Call3(Func_020048b6, 0x16, 0x01280000, 0x780000);
    Call3(Func_02004922, 0, 0xA000, 0);
    Call3(Func_0200492e, 1, 0xC000, 0);
    Call3(Func_0200493a, 2, 0xC000, 0);
    Call3(Func_02004946, 3, 0xC000, 0);
    Call3(Func_02004950, 5, 0, 0);
    Call3(Func_0200495a, 0x14, 0, 0);
    Call2(Func_02004986, 0x18000, 0x3000);
    Call4(Func_020049b8, 0xE80000, 0x280000, 0x980000, 1);
    Call3(Func_0200490a, 0x16, 0x0000cccc, 0x00006666);
    Call3(Func_02004936, 0x16, 0x110, 0x80);
    Call3(Func_02004942, 0x16, 0x108, 0x98);
    Call3(Func_0200494e, 0x16, 0x118, 0xA8);
    Call0(Func_020049f2);
    Call3(Func_020049ce, 0x16, 0x5000, 0x14);
    Call1(Func_0200491c, 0x14);
    Call2(Func_020049cc, 0x17, 0);
    Call1(Func_0200492a, 0x14);
    Call2(Func_0200499a, 0x16, 3);
    Call1(Func_02004938, 0x14);
    Call2(Func_020049e8, 0x16, 0);
    Call4(Func_02004a32, 0x01280000, 0x280000, 0xD80000, 1);
    Call3(Func_020049a6, 0x16, 0x130, 0xB0);
    Call3(Func_02004a22, 0x16, 0x2000, 0);
    Call0(Func_02004a56);
    Call1(Func_02004974, 0x14);
    Call2(Func_020049ec, 0, 1);
    Call2(Func_020049f4, 1, 1);
    Call2(Func_020049fc, 2, 1);
    Call2(Func_02004a0c, 3, 1);
    Call1(Func_0200499a_a, 0x14);
    Call1(Func_020049a0, 0x14);
    Call2(Func_02004a10, 0x16, 3);
    Call1(Func_020049ae, 0x14);
    Call2(Func_02004a5e, 0x16, 0);
    Call1(Func_020049bc, 0xA);
    Call3(Func_02004a8e, 0, 0x00000107, 0);
    Call3(Func_02004a98, 1, 0x00000107, 0);
    Call3(Func_02004aa2, 3, 0x00000107, 0);
    Call3(Func_02004aac, 2, 0x00000107, 0x46);
    Call2(Func_02004a64, 0x16, 1);
    Call1(Func_020049f2_a, 0x1E);
    Call2(Func_02004aa2_a, 0x16, 0);
    Call1(Func_02004a00, 0xA);
    Call3(Func_02004ad2, 0, 0x00000105, 0);
    Call3(Func_02004adc, 1, 0x00000105, 0);
    Call3(Func_02004ae6, 2, 0x00000105, 0);
    Call3(Func_02004af0, 3, 0x00000105, 0x46);
    Call2(Func_02004a98_a, 0x16, 3);
    Call1(Func_02004a36, 0x14);
    Call2(Func_02004ae6_a, 0x16, 0);
    Call1(Func_02004a44, 0x14);
    Call2(Func_02004af4, 0x17, 0);
    Call1(Func_02004a52, 0xA);
    Call3(Func_02004b16, 0x16, 0x5000, 0);
    Call1(Func_02004a64_a, 0x28);
    Call2(Func_02004b14, 0x16, 0);
    Call1(Func_02004a72, 0x1E);
    Call2(Func_02004b22, 0x17, 0);
    Call1(Func_02004a80, 0xA);
    Call3(Func_02004b44, 0x16, 0x3000, 0);
    Call1(Func_02004a92, 0x1E);
    Call2(Func_02004b42, 0x16, 0);
    Call1(Func_02004aa0, 0x14);
    temp_r0_5 = Value1(Func_02004ac6, 1);
    FIELD(temp_r0_5, u8 *, 0x5A) = (u8)(0xFE & FIELD(temp_r0_5, u8 *, 0x5A));
    Call3(Func_02004afe, 1, 0x148, 0xE0);
    Call3(Func_02004b0a, 0, 0x158, 0xE0);
    Call3(Func_02004b16_a, 2, 0x158, 0xE8);
    Call1(Func_02004b2c, 1);
    flag = 1;
    temp_r0_6 = Value1(Func_02004b02, 1);
    bits = FIELD(temp_r0_6, u8 *, 0x5A) | flag;
    FIELD(temp_r0_6, u8 *, 0x5A) = bits;
    Call3(Func_02004bb0, 1, 0xC000, 0);
    Call1(Func_02004b4e, 0);
    Call3(Func_02004bc2, 0, 0xA000, 0);
    Call1(Func_02004b60, 2);
    Call3(Func_02004bd4, 2, 0xC000, 0);
    Call1(Func_02004b22_a, 0x1E);
    Call3(Func_02004b88, 0x17, 0xA80000, 0xC80000);
    Call3(Func_02004bea, 0x17, 0, 0x14);
    Call3(Func_02004ba0, 0x17, 0x680000, 0xC80000);
    Call2(Func_02004bc8, 0x16, 1);
    Call1(Func_02004b56, 0x14);
    Call3(Func_02004c10, 0x16, 0, 0x14);
    Call2(Func_02004c44, 0x30000, 0x6000);
    Call4(Func_02004c5c, 0x780000, 0xffe80000, 0xA80000, 1);
    Call0(Func_02004c68);
    Call2(Func_02004bf0, 0x17, 3);
    Call1(Func_02004b8e, 0xA);
    Call3(Func_02004c52, 0x17, 0xD000, 0x14);
    Call2(Func_02004c0a, 0x17, 3);
    Call1(Func_02004ba8, 0x14);
    Call3(Func_02004c62, 0x17, 0, 0x14);
    Call3(Func_02004c76, 5, 0x5000, 0);
    Call3(Func_02004c82, 0x14, 0x3000, 0x46);
    Call3(Func_02004c54, 5, 0x14, 0);
    Call1(Func_02004bda, 0x32);
    Call3(Func_02004c9e, 5, 0x5000, 0);
    Call3(Func_02004caa, 0x14, 0x3000, 0x14);
    Call2(Func_02004c62_a, 5, 4);
    Call1(Func_02004c00, 0x14);
    Call3(Func_02004cd4, 0x17, 0x100, 0x3C);
    Call3(Func_02004cc6, 0x17, 0, 0x14);
    Call3(Func_02004ce8, 5, 0x00000105, 0x3C);
    Call3(Func_02004ce4, 0x15, 0xD000, 0x14);
    Call2(Func_02004cac, 0x15, 1);
    Call2(Func_02004ca4, 0x17, 3);
    Call1(Func_02004c42, 0x14);
    Call3(Func_02004cfc, 0x17, 0, 0x14);
    Call2(Func_02004cbc, 0x17, 4);
    Call1(Func_02004c5a, 0x14);
    Call3(Func_02004d14, 0x17, 0, 0x14);
    Call2(Func_02004d3e, 0x15, 0x102);
    Call1(Func_02004c74, 0x3C);
    Call2(Func_02004ce4_a, 0x15, 4);
    Call1(Func_02004c82_a, 0x14);
    Call3(Func_02004d3c, 0x15, 0, 0x14);
    Call2(Func_02004d66, 5, 0x102);
    Call1(Func_02004c9c, 0x3C);
    Call3(Func_02004d56, 5, 0, 0x28);
    Call2(Func_02004d16, 0x15, 3);
    Call1(Func_02004cb4, 0x3C);
    Call3(Func_02004d78, 5, 0xA000, 0xA);
    Call2(Func_02004d30, 0x14, 3);
    Call1(Func_02004cce, 0x14);
    Call3(Func_02004da0, 0x14, 0, 0x1E);
    Call3(Func_02004d22, 5, 0x0000b333, 0x00005999);
    Call3(Func_02004d2c, 0x14, 0x0000b333, 0x00005999);
    Call3(Func_02004d4e, 5, 0x80, 0x90);
    Call3(Func_02004d60, 0x14, 0x78, 0x88);
    Call3(Func_02004ddc, 0x14, 0x5000, 0);
    Call1(Func_02004d7a, 5);
    Call3(Func_02004dec, 5, 0, 0x14);
    Call3(Func_02004df8, 0x15, 0x3000, 0x14);
    temp_r0_7 = Func_02004d66_a(0x15);
    FIELD(temp_r0_7, u8 *, 0x5A) = (u8)(0xFE & FIELD(temp_r0_7, u8 *, 0x5A));
    Call3(Func_02004da2, 0x15, 0x58, 0x98);
    temp_r0_8 = Value1(Func_02004d80, 0x15);
    bits = FIELD(temp_r0_8, u8 *, 0x5A) | flag;
    FIELD(temp_r0_8, u8 *, 0x5A) = bits;
    Call3(Func_02004e2c, 0x17, 0xB000, 0x14);
    Call2(Func_02004ddc_a, 0x17, 3);
    Call2(Func_02004dec_a, 0x15, 3);
    Call1(Func_02004d8a, 0x28);
    Call3(Func_02004dc0, 0x17, 0x30000, 0x20000);
    FIELD(Value1(Func_02004dbe, 0x17), s32 *, 0x28) = 0x40000;
    Call1(Func_02004f1a, 0x98);
    temp_r0_9 = Value1(Func_02004dd0, 0x17);
    FIELD(temp_r0_9, u8 *, 0x55) = (u8)(0x7E & FIELD(temp_r0_9, u8 *, 0x55));
    Call2(Func_02004d86, Func_02004de0(0x17), 0);
    FIELD(Func_02004dec_b(0x11), s8 *, 0x55) = 4;
    Call3(Func_02004e14, 0x17, 0x68, 0xA8);
    Call2(Func_02004da8, Func_02004e02(0x17), 1);
    FIELD(Value1(Func_02004e0e, 0x17), s8 *, 0x55) = 3;
    Call3(Func_02004eb6, 0x17, 0, 0x1E);
    Call3(Func_02004ec0, 0x15, 0, 0xA);
    Call3(Func_02004ec2, 0x15, 0, 0x14);
    Call2(Func_02004e92, 5, 2);
    Call2(Func_02004e8a, 5, 4);
    Call1(Func_02004e28, 0x14);
    Call3(Func_02004ee2, 5, 0, 0x14);
    Call3(Func_02004eec, 0x17, 0, 0x14);
    Call2(Func_02004f04, 0x11, 0);
    Call2(Func_02004f0c, 0x12, 0);
    Call0(Func_02004988);
    Call2(Func_02004f18, 0x11, 1);
    Call2(Func_02004f20, 0x12, 1);
    Call3(Func_02004f24, 0, 0x6000, 0);
    Call3(Func_02004f30, 1, 0x6000, 0);
    Call3(Func_02004f3c, 2, 0x6000, 0);
    Call3(Func_02004f48, 3, 0x6000, 0);
    Call4(Func_02004f82, 0x01300000, 0x200000, 0xD80000, 1);
    Call0(Func_02004f8e);
    Call3(Func_02004f0c_a, 0x14, 0x01100000, 0x01180000);
    Call3(Func_02004f6e, 0x14, 0, 0x14);
    Call3(Func_02004f20_a, 0x14, 0, 0);
    Call2(Func_02004f48_a, 1, 1);
    Call1(Func_02004ed6, 0x14);
    Call3(Func_02004f90, 1, 0, 0x14);
    Call3(Func_02004f16, 1, 0x18000, 0xC000);
    Call3(Func_02004f42, 1, 0x138, 0xD8);
    Call3(Func_02004fbe, 1, 0x6000, 0xA);
    Call3(Func_02004fc0, 0x16, 0, 0x14);
    Call3(Func_02004fd4, 0, 0xA000, 0);
    Call3(Func_02004fe0, 1, 0xC000, 0);
    Call3(Func_02004fec, 2, 0xC000, 0);
    Call3(Func_02004ff8, 3, 0xC000, 0);
    Call3(Func_02004f94, 0x16, 0x138, 0xB8);
    Call3(Func_02005010, 0x16, 0x3000, 0x14);
    Call3(Func_02005012, 3, 0, 0x14);
    Call3(Func_02005036, 1, 0x100, 0x3C);
    Call3(Func_02004fa4, 1, 0x20000, 0x10000);
    temp_r0_10 = Value1(Func_02004fa2, 1);
    FIELD(temp_r0_10, u8 *, 0x5A) = (u8)(0xFE & FIELD(temp_r0_10, u8 *, 0x5A));
    Call3(Func_02004fde, 1, 0x148, 0xE0);
    Call1(Func_02004f9c, 1);
    temp_r0_11 = Value1(Func_02004fc2, 1);
    flag |= FIELD(temp_r0_11, u8 *, 0x5A);
    FIELD(temp_r0_11, u8 *, 0x5A) = flag;
    Call2(Func_02005022, 0, 1);
    Call2(Func_0200502a, 1, 1);
    Call2(Func_02005032, 2, 1);
    Call2(Func_02005042, 3, 1);
    Call1(Func_02004fd0, 0x14);
    Call2(Func_02005050, 0x16, 1);
    Call1(Func_02004fde_a, 0xA);
    Call3(Func_02005098, 0x16, 0, 0x14);
    Call2(Func_02005068, 1, 2);
    Call1(Func_02004ff6, 0xA);
    Value2(Func_02005066, 1, 4);
    Call1(Func_02005004, 0xA);
    Call3(Func_020050be, 1, 0, 0x14);
    Call2(Func_0200508e, 2, 1);
    Call1(Func_0200501c, 0xA);
    Call2(Func_0200508c, 2, 3);
    Call1(Func_0200502a_a, 0x14);
    Call3(Func_020050e4, 2, 0, 0x14);
    Call3(Func_02005106, 0x16, 0x00000101, 0x3C);
    Value2(Func_020050e6, 0x16, 0);
    if (Value2(Func_02005066_a, 0, 0) == 0) {
        Call1(Func_02005058, 0x14);
        Call2(Func_020050c8, 0x16, 4);
        Value1(Func_02005066_b, 0x14);
        Call3(Func_02005120, 0x16, 0, 0x14);
        temp_r2_3 = Scene58Work;
        FIELD(temp_r2_3, u16 *, 0x1D8) = (u16)(FIELD(temp_r2_3, u16 *, 0x1D8) + 1);
    } else {
        Call1(Func_02005098_a, 0x14);
        Call2(Func_02005108, 0x16, 4);
        Call1(Func_020050a6, 0x14);
        temp_r2_4 = Scene58Work;
        FIELD(temp_r2_4, u16 *, 0x1D8) = (u16)(FIELD(temp_r2_4, u16 *, 0x1D8) + 1);
        Value3(Func_02005170, 0x16, 0, 0x14);
    }
    Call2(Func_02005140, 0x16, 2);
    Call1(Func_020050ce, 0x14);
    Call3(Func_02005188, 0x16, 0, 0x14);
    Call3(Func_0200512c, 0x16, 0x148, 0xC8);
    Call2(Func_02005164, 0x16, 2);
    Call3(Func_020051b0, 0x16, 0xB000, 0x14);
    temp_r0_12 = Value1(Func_0200511e, 0x16);
    FIELD(temp_r0_12, u8 *, 0x5A) = (u8)(0xFE & FIELD(temp_r0_12, u8 *, 0x5A));
    Call3(Func_0200515c, 0x16, 0x150, 0xD0);
    Call1(Func_0200511a, 1);
    temp_r0_13 = Func_02005140_a(0x16);
    flag2 = 1;
    flag2 |= FIELD(temp_r0_13, u8 *, 0x5A);
    FIELD(temp_r0_13, u8 *, 0x5A) = flag2;
    Call3(Func_020051fe, 0x16, 0x102, 0x3C);
    Call3(Func_02005208, 3, 0x00000101, 0x3C);
    Call3(Func_020051fa, 3, 0, 0x14);
    Call3(Func_02005204, 0x16, 0, 0x14);
    Call3(Func_02005218, 0x16, 0x5000, 0x14);
    Call2(Func_020051e0, 0x16, 2);
    Call1(Func_0200516e, 0x14);
    Call3(Func_02005228, 0x16, 0, 0x14);
    Call3(Func_020051cc, 0x16, 0x150, 0xD8);
    Call2(Func_02005294, (s32)&Value_0000003a, 2);
    do {
        Data_02000240[0x22B] = 3;
    } while (0);
    Call2(Func_0200529e, 0x24, 2);
    Call0(Func_020051b2);
}

void RunScene59Sequence(void)
{
    s32 actor9_fixed_y;
    void *actor_one_record;
    void *scene_counter_initial;
    void *scene_counter_initial_alt;
    void *scene_counter_mid_a;
    void *scene_counter_mid_b;
    void *scene_counter_first_a;
    void *scene_counter_first_b;
    void *scene_counter_system_a;
    void *scene_counter_system_b;
    void *scene_counter_later_a;
    void *scene_counter_later_b;
    void *scene_counter_final_a;
    void *scene_counter_final_b;

    Call0(Func_020051ce);
    FIELD(Value1(Func_020051ec, 9), s8 *, 0x55) = 0;
    Call3(Func_02005238, 0, 0x01580000, 0xE00000);
    Call3(Func_02005246, 1, 0x01480000, 0xE00000);
    Call3(Func_02005254, 2, 0x01580000, 0xE80000);
    Call3(Func_02005262, 3, 0x01480000, 0xE80000);
    Call3(Func_020052ce, 0, 0xA000, 0);
    Call3(Func_020052da, 1, 0xC000, 0);
    Call3(Func_020052e6, 2, 0xC000, 0);
    Call3(Func_020052f2, 3, 0xC000, 0);
    Call3(Func_020052a0, 0x16, 0x01500000, 0xB00000);
    Call2(Func_020052b0, 0x16, 9);
    Value2(Func_0200521c, Func_02005276(0x16), 0);
    Call4(Func_0200534e, 0x01500000, -1, 0xD00000, 0);
    Call1(Func_020051d4, 1);
    Call0(Func_02005220);
    Call1(Func_020051de, 1);
    /* The ROM loads this IWRAM pointer cell before the request store. */
    SCENE_REQUEST(Data_03001ebc) = 0x100;
    Call0(Func_020053ba);
    Call0(Func_020053c6);
    Call1(Func_0200529c, 0x3C);
    Call2(Func_0200531c, 1, 2);
    Call1(Func_020052aa, 0x14);
    Call3(Func_0200536c, 1, 0, 0x14);
    Call1(Func_02005352, 0x000015d4);
    Call3(Func_02005374, 1, 0, 0x14);
    Call3(Func_02005388, 0, 0x8000, 0x14);
    Call2(Func_02005340, 0, 3);
    Call1(Func_020052de, 0x14);
    Call2(Func_0200535e, 0x16, 2);
    Call1(Func_020052ec, 0x14);
    Call3(Func_020053a6, 0x16, 0, 0x14);
    Call3(Func_020053ba_a, 0, 0xC000, 0);
    Call3(Func_020053c6_a, 1, 0xC000, 0x14);
    Call3(Func_020053e0, 2, 0x00000101, 0x3C);
    Call3(Func_020053d2, 2, 0, 0x14);
    Call2(Func_020053a2, 0x16, 2);
    Call1(Func_02005330, 0x14);
    Call3(Func_020053ea, 0x16, 0, 0x1E);
    Call3(Func_020053a0, 0x18, 0x01380000, 0x700000);
    Call1(Func_020054c0, 0x120);
    Call3(Func_0200540a, 0x18, 0, 0x14);
    Call3(Func_0200542c, 3, 0x100, 0x3C);
    Call3(Func_0200541e, 3, 0, 0x1E);
    Call1(Func_020054e4, 0x1D);
    Call2(Func_02005454, 0x0000cccc, 0x00001999);
    FIELD(Value0(Func_02005470), s8 *, 0x55) = 0;
    Call4(Func_02005476, 0x01500000, -1, 0xA80000, 1);
    Call1(Func_0200539c, 0x14);
    Call3(Func_020053ce, 0x18, 0x0000cccc, 0x00006666);
    Call3(Func_020053fa, 0x18, 0x158, 0x88);
    Call3(Func_02005476_a, 0x18, 0x5000, 0x14);
    Call0(Func_020054aa);
    Call3(Func_0200547c, 3, 0, 0x1E);
    Call4(Func_020054be, 0x01500000, -1, 0xB80000, 1);
    Call3(Func_02005432, 0x18, 0x158, 0xA0);
    Call3(Func_0200543e, 0x18, 0x148, 0xA8);
    Call3(Func_0200544a, 0x18, 0x138, 0xB0);
    Call3(Func_020054c6, 0x18, 0x3000, 0x14);
    Call0(Func_020054fa);
    Call2(Func_02005482, 0x18, 3);
    Call1(Func_02005420, 0x14);
    Call3(Func_020054da, 0x18, 0, 0x14);
    Call3(Func_020054ee, 0, 0x6000, 0);
    Call3(Func_020054fa_a, 1, 0x4000, 0);
    Call3(Func_02005506, 2, 0x8000, 0x14);
    Call2(Func_020054ce, 3, 2);
    Call1(Func_0200545c, 0x14);
    Call3(Func_02005516, 3, 0, 0x14);
    Call2(Func_020054d6, 0x18, 4);
    Call1(Func_02005474, 0x14);
    Call3(Func_0200552e, 0x18, 0, 0x14);
    Call3(Func_02005542, 1, 0x2000, 0);
    Call3(Func_0200554e, 2, 0xA000, 0);
    Call3(Func_0200555a, 3, 0xE000, 0x3C);
    Call3(Func_02005566, 0x18, 0xB000, 0x14);
    Call3(Func_02005572, 0, 0xA000, 0);
    Call3(Func_0200557e, 1, 0xC000, 0);
    Call3(Func_0200558a, 2, 0xA000, 0);
    Call3(Func_02005596, 3, 0xC000, 0x14);
    Call3(Func_02005598, 0x18, 0, 0x14);
    Call2(Func_02005558, 3, 4);
    Call1(Func_020054f6, 0x14);
    Call3(Func_020055b0, 3, 0, 0x14);
    Call2(Func_02005578, 0x18, 2);
    Call3(Func_020055da, 0x18, 0x100, 0x3C);
    Call3(Func_020055d6, 0x18, 0x3000, 0x14);
    Call3(Func_020055d8, 0x18, 0, 0x14);
    Call3(Func_020055fa, 3, 0x00000101, 0x3C);
    Call2(Func_020055a2, 0x18, 4);
    Call1(Func_02005540, 0x14);
    Call3(Func_020055fa_a, 0x18, 0, 0x14);
    Call2(Func_020055ca, 3, 2);
    Call1(Func_02005558_a, 0x14);
    Call3(Func_02005612, 3, 0, 0x14);
    Call2(Func_020055d2, 0x18, 3);
    Call1(Func_02005570, 0x14);
    Call3(Func_0200562a, 0x18, 0, 0x14);
    Call2(Func_020055fa_b, 3, 1);
    Call1(Func_02005588, 0x14);
    Call3(Func_02005642, 3, 0, 0x14);
    Call3(Func_020055e6, 0x18, 0x138, 0xB8);
    Call3(Func_02005662, 0x18, 0x3000, 0x14);
    Call3(Func_02005664, 0x18, 0, 0x14);
    Call2(Func_02005634, 1, 2);
    Call1(Func_020055c2, 0xA);
    Call3(Func_02005684, 1, 0, 0x14);
    Value2(Func_02005674, 1, 0);
    if (Value2(Func_020055f4, 0, 0) == 0) {
        Call2(Func_02005660, 1, 2);
        Call3(Func_020056a2, 1, 0, 0x14);
        scene_counter_initial = Data_03001ebc;
        SCENE_SKIP_COUNT(scene_counter_initial) = (u16)(SCENE_SKIP_COUNT(scene_counter_initial) + 1);
    } else {
        scene_counter_initial_alt = Data_03001ebc;
        SCENE_SKIP_COUNT(scene_counter_initial_alt) = (u16)(SCENE_SKIP_COUNT(scene_counter_initial_alt) + 1);
        Call2(Func_020056a8, 1, 1);
        Call3(Func_020056ea, 1, 0, 0x14);
    }
    Call3(Func_020056f4, 2, 0, 0x14);
    Call3(Func_02005708, 1, 0xC000, 0x14);
    Call2(Func_020056c0, 0x18, 3);
    Call1(Func_0200565e, 0x14);
    Call3(Func_02005718, 0x18, 0, 0x14);
    Call3(Func_0200572c, 0x18, 0xD000, 0x14);
    Call3(Func_0200572e, 0x18, 0, 0x14);
    Call3(Func_02005742, 0x18, 0x3000, 0x14);
    Call2(Func_020056fa, 0x18, 4);
    Call1(Func_02005698, 0x14);
    Call3(Func_02005752, 0x18, 0, 0x14);
    Call3(Func_02005774, 3, 0x00000101, 0x3C);
    Call3(Func_02005766, 3, 0, 0x14);
    Call2(Func_02005726, 0x18, 4);
    Call1(Func_020056c4, 0x14);
    Call3(Func_0200577e, 0x18, 0, 0x14);
    Call3(Func_02005792, 0x18, 0xB000, 0x14);
    Call3(Func_02005794, 0x18, 0, 0x14);
    Call1(Func_020056ea_a, 0xA);
    Call2(Func_0200576a, 0x18, 2);
    Call1(Func_020056f8, 0x1E);
    Call2(Func_020057a8, 0x18, 0);
    Call1(Func_02005706, 0xA);
    Call2(Func_02005786, 1, 2);
    Call1(Func_02005714, 0x1E);
    Call2(Func_020057c4, 1, 0);
    Call1(Func_02005722, 0xA);
    Call3(Func_020057e6, 0x18, 0x2000, 0);
    Call1(Func_02005734, 0x1E);
    Call2(Func_020057a4, 0x18, 4);
    Call1(Func_02005742_a, 0x14);
    Call2(Func_020057f2, 0x18, 0);
    Call1(Func_02005750, 0x14);
    Call2(Func_020057d0, 0x18, 2);
    Call1(Func_0200575e, 0x1E);
    Call2(Func_0200580e, 0x18, 0);
    Call1(Func_0200576c, 0xA);
    Call3(Func_02005840, 2, 0x100, 0x3C);
    Call2(Func_02005828, 2, 0);
    Call1(Func_02005786_a, 0xA);
    Call2(Func_020057f6, 0x18, 3);
    Call1(Func_02005794_a, 0x1E);
    Call2(Func_02005844, 0x18, 0);
    Call1(Func_020057a2, 0x14);
    Call2(Func_02005822, 0x18, 2);
    Call1(Func_020057b0, 0x1E);
    Call2(Func_02005860, 0x18, 0);
    Call1(Func_020057be, 0xA);
    Call3(Func_02005882, 0x18, 0x1000, 0);
    Call1(Func_020057d0_a, 0x1E);
    Call3(Func_020058a4, 0x18, 0x108, 0x3C);
    Call1(Func_020057e2, 0xA);
    Call2(Func_02005862, 0x16, 2);
    Call1(Func_020057f0, 0x1E);
    Call2(Func_02005858, 0x16, 8);
    Call1(Func_020057fe, 0x2D);
    Call2(Func_02005866, 0x16, 1);
    Value2(Func_020057d2, Func_0200582c(0x16), 1);
    Call1(Func_02005818, 0x28);
    Call3(Func_020058ec, 2, 0x102, 0x3C);
    Call2(Func_020058d4, 2, 0);
    Call1(Func_02005832, 0xA);
    Call3(Func_020058f6, 0x16, 0x2000, 0);
    Call1(Func_02005844_a, 0x14);
    Call2(Func_020058b4, 0x16, 4);
    Call1(Func_02005852, 0x1E);
    Call2(Func_02005902, 0x16, 0);
    Call1(Func_02005860_a, 0x14);
    Call3(Func_02005924, 0x18, 0x2000, 0);
    Call1(Func_02005872, 0x1E);
    Call2(Func_02005922, 0x18, 0);
    Call1(Func_02005880, 0xA);
    Call2(Func_02005900, 3, 2);
    Call1(Func_0200588e, 0x1E);
    Call2(Func_0200593e, 3, 0);
    Call1(Func_0200589c, 0xA);
    Call3(Func_02005926, 0x18, 0x16, 0);
    Call1(Func_020058ac, 0x23);
    Call2(Func_0200591c, 0x16, 3);
    Call1(Func_020058ba, 0x1E);
    Call2(Func_0200592a, 0x18, 3);
    Call1(Func_020058c8, 0x14);
    Call3(Func_0200591c_a, 0x18, 0x148, 0xB0);
    Call3(Func_02005998, 0x18, 0x3000, 0x14);
    Call2(Func_02005948, 0x18, 5);
    Call2(Func_02005950, 0x16, 7);
    Call1(Func_020058f6_a, 0x14);
    Call2(Func_02005976, 1, 1);
    Call1(Func_02005904, 0x14);
    Call2(Func_020059b4, 1, 0);
    Call1(Func_02005912, 0xA);
    Call3(Func_020059e4, 0x18, 0x00000101, 0x3C);
    Value2(Func_020059c4, 0x18, 0);
    Call1(Func_0200592a_a, 0x1E);
    Call2(Func_020059a2, 1, 2);
    Call2(Func_020059aa, 2, 2);
    Call2(Func_020059ba, 3, 2);
    Call3(Func_02005a04, 1, 0, 0);
    Call3(Func_02005a10, 2, 0xC000, 0);
    Call3(Func_02005a1c, 3, 0xE000, 0);
    Call1(Func_0200596a, 0x14);
    if (Value2(Func_0200598a, 0, 0) == 0) {
        Call1(Func_0200597c, 0x1E);
        Call2(Func_02005a2c, 0x18, 0);
        scene_counter_first_a = Data_03001ebc;
        SCENE_SKIP_COUNT(scene_counter_first_a) = (u16)(SCENE_SKIP_COUNT(scene_counter_first_a) + 1);
    } else {
        Call1(Func_020059a4, 0x1E);
        /* This branch needs its own workspace load; stale register contents
         * are not a valid C dependency. */
        scene_counter_first_b = Data_03001ebc;
        SCENE_SKIP_COUNT(scene_counter_first_b) = (u16)(SCENE_SKIP_COUNT(scene_counter_first_b) + 1);
        Call2(Func_02005a64, 0x18, 0);
    }
    Call1(Func_020059c2, 0x14);
    Call2(Func_02005a32, 0x18, 4);
    Call1(Func_020059d0, 0x14);
    Call3(Func_02005a94, 1, 0xC000, 0);
    Call3(Func_02005aa0, 2, 0xA000, 0);
    Call3(Func_02005aac, 3, 0xC000, 0);
    Call1(Func_020059fa, 0x14);
    Call2(Func_02005aaa, 0x18, 0);
    Call2(Func_02005a82, 0x16, 1);
    Call1(Func_02005a10_a, 0x14);
    Call3(Func_02005aca, 0x16, 0, 0x14);
    Call0(Func_02005266);
    Call1(Func_02005a24, 0x14);
    Call2(Func_02005a9c, 0, 2);
    Call2(Func_02005aa4, 1, 2);
    Call2(Func_02005aac_a, 2, 2);
    Call2(Func_02005abc, 3, 2);
    Call3(Func_02005b06, 0, 0, 5);
    Call3(Func_02005b12, 1, 0xE000, 5);
    Call3(Func_02005b1e, 2, 0xA000, 5);
    Call3(Func_02005b2a, 3, 0x8000, 5);
    Call3(Func_02005b36, 0, 0xE000, 5);
    Call3(Func_02005b42, 1, 0xA000, 5);
    Call3(Func_02005b4e, 2, 0x8000, 5);
    Call3(Func_02005b58, 3, 0, 5);
    FIELD(Value0(Func_02005b94), s8 *, 0x55) = 0;
    Call4(Func_02005b9c, 0x01180000, -1, 0xE80000, 1);
    Call0(Func_02005ba8);
    Call2(Func_02005b50, 0x16, 0xF);
    Call2(Func_02005b58_a, 0x18, 0xF);
    Call3(Func_02005b36_a, 0x16, 0xF00000, 0xD00000);
    Call3(Func_02005b44, 0x18, 0xE80000, 0xD00000);
    Call3(Func_02005bb0, 0x16, 0x5000, 0);
    Call3(Func_02005bbc, 0x18, 0x3000, 0);
    Call0(Func_02005498);
    Call3(Func_02005bcc, 0, 0x8000, 0xA);
    Call3(Func_02005bd8, 1, 0x8000, 0xF);
    Call3(Func_02005be4, 2, 0x8000, 0);
    Call3(Func_02005bf0, 3, 0x8000, 0x14);
    Call3(Func_02005b84, 0, 0x148, 0xD0);
    Call3(Func_02005b90, 2, 0x150, 0xE0);
    Call3(Func_02005ba4, 1, 0x138, 0xD8);
    Call3(Func_02005c20, 1, 0x8000, 0);
    Call1(Func_02005bbe, 2);
    Call3(Func_02005c32, 2, 0x8000, 0);
    Call1(Func_02005bd0, 0);
    Call3(Func_02005c44, 0, 0x8000, 0x1E);
    Call3(Func_02005c46, 1, 0, 0x14);
    Call2(Func_02005c16, 3, 2);
    Call1(Func_02005ba4_a, 0x14);
    Call3(Func_02005c5e, 3, 0, 0x14);
    Call3(Func_02005c72, 1, 0x5000, 0x14);
    Call3(Func_02005c74, 1, 0, 0x14);
    Call3(Func_02005c88, 1, 0x8000, 0x14);
    Call2(Func_02005c50, 2, 1);
    Call3(Func_02005c9c, 2, 0x6000, 0x14);
    Call3(Func_02005c9e, 2, 0, 0x14);
    Call2(Func_02005c5e_a, 3, 4);
    Call1(Func_02005bfc, 0x14);
    Call3(Func_02005cb6, 3, 0, 0x14);
    Call3(Func_02005cca, 2, 0x8000, 0x14);
    Call3(Func_02005ccc, 0x18, 0, 0x14);
    Call2(Func_02005d00, 0x30000, 0x6000);
    {
        void **scene_system_cell;
        Call4(Func_02005d1a, 0x980000, -1, 0xD80000, 1);
        scene_system_cell = (void **)(Data_03001e70 + 0x164);
        FIELD(scene_system_cell, s32 *, 0xC) = 0x03800000;
        scene_system_cell = (void **)&Data_03001e70;
        Call0(Func_02005bf6);
        Call1(Func_02005bb4, 1);
        FIELD(Value1(Func_02005c7a, 9), s8 *, 0x55) = 0;
        Call3(Func_02005cc4, 9, 0x680000, 0x01080000);
        actor9_fixed_y = 0xffe00000;
        FIELD(Value1(Func_02005c92, 9), s32 *, 0xC) = actor9_fixed_y;
        FIELD(Func_02005c9c_a(9), s32 *, 0x3C) = actor9_fixed_y;
        Call6(Func_02005c40, 0x1D, 0x4A, 4, 0x4A, 5, 4);
        Call2(Func_02005d58, 0x11, 0);
        Call2(Func_02005d60, 0x12, 0);
        Call0(Func_020058f8);
        Call2(Func_02005d6c, 0x11, 1);
        Call2(Func_02005d74, 0x12, 1);
        Call0(Func_02005da0);
        Call1(Func_02005cbe, 0x1E);
        Call3(Func_02005d82, 0x18, 0x8000, 0x14);
        Call3(Func_02005d84, 0x18, 0, 0x14);
        Call4(Func_02005dc6, 0x01180000, -1, 0xD80000, 1);
        Call0(Func_02005dd2);
        Call3(Func_02005dae, 0x18, 0x3000, 0x14);
        Call2(Func_02005d5e, 0x18, 5);
        Call3(Func_02005db8, 0x18, 0, 0x14);
        Call2(Func_02005d88, 2, 2);
        Call1(Func_02005d16, 0x14);
        Call3(Func_02005dda, 2, 0xA000, 0x14);
        Call3(Func_02005ddc, 2, 0, 0x14);
        Call2(Func_02005dac, 1, 1);
        Call1(Func_02005d3a, 0x14);
        Call3(Func_02005df4, 1, 0, 0x14);
        Call3(Func_02005e16, 0x18, 0x00000101, 0x3C);
        Value2(Func_02005df6, 0x18, 0);
        if (Value2(Func_02005d76, 0, 0) == 0) {
            Call1(Func_02005d68, 0x14);
            Call2(Func_02005de8, 0x16, 2);
            Func_02005d76_a(0x14);
            Call3(Func_02005e30, 0x16, 0, 0x14);
            scene_counter_system_a = FIELD(scene_system_cell, void **, 0x4C);
            SCENE_SKIP_COUNT(scene_counter_system_a) = (u16)(SCENE_SKIP_COUNT(scene_counter_system_a) + 1);
        } else {
            scene_counter_system_b = FIELD(scene_system_cell, void **, 0x4C);
            SCENE_SKIP_COUNT(scene_counter_system_b) = (u16)(SCENE_SKIP_COUNT(scene_counter_system_b) + 1);
            Call1(Func_02005db6, 0x14);
            Call3(Func_02005e70, 0x16, 0, 0x14);
        }
    }
    Call2(Func_02005e40, 0x18, 2);
    Call1(Func_02005dce, 0x14);
    Call3(Func_02005e88, 0x18, 0, 0x14);
    Call3(Func_02005eaa, 0, 0x00000101, 0);
    Call3(Func_02005eb4, 1, 0x00000101, 0);
    Call3(Func_02005ebe, 2, 0x00000101, 0);
    Call3(Func_02005ec8, 3, 0x00000101, 0x3C);
    Value2(Func_02005ea8, 0x18, 0);
    if (Value2(Func_02005e28, 0, 0) == 0) {
        Call1(Func_02005e1a, 0x14);
        Call2(Func_02005e9a, 0x18, 2);
        Func_02005e28_a(0x14);
        Call3(Func_02005ee2, 0x18, 0, 0x14);
        scene_counter_mid_a = Data_03001ebc;
        SCENE_SKIP_COUNT(scene_counter_mid_a) = (u16)(SCENE_SKIP_COUNT(scene_counter_mid_a) + 1);
    } else {
        scene_counter_mid_b = Data_03001ebc;
        SCENE_SKIP_COUNT(scene_counter_mid_b) = (u16)(SCENE_SKIP_COUNT(scene_counter_mid_b) + 1);
        Call1(Func_02005e5a, 0x14);
        Call2(Func_02005eda, 0x18, 1);
        Call1(Func_02005e68, 0x14);
        Call3(Func_02005f22, 0x18, 0, 0x14);
    }
    Call1(Func_02005e78, 0x14);
    Call0(Func_020056c4_a);
    Call1(Func_02005e82, 0x14);
    Call4(Func_02005f74, 0x800000, -1, 0xC80000, 1);
    Call0(Func_02005f80);
    Call2(Func_02005f00, 0x16, 1);
    Call2(Func_02005f08, 0x18, 1);
    Call2(Func_02005f38, 0x16, 0xF);
    Call2(Func_02005f40, 0x18, 0xF);
    Call3(Func_02005f1e, 0x16, 0x780000, 0x980000);
    Call3(Func_02005f2c, 0x18, 0x700000, 0xA00000);
    Call3(Func_02005f98, 0x16, 0x5000, 0);
    Call3(Func_02005fa4, 0x18, 0x3000, 0);
    Call0(Func_02005880_a);
    Call1(Func_02005ef6, 0x1E);
    Call3(Func_02005fb8, 0x18, 0, 0x14);
    Value2(Func_02005fa8, 0x18, 0);
    if (Value2(Func_02005f28, 0, 0) == 0) {
        Call2(Func_02005f84, 0x18, 3);
        Call1(Func_02005f22_a, 0x14);
        Call3(Func_02005fdc, 0x18, 0, 0x14);
        scene_counter_later_a = Data_03001ebc;
        SCENE_SKIP_COUNT(scene_counter_later_a) = (u16)(SCENE_SKIP_COUNT(scene_counter_later_a) + 1);
    } else {
        scene_counter_later_b = Data_03001ebc;
        SCENE_SKIP_COUNT(scene_counter_later_b) = (u16)(SCENE_SKIP_COUNT(scene_counter_later_b) + 1);
        Call2(Func_02005fbe, 0x18, 4);
        Call1(Func_02005f5c, 0x14);
        Call3(Func_02006016, 0x18, 0, 0x14);
    }
    Call3(Func_02006020, 0x18, 0, 0x14);
    Call2(Func_02005ff0, 0x16, 1);
    Call1(Func_02005f7e, 0x14);
    Call3(Func_02006040, 0x16, 0, 0x14);
    Call3(Func_02006042, 0x16, 0, 0x14);
    Call1(Func_02005f98_a, 0x14);
    Call3(Func_0200605a, 0x18, 0, 0x14);
    Call2(Func_0200606a, 0x11, 0);
    Call2(Func_02006072, 0x12, 0);
    Call0(Func_02005aee);
    Call2(Func_0200607e, 0x11, 1);
    Call2(Func_02006086, 0x12, 1);
    Call1(Func_0200613c, 0x11);
    Call4(Func_020060be, 0x01500000, -1, 0xD80000, 1);
    Call0(Func_020060ca);
    Call2(Func_02006052, 2, 4);
    Call1(Func_02005ff0_a, 0x14);
    Call0(Func_0200615c);
    Call3(Func_020060ae, 2, 0, 0x14);
    Call3(Func_020060c2, 0, 0x4000, 0);
    Call3(Func_020060ce, 1, 0x3000, 0x16);
    Call2(Func_02006096, 1, 2);
    Call1(Func_02006024, 0xA);
    Call3(Func_020060de, 1, 0, 0x14);
    Call3(Func_02006100, 3, 0x00000105, 0x3C);
    Call3(Func_020060f2, 3, 0, 0x14);
    Call3(Func_02006106, 3, 0xA000, 0x14);
    Call3(Func_02006108, 3, 0, 0x14);
    Call3(Func_0200611c, 3, 0x8000, 0x14);
    Call2(Func_020060cc, 3, 0x10);
    FIELD(Value1(Func_02006092, 3), s32 *, 0x18) = (s32) 0xffff0000;
    Call1(Func_0200607c, 0x14);
    Call3(Func_02006136, 3, 0, 0x28);
    Call2(Func_02006106_a, 2, 2);
    Call1(Func_02006094, 0x14);
    Call3(Func_02006158, 2, 0xA000, 0x14);
    Call2(Func_02006110, 2, 4);
    Call1(Func_020060ae_a, 0x14);
    Value2(Func_02006156, 2, 0);
    Call2(Func_0200611e, 3, 1);
    FIELD(Value1(Func_020060e4, 3), s32 *, 0x18) = 0x10000;
    Call3(Func_0200618e, 0, 0x4000, 0);
    Call3(Func_0200619a, 1, 0xE000, 0);
    Call3(Func_020061a6, 3, 0xC000, 0x14);
    if (Value2(Func_0200610e, 0, 0) == 0) {
        Call2(Func_0200616a, 2, 3);
        Call1(Func_02006108_a, 0x14);
        Call3(Func_020061c2, 2, 0, 0x14);
        scene_counter_final_a = Data_03001ebc;
        SCENE_SKIP_COUNT(scene_counter_final_a) = (u16)(SCENE_SKIP_COUNT(scene_counter_final_a) + 1);
    } else {
        scene_counter_final_b = Data_03001ebc;
        SCENE_SKIP_COUNT(scene_counter_final_b) = (u16)(SCENE_SKIP_COUNT(scene_counter_final_b) + 1);
        Call2(Func_020061c6, 2, 2);
        Call1(Func_02006154, 0x14);
        Call3(Func_0200620e, 2, 0, 0x14);
    }
    Call3(Func_02006222, 2, 0x6000, 0x14);
    Call2(Func_020061da, 2, 3);
    Call1(Func_02006178, 0x14);
    Call3(Func_02006232, 2, 0, 0x14);
    Call3(Func_02006246, 3, 0xE000, 0x1E);
    Call3(Func_02006252, 1, 0x2000, 0x14);
    Call2(Func_0200621a, 1, 1);
    Call1(Func_020061a8, 0xA);
    Call3(Func_02006262, 1, 0, 0x14);
    Call3(Func_02006276, 3, 0xA000, 0x14);
    Call2(Func_0200622e, 3, 3);
    Call1(Func_020061cc, 0x14);
    Call3(Func_02006286, 3, 0, 0x14);
    Call2(Func_02006256, 3, 1);
    Call3(Func_02006298, 3, 0, 0x14);
    Call3(Func_020062ba, 0, 0x00000101, 0);
    Call3(Func_020062c4, 1, 0x00000101, 0);
    Call3(Func_020062ce, 2, 0x00000101, 0x3C);
    Call3(Func_02006238, 3, 0x0000cccc, 0x00006666);
    Call3(Func_02006242, 1, 0x0000cccc, 0x00006666);
    Call3(Func_0200626e, 3, 0x148, 0xD8);
    Call3(Func_020062ea, 2, 0xA000, 0);
    actor_one_record = Value1(Func_02006258, 1);
    FIELD(actor_one_record, u8 *, 0x5A) = (u8)(0xFE & FIELD(actor_one_record, u8 *, 0x5A));
    Call3(Func_0200628e, 1, 0x138, 0xC8);
    Call3(Func_0200629a, 3, 0x118, 0xD8);
    Call1(Func_020062b0, 1);
    Call3(Func_02006324, 0, 0x6000, 0);
    Call3(Func_02006330, 1, 0x6000, 0);
    Call1(Func_020062ce_a, 3);
    Call2(Func_020062fe, 3, 2);
    Call1(Func_0200628c, 0x1E);
    Call3(Func_0200634e, 3, 0, 0x14);
    Call3(Func_02006350, 3, 0, 0x14);
    Call2(Func_02006320, 1, 1);
    Call1(Func_020062ae, 0x14);
    Call3(Func_02006368, 1, 0, 0x14);
    Call2(Func_02006328, 3, 3);
    Call2(Func_02006338, 0, 2);
    Call2(Func_02006340, 2, 2);
    Call2(Func_02006348, 1, 2);
    Call2(Func_020063b2, 0, 0x102);
    Call2(Func_020063bc, 1, 0x102);
    Call2(Func_020063c6, 2, 0x102);
    Call1(Func_020062fc, 0x3C);
    Call3(Func_020063b6, 2, 0, 0x14);
    Call2(Func_02006376, 3, 4);
    Call1(Func_02006314, 0x14);
    Call3(Func_020063ce, 3, 0, 0x14);
    Call3(Func_020063e2, 3, 0xE000, 0x14);
    Call3(Func_020063e4, 3, 0, 0x14);
    Call3(Func_020063f6, 3, 0, 0x14);
    Call2(Func_020063ae, 3, 4);
    Call1(Func_0200634c, 0x14);
    Call3(Func_02006406, 3, 0, 0x1E);
    Call3(Func_0200641a, 1, 0x2000, 0);
    Call3(Func_02006426, 2, 0xA000, 0x14);
    Call2(Func_020063d6, 0, 3);
    Call2(Func_020063de, 1, 3);
    Call2(Func_020063ee, 2, 3);
    Call1(Func_0200638c, 0x1E);
    Call3(Func_02006450, 0, 0x6000, 0);
    Call3(Func_0200645c, 1, 0x6000, 0);
    Call3(Func_02006468, 2, 0x6000, 0x28);
    Call2(Func_02006418, 0, 3);
    Call2(Func_02006420, 1, 3);
    Call2(Func_02006430, 2, 3);
    Call1(Func_020063ce_a, 0x14);
    Call2(Func_0200643e, 3, 3);
    Call1(Func_020063dc, 0x14);
    Call3(Func_02006496, 3, 0, 0x14);
    Call3(Func_02006432, 0, 0x138, 0xD8);
    Call3(Func_0200643e_a, 1, 0x138, 0xD8);
    Call3(Func_0200644a, 2, 0x138, 0xD8);
    Call3(Func_02006456, 3, 0x138, 0xD8);
    Call1(Func_0200646c, 0);
    Call1(Func_02006472, 1);
    Call3(Func_02006484, 1, 0, 0);
    Call1(Func_02006482, 2);
    Call3(Func_02006494, 2, 0, 0);
    Call1(Func_02006492, 3);
    Call3(Func_020064a4, 3, 0, 0);
    {
        void **record;
        record = (void **)(Data_03001e70 + 0x164);
        FIELD(record, s32 *, 0xC) = 0x04000000;
    }
    Call0(Func_02006408);
    Call1(Func_020063c6_a, 1);
    Call6(Func_02006428, 4, 0x46, 4, 0x4A, 5, 4);
    Call1(Func_02006468_a, 0x880);
    Call1(Func_0200646e_sequence, 0x00000881);
    Call0(Func_0200649a);
}

void FieldScene_RunScene39d_02002ddc(void)
{
    u32 i;
    s32 record;
    u8 *p5;
    u8 *work;

    p5 = *(volatile s32 *)0x03001e70;
    work = p5 + 0x164;
    Func_020064c4();
    *(volatile s32 *)(work + 12) = 0x3800000;
    Func_0200646e();
    Func_0200642c(1);
    *(u8 *)(Func_020064f2(9) + 85) = 0;
    Call3(Func_0200653e, 9, 0x680000, 0x1080000);
    record = Func_0200650c(9);
    *(volatile s32 *)(record + 12) = -0x200000;
    record = Value1(Func_02006516, 9);
    *(volatile s32 *)(record + 60) = -0x200000;
    *(u8 *)(Func_020065ec() + 85) = 0;
    Call2(Func_020065e0, 0xcccc, 0x1999);
    Call4(Func_020065fa, 0x800000, -1, 0xb80000, 1);
    Func_02006606();
    Func_02006524(30);
    Call6(Func_020064e6, 29, 74, 4, 74, 5, 4);
    Func_020065fe(17, 0);
    Func_02006606_a(18, 0);
    Func_0200619e();
    Func_02006612(17, 1);
    Func_0200661a(18, 1);
    Func_02006560(20);
    Call1(Func_0200654e, 0x251);
    Func_0200657a();
}

void FieldScene_RunScene39d_02002eb8(void)
{
    u32 i;
    s32 record;

    Func_02006594();
    Call3_02002eb8(Func_020065c2, 0, 0x8000, 0x4000);
    Func_020065ec_a(0, 104, 152);
    Call3_02002eb8(Func_02006668, 0, 0x4000, 60);
    Func_02006678(17, 0);
    Func_02006680(18, 0);
    Func_020060fc();
    Call4(Func_020066b6, -1, -1, -1, 0);
    Func_020066d4(1);
    Func_020065f0();
}

void FieldScene_ConfigureValue93Scene(void)
{
    Func_02006c50(93, 1);
    Func_02006c60(24, 9);
    Func_02006c7c();
    Func_02006c5a(1);
    Func_02006c76();
    Func_02006c82();
}

void Func_02003460(u8 *work)
{
    s32 parent;
    s32 phase;
    s32 next;
    s32 x;
    s32 y;

    parent = *(s32 *)(work + 104);
    phase = *(u16 *)(work + 100);
    x = Func_02006aca(phase);
    *(volatile s32 *)(work + 8) = (*(s32 *)(parent + 8) + (x *(*(s32 *)(work + 48) + 28)));
    y = Func_02006ad8(phase);
    *(volatile s32 *)(work + 16) = ((y << 4) + 0x900000);
    *(volatile s32 *)(work + 56) = *(s32 *)(work + 8);
    *(volatile s32 *)(work + 64) = ((y << 4) + 0x900000);
    next = *(volatile u16 *)(work + 100);
    next = next + -0x200;
    *(volatile u16 *)(work + 100) = (u16)next;
}
