/*
 * NONMATCHING: 1252 bytes, candidate 1062, 616 differing halfwords.
 * First typed draft of the player-movement collision probes and update
 * branches; behavior and source structure need a further disassembly audit.
 * WALL: structural-topology: missing code and control-flow differences.
 */
#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "MAP.H"
#include "IWRAM_CALL.H"

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct GameFlagRow {
    u16 first;
    u16 second;
};

void Func_0800447c(s32 distance, s32 angle, struct Vec *position);
s32 Func_080122ac(s32, struct WorldPosition *position);
void Func_0800d14c(struct ObjectRuntime *, s32, s32, s32);
void Func_0800c300(void *, s32);
void Func_0800c2d8(void *, u32);
s32 Func_0800ba30(void *, s32);
s32 Func_0800eaf8(void);
s32 Func_080045d4(s32);
struct ObjectRuntime *Func_0800c150(s32, s32, s32, s32);
s32 Func_08012204(struct WorldPosition *);

s32 Func_0800f2f8(struct ObjectRuntime *object)
{
    struct Vec position;
    struct Vec test_position;
    s32 angle;
    s32 mode;
    s32 blocked;
    s32 direction;
    s16 angle_offsets[8];
    s32 angle_index;
    s32 collision;
    s32 difference;
    s32 collision_kind;
    s32 (*square)(s32);
    struct ObjectRuntime *effect;
    u8 *animation;
    s32 buttons;

    mode = 2;
    collision = 0;
    if (((struct GameFlagRow *)0x02000240)[135].first
        & *(u32 *)0x03001ae8) {
        object->speed_limit = 0x40000;
        object->acceleration = 0x50000;
        mode = 5;
    } else {
        object->speed_limit = 0x20000;
        object->acceleration = 0x10000;
    }
    buttons = *(u32 *)0x03001b04;
    if ((buttons & 0x200) != 0)
        object->speed_limit = 0x100000;

    angle = ((*(u32 *)0x03001ae8 >> 4) & 15);
    angle = ((s16 *)0x08013254)[angle];
    if (angle == -1) {
        collision = 4;
        goto update_object;
    }

    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Func_0800447c(0x70000, angle, &position);
    if (*(u8 *)0x03001f54 != 0
        && (*(u32 *)0x03001ae8 & 0x200) != 0)
        goto update_object;
    if (Func_080122ac((s32)object, (struct WorldPosition *)&position) != 0)
        goto choose_direction;

    test_position.x = object->x;
    test_position.y = object->y;
    test_position.z = object->z;
    Func_0800447c(0x70000, angle + 0x1000, &test_position);
    if (Func_080122ac((s32)object, (struct WorldPosition *)&test_position) != 0)
        goto choose_direction;
    test_position.x = object->x;
    test_position.y = object->y;
    test_position.z = object->z;
    Func_0800447c(0x70000, angle - 0x1000, &test_position);
    if (Func_080122ac((s32)object, (struct WorldPosition *)&test_position) != 0)
        goto choose_direction;
    test_position.x = object->x;
    test_position.y = object->y;
    test_position.z = object->z;
    Func_0800447c(0x70000, angle + 0x2000, &test_position);
    if (Func_080122ac((s32)object, (struct WorldPosition *)&test_position) != 0)
        goto choose_direction;
    test_position.x = object->x;
    test_position.y = object->y;
    test_position.z = object->z;
    Func_0800447c(0x70000, angle - 0x2000, &test_position);
    if (Func_080122ac((s32)object, (struct WorldPosition *)&test_position) != 0)
        goto choose_direction;
    goto update_object;

choose_direction:
    angle_offsets[0] = angle + 0x1000;
    angle_offsets[1] = angle - 0x1000;
    angle_offsets[2] = angle + 0x2000;
    angle_offsets[3] = angle - 0x2000;
    angle_offsets[4] = angle + 0x3000;
    angle_offsets[5] = angle - 0x3000;
    angle_offsets[6] = angle + 0x4000;
    angle_offsets[7] = angle - 0x4000;
    for (angle_index = 0; angle_index < 8; angle_index++) {
        position.x = object->x;
        position.y = object->y;
        position.z = object->z;
        Func_0800447c(0x70000, angle_offsets[angle_index], &position);
        if (Func_080122ac((s32)object, (struct WorldPosition *)&position) == 0) {
            angle = (s16)angle_offsets[angle_index];
            goto update_object;
        }
    }
    collision |= 1;

update_object:
    if (*(u8 **)0x03001ebc != 0) {
        if ((collision & 3) != 0)
            (*(u16 *)(*(u8 **)0x03001ebc + 0x19c))++;
        else
            *(u16 *)(*(u8 **)0x03001ebc + 0x19c) = 0;
    }
    if (collision != 0)
        Func_0800c300(object, 9);
    else
        Func_0800c300(object, mode);

    if (collision != 0) {
        object->target_x = -0x80000000;
        object->target_y = -0x80000000;
        object->target_z = -0x80000000;
        object->velocity_x = 0;
        object->velocity_z = 0;
        if ((collision & 3) != 0) {
            difference = (s16)(angle - object->angle);
            if (difference > 0x1000)
                difference = 0x1000;
            if (difference < -0x1000)
                difference = -0x1000;
            object->angle += difference;
        }
        object->action = 0;
        object->unknown_66 = 2;
        goto movement_done;
    }

    square = (s32 (*)(s32))0x03000118;
    Func_0800d14c(object, position.x, position.y, position.z);
    angle = Func_080045d4(square(object->velocity_x) + square(object->velocity_z));
    position.x = object->velocity_x;
    position.y = object->velocity_y;
    position.z = object->velocity_z;
    Func_0800447c(angle, (s16)((s16 *)0x08013254)[(*(u32 *)0x03001ae8 >> 4) & 15], &position);
    object->velocity_x = position.x;
    object->velocity_y = position.y;
    object->velocity_z = position.z;
    {
        u8 *work = *(u8 **)0x03001e70;
        s16 *turn = (s16 *)(work + 282);
        s32 rate = ((s32 *)0x0801328c)[(*(u32 *)0x03001ae8 >> 4) & 15];
        difference = rate - *turn;
        if (difference < 0)
            difference += 7;
        difference >>= 3;
        if (difference > 0x200)
            difference = 0x200;
        if (difference < -0x1000)
            difference = -0x1000;
        if (difference + 15 <= 30)
            difference = rate - *turn;
        *turn += difference;
    }

movement_done:
    if (object->action_flags == 1) {
        collision_kind = Func_08012204((struct WorldPosition *)&object->x);
        if (collision_kind == 9) {
            animation = object->animation;
            *(u8 *)(*(u8 **)(animation + 44) + 6) = 1;
            *(u8 *)(animation + 38) = 0x80;
        } else {
            animation = object->animation;
            *(u8 *)(*(u8 **)(animation + 44) + 6) = 9;
            *(u8 *)(animation + 38) = 1;
        }
        if (collision_kind == 6 && *(s16 *)&object->action == 0 && collision == 0) {
            effect = Func_0800c150(24, object->x, object->y, object->z);
            if (effect != 0) {
                animation = effect->animation;
                Func_0800c2d8(effect, 0x08013280);
                *((u8 *)effect + 0x55) = collision;
                *((u8 *)effect + 0x22) = 1;
                if (animation != 0) {
                    Func_0800ba30(animation, 1);
                    *(u8 *)(animation + 38) = collision;
                    animation[5] = (animation[5] & ~13) | 4;
                    animation[9] = (animation[9] & ~13) | 8;
                }
                *(u16 *)&object->action = 10;
            }
        }
    }
    Func_0800eaf8();
    object->step++;
    return 1;
}
