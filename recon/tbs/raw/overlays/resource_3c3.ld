/* resource_3c3: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_GATE/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_GATE/ACTOR_VALUE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_GATE/GET_ENTRANCES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_GATE/TABLES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_GATE/GET_PLACEMENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_GATE/GET_EVENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_GATE/ENTER_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_GATE/GATE_SCENES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_GATE/IMPORT.o"(.text)
        "*/resource_3c3_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
