SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_DOU/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_DOU/TABLE_UPDATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_DOU/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_DOU/DATA.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
