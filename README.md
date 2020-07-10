# KVS.mpi
KVS.mpi is a class library with message passing interface (MPI) for KVS.

```diff
- Moved to KVS as SupportMPI (2020.7.10)
```

## Prerequisite
The following software packages are needed to compile KVS.mpi.

### MPI Compiler
MPI compiler, such as OpenMPI and MPICH, must be installed in your computing environment before compiling KVS.mpi.

#### Linux
For Linux, OpenMPI will be available by installing libopenmpi-dev. The following shows a command example with apt-get on Ubuntu Linux.

```
$ apt-get install openmpi-bin libopenmpi-dev
```

#### Mac OS X
In case of Mac OS X environment, OpenMPI can be installed with HomeBrew as follows:

```
$ brew install openmpi
```

## KVS
The source code of KVS can be downloaded via GitHub as follows:

```
$ git clone https://github.com/naohisas/KVS.git
```

The environment variable ```KVS_DIR``` should be set to an install directory of KVS. Here is an example of setting the variable. You can change the install directory to other directory, if you want.

```
export KVS_DIR=~/local/kvs
```

KVS can be compiled as follows:

1. Change directory to KVS
```
$ cd KVS
```

2. Build and install KVS
```
$ make
$ make install
```

# Installation
You can get the source codes of KVS.mpi from GitHub.
```
$ git clone https://github.com/vizlab-kobe/KVS.mpi.git
```

## Build
First of all, you need to build a library with a python script.
```
$ cd KVS.mpi
$ cd Lib
$ ./kvsmake.py
```

You can also change the compiler for compiling Lib by using an environment parameter ```KVS_CPP```. The following is an example of the use of ```mpicxx```.
```
$ KVS_CPP=mpicxx ./kvsmake.py
```

## Test
Several test programs with KVS.mpi can be found in the App directory. Each of the programs can be easily compiled with kvsmake command.
```
$ cd App
$ cd Test/HelloWorld
$ kvsmake -G
$ KVS_CPP=mpicxx kvsmake
```

The execution result of the "HelloWorld" program is shown as follows:
```
$ ./run.sh
Size = 4, Rank = 0
Size = 4, Rank = 1
Size = 4, Rank = 2
Size = 4, Rank = 3
```

If you use KVS compiled with the different compiler from the mpicxx wrapper compiler, you need to specify the compiler by using the environment variable ```OMPI_CXX``` for OpenMP or ```MPICH_CXX``` for MPICH as follows:

- OpenMP
```
$ OMPI_CXX=g++-6 KVS_CPP=mpicxx kvsmake
```

- MPICH
```
$ MPICH_CXX=g++-6 KVS_CPP=mpicxx kvsmake
```

### HPC Environment

- K computer (RIKEN)
```
$ KVS_CPP=mpiFCCpx kvsmake
```

- Pi-computer (Kobe University)
```
$ KVS_CPP=mpiFCCpx kvsmake
```

- Pi-VizStudio (Kobe University)
```
$ KVS_CPP=mpicxx kvsmake
```
