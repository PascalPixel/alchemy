SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_IE/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_IE/IE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_IE/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_IE/DATA.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
