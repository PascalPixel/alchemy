/* Draft, not exact (2026-09-26): 640 of 632 bytes, 263 differing halfwords.
   Normalized edit distance: 122 halfwords. Typed party/runtime views recover
   base-plus-offset accesses and shared zero/one lifetimes. Confirmation
   copies use +0x240/+0x242 and +0x1c0/+0x1c2. Remaining: inventory loop
   lifetimes, runtime base/flag allocation and constant selection. */
#include "TYPES.H"
extern u8 Value_000003ff;
extern u8 Value_000001f8;
extern u8 Value_000001ff;
extern u8 Value_00000927;
extern u8 Value_00000143;
extern u8 Value_00000142;
extern u8 Value_00000400;
extern u8 Value_0000091c;
extern u8 Value_00000145;
extern u8 Value_00000144;
extern u8 Value_00000924;
extern u8 Value_00000480;
extern u8 Value_00000402;
extern u8 Value_00000482;
extern u8 Value_00000170;
extern u8 Value_000003e7;
extern u8 Value_00000cc6;
#include "ITEM.H"
#include "BATTLE_EFFECT_RUNTIME.H"

/*
 * Evidence summary (see recon/tbs/en/dossiers.json#main:0808e23c for the full
 * account):
 *
 * - The packed-argument decode (item_id = arg & 0x3ff, actor = (arg>>10)&0xf)
 *   is byte-identical to main:0808e680 (BattleCommand_ExecuteSelectedAction),
 *   which decodes the same way for an ability id. This owner calls
 *   Item_GetData (item.h), not Ability_GetData, so the low field is an item
 *   id, not an ability id; this is the item-use sibling of that dispatcher.
 * - Func_0808e14c (called here with item_id) walks the same
 *   runtime->events[] table as the exact-adjacent BattleFx_FindMatchingEvent
 *   (main:0808e4b4), filtering kind==4 (item) where 0808e4b4 filters kind==5
 *   (ability). Its 12-byte record shape (s32 flags, u16 metadata, s16 unk,
 *   u32 effect) is taken from that sibling.
 * - The action_id==149 confirm-prompt block reproduces 0808e680's
 *   actionId==0x95(=149) block instruction-for-instruction: same
 *   Data_02000240+0x1c0/0x1c2 <- +0x240/0x242 u16 copy, same
 *   runtime->+0x170 = 999 sentinel, same Func_08091d84(1) prompt call.
 * - Data_02000240 is BattleWork (battle_effect_runtime.h); its object_id
 *   field (0x1f4) and target-id byte array (0x1f8) are already named in
 *   apply_drain.c/apply_health_delta.c/apply_status_damage.c
 *   (games/THE BROKEN SEAL/src/battle/party/). This owner primes the same table via an
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
                           shop/sel/use.c's Ability_GetAvailability scan. */
};

struct ItemPartyView {
    u8 unknown_000[0x1f4];
    s32 object_id;
    u8 active_owners[8];
};

struct ItemCommandRuntime {
    u8 unknown_000[0x170];
    s16 result_code;
    u8 unknown_172[0xb54];
    u8 resolving_action;
};

typedef s32 (*BattleItemCallback)(s32 item, s32 actor, s32 slot);

union BattleItemEffect {
    s32 id;
    BattleItemCallback callback;
};

struct BattleItemEventRecord {
    s32 flags;
    u16 metadata;
    s16 unknown_06;
    union BattleItemEffect effect; /* The ID/pointer threshold is signed. */
};

/* Declared void *-returning: main:0808e680 (BattleCommand_ExecuteSelectedAction)
 * shares this symbol through a different local view (struct BattleUnitRecord,
 * for the pp field); both callers cast the shared pointer to their own
 * struct locally. */
void *Runtime_GetObject(s32 actor);
s32 Func_08077148(s32 actor);
/* Declared s32-returning with a u16 parameter to match the prototype in the
 * exact owner runtime_owner_135.c, which calls this symbol without defining
 * it; the result is cast back to struct BattleItemEventRecord * here. */
s32 Func_0808e14c(u16 item_id);
void GameFlag_Clear(s32 flag);
void GameFlag_Set(s32 flag);
s32 GameFlag_IsSet(s32 flag);
void UiText_DrawQuantity(s32 value, s32 mode);
void UiText_DrawMessage(s32 message, s32 mode);
s32 Func_08091d84(s32 mode);
void UiWork_FinalizePending(void);
s32 Func_0808ddec(s32 object_id);
void Battle_Reset(void);
void Func_08092b94(s32 effect_id);
void Func_08092f84(s32 value, s32 flag);
void Func_08091750(void);
void Func_08096fb0(s32 action_id, s32 mode);
void Func_08096810(void); /* RunBattleEffect (battle/effects/run/run_effect.c) */
void Func_08097194(void); /* named BattleFx_CleanupSceneObjects in source-paths.json */
u8 Func_08077058(s32 actor, s32 slot);
struct ItemDefinition *Item_GetData(s32 item);

s32 BattleCommand_ExecuteSelectedItem(s32 arg, s32 slot)
{
    s32 result;
    s32 item_id;
    s32 actor;
    struct BattleUnitObject *obj;
    struct BattleItemEventRecord *event;
    u16 *p;
    s32 j;
    s32 matches;

    result = -1;
    item_id = arg & 0x3ff;
    actor = (arg >> 10) & 0xf;
    {
        s32 count;
        s32 best = 0;

        count = Func_08077148(actor);

        if (actor == 15) {
            s32 i;

            actor = 0;
            i = 0;
            if (actor < count) {
                struct ItemPartyView *work =
                    (struct ItemPartyView *)&Data_02000240;
                do {
                    obj = (struct BattleUnitObject *)Runtime_GetObject(work->active_owners[i]);
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
                        actor = work->active_owners[i];
                    }
                    i++;
                } while (i < count);
            }
        } else {
            obj = (struct BattleUnitObject *)Runtime_GetObject(actor);
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
    }

    event = (struct BattleItemEventRecord *)Func_0808e14c(item_id);
    if (event != 0 && event->effect.id != 0) {
        GameFlag_Clear(0x143);
        GameFlag_Clear(0x142);
        if (!(event->metadata & (s32)&Value_00000400)) {
            UiText_DrawQuantity(actor, 1);
            UiText_DrawQuantity(item_id, 2);
            UiText_DrawMessage((s32)&Value_0000091c, 1);
        }
        if (event->effect.id < 0x10000) {
            s32 objref = Func_0808ddec(Data_02000240.object_id);
            Battle_Reset();
            Func_08092b94(event->effect.id);
            Func_08092f84(objref, 0);
            Func_08091750();
        } else {
            event->effect.callback(item_id, actor, slot);
        }
        result = 0;
    } else {
        s32 action_id;
        struct ItemCommandRuntime *rt;

        GameFlag_Clear((s32)&Value_00000143);
        GameFlag_Set(0x142);
        action_id = Item_GetData(item_id)->action_id;
        rt = (struct ItemCommandRuntime *)Data_03001ebc;

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

                {
                    u16 *work = (u16 *)&Data_02000240;
                    s32 a, b;
                    a = work[288];
                    work[224] = a;
                    b = work[289];
                    work[225] = b;
                }
                rt->result_code = (s32)&Value_000003e7;
            }

            UiText_DrawQuantity(actor, 1);
            UiText_DrawQuantity(item_id, 2);
            UiText_DrawMessage(0x91c, 1);
            Func_08096fb0(action_id, 0);
            rt->resolving_action = 1;
            Func_08096810();
            rt->resolving_action = 0;
            Func_08097194();

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
