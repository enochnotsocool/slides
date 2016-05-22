#include <iostream>
using namespace std;

class MyClass
{
public:
   MyClass ();
   ~MyClass ();

   void Call();
};

MyClass::MyClass() {}
MyClass::~MyClass() {}

void MyClass::Call()
{
   cout << "In my class!" << endl;
}
