SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/korima_ki/entry.o"(.text)
        "*/games/tbs/src/field/korima_ki/ki.o"(.text)
        "*/games/tbs/src/field/common/korima/palette.o"(.text)
        "*/games/tbs/src/field/korima_ki/import.o"(.text)
        "*/games/tbs/src/field/korima_ki/data.o"(.data)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/tbs/src/field/korima_ki/ki.o"(.bss)
        "*/resource_395_overlay.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
