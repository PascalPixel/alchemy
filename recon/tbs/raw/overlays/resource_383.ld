/* resource_383: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/HEYA.o"(.text)
        "*/resource_383_overlay.o"(.text.x02008690)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/RUN_ACTOR10.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/THANKS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/HEYA2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/HEYA3.o"(.text)
        "*/resource_383_overlay.o"(.text.x0200c8c8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/STOP_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/IMPORT.o"(.text)
        "*/resource_383_overlay.o"(.rodata)
    }
    .bss : { "*/resource_383_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
