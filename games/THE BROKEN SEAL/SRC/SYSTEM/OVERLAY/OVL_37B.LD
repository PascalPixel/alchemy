SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/SEKIZO.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/BEAMS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/DATA.o"(.data)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/BEAMS.o"(.bss COMMON)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/SEKIZO.o"(.bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
