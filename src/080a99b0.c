typedef signed int s32;

s32 Func_080a99b0(s32 *arg0, s32 *arg1, s32 arg2)
{
    s32 var_r4;
    s32 var_r3;

    var_r3 = *arg0;
    var_r4 = *arg1;
    switch (arg2) {
    case 0x40:
        var_r4 -= 1;
        if (var_r4 < 0) {
            var_r4 = 5;
        }
        if (var_r4 <= 3) {
            if (var_r4 == 3) {
                if (var_r3 <= 4) {
                    var_r3 = 0;
                } else {
                    goto set_one;
                }
            } else if (var_r3 > 1) {
set_one:
                var_r3 = 1;
            }
            if ((var_r4 == 3) && (var_r3 == 1)) {
                var_r4 = 2;
            }
        }
        break;
    case 0x80:
        var_r4 += 1;
        if (var_r4 > 5) {
            var_r4 = 0;
        }
        if ((var_r4 == 3) && (var_r3 == 1)) {
            var_r4 = 4;
        }
        if (var_r4 == 4) {
            goto set_zero;
        }
        break;
    case 0x20:
        var_r3 -= 1;
        if (var_r4 == 3) {
            var_r3 += 1;
        } else if (var_r4 > 3) {
            if (var_r3 < 0) {
                var_r3 = 7;
            }
        } else if (var_r3 < 0) {
            var_r3 = 1;
        }
        break;
    case 0x10:
        var_r3 += 1;
        if (var_r4 == 3) {
            var_r3 -= 1;
        } else if (var_r4 > 3) {
            if (var_r3 > 7) {
                goto set_zero;
            }
        } else if (var_r3 > 1) {
            goto set_zero;
        }
        break;
set_zero:
        var_r3 = 0;
        break;
    }
    *arg0 = var_r3;
    *arg1 = var_r4;
    return (var_r4 * 9) + var_r3;
}
