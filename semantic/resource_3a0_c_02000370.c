typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

struct EffectConfig_02000370 {
    s32 kind;
    s32 variant;
    u8 pad_08[16];
    u16 id;
    u8 pad_1a[2];
    u8 *data;
};

u8 *Func_0808a080();
void Func_0200013c();
extern u32 Data_03001e40;
extern u8 Data_02009740[];

/*
 * Complete conditional effect-spawn owner through return, alignment and all
 * three pool words.  The stack object passed as argument eight is reproduced
 * field-for-field from the stores in the owner.
 */
void Func_02000370(void)
{
    u8 *entity = Func_0808a080(14);

    if ((Data_03001e40 & 3) == 0) {
        struct EffectConfig_02000370 config;
        config.kind = 1;
        config.variant = 9;
        config.id = 169;
        config.data = Data_02009740;
        Func_0200013c(
            *(s32 *)(entity + 8),
            *(s32 *)(entity + 12),
            *(s32 *)(entity + 16) - 0x10000,
            0,
            -0x10000,
            -0x10000,
            0x330000,
            &config);
    }
}
