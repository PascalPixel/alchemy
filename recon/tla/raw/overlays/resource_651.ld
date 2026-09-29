/* resource_651: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/DERI_MURA/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_MURA/SCENE_TABLES.o"(.text)
        "*/resource_651_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_MURA/TALK.o"(.text)
        "*/resource_651_overlay.o"(.text.x02008288)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_MURA/HIP.o"(.text)
        "*/resource_651_overlay.o"(.text.x02008340)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_MURA/IMPORT.o"(.text)
        "*/resource_651_overlay.o"(.rodata.x02008d80)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
