#include <string>
#include <sstream>
#define SIGNALS_TEST
#include <Signals.h>

#include <iostream>

using namespace Signals;
using namespace std;

int main()
{
    Signal<int> notifyInt;
    Signal<> notifyVoid;

    cout << "ten connections..." << endl;
    for (int i = 0; i < 10; i++)
    {
        notifyInt.connect([](int i) { });
        notifyVoid.connect([]() { });
    }
    cout << endl << "notifyInt state:" << endl << notifyInt.getTextualState();
    cout << endl << "notifyVoid state:" << endl << notifyVoid.getTextualState();

    cout << endl << "disconnecting 2, 3, 5, 6, and 8." << endl;
    notifyInt.disconnect(2);
    notifyInt.disconnect(3);
    notifyInt.disconnect(5);
    notifyInt.disconnect(6);
    notifyInt.disconnect(8);
    notifyVoid.disconnect(2);
    notifyVoid.disconnect(3);
    notifyVoid.disconnect(5);
    notifyVoid.disconnect(6);
    notifyVoid.disconnect(8);
    cout << endl << "notifyInt state:" << endl << notifyInt.getTextualState();
    cout << endl << "notifyVoid state:" << endl << notifyVoid.getTextualState();

    cout << endl << "three more connections..." << endl;
    for (int i = 0; i < 3; i++)
    {
        notifyInt.connect([](int i) { });
        notifyVoid.connect([]() { });
    }
    cout << endl << "notifyInt state:" << endl << notifyInt.getTextualState();
    cout << endl << "notifyVoid state:" << endl << notifyVoid.getTextualState();

    cout << endl << "disconnecting 9..." << endl;
    notifyInt.disconnect(9);
    notifyVoid.disconnect(9);
    cout << endl << "notifyInt state:" << endl << notifyInt.getTextualState();
    cout << endl << "notifyVoid state:" << endl << notifyVoid.getTextualState();
    
    return 0;
}
