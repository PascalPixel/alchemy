SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/MURA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/DATA.o"(.data)
    }
    .bss (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/MURA.o"(.bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
