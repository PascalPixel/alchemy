/* resource_377: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/BABI.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/BABI2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/IMPORT.o"(.text)
        "*/resource_377_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
