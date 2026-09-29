/* resource_3a7: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code, then the
   soft-float members its double arithmetic pulls in; their NaN constant
   sits in the rodata where the ROM holds it. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/GET_ENTRANCES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/GET_TABLE9.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/SCENE_TABLES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/FIND_ACTIVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/PUSH_BLOCK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/APPLY_FLAG300.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/GET_EVENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/SELECT_BY_RUNTIME_SELECTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/RAISE_PRIORITY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/SET_ENTRIES16.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/RUMBLE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/SEQUENCE_B.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/LIFT_LOW_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/RUN_OPENING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/SPAWN_PUFFS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/INIT_SLOTS10.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/ENTRY_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/IMPORT.o"(.text)
        *libgcc.a:_addsub_df.o(.text)
        *libgcc.a:_si_to_df.o(.text)
        *libgcc.a:_df_to_si.o(.text)
        *libgcc.a:_lshrdi3.o(.text)
        *libgcc.a:_pack_df.o(.text)
        *libgcc.a:_unpack_df.o(.text)
        "*/resource_3a7_overlay.o"(.rodata)
        *libgcc.a:_thenan_df.o(.rodata)
        "*/resource_3a7_overlay.o"(.rodata.part2)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
