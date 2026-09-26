/* NONMATCHING: resource_377:02000a0c; 1064 / 1064 bytes, 135 differing
 * halfwords, 34 wrong instructions, 51 halfword edits. Sharing the explicit
 * alpha port across both ramps removes the reference's second-ramp reload
 * without promoting the pointer from r7 to r5. Topology remains equal. */
#include "FIELD_EVENT.H"

void Main_080000c0();
void Main_08009180(s32 x0, s32 y0, s32 x1, s32 y1, s32 arg4, s32 arg5);
void Main_080091a0();
void Main_080091e0(struct FieldActor *actor, s32 value);
void Main_0808a010();
void Main_0808a018();
struct FieldActor *Main_0808a080(s32 id);
void Main_0808a090();
void Main_0808a098();
void Main_0808a0a0();
void Main_0808a0b0();
void Main_0808a0c8();
void Main_0808a0d0();
void Main_0808a0f0();
void Main_0808a100();
void Main_0808a110();
void Main_0808a130();
void Main_0808a138();
void Main_0808a150();
void Main_0808a170();
void Main_0808a188();
void Main_0808a1b8();
void Main_0808a1e8();
void Main_0808a1f0();
void Main_0808a200();
void Main_0808a208();
void Main_0808a210();
void Main_0808a218();
void Main_0808a248();
void Main_0808a2c8();
void Main_0808a2d8();
void Main_0808a360();
void Main_0808a368();
void Main_0808a370();
void Main_080f9010();

extern u8 Data_03001e70[];
struct RampWork {
    u8 unknown_00[0x1c0];
    s32 blend_config;
    u8 unknown_1c4[4];
    s32 blend_frames;
};

struct RampRoots {
    struct RampWork *scene;
    u8 unknown_04[8];
    struct RampStatus *work;
};

struct RampStatus {
    u8 unknown_00[0x1f84];
    u16 enabled;
};

extern struct RampRoots Data_03001ebc;
extern u8 Data_02009cec[];
extern u8 Data_02009bb4[];
extern u8 Data_02009b78[];
extern u8 Data_02009c04[];
extern u8 Data_02009c54[];
extern u8 Data_02009d38[];
extern u8 Data_02009ca4[];
extern u8 Value_00003f42[];
extern u8 Value_0000100c[];
extern u8 Value_0000100e[];
extern volatile u16 Data_04000052;

static __inline__ void Call1(void (*f)(),s32 a0){f(a0);}
static __inline__ void Call2(void (*f)(),s32 a0,s32 a1){f(a0,a1);}
static __inline__ void Call3(void (*f)(),s32 a0,s32 a1,s32 a2){f(a0,a1,a2);}
static __inline__ void Call4(void (*f)(),s32 a0,s32 a1,s32 a2,s32 a3){f(a0,a1,a2,a3);}
static __inline__ void Call5(void (*f)(),s32 a0,s32 a1,s32 a2,s32 a3,s32 a4){f(a0,a1,a2,a3,a4);}

static __inline__ void SetBlendTarget(u32 value)
{
    *(volatile u16 *)0x04000050 = value;
}

static __inline__ void SetBlendAlpha(u32 value)
{
    Data_04000052 = value;
}

void FieldScene_RunPaletteRampSequence(void)
{
    s32 base;
    struct FieldActor *p1;
    struct FieldSprite *sprite;
    u32 i1;
    volatile u16 *alpha;

    p1 = Main_0808a080(10);
    sprite = p1->sprite;
    Main_0808a018();
    Call3(Main_0808a0f0, 11, 0, 0);
    Call3(Main_0808a0f0, 12, 0, 0);
    Call3(Main_0808a0f0, 13, 0, 0);
    Call3(Main_0808a0f0, 14, 0, 0);
    Call3(Main_0808a0f0, 15, 0, 0);
    Call3(Main_0808a0f0, 16, 0, 0);
    Call3(Main_0808a0f0, 8, 28246016, 25624576);
    Call3(Main_0808a0f0, 10, 30343168, 26476544);
    Main_080091e0(Main_0808a080(10), 0);
    p1->priority_flags &= 0xfe;
    p1->motion_flags = 0;
    sprite->priority = 1;
    Call2(Main_0808a098, 10, (s32)Data_02009cec);
    {
        struct RampWork *scene = Data_03001ebc.scene;

        scene->blend_config = 513;
    }
    Main_08009180(83, 15, 83, 19, 5, 4);
    Main_08009180(90, 16, 90, 20, 5, 4);
    Main_08009180(77, 23, 82, 23, 5, 7);
    Main_08009180(83, 33, 85, 33, 2, 2);
    Main_08009180(91, 28, 90, 28, 1, 1);
    Main_08009180(91, 28, 88, 30, 1, 1);
    Main_08009180(94, 27, 94, 23, 6, 4);
    Main_08009180(92, 28, 87, 23, 4, 4);
    Main_08009180(65, 53, 88, 24, 2, 2);
    Main_080091a0();
    SetBlendTarget((s32)Value_00003f42);
    alpha = &Data_04000052;
    *alpha = (s32)Value_0000100c;
    Main_0808a2c8();
    Data_03001ebc.work->enabled = 1;
    Main_0808a2d8();
    Call1(Main_080000c0, 30);
    Call2(Main_0808a200, 8, 1);
    Call3(Main_0808a090, 8, 98304, 49152);
    Call3(Main_0808a090, 0, 98304, 49152);
    Call3(Main_0808a090, 9, 98304, 49152);
    Call2(Main_0808a098, 0, (s32)Data_02009bb4);
    Call2(Main_0808a098, 8, (s32)Data_02009b78);
    Main_0808a360();
    Call1(Main_0808a0a0, 8);
    Call1(Main_080f9010, 158);
    Call3(Main_0808a1e8, 8, 256, 0);
    Call2(Main_0808a138, 8, 2);
    Call3(Main_0808a1b8, 8, 16384, 10);
    Call2(Main_0808a208, 262144, 32768);
    Call4(Main_0808a210, 27131904, -1, 34734080, 1);
    Call3(Main_0808a0f0, 9, 27131904, 34734080);
    Call3(Main_0808a0d0, 9, 427, 483);
    Main_0808a218();
    Call1(Main_0808a170, 3675);
    Call3(Main_0808a188, 32777, 0, 10);
    Call2(Main_0808a208, 98304, 12288);
    Call4(Main_0808a210, 31457280, -1, 29097984, 1);
    Main_0808a218();
    Call1(Main_0808a010, 20);
    Call3(Main_0808a1b8, 8, 32768, 20);
    Call2(Main_0808a100, 8, 3);
    Call2(Main_0808a110, 0, 3);
    Call2(Main_0808a110, 9, 3);
    Call3(Main_0808a0c8, 9, 415, 589);
    Call1(Main_0808a010, 10);
    Call2(Main_0808a098, 8, (s32)Data_02009c04);
    Call2(Main_0808a098, 0, (s32)Data_02009c54);
    Call1(Main_080f9010, 234);
    Call1(Main_0808a010, 20);
    Call2(Main_0808a098, 10, (s32)Data_02009d38);
    i1 = 0;
ramp:
    /* FAKEMATCH: a word link constant keeps the pool after both ramps. */
    *alpha = (s32)Value_0000100e + i1;
    Call1(Main_080000c0, 1);
    if (++i1 <= 3)
        goto ramp;
    Call1(Main_080f9010, 202);
    Call1(Main_080000c0, 10);
    base = 0x100f;
    {
        u32 cnt;

        for (cnt = 0; cnt <= 15; cnt++) {
            *alpha = base - cnt;
            Call1(Main_080000c0, 1);
        }
    }
    Call1(Main_0808a0a0, 0);
    Call2(Main_0808a100, 8, 1);
    Call2(Main_0808a130, 8, 2);
    Call2(Main_0808a138, 0, 2);
    Call1(Main_0808a010, 10);
    Call3(Main_0808a1b8, 8, 49152, 0);
    Call3(Main_0808a1b8, 0, 49152, 20);
    Call2(Main_0808a1f0, 8, 258);
    Call2(Main_0808a1f0, 0, 258);
    Call1(Main_0808a010, 80);
    Call3(Main_0808a0f0, 9, 0, 0);
    Call3(Main_0808a150, 8, 0, 20);
    Call2(Main_0808a100, 8, 3);
    Call2(Main_0808a110, 0, 3);
    Call1(Main_0808a010, 40);
    Call2(Main_0808a208, 52428, 6553);
    Call2(Main_0808a200, 8, 1);
    Call2(Main_0808a098, 8, (s32)Data_02009ca4);
    Call2(Main_0808a0b0, 0, (s32)Data_02009ca4);
    {
        struct RampWork *scene = Data_03001ebc.scene;

        scene->blend_config = 256;
        scene->blend_frames = 32;
    }
    Main_0808a368();
    Main_0808a370();
    Call1(Main_0808a248, 21);
}
