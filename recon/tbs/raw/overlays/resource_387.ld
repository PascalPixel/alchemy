/* resource_387: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IRIGUCHI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IRIGUCHI/EFFOBJ.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text.effect_spawn)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IRIGUCHI/IRIGUCHI.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IRIGUCHI/IMPORT.o"(.text)
        "*/resource_387_overlay.o"(.rodata.x02009234)
    }
    /DISCARD/ : { "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
