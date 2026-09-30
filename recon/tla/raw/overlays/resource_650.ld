/* resource_650: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/IDEJIMA/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/IDEJIMA/SCENE_TABLES.o"(.text)
        "*/resource_650_overlay.o"(.text.x02008098)
        "*/resource_650_overlay.o"(.text.x020080f0)
        "*/resource_650_overlay.o"(.text.x020082b4)
        "*/resource_650_overlay.o"(.text.x0200849c)
        "*/resource_650_overlay.o"(.text.x02008640)
        "*/resource_650_overlay.o"(.text.x02008664)
        "*/resource_650_overlay.o"(.text.x020086a8)
        "*/resource_650_overlay.o"(.text.x02008d14)
        "*/resource_650_overlay.o"(.text.x0200953c)
        "*/resource_650_overlay.o"(.text.x02009558)
        "*/resource_650_overlay.o"(.text.x020095a0)
        "*/resource_650_overlay.o"(.text.x0200961c)
        "*/games/THE LOST AGE/SRC/FIELD/IDEJIMA/DRIFT.o"(.text)
        "*/resource_650_overlay.o"(.text.x020096d8)
        "*/resource_650_overlay.o"(.text.x02009ee0)
        "*/resource_650_overlay.o"(.text.x0200a304)
        "*/games/THE LOST AGE/SRC/FIELD/IDEJIMA/WAKE.o"(.text)
        "*/resource_650_overlay.o"(.text.x0200ac64)
        "*/resource_650_overlay.o"(.text.x0200aec8)
        "*/games/THE LOST AGE/SRC/FIELD/IDEJIMA/PREPARE.o"(.text)
        "*/resource_650_overlay.o"(.text.x0200af88)
        "*/resource_650_overlay.o"(.text.x0200afd4)
        "*/games/THE LOST AGE/SRC/FIELD/IDEJIMA/IMPORT.o"(.text)
        "*/resource_650_overlay.o"(.rodata.x0200b330)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
