SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/gaia_rock/entry.o"(.text)
        "*/games/tla/src/scene/gaia_rock/scene_tables.o"(.text)
        "*/games/tla/src/scene/gaia_rock/exits.o"(.text)
        "*/games/tla/src/scene/gaia_rock/events_a.o"(.text)
        "*/games/tla/src/scene/gaia_rock/events_b.o"(.text)
        "*/resource_680_overlay.o"(.text.x02008164)
        "*/games/tla/src/scene/gaia_rock/events_c.o"(.text)
        "*/games/tla/src/scene/gaia_rock/wander.o"(.text)
        "*/resource_680_overlay.o"(.text.x02008528)
        "*/resource_680_overlay.o"(.text.x020085f0)
        "*/resource_680_overlay.o"(.text.x02008630)
        "*/games/tla/src/scene/gaia_rock/events_d.o"(.text)
        "*/resource_680_overlay.o"(.text.x020086ec)
        "*/games/tla/src/scene/gaia_rock/events_e.o"(.text)
        "*/resource_680_overlay.o"(.text.x02008ab4)
        "*/resource_680_overlay.o"(.text.x02008b2c)
        "*/resource_680_overlay.o"(.text.x02008bc0)
        "*/games/tla/src/scene/gaia_rock/events_f.o"(.text)
        "*/games/tla/src/scene/gaia_rock/import.o"(.text)
        "*/games/tla/src/scene/gaia_rock/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
