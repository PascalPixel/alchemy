#include "types.h"

struct ScriptState_080bd898 {
    u8 command[64];
    s32 argument[64];
    s32 command_index;
    s32 command_count;
    s32 state;
    s32 delay;
    s32 timer;
    u8 sprite[12];
    s32 animation_id;
    s32 actor_id;
    s32 sound_id;
    s32 actor_argument;
};

struct RenderObject_080bd898 {
    u8 unknown_00[40];
    u8 *attributes;
};

struct Actor_080bd898 {
    void *objects;
    u8 unknown_04[38];
    s16 flag_2a;
};

struct SpriteSource_080bd898 {
    u8 unknown_00[12];
    u16 tile;
    u16 y;
};

struct SpritePosition_080bd898 {
    u8 unknown_00[4];
    u16 x;
    u16 y;
};

struct Sprite_080bd898 {
    u32 unknown_00;
    u8 y;
    u8 unknown_05;
    u16 x;
    u16 tile;
};

s32 Func_080022fc(s32, s32);
s32 Func_08002322(s32);
void Func_0800393c(void *, s32);
void Func_080039fc(void *, s32);
s32 Func_080040d0(s32, const void *);
void Func_08003dec(void *, s32);
void Func_08009020(void *, s32);
void Func_08009080(void *, s32);
void Func_080090f8(void *, s32);
s32 Func_08015048(void);
void Func_08015118(void);
void Func_08015120(s32, s32);
void Func_08015130(u8);
void Func_080151d0(s32);
void Func_080152b8(const u16 *);
void *Func_08077008(s32);
s32 Func_080b6cd0(s32);
struct Actor_080bd898 *Func_080b7dd0(s32);
void Func_080b7e60(s32);
void *Func_080b7f70(void *, s32);
void Func_080b78e4(s32, struct Actor_080bd898 *);
void Func_080b7aac(s32);
void Func_080ba918(void *, s32);
void Func_080bac6c(s32);
void Func_080bb588(s32);
void Func_080bb8e8(s32);
void Func_080bb928(void *);
s32 Func_080bbb0c(void *, s32);
void Func_080bd850(void *, s32);
s32 Func_080c2368(s32);
s32 Func_080c24f0(s32, s32);
void Func_080f9010(s32);

void Func_080bd898(void)
{
    u8 *runtime = *(u8 **)0x03001e74;
    struct ScriptState_080bd898 *script =
        (struct ScriptState_080bd898 *)(runtime + 0x6b8);
    void *objects[4];

    if (*(s32 *)(runtime + 0x800) == 0)
        return;

    for (;;) {
        switch (script->state) {
        case 1:
            if (script->command_index < *(s8 *)(runtime + 0x655)) {
                script->command_count = 0;
                script->delay = 0;
                script->timer = 0;
                Func_080bbb0c(runtime + 0x654, script->command_index);
                script->command_index++;
                script->state = 2;
            } else {
                script->state = 4;
            }
            break;

        case 2: {
            s32 index;

            for (index = script->delay;
                 index < script->command_count && script->state == 2;
                 index = script->delay) {
                s32 argument;

                if (script->timer != 0) {
                    script->timer--;
                    return;
                }

                argument = script->argument[index];
                switch (script->command[index]) {
                case 14:
                    Func_080f9010(argument);
                    break;
                case 13:
                    Func_080bb928(script);
                    break;
                case 0:
                    Func_08015120(argument, 1);
                    break;
                case 1:
                    Func_08015120(argument, 5);
                    break;
                case 2:
                    Func_08015120(argument & 0x1ff, 2);
                    break;
                case 3:
                    Func_08015120(argument & 0x3fff, 4);
                    break;
                case 6:
                    (*(s32 **)0x03001ee4)[2] = 1;
                    break;
                case 4:
                    if (argument >= 0)
                        Func_080151d0(argument);
                    script->state = 3;
                    *(s32 *)0x03001af8 = 0;
                    break;
                case 5:
                    if (argument >= 0)
                        Func_080151d0(argument);
                    script->state = 13;
                    break;
                case 7:
                    Func_08015118();
                    break;
                case 12:
                    Func_080bb8e8(argument);
                    break;
                case 8: {
                    struct Actor_080bd898 *actor;

                    if (script->sound_id > 0)
                        Func_080f9010(script->sound_id);
                    script->actor_id = argument;
                    actor = Func_080b7dd0(argument);
                    Func_08009080(actor->objects, 5);
                    script->state = 10;
                    script->timer = 0;
                    break;
                }
                case 9: {
                    struct Actor_080bd898 *actor;
                    u8 *metadata;
                    s32 i;
                    void *object;

                    script->actor_id = argument;
                    Func_080c24f0(argument, script->actor_argument);
                    Func_080bb588(argument);
                    metadata = Func_08077008(argument);
                    for (i = 0;
                         (object = Func_080b7f70(
                              Func_080b7dd0(argument)->objects, i)) != 0;
                         i++) {
                        Func_08009020(object,
                            metadata[0x12a] == 1 ? 5 : 4);
                    }
                    if (metadata[0x12a] == 1) {
                        script->state = 11;
                        script->timer = 0;
                    }
                    break;
                }
                case 10:
                    Func_08015130(runtime[0x41]);
                    break;
                case 11: {
                    struct Actor_080bd898 *actor =
                        Func_080b7dd0(argument);

                    Func_080b78e4(argument, actor);
                    actor = Func_080b7dd0(argument);
                    Func_080ba918(actor->objects,
                        Func_080b6cd0(argument));
                    Func_080b7aac(argument);
                    break;
                }
                }
                script->delay++;
            }

            if (script->state == 2)
                script->state = 1;
            break;
        }

        case 3:
        case 13:
            if (Func_08015048() == 0)
                return;
            if (script->state == 13) {
                script->state = 2;
                script->timer = 0;
            } else {
                script->state = 5;
                script->animation_id = -1;
                script->timer = *(s32 *)0x03001800;
            }
            break;

        case 5: {
            u32 phase = (*(u32 *)0x03001e40 >> 2) & 7;
            const void *tiles = (const u8 *)0x080c3734 + phase * 128;
            void **view = *(void ***)0x03001ee4;
            struct SpriteSource_080bd898 *source = view[0];
            struct SpritePosition_080bd898 *position = view[1];
            struct Sprite_080bd898 *sprite =
                (struct Sprite_080bd898 *)script->sprite;
            s32 sine;

            if (script->animation_id == -1)
                script->animation_id = *(s32 *)(runtime + 0x54);

            Func_08015118();
            Func_080039fc((void *)0x0400004a, 4);
            Func_0800393c((void *)0x0400004a, 16);
            sprite->unknown_00 = 0xa000;
            sprite->tile = 0;
            sprite->tile =
                (sprite->tile & 0xfc00) |
                (Func_080040d0(script->animation_id, tiles) & 0x3ff);
            sprite->x =
                (sprite->x & 0xfe00) |
                ((source->tile * 8 + (position->x >> 8) + 4) & 0x1ff);
            sine = Func_08002322(*(s32 *)0x03001e40 << 12);
            if (sine < 0)
                sine += 0x7fff;
            sprite->y =
                (source->y * 8) + (position->y >> 8) +
                (sine >> 15) + 6;

            if ((*(s32 *)0x03001ae8 & 2) != 0 ||
                (*(s32 *)0x03001af8 & 0x303) != 0 ||
                ((u32)(*(s32 *)0x03001800 - script->timer) > 10 &&
                 (*(s32 *)0x03001ae8 & 0x303) != 0)) {
                Func_080f9010(0x6f);
                script->state = 2;
                script->timer = 0;
                break;
            }

            Func_08003dec(sprite, 0xf0);
            return;
        }

        case 10: {
            u16 ids[2];

            if ((script->timer & 1) != 0) {
                if ((script->timer & 2) != 0) {
                    struct Actor_080bd898 *actor;

                    ids[0] = 0xff;
                    actor = Func_080b7dd0(script->actor_id);
                    Func_080ba918(actor->objects,
                        Func_080b6cd0(script->actor_id));
                } else {
                    struct Actor_080bd898 *actor;

                    ids[0] = (u16)script->actor_id;
                    ids[1] = 0xff;
                    actor = Func_080b7dd0(script->actor_id);
                    Func_080ba918(actor->objects, 7);
                }
                Func_080152b8(ids);
            }

            script->timer++;
            if (script->timer <= 8)
                return;
            script->state = 2;
            script->timer = 0;
            break;
        }

        case 11: {
            s32 frame = script->timer;
            s32 value;
            s32 count;
            s32 i;

            if (frame == 0 || frame >= 0x400) {
                value = 6;
                if (frame == 0 && script->actor_argument != 0) {
                    u8 *metadata = Func_08077008(script->actor_id);
                    s32 sound = Func_080c2368(metadata[0x128]);

                    if (sound >= 0) {
                        sound--;
                        if (sound < 0)
                            sound = 0;
                        Func_080f9010(sound + 0x92);
                    }
                    script->timer = 0x400;
                }

                frame = script->timer;
                if (frame > 0x41d) {
                    frame = 0;
                    script->timer = 0;
                }
                if (frame == 0) {
                    u8 *metadata = Func_08077008(script->actor_id);
                    s32 sound = Func_080c2368(metadata[0x128]);

                    if (sound >= 0)
                        Func_080f9010(sound + 0x92);
                }
                if (script->timer >= 0x400) {
                    s32 elapsed = script->timer - 0x400;

                    if (elapsed < 0)
                        elapsed += 7;
                    value = Func_080022fc(elapsed >> 3, 5) + 1;
                }
                if (value == 6 || (script->timer & 7) == 0) {
                    count = 0;
                    while ((objects[count] = Func_080b7f70(
                                Func_080b7dd0(script->actor_id)->objects,
                                count)) != 0) {
                        struct RenderObject_080bd898 *object =
                            objects[count];

                        object->attributes[5] = value;
                        object->attributes[22] |= 0xff;
                        count++;
                    }
                }
                script->timer++;
                return;
            }

            if (frame == 4) {
                Func_080bac6c(script->actor_id);
                script->timer++;
                return;
            }

            if (frame > 4) {
                struct Actor_080bd898 *actor =
                    Func_080b7dd0(script->actor_id);

                actor->flag_2a = 1;
                count = 0;
                while ((objects[count] =
                            Func_080b7f70(actor->objects, count)) != 0)
                    count++;

                value = frame * 4 - 20;
                if (value > 127) {
                    for (i = 0; i < count; i++)
                        Func_080bd850(objects[i], 0);
                    Func_080b7e60(script->actor_id);
                    script->state = 2;
                    script->timer = 0;
                    return;
                }

                for (i = 0; i < count; i++) {
                    Func_080090f8(objects[i], value);
                    Func_080090f8(objects[i], value + 1);
                    Func_080090f8(objects[i], value + 2);
                    Func_080090f8(objects[i], value + 3);
                }
            }
            script->timer++;
            return;
        }

        default:
            return;
        }
    }
}
