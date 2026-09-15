/*
Write next_state, the transition function of a download state machine: it returns the state 
reached from current when e fires, and current unchanged when that pair has no entry in the table :

IDLE + START -> RUNNING, RUNNING + PAUSE -> PAUSED, RUNNING + FINISH -> DONE, 
RUNNING + ERROR -> FAILED, PAUSED + RESUME -> RUNNING, PAUSED + ERROR -> FAILED.
*/

typedef enum e_state
{
	IDLE,
	RUNNING,
	PAUSED,
	DONE,
	FAILED
}	state;

typedef enum e_event
{
	START,
	PAUSE,
	RESUME,
	FINISH,
	ERROR
}	event;

state	next_state(state current, event e)
{
	if ((current == IDLE && e == START) || (current == PAUSED && e == RESUME))
		current = RUNNING;
	else if (current == RUNNING && e == PAUSE)
		current = PAUSED;
	else if (current == RUNNING && e == FINISH)
		current = DONE;
	else if ((current == RUNNING && e == ERROR) || (current == PAUSED && e == ERROR))
		current = FAILED;
	return (current);
}
