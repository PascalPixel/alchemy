/* resource_3b7: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. The overlay's
   own work follows the image. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/IZUMI.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/IZUMI2.o"(.text)
        "*/resource_3b7_overlay.o"(.text.x02008ac8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/IZUMI3.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/IZUMI4.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3b7_overlay.o"(.rodata.x02009a08)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/IZUMI4.o"(.data .bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
