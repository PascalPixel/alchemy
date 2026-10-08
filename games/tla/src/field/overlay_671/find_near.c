#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"

struct Orbiter {
    u8 unknown_00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[15];
    u8 flags_23;
    u8 unknown_24[44];
    void **animation;
    u8 kind;
    u8 unknown_55[13];
    u8 phase;
    u8 unknown_63;
    s16 center_x;
    s16 center_z;
    void *link;
};

extern s32 Data_02005af0;
extern const s8 Data_02006b38[];
extern const u8 Data_02006b20[];
extern const u8 Data_02005fd4[];
extern const u8 Data_02006004[];
extern const u8 Data_02006244[];
extern const u8 Data_020064cc[];
extern const u8 Data_020067e4[];
extern const u8 Data_02006898[];
extern const u8 Data_02006a90[];
struct Orbiter *Object_GetById(s32 id);
s32 Func_02005834(s32 value);
void Func_020020f0(s32 value);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);

#define DIFF_WITHIN(a, b, limit) ((a) - (b) >= 0 ? (a) - (b) <= (limit) : (b) - (a) <= (limit))

struct Orbiter *Func_020005b4(struct Orbiter *origin, s32 id)
{
    s32 i;

    for (i = 8; i < 64; i++) {
        struct Orbiter *object = Object_GetById(i);

        if (object != NULL && object->kind == 1 && object->animation[10] != NULL
            && *(s16 *)object->animation[10] == id) {
            if (DIFF_WITHIN(object->x, origin->x, 0x17ffff) && DIFF_WITHIN(object->z, origin->z, 0x17ffff))
                return object;
        }
    }
    return NULL;
}
