#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <algorithm>

class Libro {
public:
    Libro(const std::string& titulo, const std::string& autor, const std::string& isbn)
        : titulo(titulo), autor(autor), isbn(isbn), disponible(true) {}

    std::string getTitulo() const {
        return titulo;
    }

    std::string getAutor() const {
        return autor;
    }

    std::string getISBN() const {
        return isbn;
    }

    bool estaDisponible() const {
        return disponible;
    }

    void prestar() {
        disponible = false;
    }

    void devolver() {
        disponible = true;
    }

private:
    std::string titulo;
    std::string autor;
    std::string isbn;
    bool disponible;
};

class Usuario {
public:
    Usuario(const std::string& nombre, const std::string& id_usuario)
        : nombre(nombre), id_usuario(id_usuario) {}

    std::string getNombre() const {
        return nombre;
    }

    std::string getIdUsuario() const {
        return id_usuario;
    }

    std::vector<Prestamo>& getHistorialPrestamos() {
        return historialPrestamos;
    }

private:
    std::string nombre;
    std::string id_usuario;
    std::vector<Prestamo> historialPrestamos;
};


class Prestamo {
public:
    Prestamo(Libro& libro, Usuario& usuario, const std::tm& fecha_prestamo)
        : libro(libro), usuario(usuario), fecha_prestamo(fecha_prestamo), fecha_devolucion({0}) {}

    void setFechaDevolucion(const std::tm& fecha_devolucion) {
        this->fecha_devolucion = fecha_devolucion;
    }

    Libro& getLibro() const {
        return libro;
    }

    Usuario& getUsuario() const {
        return usuario;
    }

    std::tm getFechaPrestamo() const {
        return fecha_prestamo;
    }

    std::tm getFechaDevolucion() const {
        return fecha_devolucion;
    }

private:
    Libro& libro;
    Usuario& usuario;
    std::tm fecha_prestamo;
    std::tm fecha_devolucion;
};

class Catalogo {
public:
    void agregarLibro(Libro& libro) {
        libros.push_back(libro);
    }

    void eliminarLibro(const std::string& isbn) {
        auto it = std::remove_if(libros.begin(), libros.end(),
            [&isbn](const Libro& libro) { return libro.getISBN() == isbn; });

        if (it != libros.end()) {
            libros.erase(it, libros.end());
        }
    }

    Libro* buscarLibro(const std::string& isbn) {
        for (auto& libro : libros) {
            if (libro.getISBN() == isbn) {
                return &libro;
            }
        }
        return nullptr;
    }

    Usuario* buscarUsuario(const std::string& id_usuario) {
        for (auto& usuario : usuarios) {
            if (usuario.getIdUsuario() == id_usuario) {
                return &usuario;
            }
        }
        return nullptr;
    }

    std::vector<Libro*> consultarLibrosDisponibles() {
        std::vector<Libro*> disponibles;
        for (auto& libro : libros) {
            if (libro.estaDisponible()) {
                disponibles.push_back(&libro);
            }
        }
        return disponibles;
    }
    Prestamo* prestarLibro(Libro& libro, Usuario& usuario, const std::tm& fecha_prestamo) {
        if (libro.estaDisponible()) {
            Prestamo prestamo(libro, usuario, fecha_prestamo);
            libro.prestar();
            usuario.getHistorialPrestamos().push_back(prestamo);
            return &usuario.getHistorialPrestamos().back();
        }
    return nullptr;
}


    void devolverLibro(Prestamo& prestamo, const std::tm& fecha_devolucion) {
        prestamo.setFechaDevolucion(fecha_devolucion);
        prestamo.getLibro().devolver();
    }

    std::vector<Prestamo>& historialPrestamosUsuario(Usuario& usuario) {
        return usuario.getHistorialPrestamos();
}
private:
    std::vector<Libro> libros;
    std::vector<Usuario> usuarios;
};

void mostrarMenu() {
    std::cout << "Bienvenido al Sistema de Gestión de Biblioteca" << std::endl;
    std::cout << "1. Agregar un libro al catálogo" << std::endl;
    std::cout << "2. Eliminar un libro del catálogo" << std::endl;
    std::cout << "3. Registrar un nuevo usuario" << std::endl;
    std::cout << "4. Prestar un libro" << std::endl;
    std::cout << "5. Devolver un libro" << std::endl;
    std::cout << "6. Consultar los libros disponibles" << std::endl;
    std::cout << "7. Ver historial de préstamos de un usuario" << std::endl;
    std::cout << "8. Salir" << std::endl;
    std::cout << "Seleccione una opción: ";
}

int main() {
    Catalogo catalogo;

    while (true) {
        mostrarMenu();
        char opcion;
        std::cin >> opcion;

        switch (opcion) {
            case '1': {
                std::string titulo, autor, isbn;
                std::cout << "Ingrese el título del libro: ";
                std::cin.ignore();
                std::getline(std::cin, titulo);
                std::cout << "Ingrese el autor del libro: ";
                std::getline(std::cin, autor);
                std::cout << "Ingrese el ISBN del libro: ";
                std::cin >> isbn;
                Libro nuevoLibro(titulo, autor, isbn);
                catalogo.agregarLibro(nuevoLibro);
                std::cout << "Libro agregado al catálogo." << std::endl;
                break;
            }
            case '2': {
                std::string isbn;
                std::cout << "Ingrese el ISBN del libro a eliminar: ";
                std::cin >> isbn;
                Libro* libro = catalogo.buscarLibro(isbn);
                if (libro) {
                    catalogo.eliminarLibro(isbn);
                    std::cout << "Libro eliminado del catálogo." << std::endl;
                } else {
                    std::cout << "Libro no encontrado en el catálogo." << std::endl;
                }
                break;
            }
            case '3': {
                std::string nombreUsuario, idUsuario;
                std::cout << "Ingrese el nombre del usuario: ";
                std::cin.ignore();
                std::getline(std::cin, nombreUsuario);
                std::cout << "Ingrese el ID del usuario: ";
                std::cin >> idUsuario;
                Usuario nuevoUsuario(nombreUsuario, idUsuario);
                catalogo.registrarUsuario(nuevoUsuario);
                std::cout << "Usuario registrado." << std::endl;
                break;
            }
            case '4': {
                std::string isbn, idUsuario;
                std::cout << "Ingrese el ISBN del libro a prestar: ";
                std::cin >> isbn;
                Libro* libro = catalogo.buscarLibro(isbn);
                if (libro) {
                    std::cout << "Ingrese el ID del usuario: ";
                    std::cin >> idUsuario;
                    Usuario* usuario = catalogo.buscarUsuario(idUsuario);
                    if (usuario) {
                        std::tm fechaPrestamo;
                        std::cout << "Ingrese la fecha de préstamo (dd-mm-aaaa): ";
                        std::cin >> std::get_time(&fechaPrestamo, "%d-%m-%Y");
                        if (!std::cin.fail()) {
                            Prestamo* prestamo = catalogo.prestarLibro(*libro, *usuario, fechaPrestamo);
                            if (prestamo) {
                                std::cout << "Libro prestado." << std::endl;
                            } else {
                                std::cout << "El libro no está disponible para préstamo." << std::endl;
                            }
                        } else {
                            std::cout << "Formato de fecha incorrecto. Use dd-mm-aaaa." << std::endl;
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        }
                    } else {
                        std::cout << "Usuario no registrado." << std::endl;
                    }
                } else {
                    std::cout << "Libro no encontrado en el catálogo." << std::endl;
                }
                break;
            }
            case '5': {
                std::string isbn, idUsuario;
                std::cout << "Ingrese el ISBN del libro a devolver: ";
                std::cin >> isbn;
                Libro* libro = catalogo.buscarLibro(isbn);
                if (libro) {
                    std::cout << "Ingrese el ID del usuario: ";
                    std::cin >> idUsuario;
                    Usuario* usuario = catalogo.buscarUsuario(idUsuario);
                    if (usuario) {
                        std::vector<Prestamo>& historial = catalogo.historialPrestamosUsuario(*usuario);
                        for (Prestamo& prestamo : historial) {
                            if (prestamo.getLibro().getISBN() == isbn && prestamo.getFechaDevolucion().tm_year == 0) {
                                std::tm fechaDevolucion;
                                std::cout << "Ingrese la fecha de devolución (dd-mm-aaaa): ";
                                std::cin >> std::get_time(&fechaDevolucion, "%d-%m-%Y");
                                if (!std::cin.fail()) {
                                    catalogo.devolverLibro(prestamo, fechaDevolucion);
                                    std::cout << "Libro devuelto." << std::endl;
                                } else {
                                    std::cout << "Formato de fecha incorrecto. Use dd-mm-aaaa." << std::endl;
                                    std::cin.clear();
                                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                }
                                break;
                            }
                        }
                        std::cout << "El usuario no tiene un préstamo activo de este libro." << std::endl;
                    } else {
                        std::cout << "Usuario no registrado." << std::endl;
                    }
                } else {
                    std::cout << "Libro no encontrado en el catálogo." << std::endl;
                }
                break;
            }
            case '6': {
                std::vector<Libro*> disponibles = catalogo.consultarLibrosDisponibles();
                if (!disponibles.empty()) {
                    std::cout << "Libros disponibles:" << std::endl;
                    for (const Libro* libro : disponibles) {
                        std::cout << "Título: " << libro->getTitulo() << " - Autor: " << libro->getAutor() << " - ISBN: " << libro->getISBN() << std::endl;
                    }
                } else {
                    std::cout << "No hay libros disponibles en este momento." << std::endl;
                }
                break;
            }
            case '7': {
                std::string idUsuario;
                std::cout << "Ingrese el ID del usuario: ";
                std::cin >> idUsuario;
                Usuario* usuario = catalogo.buscarUsuario(idUsuario);
                if (usuario) {
                    std::vector<Prestamo>& historial = catalogo.historialPrestamosUsuario(*usuario);
                    if (!historial.empty()) {
                        std::cout << "Historial de préstamos del usuario:" << std::endl;
                        for (const Prestamo& prestamo : historial) {
                            std::cout << "Título: " << prestamo.getLibro().getTitulo() << " - Fecha de préstamo: " << std::put_time(&prestamo.getFechaPrestamo(), "%d-%m-%Y") << std::endl;
                        }
                    } else {
                        std::cout << "El usuario no tiene historial de préstamos." << std::endl;
                    }
                } else {
                    std::cout << "Usuario no registrado." << std::endl;
                }
                break;
            }
            case '8':
                std::cout << "¡Hasta luego!" << std::endl;
                return 0;
            default:
                std::cout << "Opción incorrecta. Por favor, seleccione una opción válida." << std::endl;
        }
    }

    return 0;
}