#include <KVS.mpi/Lib/Environment.h>
#include <KVS.mpi/Lib/Communicator.h>
#include "../Util.h"

int main( int argc, char** argv )
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
        Util::PrintSend( rank, values );
    }

    world.broadcast( root, values );
    if ( rank != root ) { Util::PrintRecv( rank, values, root ); }

    return 0;
}
