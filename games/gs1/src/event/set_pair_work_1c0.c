#include "scene.h"
#include "abi/event/set_pair_work_1c0.h"
#include "event_runtime.h"

extern struct EventPairWork1c0 gCell;

void Event_SetPairWork1c0(u16 first, u16 second)
{
    gWork->value_170 = 999;
    gCell.first = first;
    gCell.second = second;
}
