SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/soru_funka/entry.o"(.text)
        "*/games/tbs/src/field/soru_funka/funka.o"(.text)
        "*/games/tbs/src/field/soru_star/ring_setup.o"(.text)
        "*/games/tbs/src/field/soru_funka/import.o"(.text)
        "*/games/tbs/src/field/soru_funka/data.o"(.data)
    }
    .bss (NOLOAD) :
    {
        "*/resource_381_overlay.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
