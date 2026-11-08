SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/yallam/entry.o"(.text)
        "*/games/tla/src/scene/yallam/s0038.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse/distance.o"(.text)
        "*/games/tla/src/scene/yallam/s0088.o"(.text)
        "*/games/tla/src/scene/yallam/s014c.o"(.text)
        "*/games/tla/src/scene/yallam/scene_tables.o"(.text)
        "*/games/tla/src/scene/yallam/s0218.o"(.text)
        "*/games/tla/src/scene/yallam/script0.o"(.text)
        "*/games/tla/src/scene/yallam/script1.o"(.text)
        "*/games/tla/src/scene/yallam/script2.o"(.text)
        "*/games/tla/src/scene/yallam/s04ec.o"(.text)
        "*/games/tla/src/scene/yallam/s0544.o"(.text)
        "*/games/tla/src/scene/yallam/s06a4.o"(.text)
        "*/games/tla/src/scene/yallam/s06bc.o"(.text)
        "*/games/tla/src/scene/yallam/s1688.o"(.text)
        "*/recon/tla/raw/src/scene/yallam/s182c.o"(.text)
        "*/games/tla/src/scene/yallam/s1904.o"(.text)
        "*/games/tla/src/scene/yallam/s1944.o"(.text)
        "*/games/tla/src/scene/yallam/s1d14.o"(.text)
        "*/games/tla/src/scene/yallam/s1ec0.o"(.text)
        "*/games/tla/src/scene/yallam/s2244.o"(.text)
        "*/games/tla/src/scene/yallam/s22f8.o"(.text)
        "*/games/tla/src/scene/yallam/s23c8.o"(.text)
        "*/games/tla/src/scene/yallam/import.o"(.text)
        "*/games/tla/src/scene/yallam/data.o"(.data)
    }
    .bss : { "*/resource_676_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
