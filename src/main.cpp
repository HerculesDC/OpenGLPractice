#include "headers/SingletonHolder.hpp"
#include "headers/App.hpp"

template <typename T> T& SingletonHolder<T>::s_instance = *(new T());
int main() {
	SingletonHolder<App>::s_instance.run();
	return 0;
}