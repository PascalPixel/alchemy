typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed short s16;
typedef signed int s32;

struct SceneState_080b9dc4 {
    u32 effect_flags;
    u8 unknown_004[0xc];
    s32 transition_active;
};

struct BattleState_080b9dc4 {
    u8 unknown_000[0x45];
    u8 battle_rule;
};

struct ObjectState_080b9dc4 {
    u8 unknown_000[0x13b];
    u8 flag_13b;
    u8 flag_13c;
};

s32 Func_080c10e8(const u16 *opposite_objects, s32 mode);
void Func_080151c8(s32 message_id);
void Func_080bb65c(void);
s32 Func_080b6b40(s32 groups, s16 *object_ids);
struct ObjectState_080b9dc4 *Func_08077008(s32 object_id);
void Func_080b8064(s32 object_id);
void Func_080030f8(s32 frames);
u32 Func_08004458(void);
void Func_080bac6c(s32 object_id);
s32 Func_080b7e60(s32 object_id);

s32 Func_080b9dc4(const u8 *selection)
{
    struct SceneState_080b9dc4 *scene =
        *(struct SceneState_080b9dc4 **)0x03001f00;
    struct BattleState_080b9dc4 *battle =
        *(struct BattleState_080b9dc4 **)0x03001e74;
    s32 succeeded = 0;
    u32 selected;

    scene->effect_flags = 0x2000;
    scene->transition_active = 1;
    Func_080c10e8((const u16 *)0, 0);
    selected = *selection;

    if (selected <= 7) {
        if (battle->battle_rule == 2) {
            Func_080151c8(0x847);
            Func_080bb65c();
        } else {
            s16 object_ids[14];
            s32 index = Func_080b6b40(1, object_ids) - 1;

            while (index >= 0) {
                s32 object_id = object_ids[index];
                struct ObjectState_080b9dc4 *object =
                    Func_08077008(object_id);

                if (object->flag_13b == 0 && object->flag_13c == 0) {
                    Func_080b8064(object_id);
                    Func_080030f8(8);
                }
                index--;
            }

            Func_080030f8(22);
            succeeded = 1;
        }
    } else if ((Func_08004458() * 10U >> 16) <= 6) {
        Func_080b8064((s32)*selection);
        Func_080030f8(8);
        Func_080bac6c((s32)*selection);
        Func_080b7e60((s32)*selection);
    } else {
        Func_080151c8(0x847);
        Func_080bb65c();
    }

    scene->transition_active = 0;
    return succeeded;
}
