/* 2026-09-29 alchemy permute: score 8050 to 4240 on the permuter's scorer
   (0 is exact); remaining 86 register-only, 15 operand, 22 reordered, 8
   inserted, 13 deleted. Kept rewrites: 13x reorder independent statements,
   5x introduce a temporary, 4x reorder local declarations, 4x pointer
   arithmetic or indexing, 2x swap commutative operands, 2x remove a
   temporary, 2x change loop form, 1x add a same-width cast, 1x split or
   join a compound assignment, 1x test truth or compare with zero.
   FAKEMATCH: the permuter's temporaries, register hints and swapped
   operand orders below only steer allocation and scheduling; no programmer
   would write them, so they stay tagged until a natural spelling replaces
   them. */
/* Draft, not exact (2026-09-25): 492 of 500 bytes, 229 differing halfwords.
   Written from the listing; the switch, every loop shape (including the
   empty tails that run the counter up to ten) and the constants line up.
   Remaining: the ROM gives the angle r5, z r6, out r7 and x r8 and keeps
   the counter in r4, saving it around each call in a 4-byte frame; here
   the counter takes r7 and the angle r8 (declaration order, u16/s32 angle
   types and indexed case-20 stores moved nothing). */
#include "TYPES.H"

struct ProbePoint {
    s32 x;
    s32 y;
    s32 z;
    s32 unk_0c;
};

struct MapShape {
    u8 unk_00[4];
    u8 kind;
};

struct MapState {
    u8 unk_00[0x28];
    struct MapShape *shape;
};

extern struct MapState *Data_03001e60;
extern volatile u32 Data_03001ae8;
extern s32 Data_03001800;

void Vector_AddPolarOffset(s32 radius, u16 angle, struct ProbePoint *point);

/* Lays out the ring of probe points around a map position: the map shape
   decides how many and how far apart; holding B turns the ring with the
   frame counter. */
void Map_BuildProbeRing(s32 x, s32 z, struct ProbePoint *out)
{
    s32 i;
    s16 angle;
    struct MapShape *tmp;

    angle = 0;
    tmp = Data_03001e60->shape;
    if ((Data_03001ae8 & 2) != 0)
        angle = Data_03001800 << 8;
    switch ((u32)tmp->kind) {
    case 3:
        for (i = 0; i < 6; i++) {
            out->x = x << 16;
            out->y = 0;
            out->z = z << 16;
            Vector_AddPolarOffset(0x380000, angle, out);
            out++;
            angle += 0x2aaa;
        }
        for (; i < 10; i++);
        break;
    case 5:
    case 8:
    case 44:
    case 88:
        for (i = 0; i < 8; i++) {
            out->x = x << 16;
            out->y = 0;
            out->z = z << 16;
            Vector_AddPolarOffset(0x380000, angle, out);
            out++;
            angle += 0x2000;
        }
        for (; i < 10; i++);
        break;
    case 4:
    case 6:
        i = 0;
        while (i < 10) {
            s32 tmp2;
            s32 tmp3;
            out->x = x << 16;
            tmp3 = z << 16;
            out[0].y = 0;
            tmp2 = tmp3;
            out->z = tmp2;
            i++;
            Vector_AddPolarOffset(0x380000, angle, out);
            out++;
            angle += 0x1999;
        }
        break;
    case 20:
        angle += 0x4000;
        for (i = 0; i < 2; i++) {
            struct ProbePoint *tmp4;
            out[0].x = x << 16;
            out[0].y = 0;
            out[0].z = z << 16;
            Vector_AddPolarOffset(0x280000, angle, &out[0]);
            tmp4 = out + 1;
            tmp4->x = x << 16;
            out[1].y = 0;
            out[1].z = z << 16;
            Vector_AddPolarOffset(0x280000, angle, &out[1]);
            out += 2;
            angle += (u32)0x8000;
        }
        break;
    default:
        angle = angle + 0x2000;
        i = 0;
        while (i < 4) {
            out->x = x << 16;
            out->y = 0;
            out->z = z << 16;
            i++;
            Vector_AddPolarOffset(0x380000, angle, out);
            out++;
            angle += 0x4000;
        }
        for (; i < 10; i++);
        break;
    }
}
