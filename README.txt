420 - Project 1
Chloe VanCory & Kalyn Howes
10/17/21
README!

Cracked passwords:
    - Maria: password123
    - George: red32
    - Alice: in the works...
    - Bob: was not found
    - Spickler: was not found

Notes:
    - 8 separate functions were created for checking the different prefix and suffix possibilities, 
        this may not be the most efficient way to do so, but we mimimized the use of string operations by using []++
    - our program works on one password at a time rather than giving different passwords to different nodes
    - each node is given a different section of the dictionary to work through, but they are all searching for the same password
    - once the password is found, thatn node is supposed to communicate with the others to stop & move on to the next password

(a) Theoretically, the number of nodes should "perfectly divide" the timing of the program, but there is a lot more to take into account. 
    This includes communication overhead, the number of print statements, string operations, sprintfs, or any other built-in C commands 
    that use for loops under the hood.

(b) Timing for us was not very concrete, but we were able to get a hold of a few numbers. We found that with a singluar print statement running
    everytime a rank moved to a new word, running on 200 nodes took 12.5 hours to get through 63,581 words for Chloe and 12 hours and 5 miniutes to
    check 68,071 words for Kalyn. We also found that it took aobut 2.5 minutes for a rank to switch between words in the output, meaning 
    each word took 2 minutes and thirty seconds to check all the prefixes and suffixes of. We found this by watching and timing the log output itself.
    This number however was only timed once and would greatly depend on the length of the word at hand. Looking at Chloe's timing, the program
    was going through about 85 words per minute, while for Kalyn it was about 93 words per minute.

(c) Lets say we know the password has 10 chars at most, then we would have to brute force 10^128 characters since there are 128 on the ASCII table.
    Some of these don't need to be included since they are null characters but that is the main idea. Checking a password this way is similar to a bike lock,
    where you first try 0000, 0001 etc. In our case this would be like aaaaaaaaaa, aaaaaaaaab, aaaaaaaaac, etc. This obviously takes a lot longer
    than our version since it is checking the entire password and not justa 4-digit prefix or suffix.

(d) Hackers could use this in real-life situations, or maybe even the opposite. Knowing this is how easily some passwords can be hacked, it tells
    me the importance of complex passwords for cybersecurity. However if hackers were using our program it would take them years & would need
    functions for special characters and such. The use of MPI could be very helpful though since password cracking takes a village of processors :)

(e) I don't know how but I know this can be faster/more efficient. We should have included a better structured checkpoint system that showed
    where each node was starting in the dictionary and how far it got every 30 minutes or so instead of just printing every time the rank moved up a word.