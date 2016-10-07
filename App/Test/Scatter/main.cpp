#include <KVS.mpi/Lib/Environment.h>
#include <KVS.mpi/Lib/Communicator.h>
#include "../Util.h"

int main( int argc, char** argv )
{
    kvs::mpi::Environment env( argc, argv );
    kvs::mpi::Communicator world;

    const int root = 0;
    const int rank = world.rank();

    kvs::ValueArray<int> send_values( 3 * world.size() );
    if ( rank == root )
    {
        for ( int i = 0; i < world.size(); i++ )
        {
            send_values[ 3 * i + 0 ] = i;
            send_values[ 3 * i + 1 ] = i;
            send_values[ 3 * i + 2 ] = i;
        }

        Util::PrintSend( rank, send_values );
    }

    kvs::ValueArray<int> recv_values;
    world.scatter( root, send_values, recv_values );
    Util::PrintRecv( rank, recv_values, root );

    return 0;
}
