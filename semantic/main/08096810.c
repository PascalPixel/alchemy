#include "types.h"

struct SelectionContext {
    u8 padding_000[0x18];
    s16 sourceId;
    s16 targetId;
    u8 padding_01c[2];
    s16 mode;
};

struct BattleState {
    u8 padding_000[0xCB8];
    s16 active;
};

struct SharedState {
    u8 padding_000[0x1F4];
    s32 selectedObject;
    u8 padding_1f8[0x52];
    s16 selectedId;
};

extern struct SelectionContext *Data_03001f30;
extern struct SharedState Data_02000240;

void Func_0809802c(void);
void Func_08098954(void);
void Func_0809a294(void);
void Func_08098cd8(void);
void Func_080999f0(void);
void Func_0809a8c4(void);
void Func_08099160(void);
void Func_080994d0(void);
void Func_080985fc(void);
void Func_0809ae64(void);
void Func_0809ade8(s32 objectId);
s32 Func_0808df1c(s32 objectId, s32 mode);
s32 Func_0809ae3c(s32 objectId);
s32 Func_0808d5a4(s32 objectId);
void Func_080970f8(s32 selectedObject, s32 objectId);
void Func_0809ab98(s32 objectId);
void Func_0809ad90(s32 objectId);
void Func_0809abb4(void);
void Func_080984c0(void);
void Func_08097540(s32 sourceId, s32 targetId);
void Func_080983a0(void);
void Func_08099838(void);
void Func_0809b208(void);
void Func_0809b698(void);

void Func_08096810(void)
{
    struct SelectionContext *context;
    struct BattleState *battle;
    s32 mode;
    s32 targetId;
    s32 objectId;

    context = Data_03001f30;
    mode = context->mode;
    battle = *(struct BattleState **)((u8 *)&Data_03001f30 - 0x74);
    targetId = context->targetId;

    switch (mode) {
    case 1:
        Func_0809802c();
        return;
    case 7:
        Func_08098954();
        return;
    case 11:
        Func_0809a294();
        return;
    case 4:
        Func_08098cd8();
        return;
    case 5:
        Func_080999f0();
        return;
    case 14:
        Func_0809a8c4();
        return;
    case 6:
        Func_08099160();
        return;
    case 3:
        Func_080994d0();
        return;
    case 12:
        Func_080985fc();
        return;
    case 13:
        Func_0809ae64();
        return;
    case 9:
        if (Data_02000240.selectedId != -1) {
            Func_0809ade8(Data_02000240.selectedId);
            Data_02000240.selectedId = -1;
        }

        objectId = Func_0808df1c(Data_02000240.selectedObject, mode);
        objectId = Func_0809ae3c(objectId);
        if (Func_0808d5a4(objectId) != 0) {
            Func_080970f8(Data_02000240.selectedObject, objectId);
            Func_0809ab98(objectId);
            Func_0809ad90(objectId);
            Data_02000240.selectedId = objectId;
        } else {
            Func_0809abb4();
        }
        return;
    case 2:
        if (battle->active != 0)
            Func_080984c0();
        Func_08097540(context->sourceId, targetId);
        return;
    case 8:
        Func_080983a0();
        return;
    case 10:
        Func_08099838();
        return;
    case 15:
        Func_0809b208();
        return;
    case 16:
        Func_0809b698();
        return;
    }
}
