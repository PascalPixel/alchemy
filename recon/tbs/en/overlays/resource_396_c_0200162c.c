/* NONMATCHING: 448 of 448 bytes, 53 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: the reference keeps the frame counter's address in r7 across the switch (reloaded after the spark loop, where r7 is the loop index) and computes the spark count into r1 before moving it to r8; this draft reloads the address into a scratch register at both ends and allocates the count straight to r8. Structure, division calls and the spark struct match. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 Main_0808a498(struct FieldSprite *sprite, s32 previous);
void ToretoHeya_Func020015ac(union FieldObject *object);

struct Spark {
    u8 unknown_00[0x64];
    u16 phase;
    u16 angle;
};

struct Vec3 {
    s32 x;
    s32 y;
    s32 z;
};

extern struct Vec3 Data_0200adc0;
extern s32 Data_0200adcc;

void Local_0200162c(void)
{
    struct FieldActor *spark;
    u32 frame;
    s32 previous;
    s32 wave;
    u32 count;
    u32 i;

    frame = Data_0200adcc;
    previous = 0;
    wave = Engine_MathDivide(frame, 10);
    switch (frame) {
    case 0:
    case 10:
    case 20:
    case 30:
    case 40:
        Engine_AudioPlayCue(220);
        count = 6 - wave;
        for (i = 0; i < count; i++) {
            spark = Engine_ObjectCreate(0x11d, Data_0200adc0.x, Data_0200adc0.y, Data_0200adc0.z);
            if (spark != 0) {
                previous = Main_0808a498(spark->sprite, previous);
                spark->motion_flags = 0;
                spark->sprite->priority = 0;
                Engine_ActorSetSpriteFlags(spark, 0);
                Engine_ObjectSetAnimation(spark, 1);
                ((struct Spark *)spark)->phase = 0;
                ((struct Spark *)spark)->angle = ((360 / count * i) << 16) / 360;
                spark->target_x = Data_0200adc0.x;
                spark->target_y = Data_0200adc0.y;
                spark->target_z = Data_0200adc0.z;
                spark->speed = 0x19999;
                spark->update = ToretoHeya_Func020015ac;
            }
        }
    case 44:
        Engine_AudioPlayCue(0x121);
        break;
    }
    if (++Data_0200adcc > 120) {
        Data_0200adcc = 0;
    }
}
