#ifndef ALCHEMY_MESSAGE_ARGUMENT_H
#define ALCHEMY_MESSAGE_ARGUMENT_H

// The kinds of message argument (mnMessageArgumentPush), which the message codes such as
// {argument_item_name} read; the window state keeps each kind beside its value
// (WIN_STATE.argumentKinds).

enum
{
	MESSAGE_ARGUMENT_NONE     = 0, // an empty place in the argument table
	MESSAGE_ARGUMENT_ACTOR    = 1,
	MESSAGE_ARGUMENT_ITEM     = 2,
	MESSAGE_ARGUMENT_CLASS    = 3,
	MESSAGE_ARGUMENT_ABILITY  = 4,
	MESSAGE_ARGUMENT_NUMBER   = 5,
	MESSAGE_ARGUMENT_LOCATION = 6,
};

#endif // ALCHEMY_MESSAGE_ARGUMENT_H
