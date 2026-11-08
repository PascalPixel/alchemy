SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/gaia_rock/entry.o"(.text)
        "*/games/tla/src/scene/gaia_rock/scene_tables.o"(.text)
        "*/games/tla/src/scene/gaia_rock/exits.o"(.text)
        "*/games/tla/src/scene/gaia_rock/events_a.o"(.text)
        "*/games/tla/src/scene/gaia_rock/events_b.o"(.text)
        "*/games/tla/src/scene/gaia_rock/s0164.o"(.text)
        "*/games/tla/src/scene/gaia_rock/events_c.o"(.text)
        "*/games/tla/src/scene/gaia_rock/wander.o"(.text)
        "*/games/tla/src/scene/gaia_rock/s0528.o"(.text)
        "*/games/tla/src/scene/gaia_rock/s05f0.o"(.text)
        "*/games/tla/src/scene/gaia_rock/events_d.o"(.text)
        "*/recon/tla/raw/src/scene/gaia_rock/s06ec.o"(.text)
        "*/games/tla/src/scene/gaia_rock/events_e.o"(.text)
        "*/games/tla/src/scene/gaia_rock/s0ab4.o"(.text)
        "*/games/tla/src/scene/gaia_rock/s0b2c.o"(.text)
        "*/recon/tla/raw/src/scene/gaia_rock/s0bc0.o"(.text)
        "*/games/tla/src/scene/gaia_rock/events_f.o"(.text)
        "*/games/tla/src/scene/gaia_rock/import.o"(.text)
        "*/games/tla/src/scene/gaia_rock/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
