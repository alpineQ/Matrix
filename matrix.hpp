#include <ostream>
#include <fstream>
#include <iomanip>
#include <ctime>

template <typename T>
class Matrix
{
	T** matrix;

	unsigned length;
	unsigned width;

public:
	Matrix(const char* filepath);
	Matrix(unsigned length, unsigned width);
	Matrix(T** matrix, unsigned length, unsigned width);
	Matrix(const Matrix<T>& other);
	Matrix(Matrix<T>&& other) noexcept;
	~Matrix();

	inline T getValue(unsigned indL, unsigned indW) const;
	inline T setValue(unsigned indL, unsigned indW, T value) const;
	void fillRandom(unsigned max = 10) const;
	void clear(bool makeE = false) const;
	Matrix<T> transpose() const;
	Matrix<T> getMinor(unsigned i, unsigned j);
	Matrix<T> inverse() const;// TODO
	T determinat();
	Matrix<T> appendRight(Matrix<T>);
	Matrix<T> gaussView();// TODO

    template <typename Y>
    Matrix<Y> convert(Y) const;

	template <typename Y>
	friend std::ostream& operator<<(std::ostream& os, const Matrix<Y>& m);

    Matrix<T> operator+(const Matrix<T>& other) const;
    Matrix<T> operator-(const Matrix<T>& other) const;
    Matrix<T> operator*(const Matrix<T>& other) const;
    Matrix<T> operator/(const Matrix<T>& other) const;

    Matrix<T> operator-() const;

	Matrix<T> operator^(const int degree);

    Matrix<T>& operator=(const Matrix<T>& other);
    Matrix<T>& operator=(Matrix<T>&& other) noexcept;

    Matrix<T> operator+=(const Matrix<T>& other);
    Matrix<T> operator-=(const Matrix<T>& other);
    Matrix<T> operator*=(const Matrix<T>& other);
    Matrix<T> operator/=(const Matrix<T>& other);

};

template <typename T>
Matrix<T>::Matrix(const char* filepath)
{
	std::ifstream input(filepath);

	if (!input.is_open())
	{
        throw std::runtime_error("Cannot open file");
        return;
    }

	input >> length;
	input >> width;
	if (width <= 0 || length <= 0)
		throw std::runtime_error("Bad input");

	matrix = new T*[length];
	for (unsigned i = 0; i < length; ++i)
		matrix[i] = new T[width];

	for (unsigned i = 0; i < length; ++i)
		for (unsigned j = 0; j < width; ++j)
			input >> matrix[i][j];
}

template <typename T>
Matrix<T>::Matrix(const unsigned length, const unsigned width)
{
	this->length = length;
	this->width = width;
	this->matrix = new T*[length];
	for (unsigned i = 0; i < length; ++i)
		matrix[i] = new T[width];
}

template <typename T>
Matrix<T>::Matrix(T** matrix, const unsigned length, const unsigned width)
{
	this->matrix = matrix;
	this->length = length;
	this->width = width;
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& other)
{
	width = other.width;
	length = other.length;
	matrix = new T*[length];
	for (unsigned i = 0; i < length; ++i)
		matrix[i] = new T[width];

	for (unsigned i = 0; i < length; ++i)
		for (unsigned j = 0; j < width; ++j)
			matrix[i][j] = other.matrix[i][j];
}

template <typename T>
Matrix<T>::Matrix(Matrix<T>&& other) noexcept
{
	matrix = other.matrix;
	other.matrix = nullptr;
	width = other.width;
	length = other.length;
}

template <typename T>
Matrix<T>::~Matrix()
{
	if (matrix)
		for (unsigned i = 0; i < length; ++i)
			delete[] matrix[i];
}

template <typename T>
T Matrix<T>::getValue(const unsigned indL, const unsigned indW) const
{
	return matrix[indL][indW];
}

template <typename T>
T Matrix<T>::setValue(const unsigned indL, const unsigned indW, const T value) const
{
	matrix[indL][indW] = value;
	return value;
}

template <typename T>
void Matrix<T>::fillRandom(const unsigned max) const
{
	static auto seed = static_cast<unsigned>(time(nullptr));
	srand(seed++);
	for (unsigned i = 0; i < length; ++i)
		for (unsigned j = 0; j < width; ++j)
			matrix[i][j] = rand() % max;
}

template <typename T>
void Matrix<T>::clear(bool makeE) const
{
	for (unsigned i = 0; i < length; ++i)
		for (unsigned j = 0; j < width; ++j)
			matrix[i][j] = 0;
	if (makeE && width == length)
		for (unsigned j = 0; j < width; ++j)
			matrix[j][j] = 1;
}

template <typename T>
Matrix<T> Matrix<T>::transpose() const
{
	if (length != width)
	{
		throw std::runtime_error("Cannot transpose non-square matrix");
	}
	Matrix<T> result = *this;
	for (unsigned i = 0; i < length; ++i)
		for (unsigned j = 0; j < i; ++j)
			std::swap(result.matrix[i][j], result.matrix[j][i]);
	return result;
}

template<typename T>
Matrix<T> Matrix<T>::getMinor(unsigned i, unsigned j)
{
	Matrix<T> minorMat(length - 1, width - 1);

	for (unsigned k = 0; k < length; ++k)
		for (unsigned h = 0; h < width; ++h)
			if (h != j && k != i)
				minorMat.matrix[(k < i) ? k : k - 1][(h < j) ? h : h - 1] = matrix[k][h];
	return minorMat;
}

template<typename T>
Matrix<T> Matrix<T>::inverse() const
{
	return *this;
}

template<typename T>
T Matrix<T>::determinat()
{
	if (length == width && length == 1)
		return matrix[0][0];
	T result = 0;
	for (unsigned i = 0; i < width; ++i)
		result += ((i % 2) ? -1 : 1)*matrix[0][i]*getMinor(0, i).determinat();
	return result;
}

template<typename T>
Matrix<T> Matrix<T>::appendRight(Matrix<T> other)
{
	if (length != other.length)
		throw std::runtime_error("Bad matrix size");

	Matrix result(length, width + other.width);

	for (unsigned i = 0; i < result.length; ++i)
		for (unsigned j = 0; j < result.width; ++j)
			result.matrix[i][j] = (( j < width) ? matrix[i][j] : other.matrix[i][j - width]);
	
	return result;
}

template<typename T>
Matrix<T> Matrix<T>::gaussView()
{
	Matrix *result = new Matrix(length);
	result->clear(true);



	return *result;
}



template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m)
{
	for (unsigned i = 0; i < m.length; ++i)
	{
		for (unsigned j = 0; j < m.width; ++j)
				if (typeid(T) == typeid(double))
			os << std::setw(9)<< std::setprecision(2) << m.matrix[i][j] << ' ';
			else os << m.matrix[i][j] << ' ';
		os << std::endl;
	}
	return os;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix & other) const
{
	if (width != other.width || length != other.length)
		throw std::runtime_error("Wrong matrix size");

	Matrix<T> result(*this);
	for (unsigned i = 0; i < result.length; ++i)
		for (unsigned j = 0; j < result.width; ++j)
			result.matrix[i][j] += other.matrix[i][j];
	return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& other) const
{
    if (width != other.width || length != other.length)
        throw std::runtime_error("Wrong matrix size");

    Matrix<T> result(*this);
    for (unsigned i = 0; i < result.length; ++i)
        for (unsigned j = 0; j < result.width; ++j)
            result.matrix[i][j] -= other.matrix[i][j];
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) const
{
	if (width != other.length)
		throw std::runtime_error("Wrong matrix size");
	Matrix<T> result(length, other.width);
	result.clear();
	for (unsigned i = 0; i < result.length; ++i)
		for (unsigned j = 0; j < result.width; ++j)
		{
			for (unsigned k = 0; k < width; ++k)
				result.matrix[i][j] += (matrix[i][k] * other.matrix[k][j]);
		}
	return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator/(const Matrix<T>& other) const
{
	if (width != other.length)
		throw std::runtime_error("Wrong matrix size");
	Matrix<T> result(width, other.length);
	Matrix<T> divisor(other.inverse());
	result.clear();
	for (unsigned i = 0; i < result.length; ++i)
		for (unsigned j = 0; j < result.width; ++j)
		{
			for (unsigned k = 0; k < width; ++k)
				result.matrix[i][j] += (matrix[i][k] * divisor.matrix[k][j]);
		}
	return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator-() const
{
    Matrix<T> result = *this;
    for (unsigned i = 0; i < length; ++i)
        for (unsigned j = 0; j < width; ++j)
            result.matrix[i][j] = -result.matrix[i][j];
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other)
{
	width = other.width;
	length = other.length;

	for (unsigned i = 0; i < length; ++i)
		for (unsigned j = 0; j < width; ++j)
			matrix[i][j] = other.matrix[i][j];

	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator^(const int degree)
{
	if (width != length)
		throw std::runtime_error("Wrong matrix size");

	Matrix<T> result(*this);
	if (degree == 0)
	{
		result.clear(true);
		return result;
	}
	if (degree > 0)
	{
		for (unsigned z = 1; z < (unsigned)degree; ++z)
			result = result * *this;

		return result;
	}
	else
	{
		/*for (unsigned z = -1; z < abs(degree); ++z)
			*result = *result / *this;*/

		return result;
	}
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& other) noexcept
{
	matrix = other.matrix;
	other.matrix = nullptr;
	width = other.width;
	length = other.length;
	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator+=(const Matrix<T>& other)
{
    return (*this = *this + other);
}

template <typename T>
Matrix<T> Matrix<T>::operator-=(const Matrix<T>& other)
{
    return (*this = *this - other);
}


template <typename T>
Matrix<T> Matrix<T>::operator*=(const Matrix<T>& other)
{
    return (*this = *this * other);
}

template <typename T>
Matrix<T> Matrix<T>::operator/=(const Matrix<T>& other)
{
    return (*this = *this / other);
}

template <typename T>
template <typename Y>
Matrix<Y> Matrix<T>::convert(Y) const
{
    Matrix<Y> result(length, width);

    for (unsigned i = 0; i < length; ++i)
        for (unsigned j = 0; j < width; ++j)
            result.setValue(i, j ,(Y)this->matrix[i][j]);

    return result;
}
