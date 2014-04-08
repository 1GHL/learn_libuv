#include <iostream>

#include "forwserver.h"
using namespace std;

using namespace Dosee::ForwNode;

int main()
{
    cout << "Hello World!" << endl;
    start(8888,9999);
//    ForwServer forw(5006,5007);
//    forw.start_tcp();
    return 0;
}

