// Three actor scripts that four Mars Lighthouse rooms keep a copy of, in this order: a scripted
// part's, and the crumbs' and embers', which are the same.

// A scripted part waits half a second, is placed at 0, 0, 0 and released.
Sint32 ScriptedPartScript[] = {
	SC_WAIT(30),
	SC_SET(SW_HOOK, 0),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_RELEASE,
};

// A crumb (scActorScatterCrumbs) waits 20 ticks, flickers for ten pairs of ticks and is released.
Uint32 CrumbsScript[] = {
	SC_WAIT(20),
	SC_LABEL(1),
	SC_SET(SW_SPRITE_KIND, ACTOR_SPRITE_NONE),
	SC_WAIT(1),
	SC_SET(SW_SPRITE_KIND, ACTOR_SPRITE_ONE),
	SC_WAIT(1),
	SC_LOOP(10, SC_TO(1)),
	SC_RELEASE,
};

// An ice chip (scIceChipsSpawn) does the same.
Uint32 IceChipsScript[] = {
	SC_WAIT(20),
	SC_LABEL(1),
	SC_SET(SW_SPRITE_KIND, ACTOR_SPRITE_NONE),
	SC_WAIT(1),
	SC_SET(SW_SPRITE_KIND, ACTOR_SPRITE_ONE),
	SC_WAIT(1),
	SC_LOOP(10, SC_TO(1)),
	SC_RELEASE,
};
