typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

/*
 * Resource 371 owner at 0x020006ec (880 bytes) — the overlay's scene entry
 * point.  The image's fixed dispatch table at file offset 0 reaches it: its
 * first entry is `ldr r4,[pc,#0] / bx r4 / .word 0x020086ed`, and under this
 * overlay's proven 0x02008000 link base that word is this address plus the
 * Thumb bit.
 *
 * Complete owner: `push {r5, lr}` at 0x020006ec and `pop {r5} ; pop {r1} ;
 * bx r1` at 0x02000a2e.  The popped register is r1, not r0, so r0 survives
 * and IS the result — the constant 0 set at 0x02000a2c.  The 880-byte span
 * decomposes as head (0x020006ec-0x020007c7), an 80-entry jump table
 * (0x020007c8-0x02000907, data), the case bodies
 * (0x02000908-0x02000a33) and the literal pool
 * (0x02000a34-0x02000a8b).
 *
 * This is the fourth independent 0x02008000 witness in the overlay: the jump
 * table's base is loaded as the pool word 0x020087c8 while the table sits at
 * file offset 0x7c8, and all seventeen distinct entries are 0x8000 above case
 * bodies inside this function.  `mov pc, r3` is the dispatch, not a return.
 * Two more pool words are in-image code addresses with the Thumb bit set:
 * 0x02008599 is `Func_02000598` (the proximity sweep, registered here with
 * period 3200) and 0x0200b679 is `Func_02003678` (`push {r5, r6, lr}` at
 * file offset 0x3678).  Both are written below as the function pointers they
 * are.
 *
 * 0x0200e79c is a 32-bit RAM slot at exactly the first byte past the end of
 * the 0x679c-byte image, so it is scratch adjacent to the loaded overlay
 * rather than image data; it is kept under its raw pool spelling.
 *
 * The dispatch selector is `Data_02000240[225]` — the same scene id the
 * table-selector owner at 0x0200037c reads — biased by 1 and bounded
 * unsigned at 79, so the covered ids are 1..80 and everything else takes the
 * default arm.  The scene id is re-read from memory at the switch, after the
 * head has run its calls; that reload is preserved.
 *
 * UNCERTAINTY: the inventory row reports `calls=18` while 49 distinct `bl`
 * targets are present.  The undercount is a decoding artifact of the
 * 320-byte jump table sitting inside the executable span — it disassembles
 * as plausible `ldrh` pairs — not a missing call.  Every `bl` is placed
 * below.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes, which is a per-call-site label for a load-time-relocated import
 * (see resource_371_c_0200008c.c).  Old-style declarations, because the
 * interfaces are unknown.
 */

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;
extern s32 Data_0200e79c;

/* This overlay's own owners, installed here as timed callbacks. */
void Func_02000598(void);
void Func_02003678(void);

void Func_0200499a();
void Func_020049a0();
void Func_020049a6();
void Func_0200411e();
void Func_02004b2e();
void Func_02004132();
void Func_02004b42();
void Func_02004a24();
void Func_02004a1a();
void Func_02004b60();
void Func_020049f2();
void Func_02004902();
void Func_02004b76();
s32 Func_02004a2c();
void Func_02004926();
void Func_020049ee();
s32 Func_02004b98();
void Func_02004bac();
s32 Func_02004bb2();
s32 Func_02004bbc();
s32 Func_02004bc8();
void Func_02004cac();
u8 *Func_02004c52();
void Func_020030ca();
s32 Func_02004bf6();
s32 Func_02004c00();
void Func_02002e24();
s32 Func_02004c10();
s32 Func_02004c1a();
void Func_02001422();
s32 Func_02004c2a();
void Func_020015ce();
void Func_020021a8();
void Func_0200223a();
void Func_020022f0();
void Func_020023a6();
void Func_0200245c();
void Func_02002526();
void Func_02002c44();
void Func_02003f52();
void Func_02003b0c();
void Func_02004c7a();
s32 Func_02004c92();
void Func_02004b94();
void Func_02003cfe();
void Func_020036c8();
u8 *Func_02004d18();
u8 *Func_02004d24();

s32 Func_020006ec(void)
{
    s16 *scene = &Data_02000240[225];
    u8 *workspace;
    u8 *object;

    if (*scene == 99) {
        Func_0200499a(352);
        Func_020049a0(0x161);
        Func_020049a6(0x163);
    }

    if (*scene == 90) {
        Func_0200411e(0);
        Func_02004b2e(0x3a, 1);
        return 0;
    }
    if (*scene == 91) {
        Func_02004132(1);
        Func_02004b42(0xbb, 93);
        return 0;
    }
    if (*scene == 78) {
        Func_02004a24();
        Func_02004a1a(242);
        Func_02004b60(112);
        return 0;
    }

    Func_020049f2(324);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 0x400;
    *(s32 *)(workspace + 456) = 16;

    Func_02004902(1);
    Func_02004b76(0x80000, 0x10000);
    Func_02004a2c(0x12f);
    Func_02004926(Func_02000598, 3200);
    if (Func_02004a2c(0x90a) == 0) {
        Func_020049ee(128, 256, 176, 56);
    }

    switch (*scene) {
    case 1:
        if (Func_02004b98(0x815) != 0) {
            break;
        }
        Func_02004bac(0x815);
        Func_02004bb2(0x85c);
        break;

    case 33:
        if (Func_02004bb2(0x109) != 0) {
            if (Func_02004bbc(0x85d) != 0) {
                break;
            }
            if (Func_02004bc8(564) == 0) {
                break;
            }
            Data_0200e79c = 55;
            Func_02004cac(55, 0x17940000, 0x0d480000);
            object = Func_02004c52(Data_0200e79c);
            *(u16 *)(object + 6) = 0x3000;
            Func_020030ca(Data_0200e79c);
            break;
        }
        if (Func_02004bf6(0x85d) != 0) {
            break;
        }
        if (Func_02004c00(0x9b8) != 0) {
            break;
        }
        Func_02002e24();
        break;

    case 49:
        if (Func_02004c10(0x94f) != 0) {
            break;
        }
        if (Func_02004c1a(0x941) == 0) {
            break;
        }
        Func_02001422();
        break;

    case 64:
        if (Func_02004c2a(0x85a) != 0) {
            break;
        }
        Func_020015ce();
        break;

    case 65: Func_020021a8(); break;
    case 66: Func_0200223a(); break;
    case 67: Func_020022f0(); break;
    case 68: Func_020023a6(); break;
    case 69: Func_0200245c(); break;
    case 70: Func_02002526(); break;
    case 71: Func_02002c44(); break;
    case 72: Func_02003f52(); break;
    case 73: Func_02003b0c(); break;

    case 74:
    case 76:
    case 77:
        Func_02004c7a(284);
        if (Func_02004c92(760) == 0) {
            break;
        }
        ((u8 *)Data_02000240)[498] = 2;
        Func_02004b94(Func_02003678, 3200);
        break;

    case 75: Func_02003cfe(); break;
    case 80: Func_020036c8(); break;

    default:
        /* Ids 2..32, 34..48, 50..63, 78, 79 and anything outside 1..80. */
        *(s32 *)(Func_02004d18(53) + 0x18) = 0x14000;
        *(s32 *)(Func_02004d24(53) + 0x1c) = 0x14000;
        break;
    }

    return 0;
}
