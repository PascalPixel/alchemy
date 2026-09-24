/* NONMATCHING: 352 of 352 bytes, 125 differing halfwords (2026-09-24). Unit
 * source for 39a:02001c18 with absolute symbols: Engine_ObjectCreate 0x0200a2b8,
 * Engine_ObjectSetAnimation 0x0200a2a0, Engine_ObjectSetScript 0x0200a2b0,
 * Engine_ObjectSetPalette 0x0200a390, Imiru_Divide 0x0200a258 (one veneer for
 * all three divides). Remaining: the reference keeps (flags & 15) << 2 in r5 and
 * reloads the script in the 0x40000 block, flags lives in r8 and extra in sl,
 * and the -13 field mask shares fp with a3; writing the table load twice spills
 * the index instead. */
#include "TYPES.H"

u8 *Engine_ObjectCreate(s32 kind, s32 x, s32 y, s32 z);
void Engine_ObjectSetAnimation(u8 *obj, s32 anim);
void Engine_ObjectSetScript(u8 *obj, u8 *script);
void Engine_ObjectSetPalette(u8 *obj, s32 palette);
/* The same IWRAM divide, reached through one import veneer per call site. */
s32 Imiru_Divide(s32 num, s32 den);

struct ScriptTable {
    u8 *script[3];
};

struct Sprite39a {
    u8 pad[9];
    u8 lo : 2;
    u8 layer : 2;
    u8 hi : 4;
};

void Func_02001c18(s32 x, s32 y, s32 z, s32 a3, s32 a4, s32 a5, s32 flags, u8 *extra)
{
    struct ScriptTable table;
    u8 *obj;
    struct Sprite39a *spr;
    u8 *script;
    s32 num;

    table = *(struct ScriptTable *)0x0200a418;
    obj = Engine_ObjectCreate(222, x, y, z);
    if (obj == 0)
        return;
    spr = *(struct Sprite39a **)(obj + 80);
    Engine_ObjectSetAnimation(obj, (flags + 1) & 15);
    script = table.script[flags & 15];
    Engine_ObjectSetScript(obj, script);
    obj[85] = 0;
    ((u8 *)spr)[38] = 0;
    *(s32 *)(obj + 108) = 0x2009bdd;
    *(s32 *)(obj + 68) = a3;
    *(s32 *)(obj + 72) = a4;
    *(s32 *)(obj + 76) = a5;
    *(s32 *)(obj + 48) = 0;
    *(s32 *)(obj + 52) = 0;
    spr->layer = 1;
    if ((flags & 0xffff0000) == 0 || extra == 0)
        return;
    if (flags & 0x10000)
        Engine_ObjectSetPalette(obj, *(s32 *)(extra + 4));
    if (flags & 0x20000) {
        obj[35] &= 0xfe;
        spr->layer = *extra;
    }
    if (flags & 0x80000) {
        *(s32 *)(obj + 24) = *(s32 *)(extra + 8);
        *(s32 *)(obj + 28) = *(s32 *)(extra + 12);
    }
    if (flags & 0x40000) {
        if (flags & 0x80000) {
            *(s32 *)(obj + 48) = Imiru_Divide(*(s32 *)(extra + 16) - *(s32 *)(obj + 24), *(s32 *)(script + 12));
            num = *(s32 *)(extra + 20) - *(s32 *)(obj + 28);
        } else {
            *(s32 *)(obj + 48) = Imiru_Divide(*(s32 *)(extra + 16) + -0x10000, *(s32 *)(script + 12));
            num = *(s32 *)(extra + 20) + -0x10000;
        }
        *(s32 *)(obj + 52) = Imiru_Divide(num, *(s32 *)(script + 12));
    }
}
