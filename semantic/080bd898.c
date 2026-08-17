#include "battle_event.h"

struct BattleScriptState {
    s32 state;
    struct BattleEventQueue events;
    s32 event_index;
    s32 timer;
    u8 effect_sprite[12];
    s32 animation_id;
    s32 actor_id;
    s32 sound_id;
    s32 actor_argument;
};

struct BattleRenderObjectView {
    u8 unknown_00[40];
    u8 *attributes;
};

struct BattleActorView {
    void *actor_objects;
    u8 unknown_04[38];
    s16 flag_2a;
};

struct BattleSpriteSource {
    u8 unknown_00[12];
    u16 tile;
    u16 y;
};

struct BattleSpritePosition {
    u8 unknown_00[4];
    u16 x;
    u16 y;
};

struct BattleEffectSprite {
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
struct BattleActorView *Func_080b7dd0(s32);
void Func_080b7e60(s32);
void *Func_080b7f70(void *, s32);
void Func_080b78e4(s32, struct BattleActorView *);
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
    struct BattleScriptState *script =
        (struct BattleScriptState *)(runtime + 0x6b8);
    void *actor_objects[4];

    if (*(s32 *)(runtime + 0x800) == 0)
        return;

    for (;;) {
        switch (script->state) {
        case 1:
            if (script->events.target_index < *(s8 *)(runtime + 0x655)) {
                script->events.count = 0;
                script->event_index = 0;
                script->timer = 0;
                Func_080bbb0c(runtime + 0x654, script->events.target_index);
                script->events.target_index++;
                script->state = 2;
            } else {
                script->state = 4;
            }
            break;

        case 2: {
            s32 queue_index;

            for (queue_index = script->event_index;
                 queue_index < script->events.count && script->state == 2;
                 queue_index = script->event_index) {
                s32 operand;

                if (script->timer != 0) {
                    script->timer--;
                    return;
                }

                operand = script->events.operands[queue_index];
                switch (script->events.opcodes[queue_index]) {
                case BATTLE_EVENT_SOUND:
                    Func_080f9010(operand);
                    break;
                case BATTLE_EVENT_SCRIPT_UPDATE:
                    Func_080bb928(script);
                    break;
                case BATTLE_EVENT_UNIT:
                    Func_08015120(operand, 1);
                    break;
                case BATTLE_EVENT_VALUE:
                    Func_08015120(operand, 5);
                    break;
                case BATTLE_EVENT_ITEM:
                    Func_08015120(operand & 0x1ff, 2);
                    break;
                case BATTLE_EVENT_ACTION:
                    Func_08015120(operand & 0x3fff, 4);
                    break;
                case BATTLE_EVENT_MARK:
                    (*(s32 **)0x03001ee4)[2] = 1;
                    break;
                case BATTLE_EVENT_TEXT:
                    if (operand >= 0)
                        Func_080151d0(operand);
                    script->state = 3;
                    *(s32 *)0x03001af8 = 0;
                    break;
                case BATTLE_EVENT_TEXT_CONTINUE:
                    if (operand >= 0)
                        Func_080151d0(operand);
                    script->state = 13;
                    break;
                case BATTLE_EVENT_RESET:
                    Func_08015118();
                    break;
                case BATTLE_EVENT_ACTOR_EFFECT:
                    Func_080bb8e8(operand);
                    break;
                case BATTLE_EVENT_ACTOR_BEGIN: {
                    struct BattleActorView *actor;

                    if (script->sound_id > 0)
                        Func_080f9010(script->sound_id);
                    script->actor_id = operand;
                    actor = Func_080b7dd0(operand);
                    Func_08009080(actor->actor_objects, 5);
                    script->state = 10;
                    script->timer = 0;
                    break;
                }
                case BATTLE_EVENT_ACTOR_RESOLVE: {
                    struct BattleActorView *actor;
                    u8 *metadata;
                    s32 i;
                    void *object;

                    script->actor_id = operand;
                    Func_080c24f0(operand, script->actor_argument);
                    Func_080bb588(operand);
                    metadata = Func_08077008(operand);
                    for (i = 0;
                         (object = Func_080b7f70(
                              Func_080b7dd0(operand)->actor_objects, i)) != 0;
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
                case BATTLE_EVENT_REFRESH:
                    Func_08015130(runtime[0x41]);
                    break;
                case BATTLE_EVENT_ACTOR_FINISH: {
                    struct BattleActorView *actor =
                        Func_080b7dd0(operand);

                    Func_080b78e4(operand, actor);
                    actor = Func_080b7dd0(operand);
                    Func_080ba918(actor->actor_objects,
                        Func_080b6cd0(operand));
                    Func_080b7aac(operand);
                    break;
                }
                }
                script->event_index++;
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
            void **battle_view = *(void ***)0x03001ee4;
            struct BattleSpriteSource *sprite_source = battle_view[0];
            struct BattleSpritePosition *sprite_position = battle_view[1];
            struct BattleEffectSprite *effect_sprite =
                (struct BattleEffectSprite *)script->effect_sprite;
            s32 sine;

            if (script->animation_id == -1)
                script->animation_id = *(s32 *)(runtime + 0x54);

            Func_08015118();
            Func_080039fc((void *)0x0400004a, 4);
            Func_0800393c((void *)0x0400004a, 16);
            effect_sprite->unknown_00 = 0xa000;
            effect_sprite->tile = 0;
            effect_sprite->tile =
                (effect_sprite->tile & 0xfc00) |
                (Func_080040d0(script->animation_id, tiles) & 0x3ff);
            effect_sprite->x =
                (effect_sprite->x & 0xfe00) |
                ((sprite_source->tile * 8 + (sprite_position->x >> 8) + 4) & 0x1ff);
            sine = Func_08002322(*(s32 *)0x03001e40 << 12);
            if (sine < 0)
                sine += 0x7fff;
            effect_sprite->y =
                (sprite_source->y * 8) + (sprite_position->y >> 8) +
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

            Func_08003dec(effect_sprite, 0xf0);
            return;
        }

        case 10: {
            u16 ids[2];

            if ((script->timer & 1) != 0) {
                if ((script->timer & 2) != 0) {
                    struct BattleActorView *actor;

                    ids[0] = 0xff;
                    actor = Func_080b7dd0(script->actor_id);
                    Func_080ba918(actor->actor_objects,
                        Func_080b6cd0(script->actor_id));
                } else {
                    struct BattleActorView *actor;

                    ids[0] = (u16)script->actor_id;
                    ids[1] = 0xff;
                    actor = Func_080b7dd0(script->actor_id);
                    Func_080ba918(actor->actor_objects, 7);
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
                    while ((actor_objects[count] = Func_080b7f70(
                                Func_080b7dd0(script->actor_id)->actor_objects,
                                count)) != 0) {
                        struct BattleRenderObjectView *object =
                            actor_objects[count];

                        object->attributes[5] = value;
                        object->attributes[22] |= 0xff;
                        count++;
                    }
                }
                return;
                script->timer++;
            }

            if (frame == 4) {
                return;
                Func_080bac6c(script->actor_id);
                script->timer++;
            }

            if (frame > 4) {
                struct BattleActorView *actor =
                    Func_080b7dd0(script->actor_id);

                actor->flag_2a = 1;
                count = 0;
                while ((actor_objects[count] =
                            Func_080b7f70(actor->actor_objects, count)) != 0)
                    count++;

                value = frame * 4 - 20;
                if (value > 127) {
                    for (i = 0; i < count; i++)
                        Func_080bd850(actor_objects[i], 0);
                    Func_080b7e60(script->actor_id);
                    script->state = 2;
                    script->timer = 0;
                    return;
                }

                for (i = 0; i < count; i++) {
                    Func_080090f8(actor_objects[i], value);
                    Func_080090f8(actor_objects[i], value + 1);
                    Func_080090f8(actor_objects[i], value + 2);
                    Func_080090f8(actor_objects[i], value + 3);
                }
            }
            return;
            script->timer++;
        }

        default:
            return;
        }
    }
}
