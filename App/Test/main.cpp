#include <mpi.h>
#include <iostream>
#include <KVS.mpi/Lib/Environment.h>
#include <KVS.mpi/Lib/Communicator.h>
#include <KVS.mpi/Lib/Operator.h>


template <typename T>
std::ostream& operator << ( std::ostream& os, const kvs::ValueArray<T>& values )
{
    for ( size_t i = 0; i < values.size(); i++ )
    {
        os << values[i];
        if ( i != values.size() - 1 ) os << ",";
    }
    return os;
}

int size_and_rank( int argc, char** argv )
{
    kvs::mpi::Environment env( argc, argv );
    kvs::mpi::Communicator world;

    const int size = world.size();
    const int rank = world.rank();
    std::cout << "Size = " << size << ", Rank = " << rank << std::endl;

    return 0;
}

int send_and_recv_value( int argc, char** argv )
{
    kvs::mpi::Environment env( argc, argv );
    kvs::mpi::Communicator world;

    const int size = world.size();
    if ( world.rank() != 0 )
    {
        int dst = 0;
        int value = 10;
        world.send( dst, 0, value );
        std::cout << "Send value (" << value << ") to " << dst << std::endl;
    }
    else
    {
        for ( int i = 1; i < size; i++ )
        {
            int src = i;
            int value = 0;
            world.receive( src, 0, value );
            std::cout << "Receive value (" << value << ") from " << src << std::endl;
        }
    }

    return 0;
}

int send_and_recv_array( int argc, char** argv )
{
    kvs::mpi::Environment env( argc, argv );
    kvs::mpi::Communicator world;

    const int size = world.size();
    if ( world.rank() != 0 )
    {
        int dst = 0;
        kvs::ValueArray<int> values( 3 );
        values[0] = world.rank();
        values[1] = world.rank();
        values[2] = world.rank();
        world.send( dst, 0, values );
        std::cout << "Send value (" << values[0] << "," << values[1] <<"," << values[2] << ") to " << dst << std::endl;
    }
    else
    {
        for ( int i = 1; i < size; i++ )
        {
            int src = i;
            kvs::ValueArray<int> values;
            world.receive( src, 0, values );
            std::cout << "Receive value (" << values[0] << "," << values[1] << "," << values[2] << ") from " << src << std::endl;
        }
    }

    return 0;
}

int broadcast_array( int argc, char** argv )
{
    kvs::mpi::Environment env( argc, argv );
    kvs::mpi::Communicator world;

    const int root = 0;
    const int rank = world.rank();

    kvs::ValueArray<int> values;
    if ( rank == root )
    {
        values.allocate( 3 );
        values[0] = 1;
        values[1] = 2;
        values[2] = 3;
    }

    world.broadcast( root, values );
    std::cout << "Values (" << values[0] << "," << values[1] << "," << values[2] << ") at " << rank << std::endl;

    return 0;
}

int reduce_array( int argc, char** argv )
{
    kvs::mpi::Environment env( argc, argv );
    kvs::mpi::Communicator world;

    const int root = 0;
    const int rank = world.rank();

    kvs::ValueArray<int> send_values( 3 );
    send_values[0] = rank;
    send_values[1] = rank;
    send_values[2] = rank;

    kvs::ValueArray<int> recv_values;
    world.reduce( root, send_values, recv_values, kvs::mpi::Operator::Sum<int>() );
    std::cout << "Send Values (" << send_values << ") from " << rank << std::endl;

    if ( rank == root )
    {
        std::cout << "Recv Values (" << recv_values << ") from " << rank << std::endl;
    }

    return 0;
}

int gather_array( int argc, char** argv )
{
    kvs::mpi::Environment env( argc, argv );
    kvs::mpi::Communicator world;

    const int root = 0;
    const int rank = world.rank();

    kvs::ValueArray<int> send_values( 3 );
    send_values[0] = rank;
    send_values[1] = rank;
    send_values[2] = rank;

    kvs::ValueArray<int> recv_values;
    world.gather( root, send_values, recv_values );
//    world.gather( root, rank, recv_values );
    std::cout << "Send Values (" << send_values << ") from " << rank << std::endl;
//    std::cout << "Send value " << rank << " from " << rank << std::endl;

    if ( rank == root )
    {
        std::cout << "Recv Values (" << recv_values << ") from " << rank << std::endl;
    }

    return 0;
}

int main( int argc, char** argv )
{
//    return size_and_rank( argc, argv );
//    return send_and_recv_value( argc, argv );
//    return send_and_recv_array( argc, argv );
//    return broadcast_array( argc, argv );
//    return reduce_array( argc, argv );
    return gather_array( argc, argv );
}
