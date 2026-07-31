typedef signed char s8;
typedef signed int s32;

extern s8 Data_02000240[];      /* RAM workspace, also indexed as s16 elsewhere */
extern s32 Data_0200a018[];     /* in-image, file offset 0x2018: 5 topics x 3 ids */

extern s32 Func_020025f8(void);
extern s32 Func_02002610(void);
extern s32 Func_02002608(s32, s32);

s32 Func_02000d70(s32 topic)
{
    s32 cursor;
    s32 variant;

    if (topic < 0) {
        return 0;
    }

    /* Topic 5 means "any": reduce a 16-bit random to 0..4. */
    if (topic == 5) {
        topic = (s32)((unsigned int)(Func_020025f8() * 5) >> 16);
    }

    cursor = Data_02000240[308 + topic];

    /* `lsls #1 / lsrs #16` - a 0/1 coin flip from the same random source. */
    variant = Func_02002608(cursor + (s32)((unsigned int)(Func_02002610() * 2) >> 16) + 4, 3);

    Data_02000240[308 + topic] = (s8)variant;

    return Data_0200a018[topic * 3 + variant];
}
