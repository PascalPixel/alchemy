SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/epilogue/entry.o"(.text)
        "*/games/tla/src/scene/epilogue/scene_tables.o"(.text)
        "*/recon/tla/raw/src/scene/epilogue/s005c.o"(.text)
        "*/recon/tla/raw/src/scene/epilogue/s0098.o"(.text)
        "*/recon/tla/raw/src/scene/epilogue/s0158.o"(.text)
        "*/recon/tla/raw/src/scene/epilogue/s0244.o"(.text)
        "*/recon/tla/raw/src/scene/epilogue/s0318.o"(.text)
        "*/games/tla/src/scene/epilogue/prepare.o"(.text)
        "*/games/tla/src/scene/epilogue/import.o"(.text)
        "*/games/tla/src/scene/epilogue/pal.o"(.rodata)
        "*/games/tla/src/scene/epilogue/data.o"(.data)
    }
    .bss : { "*/resource_6b3_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
