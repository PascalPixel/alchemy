#include "types.h"

struct RuntimeState_080cc960 {
    u8 padding0[0x778c];
    s32 frame;
    u8 padding7790[0x94];
    s32 active;
};

struct Particle_080cc960 {
    s32 first;
    s32 second;
    s32 unused08;
    s32 sound;
    s32 tone;
    s32 volume;
    s32 unused18;
};

struct TransformInput_080cc960 {
    s32 value;
    s32 zero4;
    s32 zero8;
};

struct Point_080cc960 {
    s32 x;
    s32 y;
};

extern struct RuntimeState_080cc960 *Data_03001eec;
extern struct Particle_080cc960 Data_02010000[];

u32 Func_08004458(void);
void Func_080049ac(void);
void Func_08004c6c(s32 value);
void Func_08004bd4(s32 value);
void Func_08004c1c(s32 value);
void Func_080e3944(
    const struct TransformInput_080cc960 *input,
    struct Point_080cc960 *output);
void Func_080cde90(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);

void Func_080cc960(void)
{
    struct RuntimeState_080cc960 *state = Data_03001eec;
    s32 frame = state->frame++;
    struct Particle_080cc960 *particle;
    struct TransformInput_080cc960 input;
    struct Point_080cc960 first;
    struct Point_080cc960 second;
    s32 index;

    if (frame == 0) {
        particle = Data_02010000;
        for (index = 0; index != 0x100; index++, particle++) {
            s32 position = Func_08004458() & 0xf;

            particle->first = position + 0x30;
            particle->second = position + 0x28;
            particle->sound = Func_08004458() & 0xffff;
            particle->tone = Func_08004458() & 0xffff;
            particle->volume = Func_08004458() & 0xffff;
        }
    }

    input.zero4 = 0;
    input.zero8 = 0;
    particle = Data_02010000;

    for (index = 0; index != 0x40; index++, particle++) {
        if (frame > index / 4 && particle->first > 0) {
            s32 half;
            s32 color;

            Func_080049ac();
            Func_08004c6c(particle->volume);
            Func_08004bd4(particle->sound);
            Func_08004c1c(particle->tone);

            input.value = particle->first;
            Func_080e3944(&input, &first);
            first.x += 0x40;
            first.y += 0x50;

            input.value = particle->second;
            Func_080e3944(&input, &second);
            second.x += 0x40;
            second.y += 0x50;

            particle->second -= 4;
            particle->first -= 4;
            if (particle->second < 0)
                particle->second = 0;

            half = -particle->second;
            half = (half + ((u32)half >> 31)) >> 1;
            color = half + 0x30;

            Func_080cde90(
                second.x - 1, second.y,
                first.x - 1, first.y,
                color);
            Func_080cde90(
                second.x, second.y - 1,
                first.x, first.y - 1,
                color);
            Func_080cde90(
                second.x, second.y,
                first.x, first.y,
                half + 0x38);
        }
    }

    state->active = 1;
}
