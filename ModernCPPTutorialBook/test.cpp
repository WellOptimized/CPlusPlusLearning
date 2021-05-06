#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <vector>

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
    std::map<std::string, long long int> m {
        {"a", 1},
        {"b", 2},
        {"c", 3}
    };
    update(m, [](std::string key) -> long long int {
        return std::hash<std::string>{}(key);
    });
    for (auto&& [key, value] : m)
        std::cout << key << ":" << value << std::endl;
}

// int main() {
//     std::cout<<average(1,2,2,3,4,4,4)<<std::endl;
// }