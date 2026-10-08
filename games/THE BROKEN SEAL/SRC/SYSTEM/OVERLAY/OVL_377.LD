SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/ENTRY.o"(.text)
        "*/games/COMMON/SRC/FIELD/COMMON/OBJECT/FACING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/BABI.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/DATA.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
