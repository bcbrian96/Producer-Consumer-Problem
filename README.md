Project 3: Candy Kids

CMPT 300 D100: Operating Systems I
Instructor: Arrvindh Shriraman

Group Mu
Brian Lee 301250913 bblee@sfu.ca
SeongJin Kim 301258579 ska158@sfu.ca

This project contains the following files:

candykids.c: Main application holding factory thread, kid thread, and main() function. Plus some other helper functions, and some #defined constants

bbuff.h/.c: Bounded buffer module

stats.h/stats.c: Statistics module

Makefile: Must compile all the .c files and link together the .o files


The purpose of this project was to use the producer-consumer solution to manage access to a bounded buffer storing candy. The solution uses three semaphores.
One group of threads will model candy factories which generate candy one at a time and insert the candy into the bounded buffer. Another group of threads will model kids which eat candy one a time from the bounded buffer.
The program, called candykids, accepts three arguments: ./candykids <#factories> <#kids> <#seconds>.

For example, 

# Factories: Number of candy-factory threads to spawn.
# Kids: Number of kid threads to spawn.
# Seconds: Number of seconds to allow the factory threads to run for.

Finally, the program prints statistics that tracks the following:

Factory #: Candy factory number.
# Made: The number of candies that each factory reported making (as per the call from the candy-factory thread).
# Eaten: The number of candies which kids consumed (as per the call from the kid threads).
Min Delay[ms]: Minimum time between when a candy was created and consumed over all candies created by this factory. Measured in milliseconds.
Avg Delay[ms]: Average delay between this factory’s candy being created and consumed.
Max Delay[ms]: Maximum delay between this factory’s candy being created and consumed.