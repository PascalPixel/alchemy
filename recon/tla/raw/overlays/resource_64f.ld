SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/suhara_gate/entry.o"(.text)
        "*/games/tla/src/field/suhara_gate/events_a.o"(.text)
        "*/resource_64f_overlay.o"(.text.x02008044)
        "*/games/tla/src/field/suhara_gate/scene_tables.o"(.text)
        "*/resource_64f_overlay.o"(.text.x02008090)
        "*/resource_64f_overlay.o"(.text.x020080d0)
        "*/games/tla/src/field/suhara_gate/events_b.o"(.text)
        "*/resource_64f_overlay.o"(.text.x0200828c)
        "*/games/tla/src/field/suhara_gate/prepare.o"(.text)
        "*/resource_64f_overlay.o"(.text.x020083ac)
        "*/resource_64f_overlay.o"(.text.x02008494)
        "*/resource_64f_overlay.o"(.text.x020085e8)
        "*/games/tla/src/field/suhara_gate/events_c.o"(.text)
        "*/games/tla/src/field/suhara_gate/import.o"(.text)
        "*/resource_64f_overlay.o"(.rodata.x02008aa8)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
