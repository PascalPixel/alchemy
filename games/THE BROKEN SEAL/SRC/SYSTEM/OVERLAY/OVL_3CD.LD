SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/DEBUG/ITEM_LEVEL/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/ITEM_LEVEL/LEVEL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/ITEM_LEVEL/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/ITEM_LEVEL/DATA.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
