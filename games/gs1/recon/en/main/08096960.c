#include "types.h"

struct BattleEffectRequest {
    u8 reserved_000[0x14];
    void *object;
    s16 source_id;
    s16 target_id;
    u8 reserved_01c[2];
    s16 battle_mode;
    u8 running;
};

struct BattleEffectState {
    u8 reserved_000[0xcb8];
    s16 active;
};

struct BattleEffectGlobals {
    u8 reserved_000[0x24a];
    s16 selected_id;
};

extern struct BattleEffectRequest *Data_03001f30;
extern struct BattleEffectGlobals Data_02000240;

void Func_08097c3c(s32);
void Func_08097540(s32, s32);
void Func_080994c0(s32);
void Func_08098ccc(s32);
void Func_080999e4(s32);
void Func_08099128(s32);
void Func_08098848(s32);
void Func_080983a0(void);
void Func_08099838(void);
void Func_08099da4(s32);
void Func_080985a8(s32);
void Func_0809ae58(s32);
void Func_0809a8b8(s32);
void Func_0809b208(void);
void Func_0809b698(void);
void Func_080984c0(void);
void Func_0809ade8(s32);
void Func_0809ad90(s32);
void Func_0809ab98(s32);

void Func_08096960(void)
{
    struct BattleEffectRequest *request = Data_03001f30;
    struct BattleEffectState *battle = *(struct BattleEffectState **)((u8 *)&Data_03001f30 - 0x74);
    s32 battle_mode = request->battle_mode;
    s32 target_id = request->target_id;

    request->running = 0;
    switch (battle_mode) {
    case 1:
        Func_08097c3c(target_id);
        break;
    case 2:
        if (battle->active != 0)
            Func_080984c0();
        if (Data_02000240.selected_id != target_id)
            *(u8 *)((u8 *)request->object + 91) = 1;
        Func_08097540(request->source_id, target_id);
        break;
    case 3:
        Func_080994c0(target_id);
        break;
    case 4:
        Func_08098ccc(target_id);
        break;
    case 5:
        Func_080999e4(target_id);
        break;
    case 6:
        Func_08099128(target_id);
        break;
    case 7:
        Func_08098848(target_id);
        break;
    case 8:
        Func_080983a0();
        break;
    case 9:
        if (Data_02000240.selected_id != -1) {
            Func_0809ade8(Data_02000240.selected_id);
            Data_02000240.selected_id = -1;
        }
        Func_0809ad90(target_id);
        Data_02000240.selected_id = target_id;
        Func_0809ab98(target_id);
        break;
    case 10:
        Func_08099838();
        break;
    case 11:
        Func_08099da4(target_id);
        break;
    case 12:
        Func_080985a8(target_id);
        break;
    case 13:
        Func_0809ae58(target_id);
        break;
    case 14:
        Func_0809a8b8(target_id);
        break;
    case 15:
        Func_0809b208();
        break;
    case 16:
        Func_0809b698();
        break;
    }
}
