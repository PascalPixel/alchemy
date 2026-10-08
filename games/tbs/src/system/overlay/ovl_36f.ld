SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/menu/title/entry.o"(.text)
        "*/games/tbs/src/menu/title/title.o"(.text)
        "*/games/tbs/src/menu/title/import.o"(.text)
        "*/games/tbs/src/menu/title/data.o"(.data)
    }
    .bss (NOLOAD) :
    {
        "*/resource_36f_overlay.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
