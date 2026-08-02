typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

extern u8 *Data_03001e70;
extern u8 *Func_0808a080(s32 actorId);
extern void Func_080091c0(s32, s32, s32, s32, s32, s32);
extern s32 Func_02000528(u32, s32, s32, u32, u32, s32);

static s32 Abs_02000ba4(s32 value)
{
    return value < 0 ? -value : value;
}

/* Map the actor's signed key to one of six displacement records, publish the
 * occupied rectangle, and seed both indexed placement layers. */
s32 Func_02000ba4(s32 actorId)
{
    const s32 *const keys = (const s32 *)0x02009e88;
    const s32 *const deltas = (const s32 *)0x02009ea0;
    u8 *object = Func_0808a080(actorId);
    s32 key = *(short *)(*(u8 **)(object + 0x50) + 0x28);
    s32 index;
    s32 baseX;
    s32 baseZ;
    s32 positionX;
    s32 positionZ;
    s32 extentX;
    s32 extentZ;
    s32 globalX;
    s32 globalZ;

    for (index = 0; index <= 5; index++) {
        if (keys[index] == key)
            break;
    }
    if (index > 5)
        return 0;

    baseX = *(s32 *)(object + 8);
    baseZ = *(s32 *)(object + 16);
    positionX = (baseX + (deltas[index * 4] << 16)) >> 20;
    positionZ = (baseZ + (deltas[index * 4 + 1] << 16)) >> 20;
    extentX = (Abs_02000ba4(deltas[index * 4]) +
               Abs_02000ba4(deltas[index * 4 + 2])) >> 4;
    extentZ = (Abs_02000ba4(deltas[index * 4 + 1]) +
               Abs_02000ba4(deltas[index * 4 + 3])) >> 4;

    globalX = (*(s32 *)(Data_03001e70 + 0x13c) >> 20) + positionX;
    globalZ = (*(s32 *)(Data_03001e70 + 0x140) >> 20) + positionZ;

    Func_080091c0(positionX, positionZ, extentX, extentZ, globalX, globalZ);
    Func_02000528(0, positionX, positionZ, (u32)extentX, (u32)extentZ, 255);
    Func_02000528(2, positionX, positionZ, (u32)extentX, (u32)extentZ, 255);
    return 1;
}
