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
    u32 kind;
    s16 angle;

    kind = Data_03001e60->shape->kind;
    angle = 0;
    if (Data_03001ae8 & 2)
        angle = Data_03001800 << 8;
    switch (kind) {
    case 3:
        for (i = 0; i < 6; i++) {
            out->x = x << 16;
            out->y = 0;
            out->z = z << 16;
            Vector_AddPolarOffset(0x380000, angle, out);
            angle += 0x2aaa;
            out++;
        }
        for (; i < 10; i++)
            ;
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
            angle += 0x2000;
            out++;
        }
        for (; i < 10; i++)
            ;
        break;
    case 4:
    case 6:
        for (i = 0; i < 10; i++) {
            out->x = x << 16;
            out->y = 0;
            out->z = z << 16;
            Vector_AddPolarOffset(0x380000, angle, out);
            angle += 0x1999;
            out++;
        }
        break;
    case 20:
        angle += 0x4000;
        for (i = 0; i < 2; i++) {
            out[0].x = x << 16;
            out[0].y = 0;
            out[0].z = z << 16;
            Vector_AddPolarOffset(0x280000, angle, &out[0]);
            out[1].x = x << 16;
            out[1].y = 0;
            out[1].z = z << 16;
            Vector_AddPolarOffset(0x280000, angle, &out[1]);
            angle += 0x8000;
            out += 2;
        }
        break;
    default:
        angle += 0x2000;
        for (i = 0; i < 4; i++) {
            out->x = x << 16;
            out->y = 0;
            out->z = z << 16;
            Vector_AddPolarOffset(0x380000, angle, out);
            angle += 0x4000;
            out++;
        }
        for (; i < 10; i++)
            ;
        break;
    }
}
