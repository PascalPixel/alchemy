/* One overlay of The Broken Seal, loaded at 0x2008000: ld_script.ld includes
   this file when the build defines OVERLAY as the overlay's number, and the
   block for that number is the overlay. The build finds the overlays from the
   lines that test OVERLAY, and each overlay's objects from its statements.

   An overlay's .text opens with the entry table every scene and debug room
   links, src/scene/entry.s, which each resolves against its own code. Its
   import table, imports.o, follows its code: the build writes it for each
   overlay from imports.inc, a stub for each main-image routine the overlay's
   code calls. The libgcc routines its code uses come next (register calls,
   and double-precision arithmetic in two scenes), then a part kept in its
   own object, where it has one. Each statement names the sections it places,
   so the objects keep their order in each. Its globals follow in .bss; where
   they are written in assembly, they are src/scene/bss/NAME.s. */

#define OVERLAY_TEXT(object) \
    .text 0x2008000 : \
    { \
        */games/tbs/src/scene/entry.o(.text .padding) \
        */games/tbs/src/object.o(.text .padding) \
        */imports.o(.text .padding) \
        *libgcc.a:_call_via_rX.o(.text .padding) \
        *libgcc.a:_addsub_df.o(.text .padding) \
        *libgcc.a:_si_to_df.o(.text .padding) \
        *libgcc.a:_df_to_si.o(.text .padding) \
        *libgcc.a:_lshrdi3.o(.text .padding) \
        *libgcc.a:_pack_df.o(.text .padding) \
        *libgcc.a:_unpack_df.o(.text .padding) \
        *libgcc.a:_thenan_df.o(.text .padding) \
        */games/tbs/src/scene/entry.o(.rodata) \
        */games/tbs/src/object.o(.rodata) \
        */imports.o(.rodata) \
        *libgcc.a:_call_via_rX.o(.rodata) \
        *libgcc.a:_addsub_df.o(.rodata) \
        *libgcc.a:_si_to_df.o(.rodata) \
        *libgcc.a:_df_to_si.o(.rodata) \
        *libgcc.a:_lshrdi3.o(.rodata) \
        *libgcc.a:_pack_df.o(.rodata) \
        *libgcc.a:_unpack_df.o(.rodata) \
        *libgcc.a:_thenan_df.o(.rodata) \
        */games/tbs/src/scene/entry.o(.data .overlays) \
        */games/tbs/src/object.o(.data .overlays) \
        */imports.o(.data .overlays) \
        *libgcc.a:_call_via_rX.o(.data .overlays) \
        *libgcc.a:_addsub_df.o(.data .overlays) \
        *libgcc.a:_si_to_df.o(.data .overlays) \
        *libgcc.a:_df_to_si.o(.data .overlays) \
        *libgcc.a:_lshrdi3.o(.data .overlays) \
        *libgcc.a:_pack_df.o(.data .overlays) \
        *libgcc.a:_unpack_df.o(.data .overlays) \
        *libgcc.a:_thenan_df.o(.data .overlays) \
    }
#define OVERLAY_TEXT_AND(object, part) \
    .text 0x2008000 : \
    { \
        */games/tbs/src/scene/entry.o(.text .padding) \
        */games/tbs/src/object.o(.text .padding) \
        */imports.o(.text .padding) \
        *libgcc.a:_call_via_rX.o(.text .padding) \
        *libgcc.a:_addsub_df.o(.text .padding) \
        *libgcc.a:_si_to_df.o(.text .padding) \
        *libgcc.a:_df_to_si.o(.text .padding) \
        *libgcc.a:_lshrdi3.o(.text .padding) \
        *libgcc.a:_pack_df.o(.text .padding) \
        *libgcc.a:_unpack_df.o(.text .padding) \
        *libgcc.a:_thenan_df.o(.text .padding) \
        */games/tbs/src/part.o(.text .padding) \
        */games/tbs/src/scene/entry.o(.rodata) \
        */games/tbs/src/object.o(.rodata) \
        */imports.o(.rodata) \
        *libgcc.a:_call_via_rX.o(.rodata) \
        *libgcc.a:_addsub_df.o(.rodata) \
        *libgcc.a:_si_to_df.o(.rodata) \
        *libgcc.a:_df_to_si.o(.rodata) \
        *libgcc.a:_lshrdi3.o(.rodata) \
        *libgcc.a:_pack_df.o(.rodata) \
        *libgcc.a:_unpack_df.o(.rodata) \
        *libgcc.a:_thenan_df.o(.rodata) \
        */games/tbs/src/part.o(.rodata) \
        */games/tbs/src/scene/entry.o(.data .overlays) \
        */games/tbs/src/object.o(.data .overlays) \
        */imports.o(.data .overlays) \
        *libgcc.a:_call_via_rX.o(.data .overlays) \
        *libgcc.a:_addsub_df.o(.data .overlays) \
        *libgcc.a:_si_to_df.o(.data .overlays) \
        *libgcc.a:_df_to_si.o(.data .overlays) \
        *libgcc.a:_lshrdi3.o(.data .overlays) \
        *libgcc.a:_pack_df.o(.data .overlays) \
        *libgcc.a:_unpack_df.o(.data .overlays) \
        *libgcc.a:_thenan_df.o(.data .overlays) \
        */games/tbs/src/part.o(.data .overlays) \
    }
/* A scene: src/scene/NAME.c, its globals in its own .bss. */
#define SCENE(name) \
SECTIONS \
{ \
    OVERLAY_TEXT(scene/name) \
    .bss : \
    { \
        */games/tbs/src/scene/name.o(.bss COMMON) \
    } \
}
/* A scene whose globals, or some of them, are written in src/scene/bss/NAME.s. */
#define SCENE_WITH_BSS(name) \
SECTIONS \
{ \
    OVERLAY_TEXT(scene/name) \
    .bss : \
    { \
        */games/tbs/src/scene/name.o(.bss COMMON) \
        */games/tbs/src/scene/bss/name.o(.bss) \
    } \
}
/* A scene with a part in an object of its own, linked after its code. */
#define SCENE_AND(name, part) \
SECTIONS \
{ \
    OVERLAY_TEXT_AND(scene/name, part) \
    .bss : \
    { \
        */games/tbs/src/scene/name.o(.bss COMMON) \
    } \
}
#define SCENE_AND_WITH_BSS(name, part) \
SECTIONS \
{ \
    OVERLAY_TEXT_AND(scene/name, part) \
    .bss : \
    { \
        */games/tbs/src/scene/name.o(.bss COMMON) \
        */games/tbs/src/scene/bss/name.o(.bss) \
    } \
}
/* A debug room: src/debug/NAME.c. */
#define DEBUG_ROOM(name) \
SECTIONS \
{ \
    OVERLAY_TEXT(debug/name) \
    .bss : \
    { \
        */games/tbs/src/debug/name.o(.bss COMMON) \
    } \
}

#if OVERLAY == 0x36f
SCENE_WITH_BSS(title)
#elif OVERLAY == 0x370
SCENE_AND(file_screen, scene/file_screen_keyboard)
#elif OVERLAY == 0x371
SCENE_AND(world_map, graphics/world_map/lighthouse_transition)
#elif OVERLAY == 0x372
SCENE(vale_prologue)
#elif OVERLAY == 0x373
SCENE(vale)
#elif OVERLAY == 0x374
SCENE(vale_plaza)
#elif OVERLAY == 0x375
SCENE(vale_kradens_cottage)
#elif OVERLAY == 0x376
SCENE(cottage_rooms_a)
#elif OVERLAY == 0x377
SCENE(cottage_rooms_b)
#elif OVERLAY == 0x378
SCENE(sanctum)
#elif OVERLAY == 0x379
SCENE(vale_eruption)
#elif OVERLAY == 0x37a
SCENE_WITH_BSS(sol_sanctum_luna_sol_rooms)
#elif OVERLAY == 0x37b
SCENE_AND(sol_sanctum_statue, graphics/sol_sanctum_statue/rows)
#elif OVERLAY == 0x37c
SCENE(sol_sanctum_psynergy_stone)
#elif OVERLAY == 0x37d
SCENE(sol_sanctum_maze)
#elif OVERLAY == 0x37e
SCENE(sol_sanctum_corridor)
#elif OVERLAY == 0x37f
SCENE(sol_sanctum_entrance)
#elif OVERLAY == 0x380
SCENE(sol_sanctum_elemental_star_room)
#elif OVERLAY == 0x381
SCENE_WITH_BSS(sol_sanctum_eruption)
#elif OVERLAY == 0x382
SCENE(vault)
#elif OVERLAY == 0x383
SCENE(log_rooms_a)
#elif OVERLAY == 0x384
SCENE(stone_rooms)
#elif OVERLAY == 0x385
SCENE(vault_revisit)
#elif OVERLAY == 0x386
SCENE(log_rooms_b)
#elif OVERLAY == 0x387
SCENE(goma_cave_entrance)
#elif OVERLAY == 0x388
SCENE(goma_cave_pond)
#elif OVERLAY == 0x389
SCENE(goma_cave_pillar)
#elif OVERLAY == 0x38a
SCENE(goma_cave_stream)
#elif OVERLAY == 0x38b
SCENE(bilibin)
#elif OVERLAY == 0x38c
SCENE(cottage_rooms_c)
#elif OVERLAY == 0x38d
SCENE(mccoy_palace)
#elif OVERLAY == 0x38e
SCENE(mccoy_palace_grounds)
#elif OVERLAY == 0x38f
SCENE(kolima)
#elif OVERLAY == 0x390
SCENE(log_rooms_c)
#elif OVERLAY == 0x391
SCENE(kolima_forest_entrance)
#elif OVERLAY == 0x392
SCENE(kolima_forest_glade)
#elif OVERLAY == 0x393
SCENE(kolima_forest_cliff)
#elif OVERLAY == 0x394
SCENE_WITH_BSS(kolima_forest_floodgate)
#elif OVERLAY == 0x395
SCENE_WITH_BSS(kolima_forest_tree)
#elif OVERLAY == 0x396
SCENE(tree_rooms)
#elif OVERLAY == 0x397
SCENE(tret_tree_branch)
#elif OVERLAY == 0x398
SCENE(bilibin_cave)
#elif OVERLAY == 0x399
SCENE(imil)
#elif OVERLAY == 0x39a
SCENE(imil_fuchin_falls)
#elif OVERLAY == 0x39b
SCENE(mercury_lighthouse_entrance)
#elif OVERLAY == 0x39c
SCENE(mercury_lighthouse)
#elif OVERLAY == 0x39d
SCENE(mercury_lighthouse_aerie)
#elif OVERLAY == 0x39e
SCENE(fuchin_temple)
#elif OVERLAY == 0x39f
SCENE(mogall_forest)
#elif OVERLAY == 0x3a0
SCENE(xian)
#elif OVERLAY == 0x3a1
SCENE(lacquer_rooms_b)
#elif OVERLAY == 0x3a2
SCENE(alpine_crossing_lama_temple)
#elif OVERLAY == 0x3a3
SCENE(altin)
#elif OVERLAY == 0x3a4
SCENE_WITH_BSS(altin_peak)
#elif OVERLAY == 0x3a5
SCENE_AND_WITH_BSS(lamakan_desert, graphics/lamakan_desert/heat_gauge)
#elif OVERLAY == 0x3a6
SCENE(vale_cave)
#elif OVERLAY == 0x3a7
SCENE(vault_cave)
#elif OVERLAY == 0x3a8
SCENE_WITH_BSS(kalay)
#elif OVERLAY == 0x3a9
SCENE(stucco_rooms_a)
#elif OVERLAY == 0x3aa
SCENE(hammet_palace)
#elif OVERLAY == 0x3ab
SCENE(lunpa)
#elif OVERLAY == 0x3ac
SCENE(rock_rooms)
#elif OVERLAY == 0x3ad
SCENE(west_lunpa_cave)
#elif OVERLAY == 0x3ae
SCENE(kalay_tolbi_docks)
#elif OVERLAY == 0x3af
SCENE(tolbi_ship_deck)
#elif OVERLAY == 0x3b0
SCENE(tolbi_ship_mast)
#elif OVERLAY == 0x3b1
SCENE(ship_rooms)
#elif OVERLAY == 0x3b2
SCENE(crossbone_isle)
#elif OVERLAY == 0x3b3
SCENE(crossbone_isle_stepping_stones)
#elif OVERLAY == 0x3b4
SCENE(crossbone_isle_keys)
#elif OVERLAY == 0x3b5
SCENE(tolbi)
#elif OVERLAY == 0x3b6
SCENE(stucco_rooms_b)
#elif OVERLAY == 0x3b7
SCENE(tolbi_games)
#elif OVERLAY == 0x3b8
SCENE(babi_palace)
#elif OVERLAY == 0x3b9
SCENE(colosseum_entrance)
#elif OVERLAY == 0x3ba
SCENE_AND_WITH_BSS(colosseum_azart_match, graphics/colosso/gauge)
#elif OVERLAY == 0x3bb
SCENE_AND_WITH_BSS(colosseum_satrage_match, graphics/colosso/gauge)
#elif OVERLAY == 0x3bc
SCENE_AND_WITH_BSS(colosseum_navampa_match, graphics/colosso/gauge)
#elif OVERLAY == 0x3bd
SCENE(altmiller_cave)
#elif OVERLAY == 0x3be
SCENE(gondowan_cave)
#elif OVERLAY == 0x3bf
SCENE(lunpa_fortress)
#elif OVERLAY == 0x3c0
SCENE(suhalla_desert)
#elif OVERLAY == 0x3c1
SCENE(suhalla)
#elif OVERLAY == 0x3c2
SCENE(timber_rooms_a)
#elif OVERLAY == 0x3c3
SCENE(suhalla_gate)
#elif OVERLAY == 0x3c4
SCENE(tunnel_ruins)
#elif OVERLAY == 0x3c5
SCENE(babi_lighthouse)
#elif OVERLAY == 0x3c6
SCENE(lalivero)
#elif OVERLAY == 0x3c7
SCENE(timber_rooms_bc)
#elif OVERLAY == 0x3c8
SCENE(venus_lighthouse)
#elif OVERLAY == 0x3c9
SCENE(venus_lighthouse_aerie)
#elif OVERLAY == 0x3ca
SCENE(lemurian_ship)
#elif OVERLAY == 0x3cb
SCENE(battle_arena)
#elif OVERLAY == 0x3cc
DEBUG_ROOM(test_scenes)
#elif OVERLAY == 0x3cd
DEBUG_ROOM(level)
#elif OVERLAY == 0x3ce
DEBUG_ROOM(test)
#endif

SECTIONS
{
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
