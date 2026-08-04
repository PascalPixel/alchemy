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

extern void Func_02003b68(void);
extern void Func_02003c12(s32 id, s32 arg1, s32 arg2);
extern void Func_02003b70(s32 frames);
extern void Func_02003c36(s32 value);
extern void Func_02003c46(s32 id, s32 arg1);
extern s32 Func_02003ba6(s32 id, s32 arg1);
extern void Func_02003c72(s32 id, s32 arg1);
extern void Func_02003bb6(void);

void Func_02000574(void)
{
    Func_02003b68();
    Func_02003c12(10, 0, 0);
    Func_02003b70(10);
    Func_02003c36(0x119f);
    Func_02003c46(10, 0);

    if (Func_02003ba6(0, 0) == 1) {
        (*(u16 *)(Data_03001ebc + 472))++;
    }

    Func_02003c72(10, 0);
    Func_02003bb6();
}
