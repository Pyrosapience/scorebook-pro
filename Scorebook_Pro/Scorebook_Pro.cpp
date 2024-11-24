#include <iostream>
#include <string>
#include <cstdlib> // Para system()
#include <cstdio>  // For FILE and popen

using namespace std;

// Función para convertir un entero a cadena (sin usar C++11)
string intACadena(int num) {
    char buffer[20]; // Buffer para almacenar el número como cadena
    sprintf(buffer, "%d", num); // Convertir el entero a cadena
    return string(buffer); // Devolver la cadena
}

// Función para crear las tablas en la base de datos
void crearTablas() {
    string sqlStudents = "CREATE TABLE IF NOT EXISTS students (id INTEGER PRIMARY KEY, name TEXT, age INTEGER);";
    string sqlCourses = "CREATE TABLE IF NOT EXISTS courses (id INTEGER PRIMARY KEY, title TEXT, code TEXT);";
    
    string comandoStudents = "sqlite3 scorebook_pro.db \"" + sqlStudents + "\"";
    string comandoCourses = "sqlite3 scorebook_pro.db \"" + sqlCourses + "\"";
    
    system(comandoStudents.c_str());
    system(comandoCourses.c_str());
}

// Función para verificar si el ID del estudiante ya existe
bool existeEstudiante(int id) {
    string comando = "sqlite3 scorebook_pro.db \"SELECT COUNT(*) FROM students WHERE id = " + intACadena(id) + ";\"";
    FILE* pipe = popen(comando.c_str(), "r");
    if (!pipe) return false;
    
    char buffer[128];
    string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) { // Replace nullptr with NULL
        result += buffer;
    }
    pclose(pipe);
    return atoi(result.c_str()) > 0; // Usar atoi para convertir cadena a entero
}

// Función para agregar un estudiante a la base de datos
void agregarEstudiante() {
    int id;
    string nombre;
    int edad;
    
    cout << "Ingrese el ID del estudiante (máx 7 dígitos): ";
    cin >> id;
    cin.ignore();

    // Verificar que el ID no exceda 7 dígitos
    if (intACadena(id).length() > 7) {
        cout << "El ID no puede exceder 7 dígitos.\n";
        return;
    }

    // Verificar si el estudiante ya existe
    if (existeEstudiante(id)) {
        cout << "Ya existe un estudiante con el ID " << id << ".\n";
        return;
    }

    cout << "Ingrese el nombre del estudiante: ";
    getline(cin, nombre);
    cout << "Ingrese la edad del estudiante: ";
    cin >> edad;
    cin.ignore();

    string sql = "INSERT INTO students (id, name, age) VALUES (" + intACadena(id) + ", '" + nombre + "', " + intACadena(edad) + ");";
    string comando = "sqlite3 scorebook_pro.db \"" + sql + "\"";
    system(comando.c_str());

    cout << "Estudiante agregado exitosamente con ID " << id << ".\n";
}

// Función para ver un estudiante específico por ID
void verEstudiante() {
    int id;
    cout << "Ingrese el ID del estudiante a ver: ";
    cin >> id;
    cin.ignore();

    string comando = "sqlite3 scorebook_pro.db \"SELECT * FROM students WHERE id = " + intACadena(id) + ";\"";
    system(comando.c_str());
}

// Función para agregar un curso a la base de datos
void agregarCurso() {
    int id;
    string titulo, codigoCurso;

    cout << "Ingrese el ID del curso (máx 7 dígitos): ";
    cin >> id;
    cin.ignore();

    // Verificar que el ID no exceda 7 dígitos
    if (intACadena(id).length() > 7) {
        cout << "El ID no puede exceder 7 dígitos.\n";
        return;
    }

    string sql = "INSERT INTO courses (id, title, code) VALUES (" + intACadena(id) + ", '";
    cout << "Ingrese el título del curso: ";
    getline(cin, titulo);
    cout << "Ingrese el código del curso: ";
    getline(cin, codigoCurso);
    
    sql += titulo + "', '" + codigoCurso + "');";
    string comando = "sqlite3 scorebook_pro.db \"" + sql + "\"";
    system(comando.c_str());

    cout << "Curso agregado exitosamente con ID " << id << ".\n";
}

// Función para ver un curso específico por ID
void verCurso() {
    int id;
    cout << "Ingrese el ID del curso a ver: ";
    cin >> id;
    cin.ignore();

    string comando = "sqlite3 scorebook_pro.db \"SELECT * FROM courses WHERE id = " + intACadena(id) + ";\"";
    system(comando.c_str());
}

// Función para actualizar la información de un estudiante
void actualizarEstudiante() {
    int id;
    string nombre;
    int edad;
    cout << "Ingrese el ID del estudiante a actualizar: ";
    cin >> id;
    cin.ignore();
    cout << "Ingrese el nuevo nombre: ";
    getline(cin, nombre);
    cout << "Ingrese la nueva edad: ";
    cin >> edad;
    cin.ignore();

    string sql = "UPDATE students SET name = '" + nombre + "', age = " + intACadena(edad) + " WHERE id = " + intACadena(id) + ";";
    string comando = "sqlite3 scorebook_pro.db \"" + sql + "\"";
    system(comando.c_str());

    cout << "Estudiante actualizado exitosamente con ID " << id << ".\n";
}

// Función para actualizar la información de un curso
void actualizarCurso() {
    int id;
    string titulo, codigoCurso;
    cout << "Ingrese el ID del curso a actualizar: ";
    cin >> id;
    cin.ignore();
    cout << "Ingrese el nuevo título: ";
    getline(cin, titulo);
    cout << "Ingrese el nuevo código del curso: ";
    getline(cin, codigoCurso);

    string sql = "UPDATE courses SET title = '" + titulo + "', code = '" + codigoCurso + "' WHERE id = " + intACadena(id) + ";";
    string comando = "sqlite3 scorebook_pro.db \"" + sql + "\"";
    system(comando.c_str());

    cout << "Curso actualizado exitosamente con ID " << id << ".\n";
}

// Función para eliminar un estudiante de la base de datos
void eliminarEstudiante() {
    int id;
    cout << "Ingrese el ID del estudiante a eliminar: ";
    cin >> id;
    cin.ignore();

    string sql = "DELETE FROM students WHERE id = " + intACadena(id) + ";";
    string comando = "sqlite3 scorebook_pro.db \"" + sql + "\"";
    system(comando.c_str());

    cout << "Estudiante eliminado exitosamente con ID " << id << ".\n";
}

// Función para eliminar un curso de la base de datos
void eliminarCurso() {
    int id;
    cout << "Ingrese el ID del curso a eliminar: ";
    cin >> id;
    cin.ignore();

    string sql = "DELETE FROM courses WHERE id = " + intACadena(id) + ";";
    string comando = "sqlite3 scorebook_pro.db \"" + sql + "\"";
    system(comando.c_str());

    cout << "Curso eliminado exitosamente con ID " << id << ".\n";
}

int main() {
    crearTablas(); // Crear tablas al iniciar el programa
    int opcion;
    while (true) {
        cout << "\n--- Sistema de Gestión Escolar Scorebook Pro ---\n";
        cout << "1. Agregar Estudiante\n";
        cout << "2. Ver Estudiante\n";
        cout << "3. Actualizar Estudiante\n";
        cout << "4. Eliminar Estudiante\n";
        cout << "5. Agregar Curso\n";
        cout << "6. Ver Curso\n";
        cout << "7. Actualizar Curso\n";
        cout << "8. Eliminar Curso\n";
        cout << "9. Salir\n";
        cout << "Ingrese una opción: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1: agregarEstudiante(); break;
            case 2: verEstudiante(); break;
            case 3: actualizarEstudiante(); break;
            case 4: eliminarEstudiante(); break;
            case 5: agregarCurso(); break;
            case 6: verCurso(); break;
            case 7: actualizarCurso(); break;
            case 8: eliminarCurso(); break;
            case 9: exit(0);
            default: cout << "Opción no válida.\n"; break;
        }
    }
    return 0;
}

