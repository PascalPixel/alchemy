/* resource_3a7: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/ENTRY.o"(.text)
        "*/resource_3a7_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/GET_TABLE9.o"(.text)
        "*/resource_3a7_overlay.o"(.text.x0200808c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/FIND_ACTIVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/PUSH_BLOCK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/APPLY_FLAG300.o"(.text)
        "*/resource_3a7_overlay.o"(.text.x020082e0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/SELECT_BY_RUNTIME_SELECTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/RAISE_PRIORITY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/SET_ENTRIES16.o"(.text)
        "*/resource_3a7_overlay.o"(.text.x02008754)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/SEQUENCE_B.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/LIFT_LOW_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/RUN_OPENING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/SPAWN_PUFFS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/INIT_SLOTS10.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/ENTRY_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/IMPORT.o"(.text)
        "*/resource_3a7_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
