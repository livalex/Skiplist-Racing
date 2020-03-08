// Copyright[2019] <Livadaru Alexandru-Valentin 313CA>

#include<iostream>
#include<fstream>
#include<cstring>
#include<cstdlib>
#include<string>
#include<algorithm>
#include<limits>
#include "./player.h"

// Structura nodului meu formata din
// Pointeri pe directiile stanga,dreapta,sus si jos.
// De asemenea voi stoca informatia(scorul din problema)
// Si ID-ul concurentilor din problema.
template <typename T>
struct Skiplist_Node {
    T information;
    T ID;
    struct Skiplist_Node<T> *next;
    struct Skiplist_Node<T> *prev;
    struct Skiplist_Node<T> *up;
    struct Skiplist_Node<T> *down;

    // Constructor folosit la operatia de insert
    // La creare,acesta nu pointeaza nicaieri,avand
    // O anumita informatie si un ID dat.
    Skiplist_Node(T ID, T information) {
        next = NULL;
        prev = NULL;
        up = NULL;
        down = NULL;
        this->ID = ID;
        this->information = information;
    }

    // Constructor folosit la celelalte opeartii din skiplist.
    Skiplist_Node(){
        next = NULL;
        prev = NULL;
        up = NULL;
        down = NULL;
    }

    // Destructor gol deorece nu avem memorie de eliberat.
    ~Skiplist_Node() {}
};

// Implementarea skiplistului
template <class T>
class Skiplist {
 private:
    // Inaltime skiplist precum si pointeri la inceput
    // Si sfarsit. De asemenea constanta ma va ajuta
    // La functia flipcoin,mai multe detalii acolo.
    int block_height;
    struct Skiplist_Node<T> *head, *tail;
    const float probability = 0.5;

 public:
    // Constructorul skiplistului care formeaza capatul
    // Si sfarsitul skiplistului. Valorile luate sunt
    // Menite sa nu fie atinse niciodata,imitand infinit
    // Si minus infinit.
    Skiplist() {
        block_height = 0;
        head = new struct Skiplist_Node<T>;
        tail = new struct Skiplist_Node<T>;
        head->ID = -9999;
        tail->ID = 9999;
        head->information = -1000;
        tail->information = 1000;
        head->next = tail;
        tail->prev = head;
    }

    // Destructor skiplist. Elibereaza toata memoria alocata
    // Casetelor din skiplist. Sterge de la stanga la dreapta
    // Liniile,apoi se duce in jos si continua procesul.
    ~Skiplist() {
        struct Skiplist_Node<T> *vertical = head;
        struct Skiplist_Node<T> *aux;
        while (vertical != NULL) {
            vertical = vertical->down;
            while (head != NULL) {
                aux = head;
                head = head->next;
                delete aux;
            }
            head = vertical;
        }
        delete vertical;
    }

    // Functie care ma ajuta la atribuirea de nivele aletorii
    // Elementelor introduse in skiplist. Mimeaza capul si pajura
    // Aruncarii undei monede.
    int flipcoin() {
        int lvls = 0;
        float randomize = (float)rand() / RAND_MAX;
        while (randomize < probability) {
            lvls = lvls + 1;
            randomize = (float)rand() / RAND_MAX;
        }
        return lvls;
    }

    // Am creat o copie a head-ului si a tail-ului pentru
    // A ne plimba in skiplist. Ne plimbam spre dreapta in
    // Cautarea ID-ului pe care il vrem si daca ajungem la capat
    // Sau valoarea urmatoare e mai mai mare decat ce cautam coboram
    // Un nivel cu copiile si tot asa. La sfarsit vom returna un pointer
    // La elementul cautat de pe ultimul nivel.
    struct Skiplist_Node<T>* search(T ID) {
        struct Skiplist_Node<T> *current = head;
        struct Skiplist_Node<T> *aux_tail = tail;
            while (current->down != NULL) {
                while (current != aux_tail && current->next->ID <= ID) {
                    current = current->next;
                }
                current = current->down;
                aux_tail = aux_tail->down;
            }
            while (current != aux_tail && current->next->ID <= ID) {
                    current = current->next;
                }
            return current;
    }

    // Un search auxiliar care ma pozitioneaza la baza
    // oloanei elementului cautat si returneaza informatia
    // Cautata de la acel nod.
    int search2(T ID) {
        struct Skiplist_Node<T> *aux;
        aux = search(ID);
        return aux->information;
    }

    // Inca un search care face acelasi lucru ca cel de mai sus
    // Dar in loc de informatie returneaza ID-ul celulei cautate.
    int search3(T ID) {
        struct Skiplist_Node<T> *aux;
        aux = search(ID);
        return aux->ID;
    }

    // Functie care imi va mai adauga un strat de "infinituri"
    // In caz ca inaltimea coloanei unui element nou introdus
    // Va depasi inaltimea de pana atunci a skiplistului.
    void add_layer() {
        struct Skiplist_Node<T> *new_head, *new_tail;
        new_head = new struct Skiplist_Node<T>;
        new_head->ID = -9999;
        new_head->information = -1000;
        new_tail = new struct Skiplist_Node<T>;
        new_tail->ID = 9999;
        new_tail->information = 1000;
        new_head->next = new_tail;
        new_head->down = head;
        new_tail->prev = new_head;
        new_tail->down = tail;
        head->up = new_head;
        tail->up = new_tail;
        head = new_head;
        tail = new_tail;
        ++block_height;
    }

    // Ne luam un pointer care pointeaza la casuta cu valoarea
    // Cea mai aproiata de valoarea cautata. Introducem elementul nou
    // Pe ultimul nivel si-i facem legaturile. Apoi in functie de rezultatul
    // Functiei flipcoin mai cream nivele sau nu,
    // Nivelel create fiind si ele legate
    // La randul lor. O sa incerc sa detaliez in README.
    void insert(T ID, T information) {
        struct Skiplist_Node<T> *aux, *new_element;
        aux = search(ID);
        new_element = new struct Skiplist_Node<T>(ID, information);
        new_element->prev = aux;
        new_element->next = aux->next;
        aux->next->prev = new_element;
        aux->next = new_element;
        int lvls = 0;
        while (flipcoin() % 2 == 0) {
            if (block_height <= lvls) {
                add_layer();
            }
            while (aux->up == NULL && aux->prev != NULL) {
                aux = aux->prev;
            }
            aux = aux->up;
            struct Skiplist_Node<T> *add_Node_level;
            add_Node_level = new struct Skiplist_Node<T>(ID, information);

            add_Node_level->next = aux->next;
            add_Node_level->down = new_element;
            add_Node_level->prev = aux;
            aux->next->prev = add_Node_level;
            new_element->up = add_Node_level;
            aux->next = add_Node_level;
            new_element = add_Node_level;
            ++lvls;
        }
    }

    // Vom sterge nodul pozitionandu-ne pe elemntul cautat.
    // Ii vom "rupe" legaturile si in cele din urma vom sterge
    // Tot ce stocam in el.
    void delete_node(T ID) {
        struct Skiplist_Node<T> *element;
        element = search(ID);
        while (element != NULL) {
            struct Skiplist_Node<T> *aux = element;
            element = element->up;
            aux->prev->next = aux->next;
            aux->next->prev = aux->prev;
            delete aux;
        }
    }

    // Functie care nu are nicio aplicatie practica in forma finala a problemei
    // Dar m-a ajutat foarte mult la implementare,asa ca o voi lasa aici. Sper
    // Ca nu deranjeaza cu nimic.
    void print() {
        std::cout << "*SKIPLIST*\n";
        int i = block_height;
        struct Skiplist_Node<T> *vertical = head;
        while (vertical != NULL) {
            struct Skiplist_Node<T> *aux = vertical;
            std::cout << "Level " << i << ": ";
            vertical = vertical->down;
            while (aux != NULL) {
                std::cout << "/" << aux->ID << "/" << aux->information << " ";
                aux = aux->next;
            }
            std::cout << "\n";
            --i;
        }
    }
};

// Functie care interschimba doua valori de la adresele date ca parametri.
void swap_order(int& a, int& b) {
    int aux;
    aux = a;
    a = b;
    b = aux;
}

// Functie in care are efectiv loc problema.
void title_race() {
    // Deshidem fisiserul de input.
    std::ifstream fin("races.in");
    if (!fin.is_open()) {
        std::cout << "Input file could not be opened." << std::endl;
    }

    // Deschidem fisierul de output.
    std::ofstream fout("races.out");
    if (!fout.is_open()) {
        std::cout << "Output file could not be opened." << std::endl;
    }

    std::string::size_type size;  // allias of size_t
    int n, m, k, p;
    int t, points;
    std::string reading;
    fin >> n >> m >> k;
    Player *players = new Player[n];
    Details *details = new Details[n];
    Skiplist<int> skiplist;
    int *times = new int[n];
    int *order = new int[n];
    int *IDorder = new int[n];
    int *prevPrint = new int[n];
    // Va initializa ID-urile la i + 1 si ultima pozitie din
    // Clasament la 0. De asemenea va initializa ultima pozitie
    // De la print la 0.
    for (int i = 0; i < n; ++i) {
        details[i].setLastPos(0);
        details[i].setID(i + 1);
        prevPrint[i] = 0;
    }
    // Bagam in skiplist concurentii cu punctajele lor.
    for (int i = 0; i < n; ++i) {
        skiplist.insert(i+1, players[i].getScore());
    }
    int print = 0;
    // Cat timp numarul de curse sau de printuri e mai mare decat 0.
    while (m > 0 || k > 0) {
        // daca stringul e egal cu print afisam clasamentul.
        fin >> reading;
        if (reading == "print") {
            print++;
            // Punem informatia si ID-urile in vectori pentru a fi
            // Sortati ulterior.
            for (int i = 0; i < n; ++i) {
                order[i] = skiplist.search2(i + 1);
                IDorder[i] = skiplist.search3(i + 1);
            }
            // Realizeaza sortarea vectorilor de ID si informatii.
            for (int i = 0; i < n - 1; ++i) {
                for (int j = i + 1; j < n; ++j) {
                    if (order[i] < order[j]) {
                        swap_order(order[i], order[j]);
                        swap_order(IDorder[i], IDorder[j]);
                    }
                    if (order[i] == order[j]) {
                        if (IDorder[i] > IDorder[j]) {
                            swap_order(IDorder[i], IDorder[j]);
                        }
                    }
                }
            }

            // Afiseaza id-ul punctajele si diferenta de nivel.
            for (int i = 0; i < n; ++i) {
                if (print == 1) {
                fout << IDorder[i] << " " << order[i] << " 0" << "\n";
                } else {
                fout << IDorder[i] << " " << order[i]<< " "<<
                (prevPrint[IDorder[i] - 1]- i)<< "\n";
                }
                 prevPrint[IDorder[i] - 1] = i;
            }
            fout << "\n";
            --k;
        } else {
            p = 0;
            // Convertim string-ul la int-uri.
            times[p] = std::stoi(reading, &size);
            // Seteaza timpul si ID-ul curent in details.
            details[p].setTimer(times[p]);
            details[p].setID(p + 1);
            ++p;
            // Acelasi lucru pana atingem numarul de participanti.
            while (p < n) {
                fin >> t;
                times[p] = t;
                details[p].setTimer(times[p]);
                details[p].setID(p + 1);
                ++p;
            }

            // Sortam vectorul details crescator dupa timp.
            std::sort(details, details+n, sortByTimer);
            --m;
            // Numaram concurentii cu timpul diferit de 0.
            int racers = 0;
            for (int i = 0; i < n; ++i) {
                if (details[i].getTimer() != 0) {
                    ++racers;
                }
            }
            points = racers / 2;
            for (int i = 0; i < n; ++i) {
                if (details[i].getTimer() == 0) {
                    // Adaug in players punctajele precedente ale celor
                    // Care nu participa la cursa curenta.
                    players[i].setID(details[i].getID());
                    int oldscore = skiplist.search2(players[i].getID());
                    players[i].setScore(oldscore);
		            continue;
                }
                // Daca nr de raceri e par,nu primeste nimeni 0
                if (points == 0 && racers % 2 == 0) {
                    --points;
                }
                // Caut punctajul precedent in skiplist si-l adun
                // Cu cel din cursa curenta.
                players[i].setID(details[i].getID());
                int oldscore = skiplist.search2(players[i].getID());
                skiplist.delete_node(players[i].getID());
                int newscore = points + oldscore;
                players[i].setScore(newscore);
                skiplist.insert(players[i].getID(), newscore);
                --points;
            }
            // Sortez punctajele pentru afla pozitia curenta in clasament.
		    std::sort(players, players+n, sortByPoints);
		    for (int i = 0; i < n; ++i) {
                // Pozitia pe care se afla in clasament la cursa curenta.
                int k = players[i].getID();
			    details[k-1].setLastPos(i + 1);
		    }
        }
    }
    // Eliberari de memorie.
    delete[] players;
    delete[] details;
    delete[] times;
    delete[] order;
    delete[] IDorder;
    delete[] prevPrint;
}


int main() {
	srand((unsigned)time(0));
    title_race();
    return 0;
}
