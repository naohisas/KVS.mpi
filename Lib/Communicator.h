#pragma once
#include <mpi.h>
#include <kvs/Type>
#include <kvs/ValueArray>
#include "Operator.h"
#include "DataType.h"


namespace kvs
{

namespace mpi
{

class Communicator
{
private:
    MPI_Comm m_comm;

public:
    Communicator( const MPI_Comm comm = MPI_COMM_WORLD );

    int size() const;
    int rank() const;

    template <typename T>
    void send( const int dst, const int tag, const T value );

    template <typename T>
    void send( const int dst, const int tag, const kvs::ValueArray<T>& values );

    template <typename T>
    void send( const int dst, const int tag, const T* values, const size_t size );

    template <typename T>
    MPI_Status receive( const int src, const int tag, T& value );

    template <typename T>
    MPI_Status receive( const int src, const int tag, kvs::ValueArray<T>& values );

    template <typename T>
    MPI_Status receive( const int src, const int tag, T* values, const size_t size );

    template <typename T>
    void broadcast( const int root, T& value );

    template <typename T>
    void broadcast( const int root, kvs::ValueArray<T>& values );

    template <typename T>
    void broadcast( const int root, T* values, const size_t size );

    template <typename T>
    void gather( const int root, const T& send_value, kvs::ValueArray<T>& recv_values );

    template <typename T>
    void gather( const int root, const kvs::ValueArray<T>& send_values, kvs::ValueArray<T>& recv_values );

    template <typename T>
    void gather( const int root, const T* send_values, const size_t send_size, T* recv_values, const size_t recv_size );

    template <typename T>
    void gatherAll( const T& send_value, kvs::ValueArray<T>& recv_values );

    template <typename T>
    void gatherAll( const kvs::ValueArray<T>& send_values, kvs::ValueArray<T>& recv_values );

    template <typename T>
    void gatherAll( const T* send_values, const size_t send_size, T* recv_values, const size_t recv_size );

    template <typename T, typename Op>
    void reduce( const int root, const T& send_value, T& recv_value, const Op op );

    template <typename T, typename Op>
    void reduce( const int root, const kvs::ValueArray<T>& send_value, kvs::ValueArray<T>& recv_value, const Op op );

    template <typename T>
    void reduce( const int root, const T& send_value, T& recv_value, const MPI_Op op );

    template <typename T>
    void reduce( const int root, const T* send_values, T* recv_values, const size_t size, const MPI_Op op );

    template <typename T, typename Op>
    void reduceAll( const int root, const T& send_value, T& recv_value, const Op op );

    template <typename T, typename Op>
    void reduceAll( const int root, const kvs::ValueArray<T>& send_value, kvs::ValueArray<T>& recv_value, const Op op );

    template <typename T>
    void reduceAll( const int root, const T& send_value, T& recv_value, const MPI_Op op );

    template <typename T>
    void reduceAll( const int root, const T* send_values, T* recv_values, const size_t size, const MPI_Op op );
};

template <typename T>
inline void Communicator::send( const int dst, const int tag, const T value )
{
    this->send<T>( dst, tag, &value, 1 );
}

template <typename T>
inline void Communicator::send( const int dst, const int tag, const kvs::ValueArray<T>& values )
{
    this->send<kvs::UInt32>( dst, tag, kvs::UInt32( values.size() ) );
    this->send<T>( dst, tag, values.data(), values.size() );
}

template <typename T>
inline void Communicator::send( const int dst, const int tag, const T* values, const size_t size )
{
    const MPI_Datatype type = kvs::mpi::DataType<T>::Enum();
    MPI_Send( values, static_cast<int>(size), type, dst, tag, m_comm );
}

template <typename T>
inline MPI_Status Communicator::receive( const int src, const int tag, T& value )
{
    return this->receive<T>( src, tag, &value, 1 );
}

template <typename T>
inline MPI_Status Communicator::receive( const int src, const int tag, kvs::ValueArray<T>& values )
{
    kvs::UInt32 size = 0;
    MPI_Status status = this->receive<kvs::UInt32>( src, tag, size );

    if ( size > values.size() ) { values.allocate( size ); }
    status = this->receive<T>( src, tag, values.data(), values.size() );
    return status;
}

template <typename T>
inline MPI_Status Communicator::receive( const int src, const int tag, T* values, const size_t size )
{
    MPI_Status status;
    const MPI_Datatype type = kvs::mpi::DataType<T>::Enum();
    MPI_Recv( values, static_cast<int>(size), type, src, tag, m_comm, &status );
    return status;
}

template <typename T>
inline void Communicator::broadcast( const int root, T& value )
{
    this->broadcast<T>( root, &value, 1 );
}

template <typename T>
inline void Communicator::broadcast( const int root, kvs::ValueArray<T>& values )
{
    const int rank = this->rank();
    if ( rank == root )
    {
        // send
        kvs::UInt32 size = static_cast<kvs::UInt32>( values.size() );
        this->broadcast<kvs::UInt32>( root, size );
        this->broadcast<T>( root, values.data(), values.size() );
    }
    else
    {
        // recv
        kvs::UInt32 size = 0;
        this->broadcast<kvs::UInt32>( root, size );

        if ( size > values.size() ) { values.allocate( size ); }
        this->broadcast<T>( root, values.data(), values.size() );
    }
}

template <typename T>
inline void Communicator::broadcast( const int root, T* values, const size_t size )
{
    const MPI_Datatype type = kvs::mpi::DataType<T>::Enum();
    MPI_Bcast( values, static_cast<int>(size), type, root, m_comm );
}

template <typename T>
inline void Communicator::gather( const int root, const T& send_value, kvs::ValueArray<T>& recv_values )
{
    const int rank = this->rank();
    if ( rank == root )
    {
        // recv
        const kvs::UInt32 size = this->size();
        if ( size > recv_values.size() ) { recv_values.allocate( size ); }

        const int gather_size = 1;
        this->gather<T>( root, &send_value, gather_size, recv_values.data(), gather_size );
    }
    else
    {
        // send
        const int gather_size = 1;
        this->gather<T>( root, &send_value, gather_size, NULL, gather_size );
    }
}

template <typename T>
inline void Communicator::gather( const int root, const kvs::ValueArray<T>& send_values, kvs::ValueArray<T>& recv_values )
{
    const int rank = this->rank();
    if ( rank == root )
    {
        // recv
        const kvs::UInt32 size = this->size() * send_values.size();
        if ( size > recv_values.size() ) { recv_values.allocate( size ); }

        const int gather_size = send_values.size();
        this->gather<T>( root, send_values.data(), gather_size, recv_values.data(), gather_size );
    }
    else
    {
        // send
        const int gather_size = send_values.size();
        this->gather<T>( root, send_values.data(), gather_size, NULL, gather_size );
    }
}

template <typename T>
inline void Communicator::gather( const int root, const T* send_values, const size_t send_size, T* recv_values, const size_t recv_size )
{
    const MPI_Datatype type = kvs::mpi::DataType<T>::Enum();
    MPI_Gather( send_values, static_cast<int>(send_size), type, recv_values, static_cast<int>(recv_size), type, root, m_comm );
}

template <typename T>
inline void Communicator::gatherAll( const T& send_value, kvs::ValueArray<T>& recv_values )
{
    const kvs::UInt32 size = this->size();
    if ( size > recv_values.size() ) { recv_values.allocate( size ); }

    const int gather_size = 1;
    this->gatherAll<T>( &send_value, gather_size, recv_values.data(), gather_size );
}

template <typename T>
inline void Communicator::gatherAll( const kvs::ValueArray<T>& send_values, kvs::ValueArray<T>& recv_values )
{
    const kvs::UInt32 size = this->size() * send_values.size();
    if ( size > recv_values.size() ) { recv_values.allocate( size ); }

    const int gather_size = send_values.size();
    this->gatherAll<T>( send_values.data(), gather_size, recv_values.data(), gather_size );
}

template <typename T>
inline void Communicator::gatherAll( const T* send_values, const size_t send_size, T* recv_values, const size_t recv_size )
{
    const MPI_Datatype type = kvs::mpi::DataType<T>::Enum();
    MPI_Allgather( send_values, static_cast<int>(send_size), type, recv_values, static_cast<int>(recv_size), type, m_comm );
}

template <typename T, typename Op>
inline void Communicator::reduce( const int root, const T& send_value, T& recv_value, const Op op )
{
    MPI_Op type = OperatorType<Op,T>::Enum();
    this->reduce<T>( root, &send_value, &recv_value, 1, type );
}

template <typename T, typename Op>
inline void Communicator::reduce( const int root, const kvs::ValueArray<T>& send_values, kvs::ValueArray<T>& recv_values, const Op op )
{
    const int rank = this->rank();
    if ( rank == root )
    {
        // recv
        const kvs::UInt32 size = send_values.size();
        if ( size > recv_values.size() ) { recv_values.allocate( size ); }

        const MPI_Op mpi_op = OperatorType<Op,T>::Enum();
        this->reduce<T>( root, send_values.data(), recv_values.data(), send_values.size(), mpi_op );
    }
    else
    {
        // send
        const MPI_Op mpi_op = OperatorType<Op,T>::Enum();
        this->reduce<T>( root, send_values.data(), NULL, send_values.size(), mpi_op );
    }
}

template <typename T>
inline void Communicator::reduce( const int root, const T& send_value, T& recv_value, const MPI_Op op )
{
    this->reduce<T>( root, &send_value, &recv_value, 1, op );
}

template <typename T>
inline void Communicator::reduce( const int root, const T* send_values, T* recv_values, const size_t size, const MPI_Op op )
{
    const MPI_Datatype type = kvs::mpi::DataType<T>::Enum();
    MPI_Reduce( send_values, recv_values, static_cast<int>(size), type, op, root, m_comm );
}


template <typename T, typename Op>
inline void Communicator::reduceAll( const int root, const T& send_value, T& recv_value, const Op op )
{
    MPI_Op type = OperatorType<Op,T>::Enum();
    this->reduceAll<T>( root, &send_value, &recv_value, 1, type );
}

template <typename T, typename Op>
inline void Communicator::reduceAll( const int root, const kvs::ValueArray<T>& send_values, kvs::ValueArray<T>& recv_values, const Op op )
{
    const int rank = this->rank();
    if ( rank == root )
    {
        // recv
        const kvs::UInt32 size = send_values.size();
        if ( size > recv_values.size() ) { recv_values.allocate( size ); }

        const MPI_Op mpi_op = OperatorType<Op,T>::Enum();
        this->reduceAll<T>( root, send_values.data(), recv_values.data(), send_values.size(), mpi_op );
    }
    else
    {
        // send
        const MPI_Op mpi_op = OperatorType<Op,T>::Enum();
        this->reduceAll<T>( root, send_values.data(), NULL, send_values.size(), mpi_op );
    }
}

template <typename T>
inline void Communicator::reduceAll( const int root, const T& send_value, T& recv_value, const MPI_Op op )
{
    this->reduceAll<T>( root, &send_value, &recv_value, 1, op );
}

template <typename T>
inline void Communicator::reduceAll( const int root, const T* send_values, T* recv_values, const size_t size, const MPI_Op op )
{
    const MPI_Datatype type = kvs::mpi::DataType<T>::Enum();
    MPI_Allreduce( send_values, recv_values, static_cast<int>(size), type, op, root, m_comm );
}

} // end of namespace mpi

} // end of namespace kvs
