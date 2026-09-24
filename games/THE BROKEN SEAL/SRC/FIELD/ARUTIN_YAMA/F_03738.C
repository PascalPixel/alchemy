#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IWRAM_CALL.H"

void Main_08000128(s32 distance, s32 angle, union FieldCoordinate *pos);
s32 Main_080091a8(s32 layer, s32 x, s32 z);

/* Step lengths by height difference, centred on entry 16. */
extern s32 Data_0200d1b4[];

/* Rolls the object a step along its heading, sized by how far it sits
 * below its target height; counts down its rolling timer (cues at 20 and
 * 0) and then picks the animation from the slope ahead of it. */
void ArutinYama_Func02003738(struct FieldActor *object)
{
    s32 angle = 0xc000;
    s32 radius;
    s16 *timer;
    union FieldCoordinate pos[3];
    union FieldCoordinate *p;
    s32 ahead;
    s32 behind;

    angle &= object->facing;
    radius = object->y.fixed / 0x10000;
    radius = Data_0200d1b4[(s16)object->unknown_64 - radius + 16];
    timer = (s16 *)&object->unknown_66;
    if (*timer != 0) {
        if ((s16)--*timer == 20) {
            Engine_AudioPlayCue(184);
        }
        if (*timer == 0) {
            Engine_AudioPlayCue(233);
        }
    }
    p = pos;
    p[0].fixed = object->x.fixed;
    p[1].fixed = object->y.fixed;
    p[2].fixed = object->z.fixed;
    Main_08000128(Iwram_MulQ16(radius, 0xc000), angle, p);
    object->x.fixed = p[0].fixed;
    object->z.fixed = p[2].fixed;
    ahead = Main_080091a8(2, p[0].fixed, p[2].fixed);
    Main_08000128(-Iwram_MulQ16(radius, 0x18000), angle, p);
    behind = Main_080091a8(2, p[0].fixed, p[2].fixed);
    if (*timer <= 20) {
        if (ahead == behind) {
            Engine_ObjectSetAnimation(object, 2);
        } else if (ahead > behind) {
            Engine_ObjectSetAnimation(object, 3);
        } else {
            Engine_ObjectSetAnimation(object, 4);
        }
    }
}
