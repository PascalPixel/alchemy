typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_3b2 owner at 0x020012b4, 192 bytes: code 0x020012b4-0x02001365,
 * a two-byte alignment `movs r0, r0` at 0x02001366, and the three-word literal
 * pool 0x02001368-0x02001373.
 *
 * Callee names are the RAW per-site names of HANDOVER §5b3a
 * (`name = insn_address + 2 + true_target_offset`), read straight off
 * `overlay_show resource_3b2 0x12b4`.  They are arithmetic operands, not
 * labels: two sites calling the same routine carry different names, and the
 * veneer-resolved names the semantic draft used do not emit these bytes.
 */

extern s16 Data_02000240[];             /* the overlay's scene table */
extern u8 Value_0000007e;
extern u8 Value_000008d2;

u8 *Func_0200434c();
u8 *Func_02004344();
void Func_02004368();
void Func_02002106();
void Func_02004424();
void Func_0200437a();
u8 *Func_020043a0();
void Func_02004356();
void Func_020043f6();
void Func_02004370();
void Func_020043b4();

void Func_020012b4(void)
{
    u8 *entity;
    s16 *slot;
    s32 column;

    entity = Func_0200434c(8);
    column = *(s32 *)(entity + 8) >> 20;    /* 16.16 -> 16-pixel tile grid */
    if (column != 40) {
        return;
    }

    {
        s32 off = 448;
        slot = (s16 *) ((u8 *) Data_02000240 + off);
    }
    if (Func_02004344(*slot + ((s32) &Value_000008d2 - (s32) &Value_0000007e)) != 0) {
        return;                             /* handled by 0x02001214 instead */
    }

    entity[85] = 3;

    Func_02004368(8);
    Func_02002106(8);
    Func_02004424(136);
    Func_0200437a(40);

    Func_02004356(Func_020043a0(8), 0);
    Func_020043f6(8, 3);

    entity[85] = 0;
    entity[35] = (u8)(entity[35] | 2);

    Func_02004370(42, 10, 1, 1, column, 10);

    Func_020043b4(*slot + ((s32) &Value_000008d2 - (s32) &Value_0000007e));
}
