#include "types.h"

#define StoryScene_SelectPlacementTable Func_0200037c
#define StoryScene_SetBranchValueFromX Func_020004a0
#define StoryScene_SetBranchValueFromZ Func_020004f0
#define StoryScene_ShowRewardDialogue Func_020028a8
#define StoryScene_StartTransition Func_02003130
#define StoryScene_ComputeOpposingSlotDelta Func_0200341c

struct Object {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
};

struct StoryDialogueWork {
    u8 reserved000[370];
    u16 story_result;
};

extern u8 Data_0200db84[];
extern u8 Data_0200e04c[];
extern u8 Data_0200e154[];
extern u8 Data_0200e1e4[];
extern u8 Data_0200e25c[];
extern u8 Data_0200e28c[];
extern u8 Data_0200e2ec[];
extern u8 Data_0200e394[];
extern u8 Data_0200e3c4[];
extern s16 Data_02000240[];
extern struct StoryDialogueWork *Data_03001ebc;

/* Each Func_ name identifies one call site's loader-relocated call word rather
 * than a runtime address, so several names here reach the same import. The
 * declarations are old-style because the interfaces are not established. */
s32 Func_020046a8();
s32 Func_020046b2();
s32 Func_020046c0();
void Func_020046ee();
struct Object *Func_020047a6(u8 *);
struct Object *Func_020047b6(s32);
void Func_02004982(s32);
struct Object *Func_020047f6(u8 *);
struct Object *Func_02004806(s32);
void Func_020049d2(s32);
void Func_02006b88(void);
void Func_02006bbe(void);
void Func_02006d0c(void);
void Func_02006d3a(void);
void Func_02006b1c(s32 dialogue, s32 mode);
s32 Func_02006b4c(s32 flag);
void Func_02007410(void);
void Func_020075de(s32);
void Func_0200756e(s32, s32);
void Func_0200756e_a(s32, s32);
void Func_02007584(s32);
void Func_020072da(s32);
void Func_020075a4(void);
void Func_020075b8(void);
void Func_02007508(s32, s32);
u8 *Func_02007476(s32);
void Func_020073c4(u8 *, s32);
void Func_0200730a(s32);
void Func_0200757e(s32, s32);
void Func_02007328(void *, s32);
void Func_020075d0(s32, s32);
void Func_020075d0_a(s32, s32);
void Func_020075da(s32, s32);
void Func_020075f0(s32);
void Func_02007496(s32);
void Func_020075ee(s32, s32);
void Func_02007604(s32);
void Func_0200735a(s32);
void Func_020075e8(s32);
void Func_02007478(s32);
void Func_020074cc(void);
s32 Func_0200686a(s32 slot);
s32 Func_02006872(s32 slot);
s32 Func_0200687a(s32 slot);
s32 Func_02006882(s32 slot);

/*
 * Selects the placement and spawn table for the current scene. The selector is
 * the signed halfword Data_02000240[225]; only 49 through 80 are covered and
 * everything else takes the default arm, which calls Func_020046ee before
 * returning. Cases 49 and 64 are conditional and fall through to the default
 * when their test fails. The case arms are in the order the reference uses,
 * not ascending, and that order is what reproduces it.
 */
u8 *StoryScene_SelectPlacementTable(void)
{
    s16 *scene_table = Data_02000240;
    s32 scene_id = scene_table[225];

    switch (scene_id) {
    case 49:
        if (Func_020046a8(0x94f) == 0 && Func_020046b2(0x941) != 0) {
            return Data_0200e154;
        }
        break;
    case 64:
        if (Func_020046c0(0x85a) == 0) {
            return Data_0200e04c;
        }
        break;
    case 65:
    case 70:
        return Data_0200e1e4;
    case 71:
        return Data_0200e28c;
    case 72:
        return Data_0200e394;
    case 73:
        return Data_0200e3c4;
    case 66:
    case 67:
    case 68:
    case 69:
    case 75:
        return Data_0200e25c;
    case 80:
        return Data_0200e2ec;
    default:
        break;
    }

    Func_020046ee(0x235);
    return Data_0200db84;
}

/* Publishes one of two branch values at +0x170 of the scene state, chosen by
 * comparing the other actor's x against the subject's. */
void StoryScene_SetBranchValueFromX(
    u8 *actor_object, s32 val_lower,
    s32 val_other)
{
    u8 *scene_state;
    s16 *scene_table;
    struct Object *subject_actor;
    struct Object *other_actor;

    subject_actor = Func_020047a6(actor_object - 0x64);
    scene_table = Data_02000240;
    other_actor = Func_020047b6(*(s32 *)&scene_table[250]);
    scene_state = *(u8 **)0x03001ebc;
    if (other_actor->x < subject_actor->x) {
        *(u16 *)(scene_state + 0x170) = val_lower;
    } else {
        *(u16 *)(scene_state + 0x170) = val_other;
    }
    Func_02004982(0x7B);
}

/* The same branch value, chosen on z instead of x. */
void StoryScene_SetBranchValueFromZ(
    u8 *actor_object, s32 val_lower,
    s32 val_other)
{
    u8 *scene_state;
    s16 *scene_table;
    struct Object *subject_actor;
    struct Object *other_actor;

    subject_actor = Func_020047f6(actor_object - 0x64);
    scene_table = Data_02000240;
    other_actor = Func_02004806(*(s32 *)&scene_table[250]);
    scene_state = *(u8 **)0x03001ebc;
    if (other_actor->z < subject_actor->z) {
        *(u16 *)(scene_state + 0x170) = val_lower;
    } else {
        *(u16 *)(scene_state + 0x170) = val_other;
    }
    Func_020049d2(0x7B);
}

/* Runs dialogue 0x264c and publishes the story result when flag 0x234 is
 * set. */
void StoryScene_ShowRewardDialogue(void)
{
    Func_02006b88();
    Func_02006d0c();
    Func_02006b1c(0x264c, 1);
    if (Func_02006b4c(0x234) != 0) {
        Data_03001ebc->story_result = 1;
    }
    Func_02006d3a();
    Func_02006bbe();
}

/* Stages the scene transition and arms the timed callback that drives it. */
void StoryScene_StartTransition(void)
{
    Func_02007410();
    Func_020075de(141);
    Func_0200756e(0, 0);
    Func_0200756e_a(0, 0);
    Func_02007584(1);
    Func_020072da(2);
    *(s32 *)(*(u8 **)0x03001ebc + 456) = 1;
    Func_020075a4();
    Func_020075b8();
    Func_02007508(0, 15);
    Func_020073c4(Func_02007476(0), 0);
    Func_0200730a(1);
    Func_0200757e(0x40000, 0x8000);
    {
        s32 transition_delay = 3200;
        void *transition_callback = (void *)0x0200b1fd;
        Func_02007328(transition_callback, transition_delay);
    }
    Func_020075d0(0, 0);
    Func_020075d0_a(0x10004, 1);
    Func_020075da(0x10000, 2);
    Func_020075f0(40);
    Func_02007496(240);
    Func_020075ee(0, 0);
    Func_02007604(80);
    Func_0200735a(90);
    Func_020075e8(109);
    Func_02007478(282);
    Func_020074cc();
}

/* Totals slots 0 and 2 against slots 1 and 3 and returns the difference. */
s32 StoryScene_ComputeOpposingSlotDelta(void)
{
    s32 positive_total = Func_0200686a(0);
    s32 negative_total;

    positive_total += Func_02006872(2);
    negative_total = Func_0200687a(1);
    negative_total += Func_02006882(3);
    return positive_total - negative_total;
}
