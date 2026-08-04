typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_3be owner at 0x020008ec, 696 bytes. It configures one selected
 * actor, derives and publishes a tile rectangle in local and global
 * coordinates, optionally invokes the caller's callback, and finalizes the
 * actor placement. The interworking return ends at 0x02000b84; alignment and
 * seven pool words extend the owner through 0x02000ba3. All 31 calls remain in
 * machine order, including the explicit indirect callback.
 */
extern u8 *Data_03001e70;
extern s32 Func_02000528(s32, s32, s32, s32, s32, s32);
extern void Func_08009080(u8 *object, s32 mode);
extern void Func_08009150(u8 *object, s32 x, s32 y, s32 z);
extern void Func_08009158(u8 *object);
extern void Func_080091c0(s32, s32, s32, s32, s32, s32);
extern u8 *Func_0808a010(s32 index);
extern u8 *Func_0808a080(s32 index);
extern void Func_0808a090(s32, s32, s32);
extern void Func_0808a0e0(s32 index, s32 x, s32 z);
extern void Func_0808a0e8(s32 index);
extern void Func_0808a100(s32 index, s32 value);
extern void Func_0808a5e8(void);
extern void Func_080f9010(s32 value);

static s32 Abs_020008ec(s32 value)
{
    return value < 0 ? -value : value;
}

static s32 Fixed17TowardZero_020008ec(s32 value)
{
    if (value < 0)
        value += 0x1ffff;
    return value >> 17;
}

void Func_020008ec(s32 deltaIndex, s32 objectIndex, s32 x, s32 y, s32 z,
                   void (*callback)(void))
{
    const s32 *const deltaTable = (const s32 *)0x02009718;
    const s32 *const referenceDeltas = (const s32 *)0x020096c0;
    u8 *runtime = Data_03001e70;
    u8 *reference = Func_0808a080(0);
    s32 referenceBucket = *(unsigned short *)(reference + 6) >> 12;
    u8 *object = Func_0808a080(objectIndex);
    const s32 *delta = &deltaTable[deltaIndex * 4];
    s32 depth = (Abs_020008ec(delta[1]) + Abs_020008ec(delta[3])) >> 4;
    s32 width = (Abs_020008ec(delta[0]) + Abs_020008ec(delta[2])) >> 4;
    s32 originalX = *(s32 *)(object + 8);
    s32 originalZ = *(s32 *)(object + 16);
    s32 tileX = (originalX + delta[0] * 0x10000) >> 20;
    s32 tileZ = (originalZ + delta[1] * 0x10000) >> 20;
    s32 packedStep;
    s32 halfStepX;
    s32 halfStepZ;
    s32 globalX;
    s32 globalZ;
    u8 *current;

    *(s32 *)(object + 48) = 0x8000;
    *(s32 *)(object + 52) = 0x1999;

    Func_02000528(0, tileX, tileZ, width, depth, 0);
    Func_0808a090(0, 0x8000, 0x1999);
    Func_0808a100(0, 8);
    Func_0808a010(15);
    Func_0808a0e0(0, Fixed17TowardZero_020008ec(x - originalX),
                  Fixed17TowardZero_020008ec(z - originalZ));

    current = Func_0808a080(0);
    *(s32 *)(current + 108) = 0x0200858d;
    Func_0808a010(4);

    if ((unsigned)(referenceBucket - 6) <= 7)
        Func_08009080(object, 3);
    else
        Func_08009080(object, 2);

    Func_080f9010(239);
    Func_08009150(object, x, y, z);
    Func_0808a0e8(0);
    Func_0808a100(0, 2);
    Func_0808a090(0, 0x4ccc, 0x1999);

    packedStep = referenceDeltas[referenceBucket];
    halfStepX = (s16)(packedStep >> 16) / 2;
    halfStepZ = (s16)packedStep / 2;
    Func_0808a0e0(0, halfStepX, halfStepZ);

    if (callback != 0)
        callback();

    Func_0808a0e8(0);
    Func_0808a100(0, 1);
    current = Func_0808a080(0);
    *(s32 *)(current + 108) = 0;
    Func_08009158(object);
    Func_080f9010(0x120);
    Func_080f9010(0xd5);

    *(s32 *)(object + 8) = x;
    *(s32 *)(object + 16) = z;
    *(s32 *)(object + 36) = 0;
    *(s32 *)(object + 44) = 0;
    Func_08009080(object, 1);

    tileX = (x + delta[0] * 0x10000) >> 20;
    tileZ = (z + delta[1] * 0x10000) >> 20;
    globalX = (*(s32 *)(runtime + 316) >> 20) + tileX;
    globalZ = (*(s32 *)(runtime + 320) >> 20) + tileZ;
    Func_080091c0(tileX, tileZ, width, depth, globalX, globalZ);
    Func_02000528(0, tileX, tileZ, width, depth, 255);
    Func_02000528(2, tileX, tileZ, width, depth, 255);

    tileX = (originalX + delta[0] * 0x10000) >> 20;
    tileZ = (originalZ + delta[1] * 0x10000) >> 20;
    globalX = (*(s32 *)(runtime + 316) >> 20) + tileX;
    globalZ = (*(s32 *)(runtime + 320) >> 20) + tileZ;
    Func_080091c0(globalX, globalZ, width, depth, tileX, tileZ);
    Func_02000528(2, tileX, tileZ, width, depth, 0);
    Func_0808a5e8();
}
