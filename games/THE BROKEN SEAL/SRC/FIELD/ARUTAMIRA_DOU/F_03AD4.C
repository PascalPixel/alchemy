#include "TYPES.H"
#include "FIELD_EVENT.H"

void Main_0808a550(void);
void Main_0808a558(void);
void Main_080b0048(s32 mode);
void Main_080b0050(void);
void Main_0808a538(s32 *pos);
void Main_0808a528(u8 *object, s32 type, s32 x, s32 z);
void Main_0808a520(u8 *object, void (*update)());
void Main_0808a518(u8 *object, s32 mode);
void Main_08009248(s32 handle, s32 frame);
s32 Main_030003f0(s32 value, s32 divisor);
void Main_080091c8(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void OverlayObject_UpdateThreeStateMotion();

/* The overlay object records (72 bytes each), from +88. */
extern u8 *Data_03001f30;

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Releases 24 objects (type 284) one per frame from a point by the cave
 * wall, each with a random frame and speed, opens the wall's cells, then
 * marks every object still active once the screen has faded. */
void ArutamiraDou_Func02003ad4(void)
{
    u8 *work;
    s32 pos[3];
    s32 *p;
    u8 *object;
    s32 n;

    Main_0808a550();
    work = Data_03001f30;
    Main_080b0048(0x202108);
    p = pos;
    p[0] = 0x1f80000;
    p[1] = 0x180000;
    p[2] = 0x900000;
    Main_0808a538(p);
    object = work + 88;
    for (n = 23; n >= 0; n--) {
        s32 speed;

        Main_0808a528(object, 284, p[0], p[2]);
        Main_0808a520(object, OverlayObject_UpdateThreeStateMotion);
        Main_0808a518(object, 7);
        Main_08009248(*(s32 *)object, (u32)(Engine_RandomNext() * 7) >> 16);
        speed = Main_030003f0(Engine_RandomNext(), 3) + 0x18000;
        *(s32 *)(object + 44) = speed;
        *(s32 *)(object + 40) = speed;
        Engine_TaskWait(1);
        object += 72;
    }
    Engine_TaskWait(80);
    Call6((void (*)())Main_080091c8, 41, 55, 3, 2, 30, 55);
    Call6((void (*)())Engine_MapCopyCellAttributes, 42, 8, 1, 1, 31, 8);
    Engine_TaskWait(50);
    Call3((void (*)())Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_TaskWait(30);
    {
        s32 mode = 2;
        u8 *state = work + 152;

        for (n = 23; n >= 0; n--) {
            if (((s8 *)state)[5] != 0) {
                *state = mode;
            }
            state += 72;
        }
    }
    Engine_MapWaitWorkValuesBelow256();
    Main_080b0050();
    Main_0808a558();
}
