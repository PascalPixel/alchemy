SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/yallam/entry.o"(.text)
        "*/games/tla/src/scene/yallam/s0038.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse/distance.o"(.text)
        "*/resource_676_overlay.o"(.text.x02008088)
        "*/resource_676_overlay.o"(.text.x0200814c)
        "*/games/tla/src/scene/yallam/scene_tables.o"(.text)
        "*/games/tla/src/scene/yallam/s0218.o"(.text)
        "*/games/tla/src/scene/yallam/script0.o"(.text)
        "*/games/tla/src/scene/yallam/script1.o"(.text)
        "*/games/tla/src/scene/yallam/script2.o"(.text)
        "*/games/tla/src/scene/yallam/s04ec.o"(.text)
        "*/games/tla/src/scene/yallam/s0544.o"(.text)
        "*/games/tla/src/scene/yallam/s06a4.o"(.text)
        "*/resource_676_overlay.o"(.text.x020086bc)
        "*/games/tla/src/scene/yallam/s1688.o"(.text)
        "*/resource_676_overlay.o"(.text.x0200982c)
        "*/resource_676_overlay.o"(.text.x02009904)
        "*/resource_676_overlay.o"(.text.x02009944)
        "*/resource_676_overlay.o"(.text.x02009d14)
        "*/resource_676_overlay.o"(.text.x02009ec0)
        "*/games/tla/src/scene/yallam/s2244.o"(.text)
        "*/resource_676_overlay.o"(.text.x0200a2f8)
        "*/resource_676_overlay.o"(.text.x0200a3c8)
        "*/games/tla/src/scene/yallam/import.o"(.text)
        "*/games/tla/src/scene/yallam/data.o"(.data)
    }
    .bss : { "*/resource_676_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
