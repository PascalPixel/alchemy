SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/soru_sekizo/entry.o"(.text)
        "*/games/tbs/src/field/soru_sekizo/sekizo.o"(.text)
        "*/games/tbs/src/field/soru_sekizo/import.o"(.text)
        "*/games/tbs/src/field/soru_sekizo/data.o"(.data)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/tbs/src/field/soru_sekizo/sekizo.o"(.bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
