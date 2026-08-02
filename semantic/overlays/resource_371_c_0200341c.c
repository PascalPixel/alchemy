typedef signed int s32;

/* Complete four-slot opposing-pair total helper. */
extern s32 Func_02003448(s32 slot);

s32 Func_0200341c(void)
{
    return Func_02003448(0) + Func_02003448(2)
         - Func_02003448(1) - Func_02003448(3);
}
