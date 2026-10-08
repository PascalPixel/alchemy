SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_ROKA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_ROKA/SCENE_37E.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_ROKA/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_ROKA/DATA.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
