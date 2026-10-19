/// @file
/// Title, introduction and splash-screen presentation.
/// Far-call interfaces; their ordered jump stubs live in
/// games/tbs/src/system/far_call/PALETTE.S.

/// Far call to Title_ShowIntro.
s32 Title_ShowIntroFar(s32 prompt);
/// Far call to Title_ShowSplashScreen.
s32 Title_ShowSplashScreenFar(s32 mode);
/// Far call to Title_ShowAnimatedSplash.
s32 Title_ShowAnimatedSplashFar(void);
