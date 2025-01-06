#include "TcpSocketClient.h"

using namespace std;

void testClient() {
    TcpSocketClient client;
    client.Request(54000);
    cout << "Client started" << endl;
    cout << "Received: " << client.ReceiveData(1024) << endl;
}
