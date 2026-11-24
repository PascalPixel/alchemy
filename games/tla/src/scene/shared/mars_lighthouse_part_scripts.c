/// @file
/// Three actor scripts that four Mars Lighthouse rooms keep a copy of, in this
/// order: a scripted part's, and the crumbs' and embers', which are the same.

/// A scripted part waits half a second, is placed at 0, 0, 0 and released.
s32 gScriptedPart_Script[] = {
    SC_WAIT(30), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0),
    SC_WAIT(1),  SC_RELEASE,
};

/// A crumb (Actor_ScatterCrumbs) waits 20 frames, flickers for ten pairs of
/// frames and is released.
u32 gCrumbs_Script[] = {
    SC_WAIT(20),
    SC_LABEL(1),
    SC_SET(SW_SPRITE_KIND, ACTOR_SPRITE_NONE),
    SC_WAIT(1),
    SC_SET(SW_SPRITE_KIND, ACTOR_SPRITE_ONE),
    SC_WAIT(1),
    SC_LOOP(10, SC_TO(1)),
    SC_RELEASE,
};

/// An ice chip (IceChips_Spawn) does the same.
u32 gIceChips_Script[] = {
    SC_WAIT(20),
    SC_LABEL(1),
    SC_SET(SW_SPRITE_KIND, ACTOR_SPRITE_NONE),
    SC_WAIT(1),
    SC_SET(SW_SPRITE_KIND, ACTOR_SPRITE_ONE),
    SC_WAIT(1),
    SC_LOOP(10, SC_TO(1)),
    SC_RELEASE,
};
