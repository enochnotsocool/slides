#include <iostream>
using namespace std;

class MyClass
{
public:
   MyClass ();
   ~MyClass ();
   void Call();
}; // in class.cc;

void function_1(); // in file_1.cc
void function_2(); // in file_2.cc
void function_3(); // in file_2.cc

int main() {
   MyClass x;
   cout << "In main function!" << endl;
   function_1();
   function_2();
   function_3();
   x.Call();
   return 0;
}
