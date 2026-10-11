SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_676/entry.o"(.text)
        "*/games/tla/src/field/overlay_676/s0038.o"(.text)
        "*/games/tla/src/field/vinasu_chojo/distance.o"(.text)
        "*/resource_676_overlay.o"(.text.x02008088)
        "*/resource_676_overlay.o"(.text.x0200814c)
        "*/games/tla/src/field/overlay_676/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_676/s0218.o"(.text)
        "*/resource_676_overlay.o"(.text.x02008270)
        "*/games/tla/src/field/overlay_676/script0.o"(.text)
        "*/games/tla/src/field/overlay_676/script1.o"(.text)
        "*/games/tla/src/field/overlay_676/script2.o"(.text)
        "*/games/tla/src/field/overlay_676/s04ec.o"(.text)
        "*/resource_676_overlay.o"(.text.x02008544)
        "*/resource_676_overlay.o"(.text.x02008584)
        "*/resource_676_overlay.o"(.text.x02008614)
        "*/games/tla/src/field/overlay_676/s06a4.o"(.text)
        "*/resource_676_overlay.o"(.text.x020086bc)
        "*/resource_676_overlay.o"(.text.x02009688)
        "*/resource_676_overlay.o"(.text.x02009730)
        "*/resource_676_overlay.o"(.text.x0200982c)
        "*/resource_676_overlay.o"(.text.x02009904)
        "*/resource_676_overlay.o"(.text.x02009944)
        "*/resource_676_overlay.o"(.text.x02009d14)
        "*/resource_676_overlay.o"(.text.x02009ec0)
        "*/games/tla/src/field/overlay_676/s2244.o"(.text)
        "*/resource_676_overlay.o"(.text.x0200a2f8)
        "*/resource_676_overlay.o"(.text.x0200a3c8)
        "*/games/tla/src/field/overlay_676/import.o"(.text)
        "*/games/tla/src/field/overlay_676/data.o"(.data)
    }
    .bss : { "*/resource_676_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
