#ifndef N_BODIES_SYSTEM_IPARAMS_HEADER
#define N_BODIES_SYSTEM_IPARAMS_HEADER

#include <iostream>

#include "../types.h"

static const int X = 0;
static const int Y = 1;
static const int Z = 2;

#define INTERLEAVED Params<position_type>::Interleaved
#define INTERLEAVED_INT 1
#define SEPERATED Params<position_type>::Seperated
#define SEPERATED_INT 0
#define CHOOSEN_DATA_ALIGNMENT SEPERATED

#define ALIGN CHOOSEN_DATA_ALIGNMENT
#define ALIGN_INT 0



/*
	@Description:	
		Two dimensional container, which can store data in two ways,
			or as x0 y0 z0 ... x1 y1 z1 ...
			or as x0 x1 x2 x3 x4 ... y0 y1 y2 y3 y4 ... z0 z1 z2 z3 z4 ... ......
*/
template <typename T>
class Params {
public:
	enum StorageMethod {
		Seperated   = 0,
		Interleaved = 1
	};
	Params(int D, int N, StorageMethod storageMethod);
	Params(const Params& other);
	const Params& operator = (const Params& other);

	inline unsigned getD() const;
	inline unsigned getN() const;
	inline StorageMethod getStorageMethod() const;

	inline T getVal(
		unsigned dimIdx, 
		unsigned elementIdx) const;
	inline void setVal(
		unsigned dimIdx, 
		unsigned elementIdx, 
		T value);

	inline void setAllElementsTo (T value);
	inline void printAllElements (
		std::string name = "this") const;

	/*
		CUDA versions needs pointer to data;
	*/
	T* data;

private:
	int D, N;
	StorageMethod storageMethod;
};



template <typename T>
Params<T>::Params(
	int D, int N, 
	StorageMethod storageMethod) 
{  
	data = new T[D*N]; 
	this->D = D; 
	this->N = N; 
	this->storageMethod = storageMethod;
}

template <typename T>
Params<T>::Params(const Params<T>& other) {  
	data = new T[other.D*other.N];
	for (int i = 0; i<other.D*other.N; ++i) {
		data[i] = other.data[i];
	}
	this->D = other.D; 
	this->N = other.N; 
	this->storageMethod = other.storageMethod;
}

template <typename T>
const Params<T>& 
	Params<T>::operator = (
		const Params<T>& other) 
{
	for (int d = 0; d < D; ++d)
	{
		for (int i = 0; i < N; ++i)
		{
			setVal(d, i, other.getVal(d, i));
		}
	}
	return *this;
}

template <typename T>
unsigned Params<T>::getD() const {
	return this->D;
}

template <typename T>
unsigned Params<T>::getN() const {
	return this->N;
}

template <typename T>
typename Params<T>::StorageMethod 
	Params<T>::getStorageMethod() const 
{
	return this->storageMethod;
}

template <typename T>
T 	 Params<T>::getVal(
	unsigned dimIdx, 
	unsigned elementIdx) const 
{
	if (this->storageMethod == Params<T>::Interleaved)
		return this->data[elementIdx*D + dimIdx];
	else
		return this->data[dimIdx*N + elementIdx];
}

template <typename T>
void Params<T>::setVal(
	unsigned dimIdx, 
	unsigned elementIdx, 
	T value) 
{
	if (this->storageMethod == Params<T>::Interleaved)
		this->data[elementIdx*D + dimIdx] = value;
	else
		this->data[dimIdx*N + elementIdx] = value;
}

template <typename T>
void Params<T>::setAllElementsTo(T value) {
	for (int i = 0; i < N*D; ++i)
		this->data[i] = value;
}

template <typename T>
void Params<T>::printAllElements (std::string name) const { 
	std::cout << name << "->data ";
	if (this->storageMethod == Params<T>::Interleaved)
		std::cout << "(Interleaved): ";
	else
		std::cout << "(Seperated): ";

	for (int i = 0; i < D*N; ++i)
		std::cout << data[i] << " ";
	std::cout << "\n";
}



template<typename T>
std::ostream &
	operator<<(std::ostream &os, Params<T> &p) 
{ 
	for (int d = 0; d < p.getD(); ++d) {
		os << "dim: " << d << std::endl << "\t";
		for (int i = 0; i < p.getN(); ++i)
			os << p.getVal(d, i) << " ";
		os << std::endl;
	}
    return os;
}

#endif