#pragma once
#include <mpi.h>
#include <kvs/Type>
#include <kvs/ValueArray>


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

private:
    template <typename T>
    MPI_Datatype data_type() const { return MPI_DATATYPE_NULL; }
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
    const MPI_Datatype type = this->data_type<T>();
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
    const MPI_Datatype type = this->data_type<T>();
    MPI_Recv( values, static_cast<int>(size), type, src, tag, m_comm, &status );
    return status;
}

template <> inline MPI_Datatype Communicator::data_type<kvs::Int8>() const { return MPI_CHAR; }
template <> inline MPI_Datatype Communicator::data_type<kvs::UInt8>() const { return MPI_UNSIGNED_CHAR; }
template <> inline MPI_Datatype Communicator::data_type<kvs::Int16>() const { return MPI_SHORT; }
template <> inline MPI_Datatype Communicator::data_type<kvs::UInt16>() const { return MPI_UNSIGNED_SHORT; }
template <> inline MPI_Datatype Communicator::data_type<kvs::Int32>() const { return MPI_INT; }
template <> inline MPI_Datatype Communicator::data_type<kvs::UInt32>() const { return MPI_UNSIGNED; }
template <> inline MPI_Datatype Communicator::data_type<kvs::Real32>() const { return MPI_FLOAT; }
template <> inline MPI_Datatype Communicator::data_type<kvs::Real64>() const { return MPI_DOUBLE; }

} // end of namespace mpi

} // end of namespace kvs
