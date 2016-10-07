#include <KVS.mpi/Lib/Environment.h>
#include <KVS.mpi/Lib/Communicator.h>
#include "../Util.h"

int main( int argc, char** argv )
{
    kvs::mpi::Environment env( argc, argv );
    kvs::mpi::Communicator world;

    const int root = 0;
    const int size = world.size();
    const int rank = world.rank();
    if ( rank != root )
    {
        const int to = root;
        kvs::ValueArray<int> values( 3 );
        values[0] = rank;
        values[1] = rank;
        values[2] = rank;
        Util::PrintSend( rank, values, to );
        world.send( to, 0, values );
    }
    else
    {
        for ( int i = 0; i < size; i++ )
        {
            if ( i == root ) { continue; }
            const int from = i;
            kvs::ValueArray<int> values;
            world.receive( from, 0, values );
            Util::PrintRecv( rank, values, from );
        }
    }

    return 0;
}
