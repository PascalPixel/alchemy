typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Resource 371 owner at 0x0200013c (136 bytes, 4 calls).
 *
 * Complete owner: `push {r5, r6, lr}` at 0x0200013c and the interworking
 * return `pop {r5, r6} ; pop {r0} ; bx r0` at 0x020001ac.  r0 holds the
 * popped return address, so the owner returns nothing.  Bytes
 * 0x020001b2-0x020001c3 are alignment plus the literal pool (0x03001e40,
 * 0x00014ccc, 0x0000011d, 0x0200811d).
 *
 * Two independent tests of the IWRAM status word 0x03001e40: bit 2 selects
 * the scale written into +0x18/+0x1c, and bit 1 gates spawning a companion
 * object at this object's position.
 *
 * The companion's per-frame step is installed into its +0x6c as the pool
 * word 0x0200811d.  Under this overlay's proven 0x02008000 link base that is
 * `Func_0200011c` plus the Thumb bit, so it is written here as the function
 * pointer it is.  The byte-exact sibling
 * `assets/code/resource_371_c_02000250.c` performs the same store with
 * 0x020081c5 (-> Func_020001c4), and 0x02002768 installs 0x0200a859
 * (-> Func_02002858) into the spawn table, so the field is unambiguously a
 * step callback.
 *
 * `movs r3,#13 ; negs r3,r3 ; ands ; movs r2,#4 ; orrs` is a two-bit field at
 * bits 2-3 of the render record's byte +9 being set to 1.
 *
 * UNCERTAINTIES: 0x02004378 is a spawn/lookup that takes an identifier and a
 * 16.16 position triple and returns the new object or NULL; 0x02004618 takes
 * a single small identifier and is called unconditionally between the spawn
 * and the null test, so its result cannot be what the test reads.  Neither
 * interface is resolved, hence the old-style declarations.
 */

extern u32 Data_03001e40;

void *Func_02004378();
void Func_02004618();
void Func_020043dc();
void Func_0200439c();

/* This overlay's own owner, installed as the companion's step callback. */
void Func_0200011c(u8 *object);

void Func_0200013c(u8 *object)
{
    s32 scale;
    u8 *spawned;
    u8 *record;

    if ((Data_03001e40 & 4) != 0) {
        scale = 0x14ccc;
    } else {
        scale = 0x10000;
    }
    *(s32 *)(object + 0x18) = scale;
    *(s32 *)(object + 0x1c) = scale;

    if ((Data_03001e40 & 2) == 0) {
        return;
    }

    spawned = Func_02004378(0x11d,
                            *(s32 *)(object + 0x08),
                            *(s32 *)(object + 0x0c),
                            *(s32 *)(object + 0x10));
    Func_02004618(0xf6);
    if (spawned == 0) {
        return;
    }

    spawned[0x55] = 0;
    record = *(u8 **)(spawned + 0x50);
    record[9] = (u8)((record[9] & ~0x0c) | 0x04);
    Func_020043dc(spawned, 0);
    Func_0200439c(spawned, 1);
    *(u16 *)(spawned + 0x64) = 0;
    *(void (**)(u8 *))(spawned + 0x6c) = Func_0200011c;
}
