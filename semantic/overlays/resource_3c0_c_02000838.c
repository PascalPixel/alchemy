typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 actorId);
extern void Func_0808a090(s32 actorId, s32 width, s32 height);
extern void Func_0808a0b8(s32 actorId, s32 x, s32 y);
extern void Func_0808a0d0(s32 actorId, s32 x, s32 y);
extern void Func_0808a0e8(s32 actorId);
extern void Func_0808a0f0(s32 actorId, s32 x, s32 y);
extern void Func_0808a100(s32 actorId, s32 direction);
extern void Func_0808a110(s32 actorId, s32 direction);
extern void Func_0808a138(s32 actorId, s32 mode);
extern void Func_0808a148(s32 actorId, s32 distance, s32 frames);
extern void Func_0808a170(s32 messageId);
extern void Func_0808a180(s32 actorId, s32 mode);
extern void Func_0808a190(s32 actorId, s32 mode);
extern void Func_0808a1b8(s32 actorId, s32 distance, s32 frames);
extern void Func_0808a1e8(s32 actorId, s32 animationId, s32 frames);

/* Play the gated actor-13 encounter, then return both actors to the ordinary
 * map presentation once the dialogue and movement sequence completes. */
void Func_02000838(void)
{
    u8 *leader;

    if (Func_080770c0(0x9a0) == 0)
        return;
    if (Func_080770c0(0x1b7) != 0)
        return;
    if (Func_080770c0(0x9b0) == 0)
        return;

    Func_080770c8(0x9b5);
    Func_0808a018();
    Func_0808a170(0x2633);
    leader = Func_0808a080(0);
    if (leader != 0)
        Func_0808a0f0(13, *(s32 *)(leader + 8), *(s32 *)(leader + 16));

    Func_0808a148(13, 0xc000, 0);
    Func_0808a0d0(0, 440, 1256);
    Func_0808a1b8(13, 0x4000, 0);
    Func_0808a0d0(0, 444, 1240);
    Func_0808a1e8(0, 0x100, 40);
    Func_0808a1b8(0, 0x4000, 30);
    Func_0808a110(13, 4);
    Func_0808a180(13, 0);
    Func_0808a1e8(0, 0x105, 60);
    Func_0808a1e8(13, 0x105, 60);
    Func_0808a180(13, 0);
    Func_0808a010(30);
    Func_0808a138(13, 2);
    Func_0808a180(13, 0);
    Func_0808a1b8(13, 0xc000, 30);
    Func_0808a190(13, 0);
    Func_0808a010(30);
    Func_0808a1e8(13, 0x106, 60);
    Func_0808a180(13, 0);
    Func_0808a110(13, 3);
    Func_0808a180(13, 0);
    Func_0808a090(13, 0xb333, 0x5999);
    Func_0808a0d0(13, 440, 1256);
    Func_0808a180(13, 0);
    Func_0808a110(0, 3);
    Func_0808a100(13, 2);

    leader = Func_0808a080(0);
    if (leader != 0)
        Func_0808a0b8(13, *(s16 *)(leader + 10), *(s16 *)(leader + 18));
    Func_0808a0e8(13);
    Func_0808a0f0(13, 0, 0);
    Func_0808a020();
}
