/* One overlay of The Broken Seal, loaded at 0x2008000: ld_script.ld includes
   this file when the build defines OVERLAY as the overlay's number, and the
   block for that number is the overlay. The build finds the overlays from the
   lines that test OVERLAY, and each overlay's objects from its statements.

   A scene's .text opens with its entry table, src/scene/entry.s, which most
   scenes link (each resolves its names against its own code), or
   DIR/entry/NAME.s for the few whose routines are named otherwise. Its import
   table, imports.o, follows its code: the build writes it for each overlay
   from imports.inc, a stub for each main-image routine the overlay's code
   calls. libgcc's _call_via_rX follows where the scene calls through a
   register. Each statement names the sections it places, so the objects keep
   their order in each. Where its globals are written in assembly, its .bss is
   DIR/bss/NAME.s. */

/* Most scenes are one overlay of one shape: the entry table, the scene's
   code, its import table and libgcc's register calls, then its globals.
   SCENE_WITH_TABLE links the scene's own entry table, and SCENE_WITH_BSS
   places the globals its bss/NAME.s defines. A scene of another shape
   writes its block out. */
#define SCENE_TEXT(name) \
    .text 0x2008000 : \
    { \
        */games/tbs/src/scene/entry.o(.text .padding) \
        */games/tbs/src/scene/name.o(.text .padding) \
        */imports.o(.text .padding) \
        *libgcc.a:_call_via_rX.o(.text .padding) \
        */games/tbs/src/scene/entry.o(.rodata) \
        */games/tbs/src/scene/name.o(.rodata) \
        */imports.o(.rodata) \
        *libgcc.a:_call_via_rX.o(.rodata) \
        */games/tbs/src/scene/entry.o(.data .overlays) \
        */games/tbs/src/scene/name.o(.data .overlays) \
        */imports.o(.data .overlays) \
        *libgcc.a:_call_via_rX.o(.data .overlays) \
    }
#define SCENE_WITH_TABLE_TEXT(name) \
    .text 0x2008000 : \
    { \
        */games/tbs/src/scene/entry/name.o(.text .padding) \
        */games/tbs/src/scene/name.o(.text .padding) \
        */imports.o(.text .padding) \
        *libgcc.a:_call_via_rX.o(.text .padding) \
        */games/tbs/src/scene/entry/name.o(.rodata) \
        */games/tbs/src/scene/name.o(.rodata) \
        */imports.o(.rodata) \
        *libgcc.a:_call_via_rX.o(.rodata) \
        */games/tbs/src/scene/entry/name.o(.data .overlays) \
        */games/tbs/src/scene/name.o(.data .overlays) \
        */imports.o(.data .overlays) \
        *libgcc.a:_call_via_rX.o(.data .overlays) \
    }
#define SCENE(name) \
SECTIONS \
{ \
    SCENE_TEXT(name) \
    .bss : \
    { \
        */games/tbs/src/scene/name.o(.bss COMMON) \
    } \
}
#define SCENE_WITH_TABLE(name) \
SECTIONS \
{ \
    SCENE_WITH_TABLE_TEXT(name) \
    .bss : \
    { \
        */games/tbs/src/scene/name.o(.bss COMMON) \
    } \
}
#define SCENE_WITH_BSS(name) \
SECTIONS \
{ \
    SCENE_TEXT(name) \
    .bss : \
    { \
        */games/tbs/src/scene/bss/name.o(.bss) \
    } \
}

#if OVERLAY == 0x36f
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry/title.o"(.text .padding)
        "*/games/tbs/src/scene/title.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry/title.o"(.rodata)
        "*/games/tbs/src/scene/title.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry/title.o"(.data .overlays)
        "*/games/tbs/src/scene/title.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/bss/title.o"(.bss)
    }
}
#elif OVERLAY == 0x370
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry/file_screen.o"(.text .padding)
        "*/games/tbs/src/scene/file_screen.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/file_screen_keyboard.o"(.text .padding)
        "*/games/tbs/src/scene/entry/file_screen.o"(.rodata)
        "*/games/tbs/src/scene/file_screen.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/file_screen_keyboard.o"(.rodata)
        "*/games/tbs/src/scene/entry/file_screen.o"(.data .overlays)
        "*/games/tbs/src/scene/file_screen.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*/games/tbs/src/scene/file_screen_keyboard.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x371
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/world_map.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/graphics/world_map/lighthouse_transition.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/world_map.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/graphics/world_map/lighthouse_transition.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/world_map.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*/games/tbs/src/graphics/world_map/lighthouse_transition.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/world_map.o"(.bss COMMON)
    }
}
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
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/sol_sanctum_statue.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/graphics/sol_sanctum_statue/rows.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/sol_sanctum_statue.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/graphics/sol_sanctum_statue/rows.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/sol_sanctum_statue.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*/games/tbs/src/graphics/sol_sanctum_statue/rows.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/sol_sanctum_statue.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x37c
SCENE(sol_sanctum_psynergy_stone)
#elif OVERLAY == 0x37d
SCENE_WITH_TABLE(sol_sanctum_maze)
#elif OVERLAY == 0x37e
SCENE_WITH_TABLE(sol_sanctum_corridor)
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
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/kolima_forest_tree.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/kolima_forest_tree.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/kolima_forest_tree.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/kolima_forest_tree.o"(.bss)
        "*/games/tbs/src/scene/bss/kolima_forest_tree.o"(.bss)
    }
}
#elif OVERLAY == 0x396
SCENE_WITH_TABLE(tree_rooms)
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
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/lamakan_desert.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/graphics/lamakan_desert/heat_gauge.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/lamakan_desert.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/graphics/lamakan_desert/heat_gauge.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/lamakan_desert.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
        "*/games/tbs/src/graphics/lamakan_desert/heat_gauge.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/bss/lamakan_desert.o"(.bss)
    }
}
#elif OVERLAY == 0x3a6
SCENE(vale_cave)
#elif OVERLAY == 0x3a7
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/vault_cave.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_addsub_df.o"(.text .padding)
        "*libgcc.a:_si_to_df.o"(.text .padding)
        "*libgcc.a:_df_to_si.o"(.text .padding)
        "*libgcc.a:_lshrdi3.o"(.text .padding)
        "*libgcc.a:_pack_df.o"(.text .padding)
        "*libgcc.a:_unpack_df.o"(.text .padding)
        "*libgcc.a:_thenan_df.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/vault_cave.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_addsub_df.o"(.rodata)
        "*libgcc.a:_si_to_df.o"(.rodata)
        "*libgcc.a:_df_to_si.o"(.rodata)
        "*libgcc.a:_lshrdi3.o"(.rodata)
        "*libgcc.a:_pack_df.o"(.rodata)
        "*libgcc.a:_unpack_df.o"(.rodata)
        "*libgcc.a:_thenan_df.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/vault_cave.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_addsub_df.o"(.data .overlays)
        "*libgcc.a:_si_to_df.o"(.data .overlays)
        "*libgcc.a:_df_to_si.o"(.data .overlays)
        "*libgcc.a:_lshrdi3.o"(.data .overlays)
        "*libgcc.a:_pack_df.o"(.data .overlays)
        "*libgcc.a:_unpack_df.o"(.data .overlays)
        "*libgcc.a:_thenan_df.o"(.data .overlays)
    }
}
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
SCENE_WITH_TABLE(ship_rooms)
#elif OVERLAY == 0x3b2
SCENE(crossbone_isle)
#elif OVERLAY == 0x3b3
SCENE(crossbone_isle_stepping_stones)
#elif OVERLAY == 0x3b4
SCENE(crossbone_isle_keys)
#elif OVERLAY == 0x3b5
SCENE_WITH_TABLE(tolbi)
#elif OVERLAY == 0x3b6
SCENE_WITH_TABLE(stucco_rooms_b)
#elif OVERLAY == 0x3b7
SCENE(tolbi_games)
#elif OVERLAY == 0x3b8
SCENE_WITH_TABLE(babi_palace)
#elif OVERLAY == 0x3b9
SCENE(colosseum_entrance)
#elif OVERLAY == 0x3ba
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/colosseum_azart_match.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/graphics/colosso/gauge.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/colosseum_azart_match.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/graphics/colosso/gauge.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/colosseum_azart_match.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
        "*/games/tbs/src/graphics/colosso/gauge.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/bss/colosseum_azart_match.o"(.bss)
    }
}
#elif OVERLAY == 0x3bb
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry/colosseum_satrage_match.o"(.text .padding)
        "*/games/tbs/src/scene/colosseum_satrage_match.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/graphics/colosso/gauge.o"(.text .padding)
        "*/games/tbs/src/scene/entry/colosseum_satrage_match.o"(.rodata)
        "*/games/tbs/src/scene/colosseum_satrage_match.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/graphics/colosso/gauge.o"(.rodata)
        "*/games/tbs/src/scene/entry/colosseum_satrage_match.o"(.data .overlays)
        "*/games/tbs/src/scene/colosseum_satrage_match.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*/games/tbs/src/graphics/colosso/gauge.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/bss/colosseum_satrage_match.o"(.bss)
    }
}
#elif OVERLAY == 0x3bc
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/colosseum_navampa_match.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/graphics/colosso/gauge.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/colosseum_navampa_match.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/graphics/colosso/gauge.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/colosseum_navampa_match.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
        "*/games/tbs/src/graphics/colosso/gauge.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/bss/colosseum_navampa_match.o"(.bss)
    }
}
#elif OVERLAY == 0x3bd
SCENE(altmiller_cave)
#elif OVERLAY == 0x3be
SCENE(gondowan_cave)
#elif OVERLAY == 0x3bf
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/lunpa_fortress.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*libgcc.a:_addsub_df.o"(.text .padding)
        "*libgcc.a:_si_to_df.o"(.text .padding)
        "*libgcc.a:_df_to_si.o"(.text .padding)
        "*libgcc.a:_lshrdi3.o"(.text .padding)
        "*libgcc.a:_pack_df.o"(.text .padding)
        "*libgcc.a:_unpack_df.o"(.text .padding)
        "*libgcc.a:_thenan_df.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/lunpa_fortress.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*libgcc.a:_addsub_df.o"(.rodata)
        "*libgcc.a:_si_to_df.o"(.rodata)
        "*libgcc.a:_df_to_si.o"(.rodata)
        "*libgcc.a:_lshrdi3.o"(.rodata)
        "*libgcc.a:_pack_df.o"(.rodata)
        "*libgcc.a:_unpack_df.o"(.rodata)
        "*libgcc.a:_thenan_df.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/lunpa_fortress.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
        "*libgcc.a:_addsub_df.o"(.data .overlays)
        "*libgcc.a:_si_to_df.o"(.data .overlays)
        "*libgcc.a:_df_to_si.o"(.data .overlays)
        "*libgcc.a:_lshrdi3.o"(.data .overlays)
        "*libgcc.a:_pack_df.o"(.data .overlays)
        "*libgcc.a:_unpack_df.o"(.data .overlays)
        "*libgcc.a:_thenan_df.o"(.data .overlays)
    }
}
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
SCENE_WITH_TABLE(babi_lighthouse)
#elif OVERLAY == 0x3c6
SCENE(lalivero)
#elif OVERLAY == 0x3c7
SCENE(timber_rooms_bc)
#elif OVERLAY == 0x3c8
SCENE(venus_lighthouse)
#elif OVERLAY == 0x3c9
SCENE(venus_lighthouse_aerie)
#elif OVERLAY == 0x3ca
SCENE_WITH_TABLE(lemurian_ship)
#elif OVERLAY == 0x3cb
SCENE_WITH_TABLE(battle_arena)
#elif OVERLAY == 0x3cc
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/debug/entry/test_scenes.o"(.text .padding)
        "*/games/tbs/src/debug/test_scenes.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/debug/entry/test_scenes.o"(.rodata)
        "*/games/tbs/src/debug/test_scenes.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/debug/entry/test_scenes.o"(.data .overlays)
        "*/games/tbs/src/debug/test_scenes.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3cd
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/debug/entry/level.o"(.text .padding)
        "*/games/tbs/src/debug/level.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/debug/entry/level.o"(.rodata)
        "*/games/tbs/src/debug/level.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/debug/entry/level.o"(.data .overlays)
        "*/games/tbs/src/debug/level.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3ce
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/debug/entry/test.o"(.text .padding)
        "*/games/tbs/src/debug/test.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/debug/entry/test.o"(.rodata)
        "*/games/tbs/src/debug/test.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/debug/entry/test.o"(.data .overlays)
        "*/games/tbs/src/debug/test.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#endif

SECTIONS
{
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
