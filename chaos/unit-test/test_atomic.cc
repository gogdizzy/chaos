#include <stdio.h>
#include <atomic>

enum class E { A, B };

template <typename T>
class Atomic {
public:
	Atomic( T x ) : _x(x) {}
	T load() { printf( "enter load()\n" ); return _x; }
	operator T() noexcept { printf("enter operator T()\n"); return load(); }
private:
	T _x;

};

int main() {

	Atomic<double> x(1);
	if( x == 1U ) printf( "%f\n", x.load() );
	return 0;
}
