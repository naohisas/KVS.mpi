#include "Communicator.h"
#include <kvs/Type>


namespace kvs
{

namespace mpi
{

Communicator::Communicator( const MPI_Comm comm ):
    m_comm( comm )
{
}

int Communicator::size() const
{
    int size;
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    return size;
}

int Communicator::rank() const
{
    int rank;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );

    return rank;
}

} // end of namespace mpi

} // end of namespace kvs
