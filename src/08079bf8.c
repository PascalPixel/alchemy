typedef signed int s32;

s32 Func_08079b24(s32 selector, s32 mode);

s32 Func_08079bf8(s32 amount, s32 reduction, s32 adjustment, s32 selector)
{
    s32 result;
    s32 product;

    amount -= reduction;
    result = selector;
    if (amount < 0) {
        amount = 0;
    }
    product = Func_08079b24(result, 1) * (amount + (adjustment * 2));
    if (product < 0) {
        product += 0x1FF;
    }
    result = product >> 9;
    if (result < 0) {
        result = 0;
    }
    return result;
}
