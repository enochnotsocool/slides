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
   for( int i = 0 ; i < 10 ; ++i ){
      function_3();
   }
   MyClass x;
   for( int i = 0 ; i < 10 ; ++i ){
      x.Call();
   }
}
