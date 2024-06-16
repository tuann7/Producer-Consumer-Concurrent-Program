# Producer-Consumer-Concurrent-Program
## In this project, we're dealing with 2 kinds of threads: producers and consumers. This problem is well-known in concurrent programming and this project addresses this problem.
#### There are 2 kinds of threads: producers and consumers. There can be many of each. Each producer and each consumer must execute
#### in a different thread. A producers’ job is to produce items and put them into a bounded buffer. A consumer’s job is to remove 
#### items from the buffer. The buffer can hold 8 items.
#### The project is written following these conditions:
#### 1. Only one thread can access the buffer at a time.
#### 2. A producer cannot write into the buffer if it is full.
#### 3. A consumer cannot read from the buffer if it is empty.
