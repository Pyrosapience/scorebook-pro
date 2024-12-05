#include <iostream>
#include <string>
#include <cstdlib> // Para system()
#include <cstdio>  // For FILE and popen

using namespace std;

// Funcion para convertir un entero a un string
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

// Funcion para ver un estudiante especifico por ID, junto con sus cursos y calificaciones
void verEstudiante() {
    int id;
    cout << "Ingrese el ID del estudiante a ver: ";
    cin >> id;
    cin.ignore();

    // Mostrar la información básica del estudiante
    string comandoEstudiante = "sqlite3 scorebook_pro.db \"SELECT * FROM students WHERE id = " + intACadena(id) + ";\"";
    cout << "\n--- Informacion del Estudiante ---\n";
    system(comandoEstudiante.c_str());

    // Mostrar los cursos asignados al estudiante con sus calificaciones
    string comandoCursos = "sqlite3 scorebook_pro.db \"SELECT c.title, g.semester, g.lab1, g.lab2, g.lab3, g.lab4, g.exam1, g.exam2, g.exam3, g.exam4 FROM courses c JOIN grades g ON c.id = g.course_id WHERE g.student_id = " + intACadena(id) + ";\"";
    FILE* pipe = popen(comandoCursos.c_str(), "r");
    if (!pipe) {
        cout << "Error al ejecutar el comando.\n";
        return;
    }

    char buffer[256];
    string result = "";
    cout << "\n--- Cursos Asignados y Calificaciones ---\n";
    cout << "Curso | Semestre | Lab1 | Lab2 | Lab3 | Lab4 | Exam1 | Exam2 | Exam3 | Exam4 | Promedio | Estado\n";

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result = buffer;

        // Parsear los resultados para extraer las calificaciones
        char curso[50];
        int semestre, lab1, lab2, lab3, lab4, exam1, exam2, exam3, exam4;
        sscanf(result.c_str(), "%[^|]|%d|%d|%d|%d|%d|%d|%d|%d|%d",
               curso, &semestre, &lab1, &lab2, &lab3, &lab4, &exam1, &exam2, &exam3, &exam4);

        // Calcular el promedio
        double promedio = (lab1 + lab2 + lab3 + lab4 + exam1 + exam2 + exam3 + exam4) / 8.0;

        // Determinar el estado del estudiante (Aprobado o Reprobado)
        string estado = (promedio >= 6.0) ? "Aprobado" : "Reprobado";

        // Mostrar la información
        cout << curso << " | " << semestre << " | " << lab1 << " | " << lab2 << " | " << lab3 << " | " << lab4
             << " | " << exam1 << " | " << exam2 << " | " << exam3 << " | " << exam4
             << " | " << promedio << " | " << estado << "\n";
    }
    pclose(pipe);
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

// Funcion principal
int main() {
    crearTablas(); // Crear tablas al iniciar el programa
    int opcion;
    while (true) {
        cout << "\n--- Sistema de Gestion Escolar Scorebook Pro ---\n";
        cout << "1. Agregar Estudiante\n";
        cout << "2. Ver Estudiante\n";
        cout << "3. Agregar Curso\n";
        cout << "4. Ver Cursos Disponibles\n";
        cout << "5. Asignar Curso a Estudiante\n";
        cout << "6. Salir\n";
        cout << "Ingrese una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1: agregarEstudiante(); break;
            case 2: verEstudiante(); break;
            case 3: agregarCurso(); break;
            case 4: verCursosDisponibles(); break;
            case 5: asignarCurso(); break;
            case 6: exit(0);
            default: cout << "Opcion no valida.\n"; break;
        }
    }
    return 0;
}