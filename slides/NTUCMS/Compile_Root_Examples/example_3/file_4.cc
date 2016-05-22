#include <iostream>
using namespace std;

void function_2();
void function_3();

void function_4(){
   cout << "=== function 4 ===" << endl;
   for( int i = 0 ; i < 5 ; ++i ){
      function_2();
      function_3();
   }
}
