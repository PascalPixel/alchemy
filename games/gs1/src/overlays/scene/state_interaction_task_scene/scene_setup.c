#include "types.h"

#define SceneState_ConfigureRegionByActorElevenColumn Func_0200005c
#define FieldScene_RunTwoStepSequence Func_020000b0
#define SceneState_ApplyRectsForActors15To17 Func_0200062c
#define FieldScene_RunStep15At29By26 Func_0200069c
#define FieldScene_RunStep15At33By26 Func_020006b0
#define FieldScene_RunStep16At45By26 Func_020006c4
#define FieldScene_RunStep16At49By26 Func_020006d8
#define FieldScene_RunStep17At40By23 Func_020006ec
#define FieldScene_RunStep17At40By25 Func_02000700
#define FieldScene_RunSixSteps896To936 Func_02001fb8
#define FieldScene_BuildDescriptorAndInstallTask Func_020039fc
#define SceneState_InitControlRecordAndStartTask Func_02003ae4
typedef struct Ctl {
    s16 f0;
    s16 f2;
    s16 f4;
    s16 f6;
    s16 f8;
} Ctl;

extern u8 Data_0200c194[];
extern Ctl Data_02001000;
extern u8 *Data_03001f3c;
extern u8 Data_0200b459[];

u8 Func_02003f0e(s32, s32, s32, s32, s32, s32);
u8 Func_02003f2a(s32, s32, s32, s32, s32, s32);
u8 Func_02003f62(s32);
u8 Func_02003f86(s32);
void *Func_02003fb4(s32);
void Func_02003c44(void);
void Func_02000114(void);
void Func_020044ca();
s32 *Func_02004598();
void Func_020044e4();
s32 *Func_020045b2();
void Func_020044fe();
s32 *Func_020045cc();
void Func_0200451a();
void Func_02000b66(s32, s32, s32);
void Func_02000cd6(void);
void Func_02000b7a(s32, s32, s32);
void Func_02000cea(void);
void Func_02000b8e(s32, s32, s32);
void Func_02000cfe(void);
void Func_02000ba2(s32, s32, s32);
void Func_02000d12(void);
void Func_02000bb6(s32, s32, s32);
void Func_02000d26(void);
void Func_02000bca(s32, s32, s32);
void Func_02000d3a(void);
void Func_02005eca(s32 arg0, s32 arg1);
void Func_02005ed4(s32 arg0, s32 arg1);
void Func_02005ede(s32 arg0, s32 arg1);
void Func_02005ee8(s32 arg0, s32 arg1);
void Func_02005ef2(s32 arg0, s32 arg1);
void Func_02005efc(s32 arg0, s32 arg1);
u8 *Func_020077f6();
s32 Func_02007810();
u8 *Func_020079a8();
u8 *Func_020079b0();
s32 Func_02007950();
void Func_02007886();
s32 Func_020078aa();
s32 Func_020078b6();
void Func_02007856();
void Func_020078a4();
void Func_0200b638();
s32 Func_0200791e(void);
void Func_020078ee(s32, s32);
s32 Func_020079e4(s32);
void Func_020078c2(s32, s32);

/* Rect setup for resource_3bb.  Each call site is spelled with its own import
 * name even though several of them reach the same import. */

/* Contiguous unnamed leaf-owner run for resource_3bb. */

/*
 * Scene setup for resource_3bb: allocates a descriptor, stamps its parameter
 * block, binds the in-image data to a graphics handle, and installs the
 * per-frame task.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */

/* This overlay's own routine at file offset 0x3638, installed as a task. */

/* In-image descriptor at file offset 0x4194. */

void SceneState_ConfigureRegionByActorElevenColumn(void)
{
    u8 *work;
    s32 v0;
    s32 v1;

    work = Func_02003fb4(11);
    if ((*(s32 *)(work + 8) >> 20) == 36) {
        Func_02003f62(0x335);
        v0 = 0x23;
        v1 = 0x4D;
        Func_02003f0e(0x23, 0x4E, 1, 1, v0, v1);
    } else {
        Func_02003f86(0x335);
        v0 = 0x23;
        v1 = 0x4D;
        Func_02003f2a(0x22, 0x4D, 1, 1, v0, v1);
    }
}

void FieldScene_RunTwoStepSequence(void)
{
    Func_02003c44();
    Func_02000114();
}

/*
 * One fixed line, then three whose fifth or sixth argument is a field of the
 * record fetched for participants 15, 16 and 17.  Records 15 and 16 contribute
 * their word at +8, record 17 its word at +16, which moves to the sixth
 * argument slot while a literal 18 takes the fifth.  The shift is arithmetic,
 * so the fields are signed fixed-point with 20 fractional bits.  Only those two
 * fields are asserted; what the six arguments mean is not established here.
 */
void SceneState_ApplyRectsForActors15To17(void)
{
    s32 field;

    Func_020044ca(100, 11, 12, 4, 14, 11);

    field = Func_02004598(15)[2] >> 20;
    Func_020044e4(13, 28, 1, 4, field, 11);

    field = Func_020045b2(16)[2] >> 20;
    Func_020044fe(13, 28, 1, 4, field, 11);

    field = Func_020045cc(17)[4] >> 20;
    Func_0200451a(13, 28, 4, 1, 18, field);
}

void FieldScene_RunStep15At29By26(void)
{
    Func_02000b66(15, 29, 26);
    Func_02000cd6();
}

void FieldScene_RunStep15At33By26(void)
{
    Func_02000b7a(15, 33, 26);
    Func_02000cea();
}

void FieldScene_RunStep16At45By26(void)
{
    Func_02000b8e(16, 45, 26);
    Func_02000cfe();
}

void FieldScene_RunStep16At49By26(void)
{
    Func_02000ba2(16, 49, 26);
    Func_02000d12();
}

void FieldScene_RunStep17At40By23(void)
{
    Func_02000bb6(17, 40, 23);
    Func_02000d26();
}

void FieldScene_RunStep17At40By25(void)
{
    Func_02000bca(17, 40, 25);
    Func_02000d3a();
}

/*
 * resource_3bb: six steps, each calling its own near-identical handler stub
 * with a fixed id -- 896 through 936, eight apart -- and a second argument of
 * zero.  The owner at 0x02001fb8 is 66 bytes: its code plus one trailing
 * alignment halfword.  It reads no incoming argument, so it takes none.  The
 * six callees are identified by call shape only.
 */
void FieldScene_RunSixSteps896To936(void)
{
    Func_02005eca(896, 0);
    Func_02005ed4(904, 0);
    Func_02005ede(912, 0);
    Func_02005ee8(920, 0);
    Func_02005ef2(928, 0);
    Func_02005efc(936, 0);
}

/*
 * Seven arguments: four in registers, three from the caller's stack; five are
 * stored as halfwords and two as words, which is how they are typed here. The
 * 232-byte owner includes the seven-word literal pool the body branches over.
 * When the story flag is clear, `second` is mirrored about `centre`. The
 * extent stored at +216 is passed back sign-extended from sixteen bits.
 * Descriptor layout is asserted only for the fields written here.
 */
void FieldScene_BuildDescriptorAndInstallTask(s32 first, s32 second, s32 mode, s32 centre,
                   s32 extra, s32 third, s32 fourth)
{
    u8 *descriptor;
    u8 *first_record;
    u8 *second_record;
    s32 handle;
    s32 extent;

    descriptor = Func_020077f6(59, 0x7170);
    handle = Func_02007810(512);                /* 128 << 2 */

    *(u16 *)(descriptor + 222) = (u16)first;
    *(u16 *)(descriptor + 224) = (u16)second;
    *(u16 *)(descriptor + 226) = (u16)third;
    *(u16 *)(descriptor + 228) = (u16)fourth;
    *(u16 *)(descriptor + 230) = (u16)mode;
    *(s32 *)(descriptor + 232) = centre;
    *(s32 *)(descriptor + 236) = extra;

    first_record = Func_020079a8(first);
    second_record = Func_020079b0(second);

    if (Func_02007950(0x109) == 0) {
        *(s32 *)(second_record + 8) =
            (centre << 1) - *(s32 *)(first_record + 8);
        *(s32 *)(second_record + 16) = *(s32 *)(first_record + 16);
    }

    *(u16 *)(descriptor + 218) = 0;
    *(u16 *)(descriptor + 220) = 0;

    Func_02007886(Data_0200c194, handle);

    extent = Func_020078aa();
    *(u16 *)(descriptor + 216) = (u16)extent;
    Func_020078b6((s16)extent, 512, handle);

    /* The task is published as its entry address plus the Thumb bit. */
    Func_02007856((s32)Func_0200b638 + 1, 0xc76);

    Func_020078a4(handle);
}

void SceneState_InitControlRecordAndStartTask(void)
{
    u8 *state = Data_03001f3c;
    Ctl *m = &Data_02001000;

    Func_020078ee(Func_0200791e(), (s32)(state + 240));
    if (Func_020079e4(0x109) == 0) {
        m->f0 = 1;
        m->f2 = 1;
        m->f4 = *(u16 *)(state + 224);
        m->f8 = 0;
        m->f6 = 0;
    }
    {
        s32 e = 0xc85;

        Func_020078c2((s32)Data_0200b459, e);
    }
}
