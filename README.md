# Project 2 - Custom Top Implementation

The goal of this project is to implement a `top(1)`-like command for gleaning
information about a Linux system. For a general idea of the type of program
you'll implement, try running the real `top` executable on an Alamode machine.
You'll see something like the following that refreshes every so often:

```
jthomas > top
21:12:35 up 12 days, 11:39,  4 users,  load average: 0.00, 0.01, 0.05
Tasks: 165 total,   1 running, 164 sleeping,   0 stopped,   0 zombie
Cpu(s):  0.4%us,  0.2%sy, 11.2%ni, 88.2%id,  0.0%wa,  0.0%hi,  0.0%si,  0.0%st
Mem:   8133452k total,  3009824k used,  5123628k free,   498784k buffers
Swap: 16385272k total,    42648k used, 16342624k free,  1854228k cached

  PID USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND
    1 root      20   0 26812 1816  736 S    0  0.0   0:01.60 init
    2 root      20   0     0    0    0 S    0  0.0   0:00.04 kthreadd
    3 root      20   0     0    0    0 S    0  0.0   1:27.20 ksoftirqd/0
    6 root      RT   0     0    0    0 S    0  0.0   0:43.42 migration/0
    7 root      RT   0     0    0    0 S    0  0.0   0:02.36 watchdog/0
    8 root      RT   0     0    0    0 S    0  0.0   0:32.60 migration/1
    9 root      20   0     0    0    0 S    0  0.0   0:00.00 kworker/1:0
   10 root      20   0     0    0    0 S    0  0.0   0:02.25 ksoftirqd/1
 ```

If you want to see what's going on under the hood on each refresh cycle, try
running this command on an Alamode machine:

`strace top -b -n 1  > /dev/null`


## Building and running the program

To build the main program (called 'mytop' by default), type:

`make`

To both build and run the program, use:

`make run`

To run the unit tests, type:

`make test`

The first time you run the tests after cloning the code, you'll be prompted to
initialize the `googletest` Git submodule. If you typically access GitHub
via HTTPS rather than SSH, you'll need to update the URL in the `.gitmodules`
file prior to this step. The following command will set up the `googletest`
submodule so that you're ready to run the tests:

`git submodule update --init`

For this starter code, I did my best to ensure that `make` knows all about the
dependencies for all of the code. So, if you change a file, re-running `make`
should rebuild everything with your changes. You shouldn't need to run
`make clean` at all, though you can if you want.

The tests run using a snapshot of the `proc` filesystem that I took from
bb136-21. It's included in the repository as `proc.tar.bz2`, which is a
compressed archive. Using `make test` should automatically decompress and
unarchive this snapshot and make it available via a symlink in the root of your
repository.

On Alamode machines, the snapshot is unarchived under `/dev/shm`, which is an
in-memory file system. Since it's in memory, it's really fast. Your home
directory on Alamode machines is on a file server somewhere (not local to the
machine), so it's really really slow. On a personal computer, the snapshot is
simply placed in your $TMPDIR. It's not as fast as an in-memory file system, but
at least it's not on some remote file server.

The snapshot is also available as a repository here:

https://github.com/csm-csci442/proc-snapshot


## Useful links

 - `man 5 proc` - http://linux.die.net/man/5/proc
 - NCURSES Reference - http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/
 - `fscanf` reference - http://www.cplusplus.com/reference/cstdio/fscanf/


## Coding on a Mac

You can build this program on a Mac, but since the `/proc` filesystem is a Linux
construct, your finished product won't work correctly under OS X. That doesn't
mean you can't do much of your work on your Mac.

All of deliverable 1, which is simply getting unit tests to pass, is feasible.
Likewise, since much of your work for the final deliverable is just displaying
data, you can work with fake data to get things looking right. Once you're ready
to test your program for real, though, you need to work on a Linux machine.

As always, you need to check that your program compiles and runs on Alamode
machines prior to submitting them.


## Final thoughts

Start early and ask questions!

...or not. I'm a sign, not a cop. =)
