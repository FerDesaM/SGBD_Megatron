#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include <cstring>
class Sector {
private:
    int num_sector;
    int num_bytes;
    char* data;

public:
    Sector() {}
    Sector(int num, int bytes) : num_sector(num), num_bytes(bytes) {
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
    
    Pista(int num_pista, int num_sect, int tam_sec) : pista(num_pista), num_sectores(num_sect) {
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
    
    Superficie(int num_surper, int num_pist, int num_sect, int tam_sec)
        : num_surface(num_surper), num_pistas(num_pist) {
        pistas = new Pista[num_pistas];
        for (int i = 0; i < num_pistas; ++i) {
            pistas[i] = Pista(i+1, num_sect, tam_sec);
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
            superficie[i] = new Superficie(i, num_pist, num_sect, tam_sec); // Crea nuevas instancias de Superficie
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
            platos[i] = Plato(num_pist, num_sect, tam_sect);
        }
    }

    ~DiscoDuro() {
        delete[] platos;
    }

    Plato& getPlato(int index) const {
        return platos[index];
    }

    void crearCarpeta(const std::string &rutaCarpeta) {
        if (mkdir(rutaCarpeta.c_str(), 0777) == -1) {
            std::cerr << "Error al crear la carpeta: " << strerror(errno) << std::endl;
        } else {
            std::cout << "Carpeta creada exitosamente: " << rutaCarpeta << std::endl;
        }
    }

    void crearArchivo(const std::string &rutaArchivo) {
        std::ofstream archivo(rutaArchivo);
        std::cout << "Creando archivo: " << rutaArchivo << std::endl;
        // Lógica para crear un archivo en el sistema de archivos
    }

    void crearEstructuraDisco() {
        std::string carpeta = "discoDuro";
        mkdir(carpeta.c_str(), 0777);

        for (int plato = 0; plato < num_platos; plato++) {
            std::string carpetaPlato = carpeta + "/plato_" + std::to_string(plato + 1);
            mkdir(carpetaPlato.c_str(), 0777);

            for (int superficie = 0; superficie < 2; superficie++) {
                std::string carpetaSuperficie = carpetaPlato + "/superficie_" + std::to_string(superficie + 1);
                mkdir(carpetaSuperficie.c_str(), 0777);

                for (int pista = 0; pista <  num_platos; pista++) {
                    std::string carpetaPista = carpetaSuperficie + "/pista_" + std::to_string(pista + 1);
                    mkdir(carpetaPista.c_str(), 0777);

                    for (int sector = 0; sector < num_platos; sector++) {
                        std::string archivoSector = carpetaPista + "/sector_" + std::to_string(sector + 1) + ".txt";
                        crearArchivo(archivoSector);
                    }
                }
            }
        }
        std::cout << "La estructura del disco ha sido creada exitosamente." << std::endl;
    }

    bool sectorDisponible(const std::string &sector) {
        std::ifstream archivo(sector);

        if (!archivo.is_open()) {
            std::cout << "No existe ese sector" << std::endl;
            // manejar error
        }

        std::string linea;
        while (getline(archivo, linea)) {
            if (!linea.empty() && linea.find("#") != std::string::npos) {
                // El archivo contiene contenido y ya está ocupado
                archivo.close();
                return false;
            }
        }

        // Terminó el archivo sin encontrar contenido, está disponible
        archivo.close();
        return true;
    }
};

int main() {
    int num_platos, num_pistas, tam_pistas, num_sectores;
    
    std::cout << "Ingrese el número de platos: ";
    std::cin >> num_platos;
    
    std::cout << "Ingrese el número de pistas por superficie: ";
    std::cin >> num_pistas;
    
    std::cout << "Ingrese el número de sectores por pista: ";
    std::cin >> num_sectores;
    
    std::cout << "Ingrese el tamaño de los sectores (en bytes): ";
    std::cin >> tam_pistas;
    
    DiscoDuro disco(num_platos, num_pistas, num_sectores, tam_pistas);
    char datos[512] = "Primera prueba de ingreso de valores en un disco"; // Datos a escribir en el disco
    
    // Crear la estructura del disco
    disco.crearEstructuraDisco();
    
    // Aquí puedes agregar las funcionalidades adicionales que necesites para tu aplicación
    
    return 0;
}