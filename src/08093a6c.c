typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

extern s16 Data_02000240[];
extern s32 Func_08092054(u32);
extern void Func_08009098(void *, void *);

void Func_08093a6c(s8 *object, s32 kind)
{
    s32 index;
    switch ((u32)(kind - 1)) {
    case 0:
        kind = 0x0809fe00;
        break;
    case 1:
        kind = 0x0809fd44;
        break;
    case 2:
        kind = 0x0809fe10;
        break;
    case 3:
        kind = 0x0809fecc;
        break;
    case 4:
        kind = 0x0809ff18;
        break;
    case 5:
        index = 250;
        *(s32 *)(object + 0x68) = Func_08092054(*(u32 *)&Data_02000240[index]);
        kind = 0x0809ff2c;
        break;
    case 6:
        kind = 0x0809fe04;
        break;
    default:
        break;
    }
    Func_08009098(object, (void *)kind);
}
