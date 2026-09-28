    s16 selector = gGameState.scene;

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData;
    }
    return (s32)DefaultOverlayData;
