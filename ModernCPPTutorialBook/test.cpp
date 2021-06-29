#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <vector>
using namespace std;
template <typename ... T>
auto average(T ... a){
    return (a + ...)/sizeof...(a);
}
     
template <typename Key, typename Value, typename F>
void update(std::map<Key, Value>& m, F foo) {
    for(auto&& [key,value]:m){
        value=foo(key);
    }
}

int main() {
    {
        int a=3;
    }
    cout<<a<<endl;
}

// int main() {
//     std::cout<<average(1,2,2,3,4,4,4)<<std::endl;
// }