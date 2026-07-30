/*
 * resource_3c4 @ 0x020019cc (22 bytes).
 *
 * Func_02004ac4 is a resident service outside the overlay image; the value it
 * leaves in r0 is the only possible source for the object pointer that
 * Func_02001a98 consumes in r0, so it is threaded through explicitly.  The
 * remaining two calls set up no arguments.  `pop {r0} ; bx r0` return: void.
 */
typedef signed int s32;

struct Object_02001a98;

struct Object_02001a98 *Func_02004ac4(void);
s32 Func_02001a98(struct Object_02001a98 *object);
void Func_02004ad4(void);
void Func_02003398(void);

void Func_020019cc(void)
{
    Func_02001a98(Func_02004ac4());
    Func_02004ad4();
    Func_02003398();
}
