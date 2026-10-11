/* FarCall_PaletteTable: one entry per 8-byte stub, in table order. */
FAR_CALL(Title_ShowIntroFar, Title_ShowIntro, s32, (s32 prompt))
FAR_SLOT(Graphics_NoOpColor)
FAR_SLOT(Title_NoOpPalette)
FAR_CALL(Title_ShowSplashScreenFar, Title_ShowSplashScreen, s32, (s32 mode))
FAR_CALL(Title_ShowAnimatedSplashFar, Title_ShowAnimatedSplash, s32, (void))
