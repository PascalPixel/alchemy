SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/alhafran_cave/entry.o"(.text)
        "*/games/tla/src/scene/alhafran_cave/scene_tables.o"(.text)
        "*/recon/tla/raw/src/scene/alhafran_cave/s004c.o"(.text)
        "*/recon/tla/raw/src/scene/alhafran_cave/s02f8.o"(.text)
        "*/recon/tla/raw/src/scene/alhafran_cave/s07a4.o"(.text)
        "*/recon/tla/raw/src/scene/alhafran_cave/s0818.o"(.text)
        "*/games/tla/src/scene/alhafran_cave/s1220.o"(.text)
        "*/recon/tla/raw/src/scene/alhafran_cave/s1230.o"(.text)
        "*/recon/tla/raw/src/scene/alhafran_cave/s129c.o"(.text)
        "*/games/tla/src/scene/alhafran_cave/s1320.o"(.text)
        "*/games/tla/src/scene/alhafran_cave/import.o"(.text)
        "*/games/tla/src/scene/alhafran_cave/data.o"(.data)
    }
    .bss : { "*/resource_66a_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
