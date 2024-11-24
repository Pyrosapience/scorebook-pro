#include <iostream>
#include <string>
#include <cstdlib> // Para system()
#include <cstdio>  // For FILE and popen

using namespace std;

// Funcion para convertir un entero a cadena (sin usar C++11)
string intACadena(int num) {
    char buffer[20]; // Buffer para almacenar el numero como cadena
    sprintf(buffer, "%d", num); // Convertir el entero a cadena
    return string(buffer); // Devolver la cadena
}

// Funcion para crear las tablas en la base de datos
void crearTablas() {
    string sqlStudents = "CREATE TABLE IF NOT EXISTS students (id INTEGER PRIMARY KEY, name TEXT, age INTEGER);";
    string sqlCourses = "CREATE TABLE IF NOT EXISTS courses (id INTEGER PRIMARY KEY, title TEXT, code TEXT);";
    string sqlGrades = R"(CREATE TABLE IF NOT EXISTS grades (
        student_id INTEGER,
        course_id INTEGER,
        semester INTEGER,
        lab1 INTEGER,
        lab2 INTEGER,
        lab3 INTEGER,
        lab4 INTEGER,
        exam1 INTEGER,
        exam2 INTEGER,
        exam3 INTEGER,
        exam4 INTEGER,
        PRIMARY KEY (student_id, course_id, semester),
        FOREIGN KEY (student_id) REFERENCES students(id),
        FOREIGN KEY (course_id) REFERENCES courses(id)
    );)";
    
    string comandoStudents = "sqlite3 scorebook_pro.db \"" + sqlStudents + "\"";
    string comandoCourses = "sqlite3 scorebook_pro.db \"" + sqlCourses + "\"";
    string comandoGrades = "sqlite3 scorebook_pro.db \"" + sqlGrades + "\"";

    system(comandoStudents.c_str());
    system(comandoCourses.c_str());
    system(comandoGrades.c_str());
}

// Funcion para verificar si el ID del estudiante ya existe
bool existeEstudiante(int id) {
    string comando = "sqlite3 scorebook_pro.db \"SELECT COUNT(*) FROM students WHERE id = " + intACadena(id) + ";\"";
    FILE* pipe = popen(comando.c_str(), "r");
    if (!pipe) return false;
    
    char buffer[128];
    string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
    }
    pclose(pipe);
    return atoi(result.c_str()) > 0;
}

// Funcion para agregar un estudiante a la base de datos
void agregarEstudiante() {
    int id;
    string nombre;
    int edad;
    
    cout << "Ingrese el ID del estudiante (max 7 digitos): ";
    cin >> id;
    cin.ignore();

    if (intACadena(id).length() > 7) {
        cout << "El ID no puede exceder 7 digitos.\n";
        return;
    }

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

// Funcion para ver un estudiante especifico por ID
void verEstudiante() {
    int id;
    cout << "Ingrese el ID del estudiante a ver: ";
    cin >> id;
    cin.ignore();

    string comando = "sqlite3 scorebook_pro.db \"SELECT * FROM students WHERE id = " + intACadena(id) + ";\"";
    system(comando.c_str());
}

// Funcion para agregar un curso a la base de datos
void agregarCurso() {
    int id;
    string titulo, codigoCurso;

    cout << "Ingrese el ID del curso (max 7 digitos): ";
    cin >> id;
    cin.ignore();

    if (intACadena(id).length() > 7) {
        cout << "El ID no puede exceder 7 digitos.\n";
        return;
    }

    cout << "Ingrese el titulo del curso: ";
    getline(cin, titulo);
    cout << "Ingrese el codigo del curso: ";
    getline(cin, codigoCurso);
    
    string sql = "INSERT INTO courses (id, title, code) VALUES (" + intACadena(id) + ", '" + titulo + "', '" + codigoCurso + "');";
    string comando = "sqlite3 scorebook_pro.db \"" + sql + "\"";
    system(comando.c_str());

    cout << "Curso agregado exitosamente con ID " << id << ".\n";
}

// Funcion para ver un curso especifico por ID
void verCurso() {
    int id;
    cout << "Ingrese el ID del curso a ver: ";
    cin >> id;
    cin.ignore();

    string comando = "sqlite3 scorebook_pro.db \"SELECT * FROM courses WHERE id = " + intACadena(id) + ";\"";
    system(comando.c_str());
}

// Funcion para ver todos los cursos disponibles
void verCursosDisponibles() {
    cout << "\n--- Cursos Disponibles ---\n";
    string comando = "sqlite3 scorebook_pro.db \"SELECT id, title, code FROM courses;\"";
    system(comando.c_str());
}

// Funcion para asignar un curso a un estudiante
void asignarCurso() {
    int studentId, courseId, semester;
    int labs[4], exams[4];

    cout << "Ingrese el ID del estudiante: ";
    cin >> studentId;
    cin.ignore();

    cout << "Ingrese el ID del curso: ";
    cin >> courseId;
    cin.ignore();

    cout << "Ingrese el semestre: ";
    cin >> semester;
    cin.ignore();

    cout << "Ingrese las calificaciones para los 4 laboratorios:\n";
    for (int i = 0; i < 4; i++) {
        cout << "Lab " << i + 1 << ": ";
        cin >> labs[i];
    }

    cout << "Ingrese las calificaciones para los 4 examenes parciales:\n";
    for (int i = 0; i < 4; i++) {
        cout << "Examen " << i + 1 << ": ";
        cin >> exams[i];
    }

    string sql = "INSERT INTO grades (student_id, course_id, semester, lab1, lab2, lab3, lab4, exam1, exam2, exam3, exam4) VALUES (" +
                 intACadena(studentId) + ", " + intACadena(courseId) + ", " + intACadena(semester) + ", " +
                 intACadena(labs[0]) + ", " + intACadena(labs[1]) + ", " + intACadena(labs[2]) + ", " + intACadena(labs[3]) + ", " +
                 intACadena(exams[0]) + ", " + intACadena(exams[1]) + ", " + intACadena(exams[2]) + ", " + intACadena(exams[3]) + ");";
    string comando = "sqlite3 scorebook_pro.db \"" + sql + "\"";

    system(comando.c_str());
    cout << "Curso asignado y calificaciones registradas correctamente.\n";
}

// Funcion para ver calificaciones por estudiante y semestre
void verCalificaciones() {
    int studentId, semester;

    cout << "Ingrese el ID del estudiante: ";
    cin >> studentId;
    cin.ignore();

    cout << "Ingrese el semestre: ";
    cin >> semester;
    cin.ignore();

    string comando = "sqlite3 scorebook_pro.db \"SELECT course_id, lab1, lab2, lab3, lab4, exam1, exam2, exam3, exam4 FROM grades WHERE student_id = " +
                     intACadena(studentId) + " AND semester = " + intACadena(semester) + ";\"";

    system(comando.c_str());
}

// Funcion principal
int main() {
    crearTablas(); // Crear tablas al iniciar el programa
    int opcion;
    while (true) {
        cout << "\n--- Sistema de Gestion Escolar Scorebook Pro ---\n";
        cout << "1. Agregar Estudiante\n";
        cout << "2. Ver Estudiante\n";
        cout << "3. Agregar Curso\n";
        cout << "4. Ver Curso\n";
        cout << "5. Ver Cursos Disponibles\n";
        cout << "6. Asignar Curso a Estudiante\n";
        cout << "7. Ver Calificaciones\n";
        cout << "8. Salir\n";
        cout << "Ingrese una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1: agregarEstudiante(); break;
            case 2: verEstudiante(); break;
            case 3: agregarCurso(); break;
            case 4: verCurso(); break;
            case 5: verCursosDisponibles(); break;
            case 6: asignarCurso(); break;
            case 7: verCalificaciones(); break;
            case 8: exit(0);
            default: cout << "Opcion no valida.\n"; break;
        }
    }
    return 0;
}