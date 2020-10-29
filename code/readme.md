## Changelog/TODO/Random

So far:
- test_buffer was used to verify that the buffer is putting elements. It is now obsolete.
- main_buffer was used to put and get elements before we integrated the buffer to the general lsm architecture. It is, technically, now obsolete.
- main is the main working file. Right now it only supports elementary functions on the buffer, but in the future we will implement the lsm tree architecture.

Right now we are working on the main structure of the lsm tree.

Regarding the workloads:
- we have a randomly generated benchmark workload (`workload_big`).
- we have a toy workload to show that filling up the buffer does work as expected (no levels implemented yet).
- we have a workload to show that delete works as expected and doesn't affect everything else (`workload_del`).
- we have a randomly generated small workload (`workload`).

I need to do the following:
- implement merge
- implement bloom filters
- implement fence pointers

Completed:
- flush actually sends things to the first level.

Things to mention in the midway:
- Added `GRAVEYARD` values.
- Buffer works flawlessly.
- Print, get, put, delete all work, but I swapped the order a bit for efficiency.
- The only thing that feels awkward with print is that the total number of files includes deleted files (and I also print them). The reason for this is as follows; I can hide the ones that have GRAVEYARD values, but not the normal ones (same for excluding them from counts), so I give an inaccurate representation of the tree. Another idea is to have an array where I store the deleted keys as I go through the tree, so that I don't count/print them when I see them again. While this works, it has a "big" problem; it is slower (since I have to check, for every key, my whole array of deleted values).
- Levels don't have files/fence pointers yet, and deallocate doesn't work.

### TO-DO:
- do ranges before prints?
- sort based on keys
- linked lists are suboptimal
- make also the buffer arrays
- ll_node size is 16 bytes, so you can fit 256 nodes per page/block (4KB).
- Implement get so that it checks the levels below.
- Implement flush/merge/push so that things can get pushed down.
- You need to write to `*.txt` files.
- You need to make sure that the fence pointers are per **BLOCK** (4KB).
- Each level has an array of files.
- Make bloom filters (how to handle multiple keys per level?). Do I have a bloom filter for the buffer?
- To my understanding I take runs and split them into 4KB pieces (approx) where the begining of each file are the fence pointers?
- Implement deallocation.

### 04/21/2020 UPDATE

Changelog:
- Sort now works based on keys.

### 04/23/2020 UPDATE

Gameplan:
So today we plan on doing the following:
- have different files for each run.
- *ONLY* merge runs when we flush it to the next level.
- keep track of how many runs we have, to find the place in the file array

Internal note: Okay, so now we have something that kinda works; we write files correctly for different runs, and should probably work for different levels (will check in a second). Now, that I need to do is to make sure the thing somehow calculates correctly how many files it needs, so it's not wasteful.

Edit: it does work with multiple levels. :)

Okay, so how the fuck do I do it? Buffer doesn't fit, so we check levels. How to I model the cascade? With a `for` loop, it seems a horrible idea, and if I don't use a `for` loop I'm doing it by hand...

Also, make sure that if the data *after* you write the new data are smaller than `max_size`, because people are retarded.

Our current problem is that:
a. The fucking shit sucks donkey dick, mainly because it doesn't correctly clear conceptually the levels, and this gets propagated.
b. The retarded donkey fuck get penetrated, because the fucking files, suck dragon dick.


Edit: the files get written correctly in level 0, and the file array seems to work.
Edit^2: great, retardo, you were opening files in the local directory, instead of the data directory. Still, the file isn't showing, so maybe there's something else wrong.
Edit numero trés: seems to be working.

Okay, so everything works now. To do for next time is to split the buffers to multiple files, if needed, and figure out how to manage fence pointers and shit.

### 05/03/2020 UPDATE
So we have:
- fence pointers need to be an array of arrays; first, you index by level, then you index by run and in the run you keep one pointer for each page

Edit: This fucking retarded shit. My program *should* work if I have enough data, but right now it doesn't... The indexing to find the fence pointer values doesn't work.

Okay, we actually have a great update; the file system should work more or less flawlessly now. As a disclaimer, it might not work perfectly if buffer sizes are greater than what a page can fit (it should, we just haven't tested yet).

So now, our next steps are:
- print our fence points to make sure they're accurate
- bloom filters
- merging strategy
- binary search on fp
- make `print_statistics` work
- (use `mmap` instead of reading files?)

Edit: `print_statistics` works. We've also printed the fence pointers and they are correct.


### 05/04/2020 UPDATE
General plan for today: make sure `get` works. This means:
- bloom filters
- binary search on fence pointers
- (maybe) merging strategy

After we've done the above, we can finish the merge strategy, and we can also look into `mmap` if we need to. After that we can move to parallelization. I also need to implement `range`, but if I'm honest I'll do it last.

Questions for OH (Wilson probably):
- how exactly does the bits per entry work on bloom filters? It is not working for me correctly. Generally questions about which `hash_function` to use, how many, how to create the tables and stuff.
- should I do ``mmap``?
- how to do external sorting? Right now what I do is I read all the files in memory and then sort them.
- Because I don't have time, should I focus on making sure that everything works flawlessly and I have a good report on the single core setting, or should I try and force the parallelization?

### 05/05/2020 UPDATE
- there is an issue with fence pointers (they seem wrong)
- bloom filters don't work.
- seems like I'm not writing data correctly.

Edit: I fixed the writing data part.

Edit^2: the fence pointers seemed to mainly be a problem with the fact that I hadn't updated the correct indexing, it now appears to work.

TO-DO that doesn't really matter: make `serialize` to be `flush`, you don't need two.

Fixed everything.

### 05/06/2020 UPDATE
General plan for today: make sure `get` works. This means:
- binary search on fence pointers
- merging strategy

I changed the `for` loop in the buffer to go in the opposite direction; the way it was is that it would find the oldest element, which is wrong. This should also be followed in the searching over runs.

Not to jinx it, but I think my binary stuff is going well. So if we finish that, we only need to fix the merging strat, and after that we're done with the basic design.
