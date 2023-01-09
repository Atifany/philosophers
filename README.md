# philosophers
school project
<br><br><br>
description:
This is my solution of common Philosophers task in scope of school21 project
<br>
todo:
<br>
V - Handle the simulation with only one philosopher <br>
V - Check data race between philosopher and count_to_death()<br>
V - Make 1 step clear: someone eats,someone thinks! They don't starve on the first lap!<br>
V - If someone dies before someone even started - segfault!!!<br>
V - replace mutex for eating state with semaphore <br>
 - add .d file handle to the makefile<br>
 - norme<br>
 - rework mandatory maybe<br>
V - add functionality to count how many times each philospher has eaten!<br>
V -- with that said add semaphore for a number of philosophers, wich will represent<br>
	how may philosophers has eaten, and main process will sem_wait for number_of_philosophers times.<br>
 - total fail on exit if test is smthing like 5 200 200 200. Better rewrite exit COMPLETELY!<br>
