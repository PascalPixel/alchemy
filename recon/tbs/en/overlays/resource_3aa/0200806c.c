/* Draft of SceneData_SelectTable9ddcByStateWithInit, resource_3aa at 0x0200806c (split from FIELD/KAREI_KYUDEN/PARTY_EVENTS.C).
 * Remaining difference: it loads constants through address-derived symbols (Value_/Data_0000/LinkedMessage_ names) that no link defines, so the overlay keeps its listing rows. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

enum PartyEventsMessage {
    MSG_WHEN_HEARD_WERE_BACK_IVAN = 0x1b21,
    MSG_LORD_HAMMET_WILL_RELEASED_SOON = 0x1b83,
    MSG_HAS_LEGACY_LORD_HAMMETS_SILK = 0x1b88,
    MSG_ROBIN_SNEAKED_INTO_LUNPA_THATS = 0x1b91,
    MSG_ITS_IVAN_HIS_COMPANIONS_PERFECT = 0x2588
};

extern s16 Data_02000240[];
extern u8 Value_00000067;
extern u8 Data_02009c04[];
extern u8 Data_02009bd4[];
extern u8 Data_02009df4[];
extern u8 Data_02009ddc[];
extern u8 Data_02009f38[];
extern u8 Data_02009f2c[];
extern u8 Data_02009b94[];

void Func_02001b1a();
void Func_02001c06();
void Func_02001c6c();
void Func_02001c72();
void Func_02001cc0();
void Func_02001cc6();
u8 *Func_02001d46();
u8 *Func_02001d82();
void Func_02001d58_handoff(s32);
void Func_0200191e();
void Func_02001bbe();
void Func_02001f4e();
void Func_02001ffc();
void Func_02002258();
void Func_020022c2();
void Func_02002410();
void Func_02002646();
void Func_020026ae();
s32 Func_02002e84();
void Func_02002a2e();
void Func_02002d34();
s32 Func_02002ff0();
s32 Func_02003004();
s32 Func_02003018();
s32 Func_020034a2_a();

/* Signed halfword table in RAM; index 225 selects the scene. */

/*
 * Each Func_ symbol names the pre-relocation call word the image holds, not
 * a runtime address; imports are named by the main-image address in the
 * trailing word of the overlay veneer. Old-style declarations are required
 * here, because the arity varies from site to site.
 */

/*
 * Call sites spelled through these wrappers pass their constants straight
 * into the argument registers, while a direct call precomputes a costly
 * constant into a local that later uses in the block share. A call that
 * returns a value sets r0 last of its arguments.
 */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    void Func_02001c00();

    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];
    void Func_02001c00();

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void ConfigureSecond(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureThird(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureFourth(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureFirst(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureUniformSecond(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureUniformThird(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureUniformFourth(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Scene_AdvanceStep(s32 amount)
{
    extern u8 Data_03001ebc[];

    *(u16 *)(*(u8 **)Data_03001ebc + 0x1d8) += amount;
}

s32 SceneData_SelectTable9ddcByStateWithInit(void)
{
    if (gGameState.scene == (s32)&Value_00000067) {
        Func_02001b1a(Data_02009df4);
        return (s32)Data_02009df4;
    }
    return (s32)Data_02009ddc;
}
