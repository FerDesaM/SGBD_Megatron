#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// Función para obtener el esquema de un archivo
string getSchema(const string& schemaFile, const string& tableName) {
    ifstream file(schemaFile);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string table;
            getline(ss, table, '#');
            if (table == tableName) {
                string schema;
                getline(ss, schema);
                file.close();
                return schema;
            }
        }
        file.close();
    }
    return "";
}

// Función para calcular el tamaño de una línea
int calcularTamanioLineaVariable(const string& linea, const string& esquema) {
    istringstream ssLinea(linea);
    istringstream ssEsquema(esquema);
    string palabra, columna, tipoDato;
    int tamanioLinea = 0;

    while (getline(ssEsquema, columna, '#') && getline(ssEsquema, tipoDato, '#')) {
        getline(ssLinea, palabra, '#');

        // Interpretar el valor según el tipo de dato del esquema
        if (tipoDato == "INT") {
            cout << "Tipo de dato: INT, Valor: " << palabra << endl;
            tamanioLinea += palabra.length() * 4; // Cada dígito ocupa 4 bytes
        } else if (tipoDato == "STRING") {
            cout << "Tipo de dato: STRING, Valor: " << palabra << endl;
            tamanioLinea += palabra.length() * sizeof(char); // Cada carácter ocupa 1 byte
        } else if (tipoDato == "FLOAT") {
            cout << "Tipo de dato: FLOAT, Valor: " << palabra << endl;
            tamanioLinea += palabra.length() * 4; // Cada dígito ocupa 4 bytes
        } else if (tipoDato == "CHAR") {
            cout << "Tipo de dato: CHAR, Valor: " << palabra << endl;
            tamanioLinea += sizeof(char); // Cada carácter ocupa 1 byte
        } else {
            cout << "Tipo de dato desconocido, ignorando." << endl;
        }
    }

    return tamanioLinea;
}

// Función para calcular el tamaño total de todas las líneas en el archivo
void calcularTamanioTotalVariable(const string& nombreArchivo, const string& esquema) {
    ifstream archivo(nombreArchivo);
    string linea;

    if (archivo.is_open()) {
        cout << "Archivo abierto correctamente." << endl;

        // Ignorar la primera línea del archivo (cabecera)
        getline(archivo, linea);
        cout << "Cabecera ignorada." << endl;

        // Procesar cada línea del archivo
        while (getline(archivo, linea)) {
            cout << "Leyendo línea: " << linea << endl;
            int tamanioLinea = calcularTamanioLineaVariable(linea, esquema);
            cout << "Tamaño de la línea: " << tamanioLinea << " bytes" << endl << endl;
        }

        archivo.close();
        cout << "Archivo cerrado." << endl;
    } else {
        cout << "¡Error al abrir el archivo!" << endl;
    }
}
int calcularTamanioFijoLinea(const string& esquema) {
    istringstream ssEsquema(esquema);
    string columna, tipoDato;
    int tamanioLinea = 0;

    while (getline(ssEsquema, columna, '#') && getline(ssEsquema, tipoDato, '#')) {
        if (tipoDato == "INT") {
            tamanioLinea += 8; // Tamaño fijo de 8 bytes para INT
        } else if (tipoDato == "STRING") {
            tamanioLinea += 49; // Tamaño fijo de 56 bytes para STRING
        } else if (tipoDato == "FLOAT") {
            tamanioLinea += 11; // Tamaño fijo de 16 bytes para FLOAT
        } else if (tipoDato == "CHAR") {
            tamanioLinea += 1; // Tamaño fijo de 1 byte para CHAR
        } else {
            cout << "Tipo de dato desconocido en el esquema, ignorando." << endl;
        }
    }

    return tamanioLinea;
}

// Función para calcular y mostrar el tamaño fijo de cada línea en el archivo
void calcularTamanioFijoTotal(const string& nombreArchivo, const string& esquema) {
    ifstream archivo(nombreArchivo);
    string linea;
    int tamanioFijo = calcularTamanioFijoLinea(esquema);

    if (archivo.is_open()) {
        cout << "Archivo abierto correctamente." << endl;

        // Ignorar la primera línea del archivo (cabecera)
        getline(archivo, linea);
        cout << "Cabecera ignorada." << endl;

        // Procesar cada línea del archivo
        while (getline(archivo, linea)) {
            cout << "Leyendo línea: " << linea << endl;
            cout << "Tamaño fijo de la línea: " << tamanioFijo << " bytes" << endl << endl;
        }

        archivo.close();
        cout << "Archivo cerrado." << endl;
    } else {
        cout << "¡Error al abrir el archivo!" << endl;
    }
}
int main() {
    // Nombre del archivo de datos y del archivo de esquema
    string archivoTxt = "titanic.txt";
    string archivoEsquema = "esquemas.txt";

    // Obtener el esquema del archivo
    string esquema = getSchema(archivoEsquema, "titanic");

    // Calcular y mostrar el tamaño de cada línea en el archivo
    //calcularTamanioTotalVariable(archivoTxt, esquema);
    calcularTamanioFijoTotal(archivoTxt, esquema);
    return 0;
}