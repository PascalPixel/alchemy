typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

/*
 * Resource 3bc, owner at 0x02000404.  160 strict bytes: 142 bytes of code
 * (0x0404-0x0491), a 2-byte alignment `nop`, and a four-word literal pool at
 * 0x0494-0x04a3.
 *
 * All nine call sites resolved with tools/overlay_call_targets.ts (all import
 * veneers), and they exactly account for the row's calls=9:
 *   0808a100 x1, 0808a080 x2, 08009150 x2, 0808a0e8 x1,
 *   080091c0 x1, 080000c0 x1, 080770d0 x1.
 *
 * Layout evidence is borrowed rather than rederived: the byte-exact sibling
 * assets/code/resource_3bc_c_02000274.c has the identical
 * "+0x34 = 0x6666, +0x30 = 0xCCCC, then move(obj, obj->x, K, obj->z)"
 * shape twice over, and assets/code/resource_3bc_c_02000658.c proves the
 * 16.16 X at +8 / Z at +0x10 pair and the `*(s32 *)&Data_02000240[250]`
 * scene-id fetch.  The two K constants here are built by shifts:
 * 0x80 << 11 = 0x40000 and 0x80 << 14 = 0x200000; the second is the same
 * 0x200000 the exact sibling passes.
 *
 * r8 is used only as a zero source for the two `strb` stores at +0x55; it is
 * saved and restored around the body, which is why the prologue pushes it
 * separately.  r5/r6 hold the two 16.16 constants across both calls.
 *
 * Epilogue `add sp,#8 / pop {r3} / mov r8,r3 / pop {r5,r6} / pop {r0} /
 * bx r0` — r0 is the popped return address, so the owner is void.
 */

typedef struct Object {
    u8 filler00[8];
    s32 x;              /* 16.16 */
    u8 filler0C[4];
    s32 z;              /* 16.16 */
    u8 filler14[0x1C];
    s32 field30;
    s32 field34;
    u8 filler38[0x1D];
    u8 field55;
} Object;

extern s16 Data_02000240[];

void Func_0808a100();
Object *Func_0808a080();
void Func_08009150();
void Func_0808a0e8();
void Func_080091c0();
void Func_080000c0();
void Func_080770d0();

void Func_02000404(void)
{
    s16 *table;
    Object *object;

    table = Data_02000240;
    Func_0808a100(*(s32 *)&table[250], 1);

    object = Func_0808a080(11);
    object->field55 = 0;
    object->field34 = 0x6666;
    object->field30 = 0xCCCC;
    Func_08009150(object, object->x, 0x40000, object->z);

    object = Func_0808a080(10);
    object->field55 = 0;
    object->field34 = 0x6666;
    object->field30 = 0xCCCC;
    Func_08009150(object, object->x, 0x200000, object->z);

    Func_0808a0e8(10);
    Func_080091c0(0, 25, 1, 1, 9, 12);
    Func_080000c0(2);
    Func_080770d0(0x367);
}
