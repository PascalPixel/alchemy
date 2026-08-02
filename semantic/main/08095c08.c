#include "layout_guard.h"
#include "types.h"

typedef struct Position_08095c08 {
    s32 x;
    s32 y;
    s32 z;
} Position_08095c08;

typedef struct ObjectDisplay_08095c08 {
    u8 padding00[9];
    u8 attributes;
} ObjectDisplay_08095c08;

typedef struct Controller_08095c08 {
    u8 padding00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 padding14[0x3c];
    ObjectDisplay_08095c08 *display;
} Controller_08095c08;

typedef struct OrbitEffect_08095c08 {
    ObjectDisplay_08095c08 *display;
    s32 current_x;
    s32 current_z;
    s32 destination_x;
    s32 destination_z;
    s32 anchor_x;
    s32 anchor_z;
    u8 padding1c[4];
    s32 step_x;
    s32 step_z;
    u8 padding28[0x10];
    s16 age;
    s16 return_delay;
    u8 padding3c[4];
    s8 stage;
    u8 padding41;
    u8 launch_flag;
    u8 padding43;
    u8 returning;
    u8 padding45[2];
    u8 display_state;
} OrbitEffect_08095c08;

LAYOUT_OFFSET_GUARD(
    ObjectDisplay08095c08_Attributes,
    ObjectDisplay_08095c08,
    attributes,
    9);
LAYOUT_OFFSET_GUARD(
    Controller08095c08_Display,
    Controller_08095c08,
    display,
    0x50);
LAYOUT_OFFSET_GUARD(
    OrbitEffect08095c08_AnchorX,
    OrbitEffect_08095c08,
    anchor_x,
    0x14);
LAYOUT_OFFSET_GUARD(
    OrbitEffect08095c08_StepX,
    OrbitEffect_08095c08,
    step_x,
    0x20);
LAYOUT_OFFSET_GUARD(
    OrbitEffect08095c08_Age,
    OrbitEffect_08095c08,
    age,
    0x38);
LAYOUT_OFFSET_GUARD(
    OrbitEffect08095c08_Stage,
    OrbitEffect_08095c08,
    stage,
    0x40);
LAYOUT_OFFSET_GUARD(
    OrbitEffect08095c08_DisplayState,
    OrbitEffect_08095c08,
    display_state,
    0x47);

extern u32 Data_02000434;
extern u32 Data_03001800;

u32 Func_08004458(void);
void Func_0800447c(s32, s32, Position_08095c08 *);
Controller_08095c08 *Func_08092054(u32);
void Func_080974d8(Position_08095c08 *);
s8 Func_0809ba34(OrbitEffect_08095c08 *);
void Func_0809bb34(OrbitEffect_08095c08 *);
void Func_080f9010(s32);

/*
 * Advance one orbiting-object effect through launch, outbound travel, return
 * setup, inbound travel, and teardown.
 */
void Func_08095c08(OrbitEffect_08095c08 *effect)
{
    Controller_08095c08 *controller = Func_08092054(Data_02000434);
    Position_08095c08 position;

    switch (effect->stage) {
    case 0:
    {
        u32 first;
        u32 second;
        s32 heading;

        effect->current_x = effect->anchor_x;
        effect->current_z = effect->anchor_z;
        position.x = effect->anchor_x;
        position.z = effect->anchor_z;
        first = Func_08004458();
        second = Func_08004458();
        heading =
            ((first * 0x1800) >> 16) -
            ((second * 0x1800) >> 16) + 0xc000;
        Func_0800447c(0x780000, heading, &position);
        effect->destination_x = position.x;
        effect->destination_z = position.z;
        effect->step_x = 0x50000;
        effect->step_z = 0x50000;
        effect->launch_flag = 0;
        effect->stage++;
        effect->display->attributes =
            (effect->display->attributes & (u8)~0x0c) |
            (controller->display->attributes & 0x0c);
        effect->display_state = 0;
        effect->age = 0;
        if ((Data_03001800 & 1) != 0)
            Func_080f9010(0x86);
        break;
    }

    case 1:
        if (effect->age == 3) {
            effect->display->attributes &= (u8)~0x0c;
            effect->display_state = 4;
        }
        /* Fall through: stages 1 and 4 share the travel update. */
    case 4:
        if (Func_0809ba34(effect) == 0)
            effect->stage--;
        break;

    case 2:
        if (Func_0809ba34(effect) == 0) {
            effect->anchor_x = effect->current_x;
            effect->anchor_z = effect->current_z;
            effect->display->attributes &= (u8)~0x0c;
            effect->display_state = 4;
            effect->returning = 0;
            effect->stage++;
            effect->return_delay = 40;
        }
        break;

    case 3:
        effect->returning = 1;
        effect->current_x = effect->anchor_x;
        effect->current_z = effect->anchor_z;
        position.x = controller->x;
        position.y = controller->y + 0x140000;
        position.z = controller->z;
        Func_080974d8(&position);
        Func_0800447c(0x40000, Func_08004458(), &position);
        effect->destination_x = position.x;
        effect->destination_z = position.z;
        effect->stage++;
        if ((Data_03001800 & 1) != 0)
            Func_080f9010(0x91);
        break;

    case 5:
        if (Func_0809ba34(effect) == 0)
            Func_0809bb34(effect);
        break;
    }
}
