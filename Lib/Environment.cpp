#include "Environment.h"
#include "MPI.h"


namespace kvs
{

namespace mpi
{

Environment::Environment( int argc, char** argv )
{
    MPI_Init( &argc, &argv );
}

Environment::~Environment()
{
    MPI_Finalize();
}

} // end of namespace mpi

} // end of namespace kvs
