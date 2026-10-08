SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_EDA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_EDA/EDA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_EDA/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_EDA/DATA.o"(.data)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_EDA/EDA.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
