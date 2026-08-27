#include "types.h"

extern s16 Data_02001078[];
extern u8 Data_02000240[];

void *Func_08077394(s32);
void *Func_08078414(s32);
void Func_08079ae8(s32);
void Func_08077428(s32);
void Func_0807a628(s32, s32);
void Func_08079358(s32);
void Func_0807808c(s32);

s32 Func_0807a664(void)
{
    s16 flag = Data_02001078[0];
    s32 owner;
    s16 *dst = &Data_02001078[1];
    s32 savedA, savedB;

    if (flag == 0x6774) {
        savedA = *(s16 *)(Data_02000240 + 0x220);
        savedB = *(s16 *)(Data_02000240 + 0x222);
        Data_02001078[0] = 0x6774;

        for (owner = 0; owner <= 3; owner++) {
            u8 *inv = Func_08077394(owner);
            u16 *src = (u16 *)(inv + 216);
            s32 i;
            s32 count = 0;
            u16 *cur;

            for (i = 0; i < 14; i++) {
                *dst = *src;
                src++;
                dst++;
            }

            cur = (u16 *)(inv + 216);
            for (i = 0; i < 14; i++) {
                void *item = Func_08078414(*cur);
                if (*((u8 *)item + 2) != 6) {
                    *cur = 0;
                }
                cur++;
            }

            {
                u16 *rd = (u16 *)(inv + 216);
                u16 *wr = (u16 *)(inv + 216);
                s32 j;

                for (j = 0; j < 14; j++) {
                    u16 v = *rd++;
                    if (v != 0) {
                        *wr++ = v;
                        count++;
                    }
                }

                if (count <= 14) {
                    s32 n = 15 - count;
                    do {
                        n--;
                        *wr++ = 0;
                    } while (n != 0);
                }
            }

            Func_08079ae8(owner);
            Func_08077428(owner);
        }

        *dst++ = (u16)savedA;
        *dst++ = (u16)savedB;
        *dst++ = *(u16 *)(Data_02000240 + 0);
        *dst++ = *(u16 *)(Data_02000240 + 2);

        Func_0807a628(0, 16);
        Func_08079358(0x02000438);
    }

    Func_0807808c(1);
    return 0;
}
