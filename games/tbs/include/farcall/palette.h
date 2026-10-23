/// @file
/// Title, introduction and splash-screen presentation.

/// Plays the title intro into the title picture; with prompt set, "Press Start"
/// pulses. Returns 1 when A or Start was pressed on it.
s32 Title_ShowIntroFar(s32 prompt);
/// Shows the Nintendo logo with its jingle; mode 0 only fades in and holds.
/// Returns -1 when A or Start skipped it.
s32 Title_ShowNintendoLogoFar(s32 mode);
/// Shows the Camelot logo for two seconds, cycling its four frames. Returns 0.
s32 Title_ShowCamelotLogoFar(void);
