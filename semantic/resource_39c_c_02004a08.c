typedef unsigned char u8;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Run the actor-3 scene only when actor 8 occupies coarse column 48. The
 * dialogue gate contains one optional exchange followed by a repeatable
 * response loop; both machine branches rejoin before the actor movement and
 * flag tail.
 *
 * The owner starts at push {r5,lr} at 0x02004a08, returns once at
 * 0x02004c46, and owns the eight-word literal pool through 0x02004c67. The
 * next saved-register prologue begins at 0x02004c68, fixing the whole-owner
 * span at 608 bytes.
 */

extern void Func_08077010(s32 actor);
extern void Func_080770c8(s32 flag);
extern void Func_080771a8(s32 actor, s32 value, s32 mode);
extern void Func_080771b0(s32 actor, s32 value, s32 mode);
extern void Func_080091c0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a048(s32 actor, s32 value);
extern s32 Func_0808a070(s32 actor, s32 mode);
extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a090(s32 actor, s32 arg1, s32 arg2);
extern void Func_0808a0b8(s32 actor, s32 x, s32 z);
extern void Func_0808a0d0(s32 actor, s32 x, s32 z);
extern void Func_0808a0e8(s32 actor);
extern void Func_0808a0f0(s32 actor, s32 x, s32 z);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_0808a110(s32 actor, s32 mode);
extern void Func_0808a138(s32 actor, s32 mode);
extern void Func_0808a148(s32 actor, s32 target, s32 frames);
extern void Func_0808a170(s32 message);
extern void Func_0808a178(s32 actor, s32 mode);
extern void Func_0808a188(s32 actor, s32 target, s32 frames);
extern void Func_0808a1b8(s32 actor, s32 speed, s32 frames);
extern void Func_0808a1e8(s32 actor, s32 value, s32 frames);

void Func_02004a08(void)
{
    u8 *actor;
    s32 position;
    s32 response;

    actor = Func_0808a080(8);
    position = *(s32 *)(actor + 8);
    if (position < 0) {
        position = (s32)((u32)position + 0x000fffff);
    }
    if ((position >> 20) != 48) {
        return;
    }

    Func_0808a018();
    Func_0808a170(0x1591);
    Func_0808a010(20);
    Func_0808a138(3, 1);
    Func_0808a1b8(0, 0x8000, 20);
    Func_0808a188(3, 0, 20);
    Func_0808a110(3, 3);
    Func_0808a010(20);
    Func_0808a110(0, 3);
    Func_0808a010(20);
    Func_0808a010(60);
    Func_0808a100(3, 16);
    Func_0808a010(50);
    Func_0808a100(3, 1);
    Func_0808a178(3, 0);

    if (Func_0808a070(0, 0) == 1) {
        Func_0808a010(20);
        Func_0808a138(3, 2);
        Func_0808a010(20);
        Func_0808a188(3, 0, 20);
        Func_0808a110(3, 4);
        Func_0808a010(20);
        Func_0808a188(3, 0, 20);
        Func_0808a110(3, 3);
        Func_0808a010(20);
        Func_0808a178(3, 0);

        if (Func_0808a070(0, 0) == 1) {
            Func_0808a010(20);
            Func_0808a110(3, 4);
            Func_0808a010(20);
            response = 0x1596;
            for (;;) {
                if (response == 0x1639) {
                    Func_0808a010(20);
                    Func_0808a110(3, 4);
                    Func_0808a010(20);
                }
                Func_0808a170(response);
                Func_0808a178(3, 0);
                if (Func_0808a070(0, 0) != 1) {
                    break;
                }
                response = 0x1639;
            }
        }
    }

    Func_0808a170(0x1597);
    Func_0808a090(3, 0xcccc, 0x6666);
    Func_0808a0d0(3, 182 << 2, 158 << 2);
    Func_0808a010(20);
    Func_0808a188(3, 0, 20);
    Func_0808a100(3, 16);
    Func_0808a188(3, 0, 20);
    Func_0808a100(3, 1);
    Func_0808a148(3, 0, 20);
    Func_0808a110(3, 4);
    Func_0808a010(20);
    Func_0808a188(3, 0, 20);
    Func_0808a1e8(3, 0x105, 90);
    Func_0808a110(3, 3);
    Func_0808a010(20);
    Func_0808a188(3, 0, 20);
    Func_0808a048(3, 1);
    Func_080770c8(68);
    Func_080771a8(3, 1, 0);
    Func_080771b0(3, 1, 0);
    Func_08077010(3);
    Func_0808a100(3, 2);

    actor = Func_0808a080(0);
    if (actor != (void *)0) {
        Func_0808a0b8(3, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
    }
    Func_0808a0e8(3);
    Func_0808a0f0(3, 0, 0);
    Func_080091c0(110, 39, 5, 1, 46, 39);
    Func_080770c8(0x873);
    Func_0808a020();
}
