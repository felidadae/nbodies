#ifndef N_BODIES_SYSTEM_PARAMS_HEADER
#define N_BODIES_SYSTEM_PARAMS_HEADER

#include <iostream>



template<typename T>
class Params {
public:
	T* data;
	unsigned getD() const { return D; }
	unsigned getN() const { return N; }
	Params(unsigned D, unsigned N);
	~Params();
	inline T* operator [] (unsigned d);
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