# A game in C
following this beautiful post: https://prdeving.wordpress.com/2019/05/30/how-to-write-a-game-engine-in-pure-c-part-1-state-manager/

## The state manager

A mechanism to switch between scenes and keep tack of what's happening without initializing and killing things manually.

This will be a stack-based state manager.

### Stack based
The scenes is like a standalone application,we store them in a stack, one on top of the other; then in the main loop, the stack manager will use the state that is on the top of the stack.

if we are in the game state and want to show the pause menu, we would push a new state into the stack, it would go to the top of the stack.

where the pause menu is on top of the game state.
if resuming the game, we pop the state from the stack, leaving the game state as head of the stack again; the main loop will use it since it is now the top state again.

On each flip, states that are popped out of the stack have to be destroyed and states that are pushed into the stack have to be initialized.



#### CODE [state manager]

- two data structures are needed:
    - state
    - statemanager

The `state` represents the block of code that conceptually belongs to a game part or scene.
Must have an initializer, a destroyer, update, draw methods.

Here the states are a collection of pointers to functions (like methods)
state `game` will have pointers to the function that initializes the game state, to the function that deletes it, to the function that updates on the game and the function that draws the game state.


i.e define the types for the function pointers:
```C
typedef unsigned int (*fnPtr)();
typedef unsigned int (*fnPtrF1)(float);
```

And then define the State struct:
```C
typedef struct {
    fnPtr init;
    fnPtrF1 update;
    fnPtrF1 draw;
    fnPtr destroy;
} State;
```

The StateManager type will have the stack and two numbers, `capacity`=how many states the stack can hold, and `top` for the index of the top state.


Stack is an array of pointers (thats what ** means), the states will be defined elsewhere and the statemanager will keep a pointer to them, this is important so the states are kept in memory just once.


```C
typedef struct {
    State **stack;
    int capacity;
    int top;
} StateManager;
```


Now to define some functions:

```C
int STATEMANAGER_init(StateManager *statemanager);
int STATEMANAGER_free(StateManager *statemanager);
int STATEMANAGER_push(StateManager *statemanager, State *state);
int STATEMANAGER_pop(StateManager *statemanager);
State *STATEMANAGER_top(StateManager *statemanager);
int STATEMANAGER_update(StateManager *statemanager, float deltatime);
int STATEMANAGER_draw(StateManager *statemanager, float deltatime);
```

Now the `statemanager` functions, first we do the initialization and free functions.


```C
int STATEMANAGER_init(StateManager *statemanager) {
    statemanager -> capacity = 3;
    statemanager -> stack = malloc(statemanager -> capacity * sizeof(State*));
    statemanager -> top = -1;
    return 0;
}

init STATEMANAGER_free(StateManager *statemanager) {
    do {
        STATEMANAGER_pop(statemanager);
    } while (statemanager -> top > -1);
    free(statemanager -> stack);
    return 0;
}


```


STATEMANAGER_init will set the stack minimum capacity, allocate memory for it and set the index to the top of the stack.
STATEMANAGER_free will call pop until the stack is empty and then will free the memory allocated with malloc.
STATEMANAGER_push will put a state into the stack.
STATEMANAGER_pop will take the top state out.
STATEMANAGER_top will return the active state.


The stack is dynamically allocated so we save some memory, the init function has capacity set to 3, though a game can have more that 3 states.
The ability to grow due to dynamic allocation will need a function called `STATEMANAGER_scale` which will double the state managers capacity and reallocs the memory:

```C
int STATEMANAGER_scale(StateManager *statemanager) {
    statemanager -> capacity *= 2;
    statemanager -> stack = realloc(statemanager -> stack, statemanager -> capacity * sizeof(State*));
        return statemanager -> capacity;
}
```

implementing the stack management functions:

```C
int STATEMANAGER_push(StateManager *statemanager, State *state) {
    if (statemanager -> top + 1 == statemanager -> capacity) STATEMANAGER_scale(statemanager);
    statemanager -> top++;
    statemanager -> stack[statemanager -> top] = state;
    if (state -> init != NULL) state -> init();
    return statemanager -> top;
}

int STATEMANAGER_pop(StateManager *statemanager) {
    if (statemanager -> top == -1) return 0;
    State *top = STATEMANAGER_top(statemanager);
    if (top -> stack[statemanager -> top] = NULL;
    statemanager -> top--;
    return statemanager -> top;
}

State *STATEMANAGER_top(StateManager *statemanager) {
    return statemanager -> stack[statemanager -> top];
}
```

Here push will check if we reached the limit of the stack and scale if needed, it will increase the top index, add the state pointer to the stack, call it's init method if it's defined and then return the new index.


Pop gets the top state, calls it's destroy method and removes it from the stack, decreasing the index and returning the new top index.
Top returns the active state.


Now the loop functions. The game loop will call some functions, two of those are the update and draw state methods.


```C
int STATEMANAGER_update(StateManager *statemanager, float deltatime) {
    State *state = STATEMANAGER_top(statemanager);
    if (state -> update != NULL) return state -> update(deltatime);
    return 1;
}

int STATEMANAGER_draw(StateManager *statemanager, float deltatime) {
    State *state = STATEMANAGER_top(statemanager);
    if (state -> draw != NULL) return state -> draw(deltatime);
    return 1;
}
```
checking if the top state has update or draw method and if yes, then call it.

[[statemanager.h]]
[[statemanager.c]]










