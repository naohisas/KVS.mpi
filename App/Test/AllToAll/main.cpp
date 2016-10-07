#include <KVS.mpi/Lib/Environment.h>
#include <KVS.mpi/Lib/Communicator.h>
#include "../Util.h"

int main( int argc, char** argv )
{
    kvs::mpi::Environment env( argc, argv );
    kvs::mpi::Communicator world;

    const int rank = world.rank();
    kvs::ValueArray<int> send_values( world.size() );
    for ( int i = 0; i < world.size(); i++ ) { send_values[i] = i; }
    Util::PrintSend( rank, send_values );

    kvs::ValueArray<int> recv_values;
    world.allToAll( send_values, recv_values );
    Util::PrintRecv( rank, recv_values );

    return 0;
}
