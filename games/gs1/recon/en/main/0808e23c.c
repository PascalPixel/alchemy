#include "types.h"
#include "item.h"
#include "battle_effect_runtime.h"

/*
 * Evidence summary (see games/gs1/recon/en/main/0808e23c.json for the full
 * account):
 *
 * - The packed-argument decode (item_id = arg & 0x3ff, actor = (arg>>10)&0xf)
 *   is byte-identical to main:0808e680 (BattleCommand_ExecuteSelectedAction),
 *   which decodes the same way for an ability id. This owner calls
 *   Item_GetData (item.h), not Ability_GetData, so the low field is an item
 *   id, not an ability id; this is the item-use sibling of that dispatcher.
 * - Func_0808e14c (called here with item_id) walks the same
 *   runtime->events[] table as the exact-adjacent BattleEffect_FindMatchingEvent
 *   (main:0808e4b4), filtering kind==4 (item) where 0808e4b4 filters kind==5
 *   (ability). Its 12-byte record shape (s32 flags, u16 metadata, s16 unk,
 *   u32 effect) is taken from that sibling.
 * - The action_id==149 confirm-prompt block reproduces 0808e680's
 *   actionId==0x95(=149) block instruction-for-instruction: same
 *   Data_02000240+0x400/0x402 <- +0x480/0x482 u16 copy, same
 *   runtime->+0x170 = 999 sentinel, same Func_08091d84(1) prompt call.
 * - Data_02000240 is BattleWork (battle_effect_runtime.h); its object_id
 *   field (0x1f4) and target-id byte array (0x1f8) are already named in
 *   apply_drain.c/apply_health_delta.c/apply_status_damage.c
 *   (games/gs1/src/battle/party/). This owner primes the same table via an
 *   unconditional Func_08077148(actor) call before branching on actor==15,
 *   matching that priming idiom.
 * - Data_03001ebc is BattleRuntime (battle_effect_runtime.h), documented up
 *   to offset 0xcc4. This owner is the first known reader of 0x170 and
 *   0xcc6, both past what the shared header currently names, so they are
 *   read/written through raw offsets here rather than growing the header
 *   for one caller.
 * - Func_0808ddec, Func_08092b94, Func_08092f84, Func_08091750 have no
 *   evidence-backed names yet; kept as Func_ADDR with argument types taken
 *   from register roles only.
 */

extern struct BattleWork Data_02000240;
extern struct BattleRuntime *Data_03001ebc;

struct BattleUnitObject {
    u8 unknown_000[0xd8];
    u16 abilities[15]; /* 0xd8: object+216, masked 0x1ff, matches
                           shop/select_use_item.c's Ability_GetAvailability scan. */
};

struct BattleItemEventRecord {
    s32 flags;
    u16 metadata;
    s16 unknown_06;
    s32 effect; /* raw effect id when < 0x10000, else a callback pointer
                   dispatched through the __call_via_r3 veneer below (the
                   reference compares this signed: bge, not bcs). */
};

struct BattleUnitObject *Runtime_GetObject(s32 actor);
s32 Func_08077148(s32 actor);
struct BattleItemEventRecord *Func_0808e14c(s32 item_id);
void GameFlag_Clear(s32 flag);
void GameFlag_Set(s32 flag);
s32 GameFlag_IsSet(s32 flag);
void UiText_DrawQuantity(s32 value, s32 mode);
void UiText_DrawMessage(s32 message, s32 mode);
s32 Func_08091d84(s32 mode);
void UiWork_FinalizePending(void);
s32 Func_0808ddec(s32 object_id);
void BattleRuntime_Reset(void);
void Func_08092b94(s32 effect_id);
void Func_08092f84(s32 value, s32 flag);
void Func_08091750(void);
void Func_08096fb0(s32 action_id, s32 mode);
void Func_08096810(void); /* RunBattleEffect (battle/effects/run_effect.c) */
void Func_08097194(void); /* named BattleEffect_CleanupSceneObjects in source-paths.json */
#define BattleEffect_CleanupSceneObjects Func_08097194
u8 Func_08077058(s32 actor, s32 slot);

/*
 * __call_via_r3 veneer at 0x080072e4+4*3: an indirect call through whatever
 * pointer is in r3 at the call site (here, event->effect once proven
 * >= 0x10000), modeled per project convention as a direct call with the
 * target as a trailing argument.
 */
void Func_080072f0(s32 item_id, s32 actor, s32 slot, u32 target);

/*
 * Evidence-backed but unproven name (see the evidence summary above),
 * registered in source-paths.json. The retained games/gs1/asm/0808e23c.s
 * only exposes the Func_0808e23c label (no dual canonical-name label, unlike
 * its exact-adjacent siblings), so `compiler workbench` needs
 * `--owner 0x0808e23c --size 632` rather than resolving this name.
 */
#define BattleCommand_ExecuteSelectedItem Func_0808e23c

s32 BattleCommand_ExecuteSelectedItem(s32 arg, s32 slot)
{
    s32 result;
    s32 item_id;
    s32 actor;
    s32 count;
    s32 best;
    struct BattleUnitObject *obj;
    struct BattleItemEventRecord *event;
    u16 *p;
    s32 j;
    s32 matches;

    result = -1;
    item_id = arg & 0x3ff;
    actor = (arg >> 10) & 0xf;
    count = Func_08077148(actor);
    best = 0;

    if (actor == 15) {
        if (count > 0) {
            u8 *ids = (u8 *)&Data_02000240 + 0x1f8;
            s32 i = 0;
            actor = 0;
            do {
                obj = Runtime_GetObject(ids[i]);
                matches = 0;
                p = obj->abilities;
                j = 14;
                do {
                    if ((*p++ & 0x1ff) == item_id)
                        matches++;
                    j--;
                } while (j >= 0);

                if (best < matches) {
                    best = matches;
                    actor = ids[i];
                }
                i++;
            } while (i < count);
        }
    } else {
        obj = Runtime_GetObject(actor);
        p = obj->abilities;
        j = 14;
        do {
            if ((*p++ & 0x1ff) == item_id)
                best++;
            j--;
        } while (j >= 0);
    }

    if (best == 0) {
        UiText_DrawMessage(0x927, 1);
        return -1;
    }

    event = Func_0808e14c(item_id);
    if (event != 0 && event->effect != 0) {
        GameFlag_Clear(0x143);
        GameFlag_Clear(0x142);
        if (!(event->metadata & 0x400)) {
            UiText_DrawQuantity(actor, 1);
            UiText_DrawQuantity(item_id, 2);
            UiText_DrawMessage(0x91c, 1);
        }
        if (event->effect < 0x10000) {
            s32 objref = Func_0808ddec(Data_02000240.object_id);
            BattleRuntime_Reset();
            Func_08092b94(event->effect);
            Func_08092f84(objref, 0);
            Func_08091750();
        } else {
            Func_080072f0(item_id, actor, slot, event->effect);
        }
        result = 0;
    } else {
        s32 action_id;
        u8 *rt;

        GameFlag_Clear(0x143);
        GameFlag_Set(0x142);
        action_id = Item_GetData(item_id)->action_id;
        rt = (u8 *)Data_03001ebc;

        if (action_id != 0) {
            GameFlag_Set(0x145);
            GameFlag_Clear(0x142);

            if (action_id == 149 && !GameFlag_IsSet(0x144)) {
                s32 declined;

                UiText_DrawQuantity(item_id, 2);
                UiText_DrawMessage(0x924, 13);
                declined = Func_08091d84(1);
                UiWork_FinalizePending();
                if (declined != 0)
                    return 0;

                *(u16 *)((u8 *)&Data_02000240 + 0x400) =
                    *(u16 *)((u8 *)&Data_02000240 + 0x480);
                *(u16 *)((u8 *)&Data_02000240 + 0x402) =
                    *(u16 *)((u8 *)&Data_02000240 + 0x482);
                *(s16 *)(rt + 0x170) = 999;
            }

            UiText_DrawQuantity(actor, 1);
            UiText_DrawQuantity(item_id, 2);
            UiText_DrawMessage(0x91c, 1);
            Func_08096fb0(action_id, 0);
            rt = rt + 0xcc6;
            *rt = 1;
            Func_08096810();
            *rt = 0;
            BattleEffect_CleanupSceneObjects();

            if (Item_GetData(item_id)->use_type & 1)
                GameFlag_Set(0x143);
        }
    }

    if (GameFlag_IsSet(0x142))
        UiText_DrawMessage(0x927, 1);
    if (GameFlag_IsSet(0x143))
        Func_08077058(actor, slot);
    return result;
}
