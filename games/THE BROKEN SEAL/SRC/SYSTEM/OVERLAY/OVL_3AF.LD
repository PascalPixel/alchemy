SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/UPDATE_SCALE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/KANPAN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/ACTORS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/ARRIVAL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/DATA.o"(.data)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/KANPAN.o"(.data .bss COMMON)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/ARRIVAL.o"(COMMON)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/ACTORS.o"(COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
