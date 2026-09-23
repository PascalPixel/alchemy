#include "TYPES.H"
#include "FIELD_EVENT.H"
extern struct GameState Data_02000240;
extern struct EventWork *Data_03001ebc;
extern s32 Data_0200e79c;
void Func_02008598(void);
void Func_0200b678(void);
void Func_0200b9fc(s32);
void Func_0200c184(s32);
void Func_0200c18c(void (*)(void), s32);
void Func_0200c23c(s32,s32,s32,s32);
s32 Func_0200c28c(s32);
void Func_0200c294(s32);
void Func_0200c29c(s32);
s32 Func_0200c2a4(s32);
void Func_0200c2cc(s32);
void Func_0200c2dc(void);
struct FieldActor *Func_0200c2fc(s32);
void Func_0200c35c(s32,s32,s32);
void Func_0200c3ec(s32,s32);
void Func_0200c404(s32,s32);
void Func_0200c40c(s32);
void Func_0200a768(s32);
void Func_0200a4a8(void);
void Func_02008a8c(void);
void Func_02008c28(void);
void Func_020097fc(void);
void Func_02009888(void);
void Func_02009938(void);
void Func_020099e8(void);
void Func_02009a98(void);
void Func_02009b5c(void);
void Func_0200a274(void);
void Func_0200b57c(void);
void Func_0200b130(void);
void Func_0200b2f0(void);
void Func_0200acb4(void);
s32 WorldMap_EnterScene(void)
{
    if (Data_02000240.entrance == 99) {
        Func_0200c294(0x160);
        Func_0200c294(0x161);
        Func_0200c294(0x163);
    }
    if (Data_02000240.entrance == 90) {
        Func_0200b9fc(0);
        Func_0200c404(58, 1);
    } else if (Data_02000240.entrance == 91) {
        Func_0200b9fc(1);
        Func_0200c404(187, 93);
    } else if (Data_02000240.entrance == 78) {
        Func_0200c2dc();
        Func_0200c2cc(242);
        Func_0200c40c(112);
    } else {
        Func_0200c294(0x144);
        Data_03001ebc->start_transition = 0x400;
        Data_03001ebc->transition_frames = 16;
        Func_0200c184(1);
        Func_0200c3ec(0x80000,0x10000);
        Func_0200c29c(0x12f);
        Func_0200c18c(Func_02008598,0xc80);
        if (!Func_0200c28c(0x90a)) Func_0200c23c(128,256,176,56);
        switch (Data_02000240.entrance) {
        case 1:
            if (!Func_0200c28c(0x815)) { Func_0200c294(0x815); Func_0200c294(0x85c); }
            break;
        case 33:
            if (Func_0200c28c(0x109)) {
                if (!Func_0200c28c(0x85d) && Func_0200c28c(564)) {
                    Data_0200e79c=55;
                    Func_0200c35c(55,0x17940000,0x0d480000);
                    Func_0200c2fc(Data_0200e79c)->facing=0x3000;
                    Func_0200a768(Data_0200e79c);
                }
            } else if (!Func_0200c28c(0x85d) && !Func_0200c28c(0x9b8)) Func_0200a4a8();
            break;
        case 49: if (!Func_0200c28c(0x94f) && Func_0200c28c(0x941)) Func_02008a8c(); break;
        case 64: if (!Func_0200c28c(0x85a)) Func_02008c28(); break;
        case 65: Func_020097fc(); break;
        case 66: Func_02009888(); break;
        case 67: Func_02009938(); break;
        case 68: Func_020099e8(); break;
        case 69: Func_02009a98(); break;
        case 70: Func_02009b5c(); break;
        case 71: Func_0200a274(); break;
        case 72: Func_0200b57c(); break;
        case 73: Func_0200b130(); break;
        case 74: case 76: case 77:
            Func_0200c294(284);
            if (Func_0200c2a4(760)) {
                ((u8 *)&Data_02000240)[498]=2;
                Func_0200c18c(Func_0200b678,0xc80);
            }
            break;
        case 75: Func_0200b2f0(); break;
        case 80: Func_0200acb4(); break;
        default:
            Func_0200c2fc(53)->scale_x=0x14000;
            Func_0200c2fc(53)->scale_y=0x14000;
            break;
        }
    }
    return 0;
}
