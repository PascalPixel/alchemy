/* FarCall_MenuTable: one entry per 8-byte stub, in table order. */
FAR_CALL(RunAssetSelectionScreenFar, RunAssetSelectionScreen, s32, (void))
FAR_CALL(Menu_OpenConfirmPromptFar, Menu_OpenConfirmPrompt, s32, (void))
FAR_CALL(Menu_OpenActionFlow, ActionMenu_Open, s32, (void))
FAR_CALL(Menu_OpenCharacterSelectorFar, Menu_OpenCharacterSelector, s32, (void))
FAR_SLOT(UiIcon_CreateWithResourceVariant)
FAR_CALL(PsynergyMenu_InitializeEntryObjectsFar, PsynergyMenu_InitializeEntryObjects, void,
    (void *source, s32 originX, s32 originY, s32 spacing, s32 unused))
FAR_CALL(Menu_ReleaseEntryObjectsFar, Menu_ReleaseEntryObjects, void, (void))
FAR_CALL(UiIcon_PrepareObjectFar, UiIcon_PrepareObject, void, (struct RenderOutput * object))
FAR_CALL(ItemMenu_RunFar, ItemMenu_Run, s32, (void))
FAR_CALL(ItemMenu_DrawItemDetailsFar, ItemMenu_DrawItemDetails, void, (s32 window, s32 item))
