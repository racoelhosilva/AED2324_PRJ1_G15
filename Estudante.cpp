#include "Estudante.h"

Estudante::Estudante(int id, string name, list<Turma> classes) {
    studentID = id;
    studentName = name;
    schedule = classes;
}

int const Estudante::getID() const {return studentID;}
string const Estudante::getName() const {return studentName;}
list<Turma> const Estudante::getSchedule() const {return schedule;}

void Estudante::setSchedule(list<Turma> newSchedule) {schedule = newSchedule;}

