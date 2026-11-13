/* One overlay of The Broken Seal, loaded at 0x2008000: ld_script.ld includes
   this file when the build defines OVERLAY as the overlay's number, and the
   block for that number is the overlay. The build finds the overlays from the
   lines that test OVERLAY, and each overlay's objects from its statements.

   A scene's unit is three objects of its .text: its entry table, its C file
   DIR/NAME.c and its import table. The entry table is DIR/entry/NAME.s where
   that file exists and DIR/entry.s otherwise, and the import table is
   DIR/import/NAME.s or DIR/import.s the same way. Where its globals are
   written in assembly, its .bss is DIR/bss/NAME.s. */
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
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/vale_prologue.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/vale_prologue.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/vale_prologue.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/vale_prologue.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x373
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/vale.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/vale.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/vale.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x374
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/vale_plaza.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/vale_plaza.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/vale_plaza.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x375
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/vale_kradens_cottage.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/vale_kradens_cottage.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/vale_kradens_cottage.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x376
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/cottage_rooms_a.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/cottage_rooms_a.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/cottage_rooms_a.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x377
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/cottage_rooms_b.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/cottage_rooms_b.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/cottage_rooms_b.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x378
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/sanctum.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/sanctum.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/sanctum.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x379
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/vale_eruption.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/vale_eruption.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/vale_eruption.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x37a
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/sol_sanctum_luna_sol_rooms.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/sol_sanctum_luna_sol_rooms.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/sol_sanctum_luna_sol_rooms.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/bss/sol_sanctum_luna_sol_rooms.o"(.bss)
    }
}
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
    .bss ALIGN(16) :
    {
        "*/games/tbs/src/scene/sol_sanctum_statue.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x37c
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/sol_sanctum_psynergy_stone.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/sol_sanctum_psynergy_stone.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/sol_sanctum_psynergy_stone.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x37d
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry/sol_sanctum_maze.o"(.text .padding)
        "*/games/tbs/src/scene/sol_sanctum_maze.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry/sol_sanctum_maze.o"(.rodata)
        "*/games/tbs/src/scene/sol_sanctum_maze.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry/sol_sanctum_maze.o"(.data .overlays)
        "*/games/tbs/src/scene/sol_sanctum_maze.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x37e
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry/sol_sanctum_corridor.o"(.text .padding)
        "*/games/tbs/src/scene/sol_sanctum_corridor.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry/sol_sanctum_corridor.o"(.rodata)
        "*/games/tbs/src/scene/sol_sanctum_corridor.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry/sol_sanctum_corridor.o"(.data .overlays)
        "*/games/tbs/src/scene/sol_sanctum_corridor.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x37f
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/sol_sanctum_entrance.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/sol_sanctum_entrance.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/sol_sanctum_entrance.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/sol_sanctum_entrance.o"(.bss)
    }
}
#elif OVERLAY == 0x380
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/sol_sanctum_elemental_star_room.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/sol_sanctum_elemental_star_room.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/sol_sanctum_elemental_star_room.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x381
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/sol_sanctum_eruption.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/sol_sanctum_eruption.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/sol_sanctum_eruption.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/bss/sol_sanctum_eruption.o"(.bss)
    }
}
#elif OVERLAY == 0x382
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/vault.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/vault.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/vault.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x383
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/log_rooms_a.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/log_rooms_a.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/log_rooms_a.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/log_rooms_a.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x384
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/stone_rooms.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/stone_rooms.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/stone_rooms.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x385
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/vault_revisit.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/vault_revisit.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/vault_revisit.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x386
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/log_rooms_b.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/log_rooms_b.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/log_rooms_b.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x387
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/goma_cave_entrance.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/goma_cave_entrance.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/goma_cave_entrance.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x388
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/goma_cave_pond.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/goma_cave_pond.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/goma_cave_pond.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x389
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/goma_cave_pillar.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/goma_cave_pillar.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/goma_cave_pillar.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
    .bss :
    {
        *(.bss COMMON)
    }
}
#elif OVERLAY == 0x38a
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/goma_cave_stream.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/goma_cave_stream.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/goma_cave_stream.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x38b
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/bilibin.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/bilibin.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/bilibin.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/bilibin.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x38c
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/cottage_rooms_c.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/cottage_rooms_c.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/cottage_rooms_c.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x38d
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/mccoy_palace.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/mccoy_palace.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/mccoy_palace.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x38e
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/mccoy_palace_grounds.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/mccoy_palace_grounds.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/mccoy_palace_grounds.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x38f
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/kolima.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/kolima.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/kolima.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/kolima.o"(.bss)
    }
}
#elif OVERLAY == 0x390
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/log_rooms_c.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/log_rooms_c.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/log_rooms_c.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x391
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/kolima_forest_entrance.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/kolima_forest_entrance.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/kolima_forest_entrance.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/kolima_forest_entrance.o"(.bss)
    }
}
#elif OVERLAY == 0x392
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/kolima_forest_glade.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/kolima_forest_glade.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/kolima_forest_glade.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
    .bss ALIGN(16) :
    {
        "*/games/tbs/src/scene/kolima_forest_glade.o"(.bss)
    }
}
#elif OVERLAY == 0x393
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/kolima_forest_cliff.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/kolima_forest_cliff.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/kolima_forest_cliff.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
    .bss ALIGN(16) :
    {
        "*/games/tbs/src/scene/kolima_forest_cliff.o"(.bss)
    }
}
#elif OVERLAY == 0x394
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/kolima_forest_floodgate.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/kolima_forest_floodgate.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/kolima_forest_floodgate.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/bss/kolima_forest_floodgate.o"(.bss)
    }
}
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
    .bss ALIGN(16) :
    {
        "*/games/tbs/src/scene/kolima_forest_tree.o"(.bss)
        "*/games/tbs/src/scene/bss/kolima_forest_tree.o"(.bss)
    }
}
#elif OVERLAY == 0x396
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry/tree_rooms.o"(.text .padding)
        "*/games/tbs/src/scene/tree_rooms.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry/tree_rooms.o"(.rodata)
        "*/games/tbs/src/scene/tree_rooms.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry/tree_rooms.o"(.data .overlays)
        "*/games/tbs/src/scene/tree_rooms.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
    .bss ALIGN(8) :
    {
        "*/games/tbs/src/scene/tree_rooms.o"(.bss)
    }
}
#elif OVERLAY == 0x397
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/tret_tree_branch.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/tret_tree_branch.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/tret_tree_branch.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
    .bss ALIGN(16) :
    {
        "*/games/tbs/src/scene/tret_tree_branch.o"(.bss)
    }
}
#elif OVERLAY == 0x398
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/bilibin_cave.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/bilibin_cave.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/bilibin_cave.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x399
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/imil.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/imil.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/imil.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/imil.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x39a
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/imil_fuchin_falls.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/imil_fuchin_falls.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/imil_fuchin_falls.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/imil_fuchin_falls.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x39b
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/mercury_lighthouse_entrance.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/mercury_lighthouse_entrance.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/mercury_lighthouse_entrance.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x39c
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/mercury_lighthouse.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/mercury_lighthouse.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/mercury_lighthouse.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x39d
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/mercury_lighthouse_aerie.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/mercury_lighthouse_aerie.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/mercury_lighthouse_aerie.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x39e
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/fuchin_temple.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/fuchin_temple.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/fuchin_temple.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x39f
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/mogall_forest.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/mogall_forest.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/mogall_forest.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3a0
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/xian.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/xian.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/xian.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3a1
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/lacquer_rooms_b.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/lacquer_rooms_b.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/lacquer_rooms_b.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3a2
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/alpine_crossing_lama_temple.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/alpine_crossing_lama_temple.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/alpine_crossing_lama_temple.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3a3
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/altin.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/altin.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/altin.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3a4
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/altin_peak.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/altin_peak.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/altin_peak.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/bss/altin_peak.o"(.bss)
    }
}
#elif OVERLAY == 0x3a5
EXTERN(_call_via_r0)
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
    .bss ALIGN(16) :
    {
        "*/games/tbs/src/scene/bss/lamakan_desert.o"(.bss)
    }
}
#elif OVERLAY == 0x3a6
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/vale_cave.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/vale_cave.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/vale_cave.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
}
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
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/kalay.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/kalay.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/kalay.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
    .bss ALIGN(8) :
    {
        "*/games/tbs/src/scene/bss/kalay.o"(.bss)
    }
}
#elif OVERLAY == 0x3a9
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/stucco_rooms_a.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/stucco_rooms_a.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/stucco_rooms_a.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3aa
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/hammet_palace.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/hammet_palace.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/hammet_palace.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3ab
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/lunpa.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/lunpa.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/lunpa.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3ac
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/rock_rooms.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/rock_rooms.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/rock_rooms.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3ad
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/west_lunpa_cave.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/west_lunpa_cave.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/west_lunpa_cave.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3ae
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/kalay_tolbi_docks.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/kalay_tolbi_docks.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/kalay_tolbi_docks.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3af
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/tolbi_ship_deck.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/tolbi_ship_deck.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/tolbi_ship_deck.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
    .bss ALIGN(16) :
    {
        "*/games/tbs/src/scene/tolbi_ship_deck.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x3b0
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/tolbi_ship_mast.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/tolbi_ship_mast.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/tolbi_ship_mast.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
    .bss ALIGN(16) :
    {
        "*/games/tbs/src/scene/tolbi_ship_mast.o"(.bss)
    }
}
#elif OVERLAY == 0x3b1
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry/ship_rooms.o"(.text .padding)
        "*/games/tbs/src/scene/ship_rooms.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry/ship_rooms.o"(.rodata)
        "*/games/tbs/src/scene/ship_rooms.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry/ship_rooms.o"(.data .overlays)
        "*/games/tbs/src/scene/ship_rooms.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/ship_rooms.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x3b2
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/crossbone_isle.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/crossbone_isle.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/crossbone_isle.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3b3
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/crossbone_isle_stepping_stones.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/crossbone_isle_stepping_stones.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/crossbone_isle_stepping_stones.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
    .bss ALIGN(16) :
    {
        "*/games/tbs/src/scene/crossbone_isle_stepping_stones.o"(.bss)
    }
}
#elif OVERLAY == 0x3b4
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/crossbone_isle_keys.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/crossbone_isle_keys.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/crossbone_isle_keys.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3b5
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry/tolbi.o"(.text .padding)
        "*/games/tbs/src/scene/tolbi.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry/tolbi.o"(.rodata)
        "*/games/tbs/src/scene/tolbi.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry/tolbi.o"(.data .overlays)
        "*/games/tbs/src/scene/tolbi.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3b6
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry/stucco_rooms_b.o"(.text .padding)
        "*/games/tbs/src/scene/stucco_rooms_b.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry/stucco_rooms_b.o"(.rodata)
        "*/games/tbs/src/scene/stucco_rooms_b.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry/stucco_rooms_b.o"(.data .overlays)
        "*/games/tbs/src/scene/stucco_rooms_b.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3b7
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/tolbi_games.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/tolbi_games.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/tolbi_games.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
    .bss ALIGN(16) :
    {
        "*/games/tbs/src/scene/tolbi_games.o"(.bss)
    }
}
#elif OVERLAY == 0x3b8
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry/babi_palace.o"(.text .padding)
        "*/games/tbs/src/scene/babi_palace.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry/babi_palace.o"(.rodata)
        "*/games/tbs/src/scene/babi_palace.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry/babi_palace.o"(.data .overlays)
        "*/games/tbs/src/scene/babi_palace.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3b9
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/colosseum_entrance.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/colosseum_entrance.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/colosseum_entrance.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3ba
EXTERN(_call_via_r0)
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
EXTERN(_call_via_r0)
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
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/altmiller_cave.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/altmiller_cave.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/altmiller_cave.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/altmiller_cave.o"(.bss COMMON)
    }
}
#elif OVERLAY == 0x3be
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/gondowan_cave.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/gondowan_cave.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/gondowan_cave.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3bf
EXTERN(_call_via_r0)
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
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/suhalla_desert.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/suhalla_desert.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/suhalla_desert.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/suhalla_desert.o"(.bss)
    }
}
#elif OVERLAY == 0x3c1
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/suhalla.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/suhalla.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/suhalla.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3c2
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/timber_rooms_a.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/timber_rooms_a.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/timber_rooms_a.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3c3
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/suhalla_gate.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/suhalla_gate.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/suhalla_gate.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3c4
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/tunnel_ruins.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/tunnel_ruins.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/tunnel_ruins.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3c5
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry/babi_lighthouse.o"(.text .padding)
        "*/games/tbs/src/scene/babi_lighthouse.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry/babi_lighthouse.o"(.rodata)
        "*/games/tbs/src/scene/babi_lighthouse.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry/babi_lighthouse.o"(.data .overlays)
        "*/games/tbs/src/scene/babi_lighthouse.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3c6
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/lalivero.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/lalivero.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/lalivero.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3c7
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/timber_rooms_bc.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/timber_rooms_bc.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/timber_rooms_bc.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3c8
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/venus_lighthouse.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/venus_lighthouse.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/venus_lighthouse.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
}
#elif OVERLAY == 0x3c9
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry.o"(.text .padding)
        "*/games/tbs/src/scene/venus_lighthouse_aerie.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry.o"(.rodata)
        "*/games/tbs/src/scene/venus_lighthouse_aerie.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry.o"(.data .overlays)
        "*/games/tbs/src/scene/venus_lighthouse_aerie.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/venus_lighthouse_aerie.o"(.bss)
    }
}
#elif OVERLAY == 0x3ca
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry/lemurian_ship.o"(.text .padding)
        "*/games/tbs/src/scene/lemurian_ship.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*/games/tbs/src/scene/entry/lemurian_ship.o"(.rodata)
        "*/games/tbs/src/scene/lemurian_ship.o"(.rodata)
        "*/imports.o"(.rodata)
        "*/games/tbs/src/scene/entry/lemurian_ship.o"(.data .overlays)
        "*/games/tbs/src/scene/lemurian_ship.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/lemurian_ship.o"(.bss)
    }
}
#elif OVERLAY == 0x3cb
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/scene/entry/battle_arena.o"(.text .padding)
        "*/games/tbs/src/scene/battle_arena.o"(.text .padding)
        "*/imports.o"(.text .padding)
        "*libgcc.a:_call_via_rX.o"(.text .padding)
        "*/games/tbs/src/scene/entry/battle_arena.o"(.rodata)
        "*/games/tbs/src/scene/battle_arena.o"(.rodata)
        "*/imports.o"(.rodata)
        "*libgcc.a:_call_via_rX.o"(.rodata)
        "*/games/tbs/src/scene/entry/battle_arena.o"(.data .overlays)
        "*/games/tbs/src/scene/battle_arena.o"(.data .overlays)
        "*/imports.o"(.data .overlays)
        "*libgcc.a:_call_via_rX.o"(.data .overlays)
    }
    .bss :
    {
        "*/games/tbs/src/scene/battle_arena.o"(.bss)
    }
}
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
