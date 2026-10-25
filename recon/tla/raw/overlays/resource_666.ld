SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/alhafra_interiors/entry.o"(.text)
        "*/games/tla/src/scene/alhafra_interiors/scene_tables.o"(.text)
        "*/games/tla/src/scene/alhafra_interiors/s004c.o"(.text)
        "*/games/tla/src/scene/alhafra_interiors/s0084.o"(.text)
        "*/games/tla/src/scene/alhafra_interiors/s01ac.o"(.text)
        "*/games/tla/src/scene/alhafra_interiors/s0298.o"(.text)
        "*/games/tla/src/scene/alhafra_interiors/s02bc.o"(.text)
        "*/games/tla/src/scene/alhafra_interiors/s0444.o"(.text)
        "*/games/tla/src/scene/alhafra_interiors/s047c.o"(.text)
        "*/games/tla/src/scene/alhafra_interiors/s04b8.o"(.text)
        "*/games/tla/src/scene/alhafra_interiors/s04f4.o"(.text)
        "*/games/tla/src/scene/alhafra_interiors/s0578.o"(.text)
        "*/games/tla/src/scene/alhafra_interiors/s0598.o"(.text)
        "*/resource_666_overlay.o"(.text.x020085b8)
        "*/games/tla/src/scene/alhafra_interiors/s05b8.o"(.text)
        "*/games/tla/src/scene/alhafra_interiors/prepare.o"(.text)
        "*/games/tla/src/scene/alhafra_interiors/s0738.o"(.text)
        "*/games/tla/src/scene/alhafra_interiors/import.o"(.text)
        "*/games/tla/src/scene/alhafra_interiors/data.o"(.data)
    }
    .bss : { "*/resource_666_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
