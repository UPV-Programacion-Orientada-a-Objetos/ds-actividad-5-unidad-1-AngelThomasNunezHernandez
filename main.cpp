#include <iostream>

template <typename T>
class MatrizBase {
protected:
    int _filas;
    int _columnas;

public:
    MatrizBase(int f, int c) : _filas(f), _columnas(c) {}
    virtual ~MatrizBase() {}
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
    virtual void imprimir() const = 0;
    virtual T get(int f, int c) const = 0;
    virtual void set(int f, int c, T val) = 0;
    int getFilas() const { return _filas; }
    int getColumnas() const { return _columnas; }

    MatrizBase<T>* operator+(const MatrizBase<T>& otra) const {
        return this->sumar(otra);
    }
};

template <typename T>
class MatrizDinamica : public MatrizBase<T> {
private:
    T **_datos;

    void reservarMemoria() {
        _datos = new T*[this->_filas];
        for (int i = 0; i < this->_filas; ++i) {
            _datos[i] = new T[this->_columnas];
            for (int j = 0; j < this->_columnas; ++j) {
                _datos[i][j] = T();
            }
        }
    }

    void liberarMemoria() {
        if (_datos != nullptr) {
            for (int i = 0; i < this->_filas; ++i) {
                delete[] _datos[i];
            }
            delete[] _datos;
            _datos = nullptr;
        }
    }

public:
    MatrizDinamica(int f, int c) : MatrizBase<T>(f, c), _datos(nullptr) {
        reservarMemoria();
    }

    virtual ~MatrizDinamica() {
        liberarMemoria();
    }

    MatrizDinamica(const MatrizDinamica<T>& otra) 
        : MatrizBase<T>(otra._filas, otra._columnas) {
        reservarMemoria();
        for (int i = 0; i < this->_filas; ++i) {
            for (int j = 0; j < this->_columnas; ++j) {
                this->_datos[i][j] = otra._datos[i][j];
            }
        }
    }

    MatrizDinamica<T>& operator=(const MatrizDinamica<T>& otra) {
        if (this == &otra) {
            return *this;
        }
        liberarMemoria();
        this->_filas = otra._filas;
        this->_columnas = otra._columnas;
        reservarMemoria();
        for (int i = 0; i < this->_filas; ++i) {
            for (int j = 0; j < this->_columnas; ++j) {
                this->_datos[i][j] = otra._datos[i][j];
            }
        }
        return *this;
    }

    virtual T get(int f, int c) const override {
        return _datos[f][c];
    }

    virtual void set(int f, int c, T val) override {
        _datos[f][c] = val;
    }

    virtual void imprimir() const override {
        for (int i = 0; i < this->_filas; ++i) {
            std::cout << "| ";
            for (int j = 0; j < this->_columnas; ++j) {
                std::cout << _datos[i][j] << " |";
            }
            if (i < this->_filas - 1) {
                 std::cout << "| ";
            }
        }
    }

    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        if (this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
            return nullptr;
        }
        
        std::cout << "(La suma es manejada por el método virtual de MatrizDinamica)" << std::endl;
        MatrizDinamica<T>* resultado = new MatrizDinamica<T>(this->_filas, this->_columnas);

        for (int i = 0; i < this->_filas; ++i) {
            for (int j = 0; j < this->_columnas; ++j) {
                T valSuma = this->_datos[i][j] + otra.get(i, j);
                resultado->set(i, j, valSuma);
            }
        }
        return resultado;
    }
};

template <typename T, int M, int N>
class MatrizEstatica : public MatrizBase<T> {
private:
    T _datos[M][N];

public:
    MatrizEstatica() : MatrizBase<T>(M, N) {
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                _datos[i][j] = T();
            }
        }
    }

    virtual ~MatrizEstatica() {}

    virtual T get(int f, int c) const override {
        return _datos[f][c];
    }

    virtual void set(int f, int c, T val) override {
        _datos[f][c] = val;
    }

    virtual void imprimir() const override {
        for (int i = 0; i < M; ++i) {
            std::cout << "| ";
            for (int j = 0; j < N; ++j) {
                std::cout << _datos[i][j] << " |";
            }
            if (i < M - 1) {
                 std::cout << "| ";
            }
        }
    }

    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        if (this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
            return nullptr;
        }
        MatrizEstatica<T, M, N>* resultado = new MatrizEstatica<T, M, N>();
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                T valSuma = this->_datos[i][j] + otra.get(i, j);
                resultado->set(i, j, valSuma);
            }
        }
        return resultado;
    }
};

int main() {
    std::cout.precision(1);
    std::cout << std::fixed;

    std::cout << "-Sistema gérico de Álgebra Lineal-" << std::endl;

    MatrizBase<float>* A = nullptr;
    MatrizBase<float>* B = nullptr;
    MatrizBase<float>* C = nullptr;

    std::cout << "\n// 1. Creación de Matriz Dinámica (a través del puntero base)" << std::endl;
    A = new MatrizDinamica<float>(3, 2);
    std::cout << "Creando Matriz Dinámica A (3x2)" << std::endl;
    A->set(0, 0, 1.5); A->set(0, 1, 2.0);
    A->set(1, 0, 0.0); A->set(1, 1, 1.0);
    A->set(2, 0, 4.5); A->set(2, 1, 3.0);
    std::cout << "A =";
    A->imprimir();
    std::cout << std::endl;

    std::cout << "\n// 2. Creación de Matriz Estática (a través del puntero base)" << std::endl;
    B = new MatrizEstatica<float, 3, 2>();
    std::cout << "Creando Matriz Estática B (3x2)" << std::endl;
    B->set(0, 0, 0.5); B->set(0, 1, 1.0);
    B->set(1, 0, 2.0); B->set(1, 1, 3.0);
    B->set(2, 0, 1.0); B->set(2, 1, 1.0);
    std::cout << "B =";
    B->imprimir();
    std::cout << std::endl;

    std::cout << "\n// 3. Operación Polimórfica (Suma)" << std::endl;
    std::cout << "SUMANDO: Matriz C = A + B " << std::endl;

    C = *A + *B; 

    if (C != nullptr) {
        std::cout << "\nMatriz Resultado C (3x2, Tipo FLOAT):";
        C->imprimir();
        std::cout << std::endl;
    }

    std::cout << "-Demostración de Limpieza de Memoria-" << std::endl;

    std::cout << "Llamando al destructor de C" << std::endl;
    std::cout << "Liberando memoria de C (Matriz Dinámica)" << std::endl;
    delete C;

    std::cout << "Llamando al destructor de A" << std::endl;
    std::cout << "Liberando memoria de A (Matriz Dinámica)" << std::endl;
    delete A;
    
    delete B;

    std::cout << "Sistema cerrado." << std::endl;

    return 0;
}