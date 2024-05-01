#include <iostream>
#include <cstring> 
#include <string>
using namespace std;

class Sector {
private:
    int num_sector;
    int num_bytes;
    char* data;

public:
    Sector() {}
    Sector(int num, int bytes){
        num_sector=num;
        num_bytes=bytes;
        data = new char[num_bytes];
    }
    
    int getNum() const {
        return num_sector;
    }

    int getSize() const {
        return num_bytes;
    }

    const char* getData() const {
        return data;
    }
    
    void escribirDatos(const char* newData) {
        memcpy(data, newData, num_bytes);
    }

    
};

class Pista {
private:
    int pista;
    int num_sectores; 
    Sector* sectores;

public:
    Pista() : num_sectores(0), sectores(nullptr) {}
    
    Pista(int num_pista,int num_sect, int tam_sec) :pista(num_pista), num_sectores(num_sect) {
        sectores = new Sector[num_sectores];
        for (int i = 0; i < num_sectores; ++i) {
            sectores[i] = Sector(i+1, tam_sec);
        }
    }
    Pista(const Pista& other) : num_sectores(other.num_sectores) {
        sectores = new Sector[num_sectores];
        for (int i = 0; i < num_sectores; ++i) {
            sectores[i] = other.sectores[i];
        }
    }

    // Operador de asignación
    Pista& operator=(const Pista& other) {
        if (this != &other) {
            delete[] sectores;
            num_sectores = other.num_sectores;
            sectores = new Sector[num_sectores];
            for (int i = 0; i < num_sectores; ++i) {
                sectores[i] = other.sectores[i];
            }
        }
        return *this;
    }
    
    
    Sector& getSector(int index) const {
        return sectores[index];
    }    
};

class Superficie {
private:
    int num_surface;
    int num_pistas;
    Pista* pistas;

public:
    Superficie() : num_pistas(0), pistas(nullptr) {}
    
    Superficie(int num_surper,int num_pist, int num_sect, int tam_sec) : num_surface(num_surper), num_pistas(num_pist) {
        pistas = new Pista[num_pistas];
        for (int i = 0; i < num_pistas; ++i) {
            pistas[i] = Pista(i+1,num_sect, tam_sec);
        }
    }

    

    Pista& getPista(int index) const {
        return pistas[index];
    }
};

class Plato {
private:
    Superficie* superficie[2];

public:
    Plato() {
        for (int i = 0; i < 2; ++i) {
            superficie[i] = nullptr; // Inicializa los punteros como nulos
        }
    }
    
    Plato(int num_pist, int num_sect, int tam_sec) : Plato() {
        for (int i = 0; i < 2; ++i) {
            superficie[i] = new Superficie(i,num_pist, num_sect, tam_sec); // Crea nuevas instancias de Superficie
        }
    }

    Superficie& getSuperficie(int index) {
        return *superficie[index];
    }

    
};

class DiscoDuro {
private:
    int num_platos;
    Plato* platos;

public:
    DiscoDuro() : num_platos(0), platos(nullptr) {}
    
    DiscoDuro(int num_plat, int num_pist, int num_sect,int tam_sect) : num_platos(num_plat) {
        platos = new Plato[num_platos];
        for (int i = 0; i < num_platos; ++i) {
            platos[i] = Plato(num_pist, num_sect,tam_sect);
        }
    }

    ~DiscoDuro() {
        delete[] platos;
    }

    Plato& getPlato(int index) const {
        return platos[index];
    }

   
void escribir(int num_sector, int num_pista, int num_superficie, const char* datos) {
        platos[num_superficie].getSuperficie(num_pista).getPista(num_pista).getSector(num_sector).escribirDatos(datos);
    }

    void leer(int num_sector, int num_pista, int num_superficie, char* buffer) {
        const char* datos = platos[num_superficie].getSuperficie(num_pista).getPista(num_pista).getSector(num_sector).getData();
        strncpy(buffer, datos, 511); // Copiar como máximo 511 caracteres para dejar espacio para el carácter nulo terminador
        buffer[511] = '\0'; // Asegurar que el búfer esté terminado correctamente
    }

    void imprimir(int num_sector, int num_pista, int num_superficie) {
        char buffer[512]; // Buffer para almacenar los datos leídos
        leer(num_sector, num_pista, num_superficie, buffer);
        cout << "Datos en el sector " << num_sector << " de la pista " << num_pista << " de la superficie " << num_superficie << ": " << buffer << endl;
    }
};

int main() {
    int num_platos, num_pistas, tam_pistas, num_sectores;
    
    cout << "Ingrese el número de platos: ";
    cin >> num_platos;
    
    cout << "Ingrese el número de pistas por superficie: ";
    cin >> num_pistas;
    
    cout << "Ingrese el número de sectores por pista: ";
    cin >> num_sectores;
    
    cout << "Ingrese el tamaño de los sectores (en bytes): ";
    cin >> tam_pistas;
    
    DiscoDuro disco(num_platos, num_pistas, num_sectores, tam_pistas);
    char datos[512] = "Primera prueba de ingreso de valores en un disco"; // Datos a escribir en el disco

    // Escribir los datos en el sector 3 de la pista 5 de la superficie 1
    disco.escribir(3, 5, 1, datos);

    // Leer los datos del sector 3 de la pista 5 de la superficie 1
    char datos_leidos[512];
    disco.leer(3, 5, 1, datos_leidos);

    // Imprimir los datos del sector 3 de la pista 5 de la superficie 1
    disco.imprimir(3, 5, 1);

    return 0;
}