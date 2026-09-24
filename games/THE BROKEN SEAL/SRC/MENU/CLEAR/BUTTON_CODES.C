#include "TYPES.H"
#include "FIELD_EVENT.H"

extern u8 Value_00000001;
extern volatile u32 Data_03001ae8;
extern s16 Data_020096b2;
extern s16 Data_020096b4;
extern s16 Data_020096b6;
extern s16 Data_020096b8;
extern s16 Data_020096ba;
extern s16 Data_020096bc;
extern u16 Data_020094d4[];
extern u16 Data_020094dc[];

/* Advance one button code: each new press must match the next entry of the
 * sequence, a wrong press starts it over, and the zero entry unlocks it. */
#define CHECK_CODE(unlocked, held, progress, sequence)                       \
    if (unlocked == 0) {                                                     \
        if (held != 0) {                                                     \
            if (Data_03001ae8 == 0)                                          \
                held = 0;                                                    \
        } else if (Data_03001ae8 != 0) {                                     \
            if (Data_03001ae8 == sequence[progress]) {                       \
                one = (u16)(u32)&Value_00000001;                             \
                progress++;                                                  \
                held = one;                                                  \
                if (sequence[progress] == 0) {                               \
                    unlocked = one;                                          \
                    Engine_AudioPlayCue(110);                                \
                }                                                            \
            } else {                                                         \
                progress = 0;                                                \
            }                                                                \
        }                                                                    \
    }

void Clear_CheckButtonCodes(void)
{
    s32 one;

    CHECK_CODE(Data_020096b2, Data_020096ba, Data_020096b6, Data_020094d4)
    CHECK_CODE(Data_020096b4, Data_020096bc, Data_020096b8, Data_020094dc)
}
