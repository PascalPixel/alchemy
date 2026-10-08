SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/DEBUG/TEST_ROOMS/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/TEST_ROOMS/EMPTY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/TEST_ROOMS/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/TEST_ROOMS/DATA.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
