#include "types.h"

struct FlashChild_0809b698 {
    u8 unknown_00[5];
    u8 mode;
};

struct FlashState_0809b698 {
    u8 unknown_00[0x25];
    u8 primary_trigger;
    u8 secondary_trigger;
    u8 unknown_27;
    struct FlashChild_0809b698 *child;
};

struct SceneObject_0809b698 {
    u8 unknown_00[6];
    u16 graphic_id;
    u8 unknown_08[0x48];
    struct FlashState_0809b698 *flash;
    u8 unknown_54[0x10];
    s16 callback_mode;
    u16 callback_counter;
    u8 unknown_68[4];
    void (*callback)(void *);
};

struct SceneSequence_0809b698 {
    u8 unknown_000[0x10];
    struct SceneObject_0809b698 *object;
    u8 unknown_014[0x706];
    s16 graphic_handle;
};

struct TransitionState_0809b698 {
    u32 display_mode;
    s8 variant;
    u8 unknown_05[3];
    s16 result;
};

s32 Func_08004080(void);
s32 Func_08003fa4(s32 handle, s32 size, const void *source);
s32 Func_080770c0(s32 id);
void Func_08009240(struct SceneObject_0809b698 *object, s32 mode);
void Func_080091e0(struct SceneObject_0809b698 *object, s32 mode);
void Func_080f9010(s32 sound);
void Func_080030f8(s32 frames);
void Func_080041d8(void (*callback)(void), s32 priority);
void Func_08004278(void (*callback)(void));
void Func_08003f3c(u32 handle);
void Func_08015040(void *id, s32 enabled);
void Func_0809b588(void);
void Func_0809b5dc(void *object);

/*
 * Play the flashing transition for the selected scene object, temporarily
 * installing its per-frame callback and restoring its original graphic when
 * the transition finishes.
 */
void Func_0809b698(void)
{
    struct SceneSequence_0809b698 *sequence =
        *(struct SceneSequence_0809b698 **)0x03001f30;
    struct TransitionState_0809b698 *transition =
        (struct TransitionState_0809b698 *)0x02000484;
    struct SceneObject_0809b698 *object = sequence->object;
    struct FlashState_0809b698 *flash = object->flash;
    struct FlashChild_0809b698 *child = flash->child;
    u16 original_graphic = object->graphic_id;
    u32 count;

    sequence->graphic_handle = (s16)Func_08004080();
    Func_08003fa4(sequence->graphic_handle, 0x100,
                  (const void *)0x0809c510);

    transition->display_mode = 0x09600000;
    transition->variant = Func_080770c0(0x145);

    Func_08009240(object, 0);
    object->callback = Func_0809b5dc;
    object->callback_mode = 0;
    object->callback_counter = 0;

    Func_080f9010(0x8c);
    Func_080030f8(15);
    object->callback_mode = 1;
    Func_080030f8(10);

    for (count = 0; count < 20; count++) {
        child->mode = 7;
        flash->primary_trigger = 1;
        Func_080030f8(2);
        flash->primary_trigger = 1;
        child->mode = 0;
        flash->secondary_trigger = 1;
        Func_080030f8(3);
    }

    object->callback = 0;
    object->graphic_id = original_graphic;
    Func_080041d8(Func_0809b588, 0xc80);
    Func_080030f8(15);
    Func_080f9010(0xae);
    Func_080030f8(55);
    Func_08004278(Func_0809b588);

    if (transition->result != 0) {
        Func_080091e0(object, 2);
    } else {
        Func_080091e0(object, 1);
    }

    Func_08009240(object, 0);
    Func_08003f3c((u16)sequence->graphic_handle);
    Func_08015040((void *)0x922, 1);
}
