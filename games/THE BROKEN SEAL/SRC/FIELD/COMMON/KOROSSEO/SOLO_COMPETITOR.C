#include "TYPES.H"

extern s32 Data_02000240_t[][1];
extern u8 Korosseo_ZeroWord[];
void Engine_PartyRemoveActiveOwner(s32 id);
void Engine_PartyAddActiveOwner(s32 id);
void Engine_ObjectAttachWorkTarget(s32 id, s32 target);
u8 *Engine_OwnerGetState(s32 id);
s32 Engine_MathDivide(s32 dividend, s32 divisor);
void Engine_EventClearInvalidPackedValues(void);

/* Colosso: leave only the competitor id in the active party, remember it,
 * and seed its two position ratios, each clamped to 0..0x4000 and kept
 * nonzero while its source coordinate is nonzero. The same function sits in
 * each of the three Colosso trial overlays. */
void Korosseo_SelectSoloCompetitor(s32 id)
{
    u8 *rec;
    s32 v;
    s32 t;

    Engine_PartyRemoveActiveOwner(0);
    Engine_PartyRemoveActiveOwner(1);
    Engine_PartyRemoveActiveOwner(2);
    Engine_PartyRemoveActiveOwner(3);
    Engine_PartyRemoveActiveOwner(5);
    Engine_PartyAddActiveOwner(id);
    Data_02000240_t[125][0] = id;
    Engine_ObjectAttachWorkTarget(id, 0);
    rec = Engine_OwnerGetState(id);
    {
        /* FAKEMATCH: the zero byte is a HImode pool constant; its short pool
         * range dumps the literal pool at the first clamp branch. */
        s32 z = (u16)(u32)Korosseo_ZeroWord;

        *(u16 *)(rec + 56) = *(u16 *)(rec + 52);
        *(u16 *)(rec + 58) = *(u16 *)(rec + 54);
        rec[0x131] = z;
    }
    v = Engine_MathDivide(*(s16 *)(rec + 56) << 14, *(s16 *)(rec + 52));
    t = 0x4000;
    if (v <= 0x4000) {
        t = 0;
        if (v >= 0)
            t = v;
    }
    *(u16 *)(rec + 20) = t;
    if ((u16)t == 0 && *(s16 *)(rec + 56) != 0) {
        /* FAKEMATCH: a word temporary keeps 1 out of the HImode pool. */
        s32 one = 1;

        *(u16 *)(rec + 20) = one;
    }
    v = Engine_MathDivide(*(s16 *)(rec + 58) << 14, *(s16 *)(rec + 54));
    t = 0x4000;
    if (v <= 0x4000) {
        t = 0;
        if (v >= 0)
            t = v;
    }
    *(u16 *)(rec + 22) = t;
    if ((u16)t == 0 && *(s16 *)(rec + 58) != 0) {
        /* FAKEMATCH: a word temporary keeps 1 out of the HImode pool. */
        s32 one = 1;

        *(u16 *)(rec + 22) = one;
    }
    Engine_EventClearInvalidPackedValues();
}
