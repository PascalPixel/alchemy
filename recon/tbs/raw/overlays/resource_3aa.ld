/* resource_3aa: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_KYUDEN/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_KYUDEN/GET_ENTRANCES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_KYUDEN/RETURN_ZERO.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_KYUDEN/GET_PLACEMENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_KYUDEN/GET_EVENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_KYUDEN/RUN_ACTOR13.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_KYUDEN/SCENE_INIT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_KYUDEN/PARTY_EVENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_KYUDEN/IMPORT.o"(.text)
        "*/resource_3aa_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
