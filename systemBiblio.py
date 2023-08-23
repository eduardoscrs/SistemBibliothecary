from datetime import datetime

class Libro:
    def __init__(self, titulo, autor, isbn):
        self.titulo = titulo
        self.autor = autor
        self.isbn = isbn
        self.disponible = True

class Usuario:
    def __init__(self, nombre, id_usuario):
        self.nombre = nombre
        self.id_usuario = id_usuario
        self.historial_prestamos = []

class Prestamo:
    def __init__(self, libro, usuario, fecha_prestamo):
        self.libro = libro
        self.usuario = usuario
        self.fecha_prestamo = datetime.strptime(fecha_prestamo, '%d-%m-%Y').date()
        self.fecha_devolucion = None

class Catalogo:
    def __init__(self):
        self.libros = []

    def agregar_libro(self, libro):
        self.libros.append(libro)

    def eliminar_libro(self, libro):
        if libro in self.libros:
            self.libros.remove(libro)

    def buscar_libro(self, isbn):
        for libro in self.libros:
            if libro.isbn == isbn:
                return libro
        return None

    def buscar_usuario(self, id_usuario):
        for usuario in usuarios:
            if usuario.id_usuario == id_usuario:
                return usuario
        return None

    def consultar_libros_disponibles(self):
        return [libro for libro in self.libros if libro.disponible]

    def prestar_libro(self, libro, usuario, fecha_prestamo):
        if libro in self.libros and libro.disponible:
            prestamo = Prestamo(libro, usuario, fecha_prestamo)
            libro.disponible = False
            usuario.historial_prestamos.append(prestamo)
            return prestamo

    def devolver_libro(self, prestamo, fecha_devolucion):
        if prestamo in prestamo.usuario.historial_prestamos and not prestamo.fecha_devolucion:
            prestamo.fecha_devolucion = fecha_devolucion
            prestamo.libro.disponible = True

    def historial_prestamos_usuario(self, usuario):
        return usuario.historial_prestamos

def mostrar_menu():
    print("Bienvenido al Sistema de Gestión de Biblioteca")
    print("1. Agregar un libro al catálogo")
    print("2. Eliminar un libro del catálogo")
    print("3. Registrar un nuevo usuario")
    print("4. Prestar un libro")
    print("5. Devolver un libro")
    print("6. Consultar los libros disponibles")
    print("7. Ver historial de préstamos de un usuario")
    print("8. Salir")
    return input("Seleccione una opción: ")

catalogo = Catalogo()
usuarios = []

while True:
    opcion = mostrar_menu()

    if opcion == "1":
        titulo = input("Ingrese el título del libro: ")
        autor = input("Ingrese el autor del libro: ")
        isbn = input("Ingrese el ISBN del libro: ")
        nuevo_libro = Libro(titulo, autor, isbn)
        catalogo.agregar_libro(nuevo_libro)
        print("Libro agregado al catálogo.")

    elif opcion == "2":
        isbn = input("Ingrese el ISBN del libro a eliminar: ")
        libro = catalogo.buscar_libro(isbn)
        if libro:
            catalogo.eliminar_libro(libro)
            print("Libro eliminado del catálogo.")
        else:
            print("Libro no encontrado en el catálogo.")

    elif opcion == "3":
        nombre_usuario = input("Ingrese el nombre del usuario: ")
        id_usuario = input("Ingrese el ID del usuario: ")
        nuevo_usuario = Usuario(nombre_usuario, id_usuario)
        usuarios.append(nuevo_usuario)
        print("Usuario registrado.")

    elif opcion == "4":
        isbn = input("Ingrese el ISBN del libro a prestar: ")
        libro = catalogo.buscar_libro(isbn)
        if libro:
            id_usuario = input("Ingrese el ID del usuario: ")
            usuario = catalogo.buscar_usuario(id_usuario)
            if usuario:
                fecha_prestamo = input("Ingrese la fecha de préstamo: ")
                try:
                    datetime.strptime(fecha_prestamo, '%d-%m-%Y')  # Validar formato de fecha
                except ValueError:
                    print("Formato de fecha incorrecto. Use dd-mm-aaaa.")
                    continue
                prestamo = catalogo.prestar_libro(libro, usuario, fecha_prestamo)
                prestamo = catalogo.prestar_libro(libro, usuario, fecha_prestamo)
                if prestamo:
                    print("Libro prestado.")
                else:
                    print("El libro no está disponible para préstamo.")
            else:
                print("Usuario no registrado.")
        else:
            print("Libro no encontrado en el catálogo.")

    elif opcion == "5":
        isbn = input("Ingrese el ISBN del libro a devolver: ")
        libro = catalogo.buscar_libro(isbn)
        if libro:
            id_usuario = input("Ingrese el ID del usuario: ")
            usuario = catalogo.buscar_usuario(id_usuario)
            if usuario:
                for prestamo in usuario.historial_prestamos:
                    if prestamo.libro.isbn == isbn and not prestamo.fecha_devolucion:
                        fecha_devolucion = input("Ingrese la fecha de devolución: ")
                        catalogo.devolver_libro(prestamo, fecha_devolucion)
                        print("Libro devuelto.")
                        break
                else:
                    print("El usuario no tiene un préstamo activo de este libro.")
            else:
                print("Usuario no registrado.")
        else:
            print("Libro no encontrado en el catálogo.")

    elif opcion == "6":
        libros_disponibles = catalogo.consultar_libros_disponibles()
        if libros_disponibles:
            for libro in libros_disponibles:
                print(f"Libro: {libro.titulo} - Autor: {libro.autor}")
        else:
            print("No hay libros disponibles en este momento.")

    elif opcion == "7":
        id_usuario = input("Ingrese el ID del usuario: ")
        usuario = catalogo.buscar_usuario(id_usuario)
        if usuario:
            historial_prestamos = catalogo.historial_prestamos_usuario(usuario)
            if historial_prestamos:
                for prestamo in historial_prestamos:
                    print(f"Libro: {prestamo.libro.titulo} - Fecha de préstamo: {prestamo.fecha_prestamo}")
            else:
                print("El usuario no tiene historial de préstamos.")
        else:
            print("Usuario no registrado.")

    elif opcion == "8":
        print("¡Hasta luego!")
        break

    else:
        print("Opción incorrecta. Por favor, seleccione una opción válida.")
