typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * Resource 378 owner at 0x02000574 (88 bytes, through 0x020005cb).
 *
 * Eight imported calls and one conditional branch are present.  The two
 * literal words at 0x020005c4--0x020005cb are part of this owner; the next
 * entry-driver prologue begins at 0x020005cc.  The sole state write is the
 * same workspace +472 halfword increment used by the preceding owner.
 */

extern u8 *Data_03001ebc;

extern s32 Func_0808a070(s32 id, s32 arg1);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a148(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a170(s32 value);
extern void Func_0808a178(s32 id, s32 arg1);
extern void Func_0808a180(s32 id, s32 arg1);

void Func_02000574(void)
{
    Func_0808a018();
    Func_0808a148(10, 0, 0);
    Func_0808a010(10);
    Func_0808a170(0x119f);
    Func_0808a178(10, 0);

    if (Func_0808a070(0, 0) == 1) {
        (*(u16 *)(Data_03001ebc + 472))++;
    }

    Func_0808a180(10, 0);
    Func_0808a020();
}
