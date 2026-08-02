typedef signed char s8;
typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Build the 24-entry effect formation used by this scene, present its two
 * panels, then release the effect resources. The owner begins with the saved
 * high-register prologue at 0x02003ad4 and reaches its only return at
 * 0x02003bd4. The alignment halfword and four directly referenced literal
 * words through 0x02003be7 precede the import-veneer bank at 0x02003be8, so
 * the complete owner occupies 276 bytes.
 *
 * Each effect record is 0x48 bytes. Its first word is the object handed to the
 * animation helper; +0x28/+0x2c receive the same randomized height, and the
 * later pass writes mode 2 only when signed byte +5 is nonzero. The callback
 * value is retained as its linked in-image Thumb pointer because this owner
 * installs it as data and never calls it.
 */

extern u8 *Data_03001f30;

extern void Func_080000c0(s32 frames);
extern u32 Func_080000f8(void);
extern void Func_080091c0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_080091c8(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_080091f0(s32 x, s32 y, s32 z);
extern void Func_080091f8(void);
extern void Func_08009248(void *object, s32 animation);
extern void Func_0808a518(u8 *effect, s32 mode);
extern void Func_0808a520(u8 *effect, u32 callback);
extern void Func_0808a528(u8 *effect, s32 kind, s32 x, s32 z);
extern void Func_0808a538(const s32 *position);
extern void Func_0808a550(void);
extern void Func_0808a558(void);
extern void Func_080b0048(u32 resources);
extern void Func_080b0050(void);
extern s32 Func_030003f0(s32 dividend, s32 divisor);

void Func_02003ad4(void)
{
    s32 position[3];
    u8 *effect;
    s32 index;

    Func_0808a550();
    effect = Data_03001f30;
    Func_080b0048(0x00202108);

    position[0] = 252 << 17;
    position[1] = 192 << 13;
    position[2] = 144 << 16;
    Func_0808a538(position);

    effect += 0x58;
    for (index = 0; index < 24; index++, effect += 0x48) {
        u32 random;
        s32 height;

        Func_0808a528(effect, 0x11c, position[0], position[2]);
        Func_0808a520(effect, 0x0200ba31);
        Func_0808a518(effect, 7);

        random = Func_080000f8();
        Func_08009248(*(void **)effect, (s32)(((random << 3) - random) >> 16));

        height = Func_030003f0((s32)Func_080000f8(), 3) + 0x18000;
        *(s32 *)(effect + 0x2c) = height;
        *(s32 *)(effect + 0x28) = height;
        Func_080000c0(1);
    }

    Func_080000c0(80);
    Func_080091c8(41, 55, 3, 2, 30, 55);
    Func_080091c0(42, 8, 1, 1, 31, 8);
    Func_080000c0(50);
    Func_080091f0(-1, -1, 0xe666);
    Func_080000c0(30);

    effect = Data_03001f30 + 0x98;
    for (index = 0; index < 24; index++, effect += 0x48) {
        if (*(s8 *)(effect + 5) != 0) {
            effect[0] = 2;
        }
    }

    Func_080091f8();
    Func_080b0050();
    Func_0808a558();
}
