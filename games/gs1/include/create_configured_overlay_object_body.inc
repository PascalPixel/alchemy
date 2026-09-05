    u8 *result = CreateOverlayObject(arg3, arg0, arg1, arg2);

    if (result != NULL) {
        u8 *object = *(u8 **)(result + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = object[9];
        mask = -mask;
        mask &= flags;
        mask |= 4;
        object[9] = mask;
        result[0x55] = 0;
        result[0x59] = 8;
        SetOverlayObjectMode(result, 0);
        SetOverlayObjectSlot(result, 15);
        result[0x23] = (result[0x23] & 0xfe) | 2;
        return result;
    }
    return NULL;
