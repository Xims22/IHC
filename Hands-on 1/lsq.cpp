#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

// 1. ABSTRACCIÓN ORIENTADA A OBJETOS

// Clase utilitaria para manejar operaciones algebraicas de matrices
class Matrix {
public:
    int rows, cols;
    std::vector<std::vector<double>> data;

    Matrix(int r, int c) : rows(r), cols(c), data(r, std::vector<double>(c, 0.0)) {}

    // Transpuesta de una matriz
    Matrix transpose() const {
        Matrix result(cols, rows);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result.data[j][i] = data[i][j];
            }
        }
        return result;
    }

    // Multiplicación de matrices
    Matrix multiply(const Matrix& other) const {
        Matrix result(rows, other.cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < other.cols; ++j) {
                for (int k = 0; k < cols; ++k) {
                    result.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }

    // Inversa específica para una matriz de 3x3 usando determinantes y adjunta (Cramer/Co-factores)
    Matrix invert3x3() const {
        Matrix inv(3, 3);
        double det = data[0][0] * (data[1][1] * data[2][2] - data[1][2] * data[2][1]) -
                     data[0][1] * (data[1][0] * data[2][2] - data[1][2] * data[2][0]) +
                     data[0][2] * (data[1][0] * data[2][1] - data[1][1] * data[2][0]);

        if (std::abs(det) < 1e-9) {
            std::cerr << "Error: La matriz no es invertible (determinante cero)." << std::endl;
            return inv;
        }

        double invDet = 1.0 / det;
        inv.data[0][0] = (data[1][1] * data[2][2] - data[1][2] * data[2][1]) * invDet;
        inv.data[0][1] = (data[0][2] * data[2][1] - data[0][1] * data[2][2]) * invDet;
        inv.data[0][2] = (data[0][1] * data[1][2] - data[0][2] * data[1][1]) * invDet;
        inv.data[1][0] = (data[1][2] * data[2][0] - data[1][0] * data[2][2]) * invDet;
        inv.data[1][1] = (data[0][0] * data[2][2] - data[0][2] * data[2][0]) * invDet;
        inv.data[1][2] = (data[0][2] * data[1][0] - data[0][0] * data[1][2]) * invDet;
        inv.data[2][0] = (data[1][0] * data[2][1] - data[1][1] * data[2][0]) * invDet;
        inv.data[2][1] = (data[0][1] * data[2][0] - data[0][0] * data[2][1]) * invDet;
        inv.data[2][2] = (data[0][0] * data[1][1] - data[0][1] * data[1][0]) * invDet;

        return inv;
    }
};

// Clase que encapsula el Dataset predefinido de 17 experimentos químicos
class DataSet {
private:
    int n = 17;
public:
    Matrix X; // Matriz de diseño (17 filas x 3 columnas: [1, x1, x2])
    Matrix y; // Vector de respuestas (17 filas x 1 columna)

    DataSet() : X(17, 3), y(17, 1) {
        // Inicialización de datos según la tabla proporcionada de la imagen
        double raw_data[17][3] = {
            {41.9, 29.1, 251.3}, {43.4, 29.3, 251.3}, {43.9, 29.5, 248.3},
            {44.5, 29.7, 267.5}, {47.3, 29.9, 273.0}, {47.5, 30.3, 276.5},
            {47.9, 30.5, 270.3}, {50.2, 30.7, 274.9}, {52.8, 30.8, 285.0},
            {53.2, 30.9, 290.0}, {56.7, 31.5, 297.0}, {57.0, 31.7, 302.5},
            {63.5, 31.9, 304.5}, {65.3, 32.0, 309.3}, {71.1, 32.1, 321.7},
            {77.0, 32.5, 330.7}, {77.8, 32.9, 349.0}
        };

        for (int i = 0; i < n; ++i) {
            X.data[i][0] = 1.0;          // Columna de 1s para el parámetro Beta_0
            X.data[i][1] = raw_data[i][0]; // Factor 1 (x1)
            X.data[i][2] = raw_data[i][1]; // Factor 2 (x2)
            y.data[i][0] = raw_data[i][2]; // Yield (y)
        }
    }
};

// Clase Regresor por Mínimos Cuadrados
class LeastSquaredRegressor {
private:
    double beta0, beta1, beta2;
public:
    LeastSquaredRegressor() : beta0(0), beta1(0), beta2(0) {}

    // 2. IMPLEMENTACIÓN DEL MODELO MATEMÁTICO: Beta = (X^T * X)^(-1) * X^T * y
    void fit(const DataSet& ds) {
        Matrix XT = ds.X.transpose();               // X^T
        Matrix XTX = XT.multiply(ds.X);             // X^T * X
        Matrix XTX_inv = XTX.invert3x3();           // (X^T * X)^(-1)
        Matrix XT_y = XT.multiply(ds.y);            // X^T * y
        Matrix beta = XTX_inv.multiply(XT_y);       // Beta final

        beta0 = beta.data[0][0];
        beta1 = beta.data[1][0];
        beta2 = beta.data[2][0];
    }

    // Realizar predicciones/simulaciones del sistema
    double predict(double x1, double x2) const {
        return beta0 + (beta1 * x1) + (beta2 * x2);
    }

    void printEquation() const {
        std::cout << "Yield = " << beta0 << " + (" << beta1 << " * x1) + (" << beta2 << " * x2)\n";
    }

    // Getters para comprobación de parámetros
    double getBeta0() const { return beta0; }
    double getBeta1() const { return beta1; }
    double getBeta2() const { return beta2; }
};

// 2. FUNCIÓN PRINCIPAL (MAIN)

int main() {
    // Configurar salida a 2 decimales
    std::cout << std::fixed << std::setprecision(2);

    std::cout << "========================================================\n";
    std::cout << "       HANDS-ON 1: SYSTEM SIMULATION THROUGH PREDICTION \n";
    std::cout << "========================================================\n\n";

    // Instanciar el conjunto de datos y el regresor
    DataSet dataset;
    LeastSquaredRegressor regressor;

    // Entrenar el modelo con el método matemático de mínimos cuadrados
    regressor.fit(dataset);

    // REQUISITO 3: Mostrar valores calculados para B_0, B_1 y B_2
    std::cout << "   [PUNTO 3] Valores optimos calculados para los parametros:\n";
    std::cout << "   Beta_0 (Interseccion): " << regressor.getBeta0() << "\n";
    std::cout << "   Beta_1 (Factor x1)   : " << regressor.getBeta1() << "\n";
    std::cout << "   Beta_2 (Factor x2)   : " << regressor.getBeta2() << "\n\n";

    // REQUISITO 4: Comprobación con los valores del Data Set de Classroom
    double expected_b0 = -153.51;
    double expected_b1 = 1.24;
    double expected_b2 = 12.08;

    std::cout << "   [PUNTO 4] Verificacion contra Dataset de Classroom:\n";
    std::cout << "   Beta_0 -> Calculado: " << regressor.getBeta0() << " | Esperado: " << expected_b0 << "\n";
    std::cout << "   Beta_1 -> Calculado: " << regressor.getBeta1() << " | Esperado: " << expected_b1 << "\n";
    std::cout << "   Beta_2 -> Calculado: " << regressor.getBeta2() << " | Esperado: " << expected_b2 << "\n";
    
    // Comprobación de tolerancia decimal
    if (std::abs(regressor.getBeta0() - expected_b0) < 0.1 && 
        std::abs(regressor.getBeta1() - expected_b1) < 0.1 && 
        std::abs(regressor.getBeta2() - expected_b2) < 0.1) {
        std::cout << "   >>> RESULTADO: COINCIDENCIA EXITOSA CON LOS VALORES OPTIMOS!\n\n";
    } else {
        std::cout << "   >>> RESULTADO: Variacion en decimales detectable.\n\n";
    }

    // OUTPUTS EXIGIDOS POR LA GUÍA DE SPECS
    std::cout << "   [OUTPUT 1] Ecuacion de Regresion Polinomial/Lineal Multiple:\n   ";
    regressor.printEquation();
    std::cout << "\n";

    std::cout << "   [OUTPUT 2] Simulacion de 5 experimentos con valores desconocidos:\n";
    // Valores de prueba inventados fuera de la tabla para simular el comportamiento del sistema químico
    double test_cases[5][2] = {
        {45.0, 28.5},
        {50.0, 30.0},
        {60.0, 31.0},
        {70.0, 32.0},
        {80.0, 33.5}
    };

    for (int i = 0; i < 5; ++i) {
        double x1 = test_cases[i][0];
        double x2 = test_cases[i][1];
        double predicted_yield = regressor.predict(x1, x2);
        std::cout << "   Simulacion " << i + 1 << " -> Inputs: x1=" << x1 << ", x2=" << x2 
                << " | Prediccion del \"Yield\": " << predicted_yield << "\n";
    }

    std::cout << "\n========================================================\n";
    return 0;
}