#include "TYPES.H"

/* Unit bindings for scoring (declare as absolute_symbols of a unit on
 * resource_3a8:02001f14):
 *   Engine_ActorGet = 0x0200bc1c (thumb)
 *   Engine_ActorEnableActionCallback = 0x0200bc34 (thumb)
 *   KareiMachi_DanceStep = 0x0200d144 (data)
 *   KareiMachi_DanceScriptA = 0x0200bec0 (data)
 *   KareiMachi_DanceScriptB = 0x0200bfb0 (data)
 */
extern s32 KareiMachi_DanceStep;
extern u8 KareiMachi_DanceScriptA[];
extern u8 KareiMachi_DanceScriptB[];

u8 * Engine_ActorGet();
void Engine_ActorEnableActionCallback();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* NONMATCHING: 244 of 236 bytes, 15 halfword edits (2026-09-24). Alternates
 * the dance callbacks of actors 20 and 21. The two script words the reference
 * keeps after the owner (0x02002000..0x02002008 in the listing) are this
 * function's literal pool, so the reviewed extent must grow to 244; the zero
 * stored at +100 is shared in r5 here where the reference rebuilds it. */
void Func_02001f14(void)
{
    switch (KareiMachi_DanceStep) {
    case 0:
        {
            s32 shown = 0;

            *(u16 *)(Engine_ActorGet(21) + 100) = shown;
        }
        Engine_ActorEnableActionCallback(21, KareiMachi_DanceScriptA);
        KareiMachi_DanceStep++;
        break;
    case 1:
        if (*(s16 *)(Engine_ActorGet(21) + 100) != 0) {
            {
                s32 shown = 0;

                *(u16 *)(Engine_ActorGet(20) + 100) = shown;
            }
            Engine_ActorEnableActionCallback(20, KareiMachi_DanceScriptB);
            KareiMachi_DanceStep++;
        }
        break;
    case 2:
        if (*(s16 *)(Engine_ActorGet(20) + 100) != 0) {
            {
                s32 shown = 0;

                *(u16 *)(Engine_ActorGet(20) + 100) = shown;
            }
            Engine_ActorEnableActionCallback(20, KareiMachi_DanceScriptA);
            KareiMachi_DanceStep++;
        }
        break;
    case 3:
        if (*(s16 *)(Engine_ActorGet(20) + 100) != 0) {
            {
                s32 shown = 0;

                *(u16 *)(Engine_ActorGet(21) + 100) = shown;
            }
            Engine_ActorEnableActionCallback(21, KareiMachi_DanceScriptB);
            KareiMachi_DanceStep++;
        }
        break;
    case 4:
        if (*(s16 *)(Engine_ActorGet(21) + 100) != 0) {
            KareiMachi_DanceStep = 0;
        }
        break;
    }
}
