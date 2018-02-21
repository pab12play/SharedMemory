# SharedMemory
Creating deamons with fork and sharing memory through mmap


This is a sample proyect for the class Operating Systems.
here you can see many aspects from the unix approach like shared memory and forking new processes.

There are 2 branches
The master brach you have to run "generador" first and "status" next
"generador" generates random numbers and put them in a space from shared memory
With "status" you can see those numbers and clean the vector.

The parte2 branch you have almost the same processes, the difference is that I added another process and renamed them.
Proceso1 generates numbers
Proceso2 controls the other processes
Proceso3 Takes even number from process1 and puts them in another area of memory
