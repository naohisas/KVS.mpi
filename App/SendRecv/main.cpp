#include <mpi.h>
#include <iostream>
#include <KVS.mpi/Lib/Environment.h>
#include <KVS.mpi/Lib/Communicator.h>
#include <KVS.mpi/Lib/Operator.h>

namespace
{

template <typename T>
std::ostream& operator << ( std::ostream& os, const kvs::ValueArray<T>& values )
{
    for ( size_t i = 0; i < values.size(); i++ )
    {
        os << values[i];
        if ( i != values.size() - 1 ) { os << ","; }
    }
    return os;
}

template <typename T>
void PrintSend( const int rank, const int dst, const kvs::ValueArray<T>& values )
{
    std::cout << "Rank " << rank << ": Send " << "{" << values << "} to Rank " << dst << std::endl;
}

template <typename T>
void PrintRecv( const int rank, const int src, const kvs::ValueArray<T>& values )
{
    std::cout << "Rank " << rank << ": Recv " << "{" << values << "} from Rank " << src << std::endl;
}

}

int main( int argc, char** argv )
{
    kvs::mpi::Environment env( argc, argv );
    kvs::mpi::Communicator world;

    const int root = 0;
    const int size = world.size();
    const int rank = world.rank();
    if ( rank != root )
    {
        int dst = root;
        kvs::ValueArray<int> values( 3 );
        values[0] = rank;
        values[1] = rank;
        values[2] = rank;
        world.send( dst, 0, values );
        ::PrintSend( rank, dst, values );
    }
    else
    {
        for ( int i = 1; i < size; i++ )
        {
            int src = i;
            kvs::ValueArray<int> values;
            world.receive( src, 0, values );
            ::PrintRecv( rank, src, values );
        }
    }

    return 0;
}
