/* resource_3a7: its listing with maintained code
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_3a7_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/PUSH_BLOCK.o"(.text)
        "*/resource_3a7_overlay.o"(.text.x020082bc)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/RAISE_PRIORITY.o"(.text)
        "*/resource_3a7_overlay.o"(.text.x02008734)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/LIFT_LOW_ACTOR.o"(.text)
        "*/resource_3a7_overlay.o"(.text.x020089c0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/SPAWN_PUFFS.o"(.text)
        "*/resource_3a7_overlay.o"(.text.x02008c08)
        "*/resource_3a7_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
