#include "Request.h"

namespace kvs
{

namespace mpi
{

Request::Request( MPI_Request request ):
    m_request( request )
{
}

MPI_Status Request::wait()
{
    MPI_Status status;
    MPI_Wait( &m_request, &status );
    return status;
}

MPI_Status Request::test()
{
    int flag;
    MPI_Status status;
    MPI_Test( &m_request, &flag, &status );
    return status;
}

void Request::cancel()
{
    MPI_Cancel( &m_request );
}

} // end of namespace mpi

} // end of namespace kvs
