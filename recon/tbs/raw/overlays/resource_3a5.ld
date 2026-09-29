/* resource_3a5: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/SELECTED_ACTOR_SET_EFFECT_RECORD_MODE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/OBJECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/ACTOR13.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/SAND_EFFECT.o"(.text)
        "*/resource_3a5_overlay.o"(.text.x020083ac)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/SCENE_EXITS.o"(.text)
        "*/resource_3a5_overlay.o"(.text.x0200841c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/FLAGS_8B2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/ENTRY_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/AREA_LAYOUT.o"(.text)
        "*/resource_3a5_overlay.o"(.text.x02008c38)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/FLAG_MESSAGE.o"(.text)
        "*/resource_3a5_overlay.o"(.text.x02008cd0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/TRAVEL_DUST.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/NEAREST_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/DUST_PROBE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/SAND_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/ENTER_AREA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/WALK_BACK.o"(.text)
        "*/resource_3a5_overlay.o"(.text.x020098a4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3a5_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = IwramSignedDivideEntry;
}
