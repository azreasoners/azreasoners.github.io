:- begin_python.

import random
def a():
	return random.randint(0,100)

:- end_python.

:- sorts
	nums.

:- objects
	0..100	:: nums.

:- constants
	r		:: nums.

:- variables
	R		:: nums.



caused r=R where R = @a().

