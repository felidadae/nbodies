#ifndef N_BODIES_SYSTEM_PARAMS_HEADER
#define N_BODIES_SYSTEM_PARAMS_HEADER

#include <iostream>



typedef float position_type;
typedef float mass_type;
typedef double time_type;

static const int X = 0;
static const int Y = 1;
static const int Z = 2;



/*
	@Abbreviations/naming scheme
		@D <- dimensions count (2 for 2d, 3 for 3d, etc.)
		@N <- bodies count

	@Description:
		data is kept as dynamically alocated array;
			[all data from dimension 0] [all data from dimension 1] ...
		to focus only on one dimension use [] operator 
			e.g. 
			Params<float> position(3, 5); 
			position[DIM=2][ELEMENT=3];
*/
template<typename T>
class Params {
public:
	T* data;
	unsigned getD() const { return D; }
	unsigned getN() const { return N; }
	Params(unsigned D, unsigned N);
	~Params();
	inline 			T* operator [] (unsigned d);
	inline void operator = (Params& other);
	inline void setAllElementsTo (T value);
private:
	unsigned D, N;
};

template<typename T>
std::ostream &
	operator<<(std::ostream &os, Params<T> &p) 
{ 
	for (int d = 0; d < p.getD(); ++d) {
		os << "dim: " << d << std::endl << "\t";
		for (int i = 0; i < p.getN(); ++i)
			os << p[d][i] << " ";
		os << std::endl;
	}
    return os;
}

template<typename T>
Params<T>::Params(unsigned D, unsigned N) {
	data = new T[N*D];
	this->D = D;
	this->N = N;
}
template<typename T>
Params<T>::~Params() {
	delete[] data;
}
template<typename T>
T* Params<T>::operator [] (unsigned d) {
	return data + (d * N); 
}
template<typename T>
void Params<T>::operator = (Params& other) {
	for (int d = 0; d < D; ++d)
		for (int i = 0; i < N; ++i)
			(*this)[d][i] = other[d][i];
}
template<typename T>
void Params<T>::setAllElementsTo (T value) {
	for (int d = 0; d < D; ++d)
		for (int i = 0; i < N; ++i)
			(*this)[d][i] = value;
}



#endif