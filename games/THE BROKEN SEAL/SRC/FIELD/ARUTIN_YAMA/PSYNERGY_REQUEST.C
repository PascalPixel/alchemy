/* Asks the engine for psynergy request 0x1018 on the next frame. */
#include "YAMA.H"

void ArutinYama_RequestPsynergy(void)
{
    gEventWork->psynergy_request = 0x1018;
}
