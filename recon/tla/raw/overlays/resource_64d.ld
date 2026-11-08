SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/venus_lighthouse_entrance/entry.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/scene_tables.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/s00c8.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/scene_events.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/s0128.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/prepare.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/s030c.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/s0440.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/s0510.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/s1a2c.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/s1bb0.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/s1c30.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/s1fd0.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/s23a4.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/s258c.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/s260c.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/s26f4.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/sparks.o"(.text)
        "*/games/tla/src/scene/venus_lighthouse_entrance/import.o"(.text)
        "*/games/tla/src/graphics/field/tiles64d.o"(.rodata)
        "*/games/tla/src/scene/venus_lighthouse_entrance/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
