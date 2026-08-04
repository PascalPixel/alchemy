typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

/* Complete countdown/reroll owner and its two pool words. */
extern s32 **Data_03001edc;
extern u8 *Data_03001ebc;
extern u32 Data_02000434;
extern void Func_02001bc8(u8 *);
extern u32 Func_080000f8(void);

void Func_02002370(void)
{
    s32 *record = *Data_03001edc;
    u8 *slot = ((u8 **)(Data_03001ebc + 0x14))[Data_02000434];

    if (record[2] != 0)
        record[2]--;
    else {
        Func_02001bc8(slot);
        record[2] = (s32)((Func_080000f8() * 30) >> 16) + 10;
    }
}
