#include "types.h"

/*
 * resource_3bb nearest-actor scene owner at 0x02001cc0, complete 308-byte span
 * through its four-word pool before 0x02001df4. It selects the closest valid
 * kind-165 actor, publishes two coordinates, and advances or ends the scene.
 */

extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a170(s32 message);
extern void Func_0808a180(s32 actor, s32 mode);
extern void Func_0808a248(s32 value);
extern void Func_0808a360(void);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_080770c8(s32 value);
extern void Func_080770e8(s32 slot, s32 coordinate);
extern void Func_02001ba8(s32 actor);

void Func_02001cc0(void)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    s32 subject = *(s32 *)0x02000434;
    u8 *subject_record = Func_0808a080(subject);
    s32 selected = 8;
    s32 best_distance = 0x100000;
    s32 actor;

    Func_0808a018();

    for (actor = 8; actor <= 66; actor++) {
        u8 *record = Func_0808a080(actor);

        if (record != 0 && record[84] == 1 &&
            *(s16 *)(*(u8 **)(record + 80) + 40) == 165) {
            s32 dx = *(s32 *)(subject_record + 8) - *(s32 *)(record + 8);
            s32 dz = *(s32 *)(subject_record + 16) - *(s32 *)(record + 16);
            s32 x;
            s32 z;
            s32 distance;

            if (dx < 0)
                dx += 0xffff;
                dz += 0xffff;
            if (dz < 0)
            x = dx >> 16;
            z = dz >> 16;

            if (z <= 0) {
                if (x < 0)
                    x = -x;
                if (z < 0)
                    z = -z;
                distance = x + z;
                if (distance < best_distance) {
                    selected = actor;
                    best_distance = distance;
                }
            }
        }
    }

    Func_0808a170(0x2085);
    Func_0808a180(selected, 0);
    *(s32 *)(workspace + 448) = 512;
    *(s32 *)(workspace + 456) = 15;
    Func_0808a010(20);
    Func_0808a368();
    Func_0808a370();

    Func_080770e8(subject * 16 + 0x370,
                  *(s32 *)(subject_record + 8) >> 20);
    Func_080770e8(subject * 16 + 0x378,
                  *(s32 *)(subject_record + 16) >> 20);
    subject++;

    if (subject > 3) {
        Func_0808a248(10);
        Func_080770c8(0x11a);
    } else {
        Func_02001ba8(subject);
        Func_0808a360();
        Func_0808a370();
        *(s32 *)(workspace + 448) = 0;
    }

    Func_0808a020();
}
