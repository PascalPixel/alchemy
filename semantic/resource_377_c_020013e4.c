typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * resource_377 owner at 0x020013e4, 352 bytes.  Complete owner: the prologue
 * 'push {r5, r6, lr} ; mov r6,r8 ; push {r6}' and the matching interworking
 * return 'pop {r3} ; mov r8,r3 ; pop {r5, r6} ; pop {r0} ; bx r0'.  r0 holds the
 * popped return address, so the owner returns nothing; the first call is the
 * argument-less scene bracket Func_0808a018, so it takes no argument either.
 *
 * Straight-line scene setup: reset the camera, place and aim it, create the
 * scene object, load and bind its palette, publish scene id 514 into the shared
 * workspace, run the camera moves, play the object's two animation scripts, and
 * close.  There is no conditional control flow at all.
 *
 * Literal pool: owner offsets 0x152..0x15f (0x02001538, 0x0200153c, 0x02001540),
 * preceded by an alignment 'movs r0,r0' at 0x02001536.  The control-flow walk
 * from the prologue ends at the epilogue (0x0200152c), so the whole tail is
 * pool.  0x03001ebc is the family workspace pointer cell; 0x02009eac and
 * 0x02009ecc are EVEN, so under this family's proven 0x02008000 link base they
 * are in-image data at file offsets 0x1eac and 0x1ecc -- animation scripts, and
 * indeed each is the second argument of Func_08009098, which is where this
 * family's even in-image words appear.
 *
 * The scene-id store is the one-register-two-roles shape: 'movs r2,#224 ;
 * lsls r2,r2,#1' makes 448, used as the displacement, then 'adds r2,#66' makes
 * 514, the stored value.  514 (0x202) at workspace + 448 is the same slot and
 * the same kind of value assets/code/resource_380_c_02000390.c writes.
 *
 * All 29 calls resolved with
 * 'bun tools/overlay_call_targets.ts resource_377 13e4 --json' (29 sites,
 * 22 distinct veneer targets).  The per-target multiset below matches the tool's
 * histogram exactly: Func_0808a0d0 x3; Func_080000c0, Func_0808a0f0,
 * Func_08009098, Func_08009148, Func_0808a1b8 x2 each; the other seventeen once.
 * In order:
 *   0x13ea -> Func_0808a018()
 *   0x13fc -> Func_0808a210(-1, -1, -1, 0)
 *   0x1402 -> Func_080000c0(1)
 *   0x140c -> Func_0808a0f0(0, 0, 0)
 *   0x141a -> Func_0808a0f0(18, 0x1e00000, 0xca0000)
 *   0x1420 -> Func_080000c0(1)
 *   0x1428 -> Func_0808a200(18, 1)
 *   0x143a -> Func_080090c8(22, 0x1480000, 0x20000, 0xc30000)   -> object
 *   0x1470 -> Func_08000140(17, 0x608)
 *   0x1478 -> Func_08015250(224)                                -> palette base
 *   0x1488 -> Func_080001c8(sprite->index, 128, palette + 0x400)
 *   0x148e -> Func_08000150(17)
 *   0x14a0 -> Func_0808a360()
 *   0x14ae -> Func_0808a090(18, 0x10000, 0x8000)
 *   0x14ba -> Func_0808a0d0(18, 480, 176)
 *   0x14c6 -> Func_0808a0d0(18, 420, 164)
 *   0x14d2 -> Func_0808a0d0(18, 326, 185)
 *   0x14de -> Func_0808a1b8(18, 0x4000, 10)
 *   0x14e6 -> Func_08009098(object, 0x02009eac)
 *   0x14ec -> Func_08009148(object)
 *   0x14f4 -> Func_08009098(object, 0x02009ecc)
 *   0x14fa -> Func_08009148(object)
 *   0x1500 -> Func_0808a010(20)
 *   0x1506 -> Func_080090d0(object)
 *   0x1510 -> Func_0808a128(18, 2, 20)
 *   0x151a -> Func_0808a1b8(18, 0, 40)
 *   0x151e -> Func_0808a368()
 *   0x1522 -> Func_0808a370()
 *   0x1528 -> Func_0808a248(22)
 * The disassembler's own 'bl' annotations are wrong, as on every overlay.
 *
 * Note on Func_0808a248: resource_377's 0x020002e0 reaches the same import with
 * an object pointer, here with the small integer 22.  That is the documented
 * per-site arity/argument variation, so the declaration is left old-style.
 *
 * Constants built inline rather than pooled: 0x1e00000 = 240 << 17,
 * 0xca0000 = 202 << 16, 0x1480000 = 164 << 17, 0x20000 = 128 << 10,
 * 0xc30000 = 195 << 16, 0x50000 = 160 << 11, 0x608 = 193 << 3, 0x400 = 128 << 3,
 * 0x10000 = 128 << 9, 0x8000 = 128 << 8, 0x4000 = 128 << 7.
 */

struct Sprite_020013e4 {
    u8 filler00[5];
    u8 flags05;                 /* 0x05 */
    u8 filler06[3];
    u8 mode09;                  /* 0x09 */
    u8 filler0a[18];
    u8 index1c;                 /* 0x1c */
    u8 filler1d[10];
    u8 flags27;                 /* 0x27 */
};

struct Object_020013e4 {
    u8 filler00[12];
    s32 field0c;                /* 0x0c */
    u8 filler10[64];
    struct Sprite_020013e4 *sprite;  /* 0x50 */
    u8 filler54;
    u8 field55;                 /* 0x55 */
};

extern u8 *Data_03001ebc;

void Func_0808a018();
void Func_0808a210();
void Func_080000c0();
void Func_0808a0f0();
void Func_0808a200();
struct Object_020013e4 *Func_080090c8();
void Func_08000140();
u8 *Func_08015250();
void Func_080001c8();
void Func_08000150();
void Func_0808a360();
void Func_0808a090();
void Func_0808a0d0();
void Func_0808a1b8();
void Func_08009098();
void Func_08009148();
void Func_0808a010();
void Func_080090d0();
void Func_0808a128();
void Func_0808a368();
void Func_0808a370();
void Func_0808a248();

void Func_020013e4(void)
{
    struct Object_020013e4 *object;
    struct Sprite_020013e4 *sprite;
    u8 *palette;

    Func_0808a018();
    Func_0808a210(-1, -1, -1, 0);
    Func_080000c0(1);
    Func_0808a0f0(0, 0, 0);
    Func_0808a0f0(18, 0x1E00000, 0xCA0000);
    Func_080000c0(1);
    Func_0808a200(18, 1);

    object = Func_080090c8(22, 0x1480000, 0x20000, 0xC30000);
    object->field55 = 0;
    sprite = object->sprite;
    object->field0c = 0x50000;
    sprite->flags27 = 0;
    sprite->flags05 &= ~0x20;
    sprite->mode09 &= 0x0F;

    Func_08000140(17, 0x608);
    palette = Func_08015250(224) + 0x400;
    Func_080001c8(sprite->index1c, 128, palette);
    Func_08000150(17);

    *(s32 *)(Data_03001ebc + 448) = 514;

    Func_0808a360();
    Func_0808a090(18, 0x10000, 0x8000);
    Func_0808a0d0(18, 480, 176);
    Func_0808a0d0(18, 420, 164);
    Func_0808a0d0(18, 326, 185);
    Func_0808a1b8(18, 0x4000, 10);

    Func_08009098(object, 0x02009EAC);
    Func_08009148(object);
    Func_08009098(object, 0x02009ECC);
    Func_08009148(object);

    Func_0808a010(20);
    Func_080090d0(object);
    Func_0808a128(18, 2, 20);
    Func_0808a1b8(18, 0, 40);

    Func_0808a368();
    Func_0808a370();
    Func_0808a248(22);
}
