#include "types.h"

typedef struct {
    u8 unknown00[0x34];
    s8 field34;
} Context080984c0;

typedef struct {
    u8 unknown000[0xcb8];
    s16 active;
    s16 fieldcba;
} State080984c0;

typedef struct {
    u8 unknown000[0x53c];
    u8 field53c;
    u8 field53d;
    u8 field53e;
} Scene080984c0;

void Func_080030f8(s32 frames);
void Func_08004278(void (*callback)(void));
void *Func_0808e4b4(u32 kind, u32 index, s32 *size);
void Func_08091200(u32 value, s32 enabled);
void Func_08091220(u32 value, s32 enabled);
void Func_08091254(s32 value);
s32 Func_08096b28(void *resource, s32 mode, s32 size);
void Func_08098294(s32 value);
void Func_080982dc(void);
void Func_080f9010(s32 soundId);

extern Context080984c0 *Data_03001f30;
extern s32 Data_02000240[];

void Func_080984c0(void)
{
    Context080984c0 **globalSlot = &Data_03001f30;
    Context080984c0 *context = *globalSlot;
    Scene080984c0 *scene = *(Scene080984c0 **)((u8 *)globalSlot - 0x64);
    State080984c0 *state = *(State080984c0 **)((u8 *)globalSlot - 0x74);
    s16 clearedValue;
    s32 resourceSize;
    void *resource;

    if (state->active != 0) {
        Func_080f9010(0xa7);
        Func_08004278(Func_080982dc);

        clearedValue = 0;
        state->active = clearedValue;
        state->fieldcba = clearedValue;
        Func_08098294(0);

        Func_08091200(0x10000, 1);
        Func_08091254(1);
        Func_08091220(0, 0);
        Func_08091200(0x10000, 0);
        Func_08091254(30);
        Func_080030f8(1);

        resource = Func_0808e4b4(0x40000005, 8, &resourceSize);
        if (resource != NULL)
            Func_08096b28(resource, Data_02000240[125], resourceSize);

        if (context->field34 == 0) {
            scene->field53e = 0;
            scene->field53c = 1;
            scene->field53d = 1;
            Func_080030f8(10);
        }
    }
}
