typedef unsigned char u8;
typedef signed int s32;

/*
 * resource_3a6 paired-actor placement owner at 0x020017d8, complete 220-byte
 * span through its two-word pool. The current scene actor and the requested
 * actor are moved to the centres of the same signed grid offset. Both receive
 * identical movement limits; the requested actor faces mode 4 for a negative
 * axis and mode 3 otherwise.
 */

extern u8 Data_02000240[];

extern void Func_08009080(u8 *actor, s32 mode);
extern void Func_08009150(u8 *actor, s32 x, s32 y, s32 z);
extern void Func_08009158(u8 *actor);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 actor);

void Func_020017d8(s32 actorId, s32 cellX, s32 cellZ)
{
    u8 *sceneActor = Func_0808a080(*(s32 *)(Data_02000240 + 500));
    u8 *requestedActor = Func_0808a080(actorId);
    s32 x;
    s32 z;

    Func_0808a018();

    x = (*(s32 *)(sceneActor + 8) + (cellX << 16)) & (s32)0xfff00000;
    z = (*(s32 *)(sceneActor + 16) + (cellZ << 16)) & (s32)0xfff00000;
    *(s32 *)(sceneActor + 48) = 0x10000;
    *(s32 *)(sceneActor + 52) = 0x8000;
    Func_08009150(sceneActor, x + 0x80000,
                  *(s32 *)(sceneActor + 12), z + 0x80000);
    Func_08009080(sceneActor, 27);

    x = (*(s32 *)(requestedActor + 8) + (cellX << 16)) & (s32)0xfff00000;
    z = (*(s32 *)(requestedActor + 16) + (cellZ << 16)) & (s32)0xfff00000;
    *(s32 *)(requestedActor + 48) = 0x10000;
    *(s32 *)(requestedActor + 52) = 0x8000;
    Func_08009150(requestedActor, x + 0x80000,
                  *(s32 *)(requestedActor + 12), z + 0x80000);
    if (cellX < 0 || cellZ < 0) {
        Func_08009080(requestedActor, 4);
    } else {
        Func_08009080(requestedActor, 3);
    }

    Func_08009158(sceneActor);
    Func_0808a020();
}
