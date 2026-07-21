typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;

#define FIELD_S32(base, offset) (*(s32 *)((u8 *)(base) + (offset)))
#define OBJECT_X(object) FIELD_S32(object, 0x08)
#define OBJECT_Y(object) FIELD_S32(object, 0x0C)
#define OBJECT_Z(object) FIELD_S32(object, 0x10)
#define OBJECT_TARGET_Y(object) FIELD_S32(object, 0x14)
#define OBJECT_VERTICAL_STEP(object) FIELD_S32(object, 0x28)
#define OBJECT_MIRRORED_Y(object) FIELD_S32(object, 0x3C)

void *Func_08092054(void);
void Func_080f9010(s32);
void Func_08009080(void *, s32);
void Func_080030f8(u32);
void Func_08009150(void *, s32, s32, s32);
void Func_08092adc(s32, s32, s32);
void Func_08092624(void *, s32);
void Func_08009158(void *);
void Func_0809202c(void);

void Func_08092708(s32 sequence_arg, s32 mode_or_frame, s32 optional_action) {
    s32 next_y;
    s32 base_z;
    void *object;
    u8 *object_flags;

    object = Func_08092054();
    base_z = OBJECT_Z(object);
    if (object != 0) {
        Func_080f9010(0x121);
        Func_08009080(object, mode_or_frame);
        Func_080030f8(10);
        object_flags = (u8 *)object + 0x55;
        Func_08009080(object, 1);
        {
            u8 flags = 2;
            flags |= *object_flags;
            *object_flags = flags;
        }
        OBJECT_VERTICAL_STEP(object) = 0x40000;
        Func_08009150(object, OBJECT_X(object), OBJECT_Y(object),
            base_z + 0xC0000);
        Func_080030f8(6);
        Func_080f9010(0xD9);
        mode_or_frame = 0;
        Func_08092adc(sequence_arg, 0x5000, 0);
        *object_flags = 0;
        do {
            next_y = OBJECT_Y(object) + 0xFFFE0000;
            OBJECT_Y(object) = next_y;
            OBJECT_MIRRORED_Y(object) = next_y;
            Func_080030f8(1);
            if ((optional_action != -1) && (mode_or_frame & 1)) {
                Func_08092624(object, optional_action);
            }
            mode_or_frame++;
        } while ((u32)mode_or_frame <= 0xD);
        *object_flags = 3;
        OBJECT_VERTICAL_STEP(object) = 0x30000;
        Func_08009150(object, OBJECT_X(object), OBJECT_Y(object),
            base_z + 0x100000);
        Func_08009158(object);
        mode_or_frame = 0;
        if (OBJECT_Y(object) > OBJECT_TARGET_Y(object)) {
wait_for_target_y:
            Func_080030f8(1);
            mode_or_frame++;
            if ((u32)mode_or_frame <= 0xB3) {
                if (OBJECT_Y(object) > OBJECT_TARGET_Y(object)) {
                    goto wait_for_target_y;
                }
            }
        }
        Func_080030f8(2);
        Func_0809202c();
    }
}
