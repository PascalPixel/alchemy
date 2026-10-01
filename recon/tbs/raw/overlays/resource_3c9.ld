/* resource_3c9: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/OBJECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text.effect_spawn)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/CHOJO.o"(.text)
        "*/resource_3c9_overlay.o"(.text.x0200a360)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/ORBIT.o"(.text)
        "*/resource_3c9_overlay.o"(.text.x0200b6d0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/CHOJO2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3c9_overlay.o"(.rodata.x0200df10)
    }
    /* The overlay's own work, past its loaded image. */
    .bss (NOLOAD) :
    {
        "*/resource_3c9_overlay.o"(.bss)
    }
    /DISCARD/ : { "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
