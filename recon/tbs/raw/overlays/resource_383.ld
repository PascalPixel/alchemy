/* resource_383: its listing with maintained code
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_383_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/ROD_HELP.o"(.text)
        "*/resource_383_overlay.o"(.text.x02009348)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/SELECT_ACTOR_PAIR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/SCENE_021C8.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/DIALOGUE_POSE.o"(.text)
        "*/resource_383_overlay.o"(.text.x0200a6e4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/ENTRY_STATE.o"(.text)
        "*/resource_383_overlay.o"(.text.x0200aba0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/NEAREST_STOP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/RECORD_NEAR_POINT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA/ACTOR_NEAR_POINT.o"(.text)
        "*/resource_383_overlay.o"(.text.x0200c840)
        "*/resource_383_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
