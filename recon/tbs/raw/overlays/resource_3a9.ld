/* resource_3a9: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_HEYA/GET_ENTRANCES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_HEYA/RETURN_ZERO.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_HEYA/GET_TABLES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_HEYA/ARRIVAL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_HEYA/SCENE_INIT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_HEYA/CLEAR_SLOTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_HEYA/IMPORT.o"(.text)
        "*/resource_3a9_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
