# The Lost Age: window

One file of The Lost Age's [window](../../../tbs/glossary.md#window) code, `glow_update_sine.c`:
`PaletteGlow_UpdateSine`, which pulses four colours of background palette 14
along a sine. Its twin in The Broken Seal is `FileList_UpdatePulse` in
[screens/save.c](../../../tbs/src/window/screens/save.c), part of the window
system [Windows](../../../tbs/src/window/README.md) explains.

## What differs from The Broken Seal

- The rest of this game's window system, the windows and the message decoder,
  is under `games/tla/src/graphics/window/` and `games/tla/src/graphics/text/`
  (see [The Lost Age: graphics](../graphics/README.md)).
