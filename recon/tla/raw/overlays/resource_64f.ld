SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/suhara_gate/entry.o"(.text)
        "*/games/tla/src/field/suhara_gate/events_a.o"(.text)
        "*/games/tla/src/field/suhara_gate/s0044.o"(.text)
        "*/games/tla/src/field/suhara_gate/scene_tables.o"(.text)
        "*/games/tla/src/field/suhara_gate/s0090.o"(.text)
        "*/games/tla/src/field/suhara_gate/events_b.o"(.text)
        "*/games/tla/src/field/suhara_gate/s028c.o"(.text)
        "*/games/tla/src/field/suhara_gate/prepare.o"(.text)
        "*/resource_64f_overlay.o"(.text.x020083ac)
        "*/resource_64f_overlay.o"(.text.x02008494)
        "*/games/tla/src/field/suhara_gate/s05e8.o"(.text)
        "*/games/tla/src/field/suhara_gate/events_c.o"(.text)
        "*/games/tla/src/field/suhara_gate/import.o"(.text)
        "*/games/tla/src/field/suhara_gate/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
