SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/biribino_mura/entry.o"(.text)
        "*/games/tbs/src/field/biribino_mura/mura.o"(.text)
        "*/games/tbs/src/field/biribino_mura/import.o"(.text)
        "*/games/tbs/src/field/biribino_mura/data.o"(.data)
    }
    .bss (NOLOAD) :
    {
        "*/games/tbs/src/field/biribino_mura/mura.o"(.bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
