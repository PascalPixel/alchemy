/* resource_378: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/HEYA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/RESTART.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/HEYA2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/IMPORT.o"(.text)
        "*/resource_378_overlay.o"(.rodata.x0200b740)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
