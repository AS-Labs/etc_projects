#include "statemanager.h"

//#include

int STATEMANAGER_scale(StateManager *statemanager) {
	statemanager -> capacity *= 2;
	statemanager -> stack = realloc(statemanager -> stack, statemanager ->  capacity * sizeof(State*));
	return statemanager -> capacity;
}
