typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef enum {
    FALSE,
    TRUE
} bool;

#ifndef CHANGED_TYPE
#define CHANGED_TYPE u32
#endif
#ifndef VALUE_TYPE
#define VALUE_TYPE s32
#endif
#ifndef HANDLE_TYPE
#define HANDLE_TYPE s32
#endif
#ifndef SELECTION_TYPE
#define SELECTION_TYPE s32
#endif
#ifndef KEY_TYPE
#define KEY_TYPE s32
#endif
#ifndef FUNCTION_ARG_TYPE
#define FUNCTION_ARG_TYPE s32
#endif
#ifndef FUNCTION_RETURN_TYPE
#define FUNCTION_RETURN_TYPE s32
#endif
#ifndef RET_08015080
#define RET_08015080 void
#endif
#ifndef RET_080a1ac0
#define RET_080a1ac0 void
#endif
#ifndef RET_080a1a40
#define RET_080a1a40 void
#endif
#ifndef RET_080f9010
#define RET_080f9010 void
#endif
#ifndef RET_080030f8
#define RET_080030f8 void
#endif
#ifndef RET_08015018
#define RET_08015018 void
#endif

extern s32 Func_08015010();
extern s32 Func_08077018();
extern RET_08015080 Func_08015080();
extern RET_080a1ac0 Func_080a1ac0();
extern RET_080a1a40 Func_080a1a40();
extern s32 Func_080770c0();
extern s32 Func_080022fc();
extern RET_080f9010 Func_080f9010();
extern RET_080030f8 Func_080030f8();
extern RET_08015018 Func_08015018();
extern char Value_00000182;
extern char Value_00000ad4;
extern char Value_00000b2c;
extern volatile KEY_TYPE Data_03001b04;
extern volatile KEY_TYPE Data_03001c94;

FUNCTION_RETURN_TYPE Func_080a524c(FUNCTION_ARG_TYPE arg0)
{
    register VALUE_TYPE value;
    register HANDLE_TYPE handle;

    handle = Func_08015010(13, 3, 17, 10, 2);
    value = arg0 & 0x1ff;
    Func_08077018(value);
    value += (s32)&Value_00000182;
    Func_08015080(value, handle, 24, 0);
    value = (s32)&Value_00000ad4;
    Func_08015080(value, handle, 0, 16);
    value++;
    Func_08015080(value, handle, 0, 24);
    value = (s32)&Value_00000b2c;
    Func_08015080(value, handle, 24, 40);
    value++;
    Func_08015080(value, handle, 24, 56);

    {
    register CHANGED_TYPE changed;
    register SELECTION_TYPE selection;
    register volatile KEY_TYPE *keys;
    register volatile KEY_TYPE *confirm;
    selection = 1;
    changed = selection;
    Func_080a1ac0(104, 86);
    goto poll;

update:
    {
    KEY_TYPE key_value;
    Func_080a1a40(104, selection * 16 + 70);
    keys = &Data_03001b04;
    key_value = *keys;
    if (key_value & 0x40) {
        selection--;
        changed = 1;
        Func_080f9010(111);
    }
    if (*keys & 0x80) {
        selection++;
        changed = 1;
        Func_080f9010(111);
    }
    Func_080030f8(1);
    }

poll:
    if (Func_080770c0(0x150) == 0) {
        CHANGED_TYPE current_changed = changed;
        if (current_changed != 0) {
            changed = 0;
            selection = Func_080022fc(selection + 2, 2);
        }
        confirm = &Data_03001c94;
        if (*confirm & 1) {
            Func_080f9010(112);
            goto done;
        }
        if (*confirm & 2) {
            Func_080f9010(113);
            selection = 1;
            goto done;
        }
        goto update;
    }

done:
    if (Func_080770c0(0x150) != 0)
        selection = 1;
    Func_08015018(handle, 1);
    return selection;
    }
}
