#pragma once
#include <iostream>
#include <kvs/ValueArray>

namespace Util
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
void PrintSend( const int rank, const kvs::ValueArray<T>& values, const int to = -1 )
{
    std::cout << "Rank " << rank << ": Send " << "{" << values << "} to ";
    if ( to == -1 ) { std::cout << "all" << std::endl; }
    else { std::cout << "Rank " << to << std::endl; }
}

template <typename T>
void PrintRecv( const int rank, const kvs::ValueArray<T>& values, const int from = -1 )
{
    std::cout << "Rank " << rank << ": Recv " << "{" << values << "} from ";
    if ( from == -1 ) { std::cout << "all" << std::endl; }
    else { std::cout << "Rank " << from << std::endl; }
}

} // end of namespace Util
