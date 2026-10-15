SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/yallam_interiors/entry.o"(.text)
        "*/games/tla/src/scene/yallam_interiors/scene_tables.o"(.text)
        "*/games/tla/src/scene/yallam_interiors/s004c.o"(.text)
        "*/games/tla/src/scene/yallam_interiors/s008c.o"(.text)
        "*/games/tla/src/scene/yallam_interiors/s00cc.o"(.text)
        "*/games/tla/src/scene/yallam_interiors/s010c.o"(.text)
        "*/games/tla/src/scene/yallam_interiors/s014c.o"(.text)
        "*/games/tla/src/scene/yallam_interiors/s018c.o"(.text)
        "*/games/tla/src/scene/yallam_interiors/s01b4.o"(.text)
        "*/games/tla/src/scene/yallam_interiors/s01dc.o"(.text)
        "*/games/tla/src/scene/yallam_interiors/s0204.o"(.text)
        "*/games/tla/src/scene/yallam_interiors/s04e0.o"(.text)
        "*/games/tla/src/scene/yallam_interiors/s077c.o"(.text)
        "*/games/tla/src/scene/yallam_interiors/prepare.o"(.text)
        "*/games/tla/src/scene/yallam_interiors/import.o"(.text)
        "*/games/tla/src/scene/yallam_interiors/data.o"(.data)
        "*/games/tla/src/scene/yallam_interiors/data2.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
