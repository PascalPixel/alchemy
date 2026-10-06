    struct FieldActor *result = CreateOverlayObject(arg3, arg0, arg1, arg2);

    if (result != NULL) {
        result->sprite->priority = 1;
        result->motion_flags = 0;
        result->collision_flags = 8;
        SetOverlayObjectMode(result, 0);
        SetOverlayObjectSlot(result, 15);
        result->priority_flags = (result->priority_flags & 0xfe) | 2;
        return result;
    }
    return NULL;
