#pragma once
#include <mpi.h>


namespace kvs
{

namespace mpi
{

class Request
{
private:

    MPI_Request m_request;

public:

    Request( MPI_Request request = MPI_REQUEST_NULL );

    MPI_Status wait();
    MPI_Status test();
    void cancel();
};

} // end of namespace mpi

} // end of namespace kvs
