#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#define MAX_LINE 256
#define COLUMN_SIZE 200
using namespace std;

class Columna
{
private:
    string nombre;
    string tipo;

public:
    Columna(const string &nombre, const string &tipo) : nombre(nombre), tipo(tipo) {}
    Columna() : nombre(""), tipo("") {}
    string getNombre() const { return nombre; }
    string getTipo() const { return tipo; }
};

class Tabla
{
private:
    string nombre;
    Columna *campos;
    size_t capacidad;
    size_t tamano;

public:
    Tabla(const string &nombre) : nombre(nombre), campos(nullptr), capacidad(0), tamano(0) {}
    Tabla() : nombre(""), campos(nullptr), capacidad(0), tamano(0) {}
    ~Tabla(){
        delete[] campos;
    }

    void agregarCampo(const string &nombre, const string &tipo){
        if (tamano >= capacidad){
            capacidad = (capacidad == 0) ? 1 : capacidad * 2;
            Columna *nuevoArray = new Columna[capacidad];
            for (size_t i = 0; i < tamano; ++i){
                nuevoArray[i] = campos[i];
            }
            delete[] campos;
            campos = nuevoArray;
        }
        campos[tamano++] = Columna(nombre, tipo);
    }

    string getNombre() const { return nombre; }
    const Columna *getCampos() const { return campos; }
    size_t getTamano() const { return tamano; }

    string generarEsquema() const{
        string esquema = nombre + " # ";
        for (size_t i = 0; i < tamano; ++i){
            esquema += campos[i].getNombre() + " # " + campos[i].getTipo();
            if (i != tamano - 1){
                esquema += " # ";
            }
        }
        return esquema;
    }

    bool escribirEsquemaEnArchivo(const string &nombreArchivo) const{
        ifstream archivoLectura(nombreArchivo);
        string linea;
        bool existeEsquema = false;
        string esquemaActual = generarEsquema();
        while (getline(archivoLectura, linea)){
            if (linea.find(esquemaActual) != string::npos){
                existeEsquema = true;
                break;
            }
        }
        archivoLectura.close();

        if (!existeEsquema){
            ofstream archivoEscritura(nombreArchivo, ios::app);
            if (archivoEscritura.is_open()){
                archivoEscritura << esquemaActual << endl;
                archivoEscritura.close();
                cout << "Esquema de la tabla '" << nombre << "' guardado en '" << nombreArchivo << "'" << endl;
                return true;
            }
            else{
                cerr << "Error al abrir el archivo '" << nombreArchivo << "' para escritura." << endl;
                return false;
            }
        }
        else{
            cout << "El esquema de la tabla '" << nombre << "' ya esta presente en el archivo '" << nombreArchivo << "'" << endl;
            return false;
        }
    }
};

class DataBase{
private:
    Tabla *tablas;
    size_t capacidad;
    size_t tamano;

public:
    DataBase() : tablas(new Tabla[10]), capacidad(10), tamano(0) {}

    ~DataBase(){
        delete[] tablas;
    }

    void crearTabla(){
        string nombreTabla;
        cout << "Ingrese el nombre de la nueva tabla: ";
        cin >> nombreTabla;

        Columna *campos = new Columna[capacidad];
        size_t tamanoCampos = 0;
        char respuesta;
        do{
            string nombreCampo, tipoCampo;
            cout << "Ingrese el nombre del campo: ";
            cin >> nombreCampo;
            cout << "Ingrese el tipo del campo: ";
            cin >> tipoCampo;
            if (tamanoCampos >= capacidad){
                capacidad = (capacidad == 0) ? 1 : capacidad * 2;
                Columna *nuevoArray = new Columna[capacidad];
                for (size_t i = 0; i < tamanoCampos; ++i){
                    nuevoArray[i] = campos[i];
                }
                delete[] campos;
                campos = nuevoArray;
            }
            campos[tamanoCampos++] = Columna(nombreCampo, tipoCampo);
            cout << "¿Desea agregar otro campo? (s/n): ";
            cin >> respuesta;
        } while (respuesta == 's' || respuesta == 'S');

        Tabla nuevaTabla(nombreTabla);
        for (size_t i = 0; i < tamanoCampos; ++i){
            nuevaTabla.agregarCampo(campos[i].getNombre(), campos[i].getTipo());
        }
        delete[] campos;
        tablas[tamano++] = nuevaTabla;
        nuevaTabla.escribirEsquemaEnArchivo("esquemas.txt");
        cout << "Esquema de la tabla '" << nombreTabla << "':" << endl;
        cout << nuevaTabla.generarEsquema() << endl;
    }

    void agregarRegistro(const string &nombreTabla){
        ifstream archivoEsquema("esquemas.txt");
        string linea;
        bool tablaEncontrada = false;
        while (getline(archivoEsquema, linea)){
            stringstream ss(linea);
            string nombreTabl;
            getline(ss, nombreTabl, '#');
            nombreTabl.erase(remove_if(nombreTabl.begin(), nombreTabl.end(), ::isspace), nombreTabl.end());
            if (nombreTabl == nombreTabla){
                tablaEncontrada = true;
                break;
            }
        }
        archivoEsquema.close();

        if (!tablaEncontrada){
            cerr << "Error: La tabla '" << nombreTabla << "' no está definida en el archivo de esquemas." << endl;
            return;
        }
        do{
            // Solicitar y agregar los valores para los campos de la tabla
            stringstream valoresRegistro;
            archivoEsquema.open("esquemas.txt");
            while (getline(archivoEsquema, linea)){
                stringstream ssEsquema(linea); // Cambio de nombre de la variable stringstream
                string nombreTabl;
                getline(ssEsquema, nombreTabl, '#');
                nombreTabl.erase(remove_if(nombreTabl.begin(), nombreTabl.end(), ::isspace), nombreTabl.end());
                if (nombreTabl == nombreTabla){
                    string nombreCampo, tipoDato;
                    bool primerCampo = true;
                    while (getline(ssEsquema, nombreCampo, '#') && getline(ssEsquema, tipoDato, '#')){
                        if (!primerCampo){
                            valoresRegistro << " # ";
                        }
                        else{
                            primerCampo = false;
                        }
                        string valor;
                        cout << "Ingrese el valor para el campo '" << nombreCampo << "': ";
                        cin >> valor;
                        // Aquí podrías agregar validación de tipo de dato si lo deseas
                        valoresRegistro << valor;
                    }
                    break;
                }
            }
            archivoEsquema.close();

            // Agregar el registro al archivo correspondiente a la tabla
            ofstream archivoRegistro(nombreTabla + ".txt", ios::app);
            if (archivoRegistro.is_open()){
                archivoRegistro << valoresRegistro.str() << endl;
                archivoRegistro.close();
                cout << "Registro agregado a la tabla '" << nombreTabla << "' correctamente." << endl;
            }
            else{
                cerr << "Error al abrir el archivo para escritura." << endl;
            }
            char next;
            cout << "Desea agregar mas registros (s/n)";
            cin >> next;
            if (next != 's' && next != 'S'){
                break;
            }
        } while (true);
    }
    void mostrarRegistrosTabla(const string &nombreTabla) const{
        ifstream archivoRegistro(nombreTabla + ".txt");
        if (archivoRegistro.is_open()){
            cout << "Registros de la tabla '" << nombreTabla << "':" << endl;
            string linea;
            while (getline(archivoRegistro, linea)){
                cout << linea << endl;
            }
            archivoRegistro.close();
        }
        else{
            cerr << "Error al abrir el archivo para lectura." << endl;
        }
    }
    void leerCSV(const string &file_read, const string &file_out){
        ifstream read_file(file_read);
        ofstream output_file(file_out);

        char line[MAX_LINE];
        char column[COLUMN_SIZE];
        bool Quotes = false;

        while (read_file.getline(line, sizeof(line))){
            char *ptr = line;
            int columnIndex = 0;

            while (*ptr != '\0'){
                if (*ptr == '"'){
                    Quotes = !Quotes;
                }
                else if (*ptr == ',' && !Quotes){
                    column[columnIndex] = '\0';
                    output_file << column << " # ";
                    columnIndex = 0;
                }
                else{
                    column[columnIndex++] = *ptr;
                }
                ptr++;
            }

            column[columnIndex] = '\0';
            output_file << column << '\n';
        }

        read_file.close();
        output_file.close();
    }

    string extraerEsquema(const string& nombreTabla) {
    ifstream archivoEsquemas("esquemas.txt");
    string linea;
    while (getline(archivoEsquemas, linea)) {
        if (linea.find(nombreTabla) == 0) {
            archivoEsquemas.close();
            return linea.substr(linea.find("#") + 1);
        }
    }
    archivoEsquemas.close();
    return ""; // Retornar una cadena vacía si no se encuentra el esquema
}

// Función para verificar si la columna ya está presente en el archivo de esquemas
bool verificarEsquema(const string& columna) {
    ifstream archivoEsquemas("esquemas.txt");
    string linea;
    while (getline(archivoEsquemas, linea)) {
        if (linea.find(columna) == 0) {
            archivoEsquemas.close();
            return true;
        }
    }
    archivoEsquemas.close();
    return false;
}

void consultaSelect(const string& nombreTabla) {
    // Obtener el esquema de la tabla
    string esquema = extraerEsquema(nombreTabla);
    if (esquema.empty()) {
        cout << "No se encontró el esquema de la tabla '" << nombreTabla << "'." << endl;
        return;
    }

    // Solicitar y obtener la condición del usuario
    string condicion;
    cout << "Ingrese la condicion (por ejemplo, columna=valor): ";
    cin >> condicion;

    // Separar la columna, operador y valor de la condición
    size_t posicionOperador = condicion.find_first_of("=<>");
    string columna = condicion.substr(0, posicionOperador);
    string operador = condicion.substr(posicionOperador, 1);
    string valor = condicion.substr(posicionOperador + 1);

    // Contar la posición de la columna en el esquema
    int contador = 0;
    stringstream ssEsquema(esquema);
    string token;
    bool encontrado = false;
    while (getline(ssEsquema, token, '#')) {
        if (token == columna) {
            encontrado = true;
            break;
        }
        contador++;
    }

    if (!encontrado) {
        cout << "Error: No se encontró la columna '" << columna << "' en el esquema de la tabla '" << nombreTabla << "'." << endl;
        return;
    }

    // Abrir el archivo de datos
    string nombreArchivo = nombreTabla + ".txt";
    ifstream archivoDatos(nombreArchivo);
    if (!archivoDatos.is_open()) {
        cerr << "Error al abrir el archivo '" << nombreArchivo << "'." << endl;
        return;
    }

    // Crear un archivo para guardar los resultados que cumplan con la condición
    ofstream archivoResultado(columna + ".txt", ios::app);
    if (!archivoResultado.is_open()) {
        cerr << "Error al abrir el archivo para escribir los resultados." << endl;
        archivoDatos.close();
        return;
    }

    // Leer cada línea del archivo de datos y aplicar la condición
    string linea;
    while (getline(archivoDatos, linea)) {
        stringstream ss(linea);
        string valorCampo;
        for (int i = 0; i < contador; i++) {
            getline(ss, valorCampo, '#');
        }

        // Convertir el valor del campo a un tipo numérico si es necesario
        stringstream convertidor(valor);
        if (isdigit(valor[0]) || (valor[0] == '-' && isdigit(valor[1]))) {
            int valorNumerico;
            convertidor >> valorNumerico;
            int valorCampoNumerico;
            stringstream(convertidor.str()) >> valorCampoNumerico;

            // Comparar valores numéricos
            if (operador == "=" && valorCampoNumerico == valorNumerico) {
                archivoResultado << linea << endl;
            } else if (operador == "<" && valorCampoNumerico < valorNumerico) {
                archivoResultado << linea << endl;
            } else if (operador == ">" && valorCampoNumerico > valorNumerico) {
                archivoResultado << linea << endl;
            }
        } else {
            // Comparar valores como cadenas de caracteres
            if (operador == "=" && valorCampo == valor) {
                archivoResultado << linea << endl;
            } else if (operador == "<" && valorCampo < valor) {
                archivoResultado << linea << endl;
            } else if (operador == ">" && valorCampo > valor) {
                archivoResultado << linea << endl;
            }
        }
    }

    // Cerrar archivos
    archivoDatos.close();
    archivoResultado.close();

    // Actualizar el archivo de esquemas si es necesario
    if (ifstream(columna + ".txt")) {
        if (!verificarEsquema(columna)) {
            ofstream archivoEsquemas("esquemas.txt", ios::app);
            archivoEsquemas << columna << '#' << esquema << endl;
            archivoEsquemas.close();
        }
    } else {
        cout << "No se encontraron registros que cumplan con la condicion." << endl;
    }
}

    //void agregarRelacion(const &string first,const &string second){}
    int getRegisterSize(){
        int capacity;
        return capacity;
    }
};

int main(){
    DataBase db;

    int opcion;
    do{
        cout << "BIENVENIDO A MEGATRON 3000" << endl;
        cout << "Elija una opción:" << endl;
        cout << "1. Crear nueva tabla" << endl;
        cout << "2. Agregar registro a una tabla" << endl;
        cout << "3. Mostrar registros de una tabla" << endl;
        cout << "4. Leer archivo CSV y agregar registros" << endl;
        cout<<  "5. Realizar alguna consulta en la tabla"<<endl;
        cout << "6. Salir" << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion){
        case 1:
            db.crearTabla();
            break;
        case 2:{
            string nombreTabla;
            cout << "Ingrese el nombre de la tabla: ";
            cin >> nombreTabla;
            db.agregarRegistro(nombreTabla);
            break;
        }
        case 3:{
            string nombreTabla;
            cout << "Ingrese el nombre de la tabla: ";
            cin >> nombreTabla;
            db.mostrarRegistrosTabla(nombreTabla);
            break;
        }
        case 4:{
            string nombreTabla, nombreArchivoCSV;
            cout << "Ingrese el nombre del archivo CSV: ";
            cin >> nombreArchivoCSV;
            db.leerCSV(nombreArchivoCSV + ".csv", nombreArchivoCSV + ".txt");
            break;
        }
        case 5:{
            string Table;
            cout<<"Ingrese el nombre de la tabla";
            cin>>Table;
            db.consultaSelect(Table);

        }
        case 6:
            cout << "Saliendo del programa..." << endl;
            break;
        default:
            cerr << "Opción inválida. Por favor, seleccione una opción válida." << endl;
        }
    } while (opcion != 6);
    return 0;
}