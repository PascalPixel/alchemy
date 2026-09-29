/* resource_3a3: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_MURA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_MURA/GET_ENTRANCES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_MURA/SCENE_TABLES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_MURA/GET_PLACEMENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_MURA/GET_EVENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_MURA/MOTION_PARTICLE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_MURA/SCENE_INIT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_MURA/MIDDLE_SEQUENCE.o"(.text)
        "*/resource_3a3_overlay.o"(.text.x02008d58)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_MURA/IMPORT.o"(.text)
        "*/resource_3a3_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
