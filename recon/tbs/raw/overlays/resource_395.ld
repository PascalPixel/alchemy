/* resource_395: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. The overlay's own work follows the image. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_KI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_KI/SCENE.o"(.text)
        "*/resource_395_overlay.o"(.text.x02008088)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_KI/TRANSIT.o"(.text)
        "*/resource_395_overlay.o"(.text.x02009070)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_KI/EFFECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_KI/GESTURE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_KI/ORBIT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_KI/SPARKS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_KI/COLOR.o"(.text)
        "*/resource_395_overlay.o"(.text.x02009838)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_KI/IMPORT.o"(.text)
        "*/resource_395_overlay.o"(.rodata)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_KI/TRANSIT.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __udivsi3 = IwramUnsignedDivideEntry;
}
