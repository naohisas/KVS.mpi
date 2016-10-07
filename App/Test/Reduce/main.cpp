#include <KVS.mpi/Lib/Environment.h>
#include <KVS.mpi/Lib/Communicator.h>
#include <KVS.mpi/Lib/Operator.h>
#include "../Util.h"

int main( int argc, char** argv )
{
    kvs::mpi::Environment env( argc, argv );
    kvs::mpi::Communicator world;

    const int root = 0;
    const int rank = world.rank();

    kvs::ValueArray<int> send_values( 3 );
    send_values[0] = rank;
    send_values[1] = rank;
    send_values[2] = rank;
    Util::PrintSend( rank, send_values, root );

    kvs::ValueArray<int> recv_values;
    world.reduce( root, send_values, recv_values, kvs::mpi::Operator::Sum<int>() );
    if ( rank == root ) { Util::PrintRecv( rank, recv_values ); }

    return 0;
}
