#include <algorithm>
#include "Gestor.h"
#include <set>
#include <map>

void Gestor::setCap(const int newCap){
    this->cap = newCap;
}


bool Gestor::extractTurmas(string fname) {
    ifstream fileReader(fname);
    string line;

    if (!getline(fileReader, line)){
        return false;
    }

    while (getline(fileReader, line)) {
        istringstream fieldReader(line);
        string ucID, classID;
        getline(fieldReader, ucID, ',');
        getline(fieldReader, classID, '\r');
        turmas.emplace_back(ucID, classID);
    }
    return true;
}

bool Gestor::extractAulas(string fname) {
    ifstream fileReader(fname);
    string line;

    if (!getline(fileReader, line)){
        return false;
    }

    while (getline(fileReader, line)) {
        istringstream fieldReader(line);
        string classID, ucID, weekday, type;
        float start, duration;
        getline(fieldReader, classID, ',');
        getline(fieldReader, ucID, ',');
        getline(fieldReader, weekday, ',');
        char x;
        fieldReader >> start >> x >> duration >> x;
        getline(fieldReader, type, '\r');
        for (Turma &T : turmas) {
            if (T.getcodigoTurma() == classID && T.getcodigoUC() == ucID) {
                T.addAulas(Aula(weekday, start, duration, type));
            }
        }
    }
    return true;
}

bool Gestor::extractEstudantes(string fname) {
    ifstream fileReader(fname);
    string line;

    if (!getline(fileReader, line)){
        return false;
    }

    int id, previousId;
    string name, ucID, classID;

    getline(fileReader, line);
    istringstream fieldReader(line);
    char x = ',';
    fieldReader >> id >> x;
    getline(fieldReader, name, ',');
    getline(fieldReader, ucID, ',');
    getline(fieldReader, classID, '\r');
    Estudante currentEstudante = Estudante(id, name);
    previousId = id;
    list<Turma> turmasEstudante = {};
    for (Turma &T : turmas) {
        if (T.getcodigoTurma() == classID && T.getcodigoUC() == ucID) {
            turmasEstudante.push_back(T);
        }
    }

    while (getline(fileReader, line)) {
        istringstream fieldReader(line);
        fieldReader >> id >> x;
        getline(fieldReader, name, ',');
        getline(fieldReader, ucID, ',');
        getline(fieldReader, classID, '\r');
        if (id != previousId) {
            currentEstudante.setSchedule(turmasEstudante);
            estudantes.insert(currentEstudante);
            currentEstudante = Estudante(id, name);
            turmasEstudante = {};
            previousId = id;
        }
        for (Turma &T : turmas) {
            if (T.getcodigoTurma() == classID && T.getcodigoUC() == ucID) {
                turmasEstudante.push_back(T);
            }
        }
    }
    currentEstudante.setSchedule(turmasEstudante);
    estudantes.insert(currentEstudante);

    return true;
}

bool Gestor::outputHorárioEstudante(int id){
    Estudante target(id, "", {});
    auto it = find(estudantes.begin(), estudantes.end(), target);
    for (Turma turma : it->getSchedule()) {
        cout << turma.getcodigoUC() << '(' << turma.getcodigoTurma() << ')' << '\n';
        for (auto aula : turma.getAulas()){
            cout << "\t" << aula.getDia() << '\t' << aula.getHoraInicio() << '\t' << aula.getTipo() << '\n';
        }
    }
    return true;
}

bool Gestor::outputHorárioTurma(string codigoTurma){
    if(find_if(turmas.begin(), turmas.end(), [codigoTurma](const Turma& t) {return t.getcodigoTurma() == codigoTurma;}) == turmas.end())
        return false;

    this->sortTurmas();

    string ucPrefix = "L.EIC0";
    ucPrefix.append(to_string((codigoTurma[0] - '1' + '0')));
    for (int idx = 0; idx < 5; idx++) {
        Turma target(ucPrefix.append(to_string(idx + '1')), codigoTurma);


    }

    return true;
}

bool Gestor::outputHorárioUC(string codigoUC){
    return true;
}

bool Gestor::outputListaEstudanteTurma(string codigoTurma, int order){
    if(find_if(turmas.begin(), turmas.end(), [codigoTurma](const Turma& t) {return t.getcodigoTurma() == codigoTurma;}) == turmas.end())
        return false;
    if (order == 1) {
        set<Estudante, EstudanteAlphaAscending> lista;
        for (Estudante e: estudantes) {
            for (Turma t: e.getSchedule()) {
                if (t.getcodigoTurma() == codigoTurma) {
                    lista.insert(e);
                    break;
                }
            }
        }
        for (Estudante e : lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    if (order == 2) {
        set<Estudante, EstudanteAlphaDescending> lista;
        for (Estudante e: estudantes) {
            for (Turma t: e.getSchedule()) {
                if (t.getcodigoTurma() == codigoTurma) {
                    lista.insert(e);
                    break;
                }
            }
        }
        for (Estudante e : lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    if (order == 3) {
        set<Estudante, EstudanteNumAscending> lista;
        for (Estudante e: estudantes) {
            for (Turma t: e.getSchedule()) {
                if (t.getcodigoTurma() == codigoTurma) {
                    lista.insert(e);
                    break;
                }
            }
        }
        for (Estudante e : lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    if (order == 4) {
        set<Estudante, EstudanteNumDescending> lista;
        for (Estudante e: estudantes) {
            for (Turma t: e.getSchedule()) {
                if (t.getcodigoTurma() == codigoTurma) {
                    lista.insert(e);
                    break;
                }
            }
        }
        for (Estudante e : lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    return true;
}

bool Gestor::outputListaEstudanteUC(string codigoUC, int order){
    if(find_if(turmas.begin(), turmas.end(), [codigoUC](const Turma& t) {return t.getcodigoUC() == codigoUC;}) == turmas.end())
        return false;
    if (order == 1) {
        set<Estudante, EstudanteAlphaAscending> lista;
        for (Estudante e: estudantes) {
            for (Turma t: e.getSchedule()) {
                if (t.getcodigoUC() == codigoUC) {
                    lista.insert(e);
                    break;
                }
            }
        }
        for (Estudante e : lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    if (order == 2) {
        set<Estudante, EstudanteAlphaDescending> lista;
        for (Estudante e: estudantes) {
            for (Turma t: e.getSchedule()) {
                if (t.getcodigoUC() == codigoUC) {
                    lista.insert(e);
                    break;
                }
            }
        }
        for (Estudante e : lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    if (order == 3) {
        set<Estudante, EstudanteNumAscending> lista;
        for (Estudante e: estudantes) {
            for (Turma t: e.getSchedule()) {
                if (t.getcodigoUC() == codigoUC) {
                    lista.insert(e);
                    break;
                }
            }
        }
        for (Estudante e : lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    if (order == 4) {
        set<Estudante, EstudanteNumDescending> lista;
        for (Estudante e: estudantes) {
            for (Turma t: e.getSchedule()) {
                if (t.getcodigoUC() == codigoUC) {
                    lista.insert(e);
                    break;
                }
            }
        }
        for (Estudante e : lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    return true;
}
void Gestor::outputListaEstudanteAno(int ano, int order){
    if (order == 1) {
        set<Estudante, EstudanteAlphaAscending> lista;
        for (Estudante e: estudantes) {
            for (Turma t: e.getSchedule()) {
                if (t.getcodigoTurma()[0] == (char)(ano + '0')) {
                    lista.insert(e);
                    break;
                }
            }
        }
        for (Estudante e : lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    if (order == 2) {
        set<Estudante, EstudanteAlphaDescending> lista;
        for (Estudante e: estudantes) {
            for (Turma t: e.getSchedule()) {
                if (t.getcodigoTurma()[0] == (char)(ano + '0')) {
                    lista.insert(e);
                    break;
                }
            }
        }
        for (Estudante e : lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    if (order == 3) {
        set<Estudante, EstudanteNumAscending> lista;
        for (Estudante e: estudantes) {
            for (Turma t: e.getSchedule()) {
                if (t.getcodigoTurma()[0] == (char)(ano + '0')) {
                    lista.insert(e);
                    break;
                }
            }
        }
        for (Estudante e : lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    if (order == 4) {
        set<Estudante, EstudanteNumDescending> lista;
        for (Estudante e: estudantes) {
            for (Turma t: e.getSchedule()) {
                if (t.getcodigoTurma()[0] == (char)(ano + '0')) {
                    lista.insert(e);
                    break;
                }
            }
        }
        for (Estudante e : lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
}
bool Gestor::outputListaTurmas(string codigoUC){
    if(find_if(turmas.begin(), turmas.end(), [codigoUC](const Turma& t) {return t.getcodigoUC() == codigoUC;}) == turmas.end())
        return false;
    for (Turma t : turmas) {
        if(t.getcodigoUC() == codigoUC)
            cout << t.getcodigoTurma() << "\n";
    }
    return true;
}
void Gestor::outputListaUC(int ano){

    for (Turma t : turmas) {
        if (t.getcodigoTurma()[0] == (char)(ano + '0') && t.getcodigoTurma()[5] == '0' && t.getcodigoTurma()[6] == '1')
            cout << t.getcodigoUC() << "\n";
    }
}

void Gestor::outputListaEstudanteNUC(int n, int order){
    if (order == 1) {
        set<Estudante, EstudanteAlphaAscending> lista;
        for(Estudante e : estudantes) {
            if (e.getSchedule().size() == n)
                lista.insert(e);
        }
        for (Estudante e : lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    if (order == 2) {
        set<Estudante, EstudanteAlphaDescending> lista;
        for(Estudante e : estudantes) {
            if (e.getSchedule().size() == n)
                lista.insert(e);
        }
        for (Estudante e : lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    if (order == 3) {
        set<Estudante, EstudanteNumAscending> lista;
        for (Estudante e: estudantes) {
            if (e.getSchedule().size() == n)
                lista.insert(e);
        }
            for (Estudante e: lista) {
                cout << e.getID() << " " << e.getName() << "\n";
            }
        }
        if (order == 4) {
            set<Estudante, EstudanteNumDescending> lista;
            for (Estudante e: estudantes) {
                if (e.getSchedule().size() == n)
                    lista.insert(e);
            }
                for (Estudante e: lista) {
                    cout << e.getID() << " " << e.getName() << "\n";
                }
        }
}


void Gestor::outputListaEstudanteMaisNUC(int n, int order){
    if (order == 1) {
        set<Estudante, EstudanteAlphaAscending> lista;
        for(Estudante e : estudantes) {
            if (e.getSchedule().size() > n)
                lista.insert(e);
        }
        for (Estudante e : lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    if (order == 2) {
        set<Estudante, EstudanteAlphaDescending> lista;
        for(Estudante e : estudantes) {
            if (e.getSchedule().size() > n)
                lista.insert(e);
        }
        for (Estudante e : lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    if (order == 3) {
        set<Estudante, EstudanteNumAscending> lista;
        for (Estudante e: estudantes) {
            if (e.getSchedule().size() > n)
                lista.insert(e);
        }
        for (Estudante e: lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    if (order == 4) {
        set<Estudante, EstudanteNumDescending> lista;
        for (Estudante e: estudantes) {
            if (e.getSchedule().size() > n)
                lista.insert(e);
        }
        for (Estudante e: lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
}

bool Gestor::outputOcupaçãoTurma(string codigoTurma){
    if(find_if(turmas.begin(), turmas.end(), [codigoTurma](const Turma& t) {return t.getcodigoTurma() == codigoTurma;}) == turmas.end())
        return false;
    map<string, int> occupations;
    for (Turma t : turmas) {
        if (t.getcodigoTurma() == codigoTurma)
            occupations.insert({t.getcodigoUC(), 0});
    }
    for (Estudante e : estudantes) {
        for (Turma t : e.getSchedule()) {
            if (t.getcodigoTurma() == codigoTurma)
                occupations[t.getcodigoUC()] += 1;
        }
    }

    for (auto occupation : occupations) {
        cout << occupation.first << " " << occupation.second << " estudantes" << "\n";
    }
    return true;
}
bool Gestor::outputOcupaçãoUC(string codigoUC){
    if(find_if(turmas.begin(), turmas.end(), [codigoUC](const Turma& t) {return t.getcodigoUC() == codigoUC;}) == turmas.end())
        return false;
    map<string, int> occupations;
    for (Turma t : turmas) {
        if (t.getcodigoUC() == codigoUC)
            occupations.insert({t.getcodigoTurma(), 0});
    }
    for (Estudante e : estudantes) {
        for (Turma t : e.getSchedule()) {
            if (t.getcodigoUC() == codigoUC)
                occupations[t.getcodigoTurma()] += 1;
        }
    }

    for (auto occupation : occupations) {
        cout << occupation.first << " " << occupation.second << " estudantes" << "\n";
    }
    return true;
}
void Gestor::outputOcupaçãoAno(int ano) {
    map<string, int> occupations;
    for (Turma t : turmas) {
        if (t.getcodigoTurma()[0] == (char)(ano + '0'))
            occupations.insert({t.getcodigoUC(), 0});
    }
    for (Estudante e : estudantes) {
        for (Turma t : e.getSchedule()) {
            if (t.getcodigoTurma()[0] == (char)(ano + '0'))
                occupations[t.getcodigoUC()] += 1;
        }
    }

    for (auto occupation : occupations) {
        cout << occupation.first << " " << occupation.second << " estudantes" << "\n";
    }

}


bool Gestor::makePedido(int id, string codigoUC1, string codigoTurma1, string codigoUC2, string codigoTurma2) {
    /*Estudante e = Estudante(id, "");
    set<Estudante>::iterator it = find(estudantes.begin(), estudantes.end(), e);
    e = *it;
    Turma t1, t2;
    for(Turma t : turmas) {
        if (t.getcodigoTurma() == codigoTurma1 && t.getcodigoUC() == codigoUC1) {
            t1 = t;
        } else if (t.getcodigoTurma() == codigoTurma2 && t.getcodigoUC() == codigoUC2) {
            t2 = t;
        }
    }
    Pedido p = Pedido(e, t1, t2, "");*/
    return true;
}
bool Gestor::desfazerÚltimoPedido(){return true;}

void Gestor::sortTurmas() {
    sort(turmas.begin(), turmas.end(), compareTurmas);
}

bool compareTurmas(const Turma &t1, const Turma &t2){
    bool compareUC = t1.getcodigoUC() < t2.getcodigoUC();
    bool compareTurma = t1.getcodigoUC() == t2.getcodigoUC() && t1.getcodigoTurma() < t2.getcodigoTurma();
    return  compareUC || compareTurma;
}

// Testing Functions for the extract
void Gestor::outputAllTurmas() {
    for (Turma t : turmas){
        cout << t.getcodigoUC() << ' ' << t.getcodigoTurma() << '\n';
    }
}

void Gestor::outputAllAulas() {
    for (Turma t : turmas){
        cout << t.getcodigoUC() << ' ' << t.getcodigoTurma() << '\t';
            for (Aula a : t.getAulas())
                cout << '\t' << a.getDia() << ' ' << a.getDuracao() << "     \t|\t";
        cout << '\n';
    }
}

void Gestor::outputAllEstudantes(int order) {

    for (Estudante e : estudantes) {
        cout << e.getName() << ", " << e.getID() << endl;
        for (Turma t : e.getSchedule()) {
            cout << "\t" << t.getcodigoUC() << ", " << t.getcodigoTurma() << endl;
            for (Aula a : t.getAulas()) {
                cout << "        " << a.getDia() << ", " << a.getHoraInicio() << ", " << a.getDuracao() << ", " << a.getTipo() << endl;
            }
        }
    }

}

void Gestor::saveChanges(string fname) {
    cout << "Alterações guardadas!\n";
}
