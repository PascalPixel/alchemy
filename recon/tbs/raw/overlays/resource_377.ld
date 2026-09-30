/* resource_377: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/FACING_TARGET.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/KRADEN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/EXITS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/ENTRY_STATE.o"(.text)
        "*/resource_377_overlay.o"(.text.x02008578)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/RAMP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/HOME_TALK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/SICKBED.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/ACTOR_EIGHT_MESSAGE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/MYTHRIL_BAG.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/SPAWN_PAIR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/PSYNERGY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/IMPORT.o"(.text)
        "*/resource_377_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
