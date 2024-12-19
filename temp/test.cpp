#include <iostream>

class test{
    protected:
        void a(){
            std::cout << "test" << std::endl;
        }
};

class test2{
    public:
        test t;
        void b(){
            t.a();
        }
};

int main(){
    test2 t;
    t.b();
    return 0;
}