/* resource_3b7: its entry veneers, its listing with the compiler library
   linked in place from the licensed container, as pret links libgcc
   members. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/ENTRY.o"(.text)
        "*/resource_3b7_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3b7_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
