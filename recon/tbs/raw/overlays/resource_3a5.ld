/* resource_3a5: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/SABAKU.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/SABAKU2.o"(.text)
        "*/resource_3a5_overlay.o"(.text.x02008e2c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/TRAVEL_DUST.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/NEAREST_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/DUST_PROBE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/SAND_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/ENTER_AREA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/WALK_BACK.o"(.text)
        "*/resource_3a5_overlay.o"(.text.x020098a4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/SAND_SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3a5_overlay.o"(.rodata.part1)
    }
    .bss ALIGN(16) (NOLOAD) : { "*/resource_3a5_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = IwramSignedDivideEntry;
}
