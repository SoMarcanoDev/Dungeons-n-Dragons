#include <iostream>
#include <string>
#include <fstream>

using namespace std;

const int maxHeroes = 100;
const int maxEnemies = 1000;

class Coordinate {
    public:
    int X;
    int Y;

    Coordinate () {
        X = -1;
        Y = -1;
    }

    void deleteCoordinate () {
        X = -1;
        Y = -1;
    }
};

class Character {
    public:
    int maxHealth;
    int healthP;
    int attackP;
    int armorP;
    int magicRP;
    bool heroe = true;
    bool magicAttack = false;
    bool alive = false;
    Coordinate coor;
    Coordinate attackCoor[16];
    char simbol;
    
    
    Character () {
        healthP = 0;
        attackP = 0;
        armorP = 0;
        magicRP  = 0;
        coor.deleteCoordinate();
    }

    void deleteCharacter () {
        alive = false;
        coor.deleteCoordinate();
    }

    void cleanAttackCoor () {
        for (int i = 0; i < 16; i++) {
            attackCoor[i].deleteCoordinate();
        }
    }

    void lowerHealth (int damage, bool magic) {
        // se verifica el tipo de dgno recibido
        if (magic) {
            damage -= magicRP;
        } else {
            damage -= armorP;
        }

        // se verifica si el personaje debe recibir dagno
        if (damage > 0) {
            healthP -= damage;
        }
        
    }

    void heal (int heal) {
        if (healthP + heal <= maxHealth) {
            healthP += heal;
        } else {
            healthP = maxHealth;
        }
    }
    
};

class Warrior: public Character {
    public:
    Warrior () {
        simbol = 'G';
    }

};

class Cleric: public Character {
    public: 
    Cleric () {
        simbol = 'K';
        magicAttack = true;
    }
};

class Mage: public Character {
    public:
    Mage () {
        simbol = 'M';
        magicAttack = true;
    }
};

class Archer: public Character {
    public:
    Archer () {
        simbol = 'A';
    }
};

class Enemy: public Character {
    public:
    Enemy () {
        simbol = 'E';
        heroe = false;
    }
};

Warrior G[maxHeroes];
Cleric K[maxHeroes];
Mage M[maxHeroes];
Archer A[maxHeroes];
Enemy E[maxEnemies];





class Board {
    public:
    int row, col;
    char board[500][500];
    int numWarriors = 0;
    int numClerics = 0;
    int numMages = 0;
    int numArchers = 0;
    int numEnemies = 0;


    Board () {
        // se inicializa el tablero
        for (int i = 0; i < 500; i++) {
            for (int j = 0; j < 500; j++) {
                board[i][j] = ' ';
            }
        }
    }

    void readBoard (char aux, int coorY, int coorX) {
        // se cuenta la cantidad de personajes que existen
        // y se les asignan sus coordenadas iniciales
        switch (aux) {
            case 'G':
                G[numWarriors].coor.Y = coorY;
                G[numWarriors].coor.X = coorX;
                numWarriors++;
            break;
            case 'K':
                K[numClerics].coor.Y = coorY;
                K[numClerics].coor.X = coorX;
                numClerics++;
            break;
            case 'M':
                M[numMages].coor.Y = coorY;
                M[numMages].coor.X = coorX;
                numMages++;
            break;
            case 'A':
                A[numArchers].coor.Y = coorY;
                A[numArchers].coor.X = coorX;
                numArchers++;
            break;
            case 'E':
                E[numEnemies].coor.Y = coorY;
                E[numEnemies].coor.X = coorX;
                numEnemies++;
            break;
            default:
            break;
        }
    }

    void fillBoard (fstream &input) {
        string aux;

        // se rellena el tablero
        for (int i = 0; i < row; i++) {

            getline(input, aux);
            for (int j = 0; j < col; j++) {
                board[i][j] = aux[j];

                readBoard(aux[j], i, j);
            }
        }
    }

    void moveCharacter (int direction, int distance, Character &chara) {
        
        board[chara.coor.Y][chara.coor.X] = '_';

        // se modifican las cordenadas del personaje
        // y se verifica que no se encuentre con un muro u otro personaje
        switch (direction) {
            case 1:
                // arriba
                for (int i = 0; i < distance; i++) {
                    if (board[chara.coor.Y - 1][chara.coor.X] == '_') {
                        chara.coor.Y--;
                    } else {
                        // en caso de encontrarse con un muro u otro personaje se detiene
                        break;
                    }
                }
            break;
            case 2:
                // derecha
                for (int i = 0; i < distance; i++) {
                    if (board[chara.coor.Y][chara.coor.X + 1] == '_') {
                        chara.coor.X++;
                    } else {
                        // en caso de encontrarse con un muro u otro personaje se detiene
                        break;
                    }
                }
            break;
            case 3:
                // abajo
                for (int i = 0; i < distance; i++) {
                    if (board[chara.coor.Y + 1][chara.coor.X] == '_') {
                        chara.coor.Y++;
                    } else {
                        // en caso de encontrarse con un muro u otro personaje se detiene
                        break;
                    }
                }
            break;
            case 4:
                // izquierda
                for (int i = 0; i < distance; i++) {
                    if (board[chara.coor.Y][chara.coor.X - 1] == '_') {
                        chara.coor.X--;
                    } else {
                        // en caso de encontrarse con un muro u otro personaje se detiene
                        break;
                    }
                }
            break;
        }
            
        // se ubica al personaje en su nueva posicion
        board[chara.coor.Y][chara.coor.X] = chara.simbol;
    }

    int findCharacter (Character array[], int Y, int X, int num) {
        // ubica un personaje especifico dentor de su arreglo de tipo de personaje
        for (int i = 0; i < num; i++) {
            if (array[i].coor.Y == Y && array[i].coor.X == X) {
                return i;
            }
        }
        return -1;
    }

    void checkHealth (Character &chara) {
        if (chara.healthP <= 0) {
            board[chara.coor.Y][chara.coor.X] = '_';
            chara.deleteCharacter();
        }
    }

    void checkDisparoLongevo (Character array[], Character attacker, int damage, int y, int x, int num) {
        int aux;
        aux = findCharacter (array, y, x, num);

        array[aux].lowerHealth(damage, attacker.magicAttack);
        checkHealth (array[aux]);
    }

    void checkAttack (Character attacker, int damage, bool torbellino) {
        int aux;

        // se calcula el total de dagno que hace el personaje

        if (torbellino) {
            int numCharacter = 0;
            if (attacker.heroe) {
                // si el atacante es un heroe se cuentan los enemigos a los que hace dagno
                for (int i = 0; i < 16; i++) {
                    if (board[attacker.attackCoor[i].Y][attacker.attackCoor[i].X] == 'E') {
                        numCharacter++;
                    }
                }
            } else {
                // si el atacante es un enemigo se cuentan los heroes a los que hace dagno
                for (int i = 0; i < 16; i++) {
                    switch (board[attacker.attackCoor[i].Y][attacker.attackCoor[i].X]) { 
                        case 'G':
                            numCharacter++;
                        break;
                        case 'K':
                            numCharacter++;
                        break;
                        case 'M':
                            numCharacter++;
                        break;
                        case 'A':
                            numCharacter++;
                        break;
                    }
                }
            }
            damage *= numCharacter;
        }

        // se verifica si el atacante es un heroe o enemigo para evitar fuego amigo
        if (attacker.heroe) {
            // se verifica si existe un enemigo en el rango del ataque
            for (int i = 0; i < 16; i++) {
                if (board[attacker.attackCoor[i].Y][attacker.attackCoor[i].X] == 'E') {
                    aux = findCharacter (E, attacker.attackCoor[i].Y, attacker.attackCoor[i].X, numEnemies);

                    E[aux].lowerHealth(damage, attacker.magicAttack);

                    checkHealth (E[aux]);
                }
            }
        } else {
            // se verifica si existe un heroe en el rango del ataque
            for (int i = 0; i < 8; i++) {
                switch (board[attacker.attackCoor[i].Y][attacker.attackCoor[i].X]) {
                    case 'G':
                        aux = findCharacter (G, attacker.attackCoor[i].Y, attacker.attackCoor[i].X, numWarriors);

                        G[aux].lowerHealth(damage, attacker.magicAttack);

                        checkHealth (G[aux]);
                    break;
                    case 'K':
                        aux = findCharacter (K, attacker.attackCoor[i].Y, attacker.attackCoor[i].X, numClerics);

                        K[aux].lowerHealth(damage, attacker.magicAttack);

                        checkHealth (K[aux]);
                    break;
                    case 'M':
                        aux = findCharacter (M, attacker.attackCoor[i].Y, attacker.attackCoor[i].X, numMages);

                        M[aux].lowerHealth(damage, attacker.magicAttack);

                        checkHealth (M[aux]);
                    break;
                    case 'A':
                        aux = findCharacter (A, attacker.attackCoor[i].Y, attacker.attackCoor[i].X, numArchers);

                        A[aux].lowerHealth(damage, attacker.magicAttack);

                        checkHealth (A[aux]);
                    break; 
                    default:
                    break;   
                }
            }
        }
        attacker.cleanAttackCoor ();
    }

    void checkHeal (int heal, bool nova, Character chara) {
        int aux;
        if (nova) {
            int numCharacter = 0;
            for (int i = 0; i < 16; i++) {
                switch (board[chara.attackCoor[i].Y][chara.attackCoor[i].X]) { 
                    case 'G':
                        numCharacter++;
                    break;
                    case 'K':
                        numCharacter++;
                    break;
                    case 'M':
                        numCharacter++;
                    break;
                    case 'A':
                        numCharacter++;
                    break;
                }
            }
            heal += numCharacter;
        } else {
            heal += chara.attackP;
        }

        for (int j = 0; j < 16; j++) {
            switch (board[chara.attackCoor[j].Y][chara.attackCoor[j].X]) {
                case 'G':
                    aux = findCharacter (G, chara.attackCoor[j].Y, chara.attackCoor[j].X, numWarriors);
                    
                    G[aux].heal(heal); 
                break;
                case 'K':
                    aux = findCharacter (K, chara.attackCoor[j].Y, chara.attackCoor[j].X, numClerics);

                    K[aux].heal(heal); 
                break;
                case 'M':
                    aux = findCharacter (M, chara.attackCoor[j].Y, chara.attackCoor[j].X, numMages);

                    M[aux].heal(heal);
                break;
                case 'A':
                    aux = findCharacter (A, chara.attackCoor[j].Y, chara.attackCoor[j].X, numArchers);

                    A[aux].heal(heal);
                break; 
                default:
                break;   
            }
        }
    }

};

class Attack {
    int range;
    int damage;
    int heal;

    void quebrajar(int direction, Character chara, Board &board) {
        range = 3;
        damage = 2;

        switch (direction) {
            case 1:
                // arriba
                for (int i = 0; i < range; i++) {
                    chara.attackCoor[i].Y = chara.coor.Y - 1;
                    chara.attackCoor[i].X = chara.coor.X + i - 1;
                }
            break;
            case 2: 
                // derecha
                for (int i = 0; i < range; i++) {
                    chara.attackCoor[i].Y = chara.coor.Y + i - 1;
                    chara.attackCoor[i].X = chara.coor.X + 1;
                }
            break;
            case 3:
                // abajo
                for (int i = 0; i < range; i++) {
                    chara.attackCoor[i].Y = chara.coor.Y + 1;
                    chara.attackCoor[i].X = chara.coor.X + i - 1;
                }
            break;
            case 4:
                // izquierda
                for (int i = 0; i < range; i++) {
                    chara.attackCoor[i].Y = chara.coor.Y + i - 1;
                    chara.attackCoor[i].X = chara.coor.X - 1;
                }
            break;
        }

        damage += chara.attackP;

        board.checkAttack (chara, damage, false);
    }

    void estocada (int direction, Character chara, Board &board) {
        range = 2;
        damage = 4;
        switch (direction) {
            case 1:
                for (int i = 0; i < range; i++) {
                    chara.attackCoor[i].Y = chara.coor.Y - i - 1;
                    chara.attackCoor[i].X = chara.coor.X;
                }
            break;

            case 2:
                for (int i = 0; i < range; i++) {
                    chara.attackCoor[i].Y = chara.coor.Y;
                    chara.attackCoor[i].X = chara.coor.X + i + 1;
                }
            break;

            case 3: 
                for (int i = 0; i < range; i++) {
                    chara.attackCoor[i].Y = chara.coor.Y + i + 1;
                    chara.attackCoor[i].X = chara.coor.X;
                }
            break;

            case 4: 
                for (int i = 0; i < range; i++) {
                    chara.attackCoor[i].Y = chara.coor.Y;
                    chara.attackCoor[i].X = chara.coor.X - i - 1;
                }
            break;
        }

        damage += chara.attackP;

        board.checkAttack (chara, damage, false);
    }

    void torbellino (int direction, Character chara, Board &board) {
        range = 4;
        damage = 1;

        chara.attackCoor[0].Y = chara.coor.Y;
        chara.attackCoor[0].X = chara.coor.X + 1;

        chara.attackCoor[1].Y = chara.coor.Y;
        chara.attackCoor[1].X = chara.coor.X - 1;

        chara.attackCoor[2].Y = chara.coor.Y - 1;
        chara.attackCoor[2].X = chara.coor.X;

        chara.attackCoor[3].Y = chara.coor.Y + 1;
        chara.attackCoor[3].X = chara.coor.X;

        damage += chara.attackP;

        board.checkAttack (chara, damage, true);
    }

    void novaDeLuz (int direction, Character chara, Board &board) {
        range = 9;
        damage = 2;
        heal = 3;

        for (int i = 0, g = 0; i < range / 3; i++) {
            for (int j = 0; j < range / 3; j++, g++) {
                chara.attackCoor[g].Y = chara.coor.Y + i - 1;
                chara.attackCoor[g].X = chara.coor.X + j - 1 ;
            }
        }

        board.checkHeal (heal, true, chara);

        damage;

        board.checkAttack (chara, damage, false);
    }

    void impactoSagrado (int direction, Character chara, Board &board) {
        // es apuntada
        range = 1;
        damage = 2;
        heal = 4;
        
        board.checkHeal (heal, false, chara);

        damage += chara.attackP;

        board.checkAttack (chara, damage, false);
    }

    void luzSagrada (int direction, Character chara, Board &board) {
        // es apuntada
        range = 1;
        damage = 0;
        heal = 8;

        board.checkHeal (heal, false, chara);

        damage *= chara.attackP;

        board.checkAttack (chara, damage, false);
    }

    void teletransportacion (int direction, Character &chara, Board &board) {
        int distance = 2;
        board.board[chara.coor.Y][chara.coor.X] = '_';

        // se modifican las cordenadas del personaje
        switch (direction) {
            case 1:
                // arriba
                chara.coor.Y -= distance;
            break;
            case 2:
                // derecha
                chara.coor.X += distance; 
            break;
            case 3:
                // abajo
                chara.coor.Y += distance;
            break;
            case 4:
                // izquierda
                chara.coor.X -= distance;
            break;
        }


        // se evita que el personaje salga del tablero
        if (chara.coor.Y >= board.row) {
            chara.coor.Y = board.row - 1;
        }
        if (chara.coor.Y < 0) {
            chara.coor.Y = 0;
        }
        if (chara.coor.X >= board.col) {
            chara.coor.X = board.col - 1;
        }
        if (chara.coor.X < 0) {
            chara.coor.X = 0;
        }

        // se verifica que el personaje no coincida con otro en la misma casilla
        while (board.board[chara.coor.Y][chara.coor.X] != '_') {            
            switch (direction) {
                case 1:
                    // arriba
                    chara.coor.Y++;
                break;
                case 2:
                    // derecha
                    chara.coor.X--; 
                break;
                case 3:
                    // abajo
                    chara.coor.Y--;
                    break;
                 case 4:
                    // izquierda
                    chara.coor.X++;
                break;
            }
        }
            
        // se ubica al personaje en su nueva posicion
        board.board[chara.coor.Y][chara.coor.X] = chara.simbol;
    }

    void esferaDeHielo (int direction, Character chara, Board &board) {
        // es apuntada
        range = 1;
        damage = 3;
        
        damage *= chara.attackP;

        board.checkAttack (chara, damage, false);
    }

    void tempestad (int direction, Character chara, Board &board) {
        // es apuntada
        range = 9;
        damage = chara.attackP;

        for (int i = 0, g = 1; i < range / 3; i++) {
            for (int j = 0; j < range / 3; j++, g++) {
                chara.attackCoor[g].Y = chara.attackCoor[0].Y + i - 1;
                chara.attackCoor[g].X = chara.attackCoor[0].X + j - 1 ;
            }
        }

        board.checkAttack (chara, damage, false);
    }

    void multiDisparo (int direction, Character chara, Board &board) {
        range = 5;
        damage = chara.attackP;

        for (int i = 0, g = 0; i < range; i++) {
            for (int j = 0; j < range; j++) {
                if ((i == 0 || i == 4) && (j == 1 || j == 3)) {
                    continue;
                } else if ((i == 1 || i == 3) && (j == 0 || j == 4)) {
                    continue;
                } else if (i == 2 && j == 2) {
                    continue;
                }

                chara.attackCoor[g].Y = chara.coor.Y + i - 2;
                chara.attackCoor[g].X = chara.coor.X + j - 2;
                g++;
            }
        }
        
        board.checkAttack (chara, damage, false);
    }

    void disparoLongevo (int direction, Character chara, Board &board) {
        int aux;
        damage = 4;

        damage *= chara.attackP;

       
        switch (direction) {
            case 1:
                for (int i = chara.coor.Y - 1; i >= 0; i--) {
                    if (chara.heroe) {
                        if (board.board[i][chara.coor.X] == 'E') {
                                board.checkDisparoLongevo (E, chara, damage, i, chara.coor.X, board.numEnemies);  

                                break;
                            }
                    } else {
                        if (board.board[i][chara.coor.X] == 'G') {
                            board.checkDisparoLongevo (G, chara, damage, i, chara.coor.X, board.numWarriors);  

                            break;
                        }
                        if (board.board[i][chara.coor.X] == 'K') {
                            board.checkDisparoLongevo (K, chara, damage, i, chara.coor.X, board.numClerics);  

                            break;
                        }
                        if (board.board[i][chara.coor.X] == 'M') {
                            board.checkDisparoLongevo (M, chara, damage, i, chara.coor.X, board.numMages);  

                            break;
                        }
                        if (board.board[i][chara.coor.X] == 'A') {
                            board.checkDisparoLongevo (A, chara, damage, i, chara.coor.X, board.numArchers);  

                            break;
                        }
                    }
                }
            break;

            case 2:
                for (int i = chara.coor.X + 1; i < board.col; i++) {
                    if (chara.heroe) {
                        if (board.board[chara.coor.Y][i] == 'E') {
                            board.checkDisparoLongevo (E, chara, damage, chara.coor.Y, i, board.numEnemies);  

                            break;
                        }
                    } else {
                        if (board.board[chara.coor.Y][i] == 'G') {
                            board.checkDisparoLongevo (G, chara, damage, chara.coor.Y, i, board.numWarriors);  

                            break;
                        }
                        if (board.board[chara.coor.Y][i] == 'K') {
                            board.checkDisparoLongevo (K, chara, damage, chara.coor.Y, i, board.numClerics);  

                            break;
                        }
                        if (board.board[chara.coor.Y][i] == 'M') {
                            board.checkDisparoLongevo (M, chara, damage, chara.coor.Y, i, board.numMages);  

                            break;
                        }
                        if (board.board[chara.coor.Y][i] == 'A') {
                            board.checkDisparoLongevo (A, chara, damage, chara.coor.Y, i, board.numArchers);  

                            break;
                        }
                    }
                }
            break;

            case 3:
                for (int i = chara.coor.Y + 1; i < board.row; i++) {
                    if (chara.heroe) {
                        if (board.board[i][chara.coor.X] == 'E') {
                                board.checkDisparoLongevo (E, chara, damage, i, chara.coor.X, board.numEnemies);  

                                break;
                            }
                    } else {
                        if (board.board[i][chara.coor.X] == 'G') {
                            board.checkDisparoLongevo (G, chara, damage, i, chara.coor.X, board.numWarriors);  

                            break;
                        }
                        if (board.board[i][chara.coor.X] == 'K') {
                            board.checkDisparoLongevo (K, chara, damage, i, chara.coor.X, board.numClerics);  

                            break;
                        }
                        if (board.board[i][chara.coor.X] == 'M') {
                            board.checkDisparoLongevo (M, chara, damage, i, chara.coor.X, board.numMages);  

                            break;
                        }
                        if (board.board[i][chara.coor.X] == 'A') {
                            board.checkDisparoLongevo (A, chara, damage, i, chara.coor.X, board.numArchers);  

                            break;
                        }
                    }
                }
            break;

                case 4:
                    for (int i = chara.coor.X - 1; i >= 0; i--) {
                        if (chara.heroe) {
                            if (board.board[chara.coor.Y][i] == 'E') {
                                board.checkDisparoLongevo (E, chara, damage, chara.coor.Y, i, board.numEnemies);  

                                break;
                            }
                        } else {
                            if (board.board[chara.coor.Y][i] == 'G') {
                                board.checkDisparoLongevo (G, chara, damage, chara.coor.Y, i, board.numWarriors);  

                                break;
                            }
                            if (board.board[chara.coor.Y][i] == 'K') {
                                board.checkDisparoLongevo (K, chara, damage, chara.coor.Y, i, board.numClerics);  

                                break;
                            }
                            if (board.board[chara.coor.Y][i] == 'M') {
                                board.checkDisparoLongevo (M, chara, damage, chara.coor.Y, i, board.numMages);  

                                break;
                            }
                            if (board.board[chara.coor.Y][i] == 'A') {
                                board.checkDisparoLongevo (A, chara, damage, chara.coor.Y, i, board.numArchers);  

                                break;
                            }
                        }
                    }
                break;
            }
        
    }

    void voltereta (int direction, Character &chara, Board &board) {
        range = 3;
        damage = chara.attackP;

        switch (direction) {
            case 1:
                // arriba
                for (int i = 0; i < range; i++) {
                    chara.attackCoor[i].Y = chara.coor.Y - 1;
                    chara.attackCoor[i].X = chara.coor.X + i - 1;
                }

                // se mueve al personaje en la direccion contraria
                board.moveCharacter (3, 1, chara);
            break;
            case 2: 
                // derecha
                for (int i = 0; i < range; i++) {
                    chara.attackCoor[i].Y = chara.coor.Y + i - 1;
                    chara.attackCoor[i].X = chara.coor.X + 1;
                }

                // se mueve al personaje en la direccion contraria
                board.moveCharacter (4, 1, chara);
            break;
            case 3:
                // abajo
                for (int i = 0; i < range; i++) {
                    chara.attackCoor[i].Y = chara.coor.Y + 1;
                    chara.attackCoor[i].X = chara.coor.X + i - 1;
                }

                // se mueve al personaje en la direccion contraria
                board.moveCharacter (1, 1, chara);
            break;
            case 4:
                // izquierda
                for (int i = 0; i < range; i++) {
                    chara.attackCoor[i].Y = chara.coor.Y + i - 1;
                    chara.attackCoor[i].X = chara.coor.X - 1;
                }

                // se mueve al personaje en la direccion contraria
                board.moveCharacter (2, 1, chara);
            break;
        }

        board.checkAttack (chara, damage, false);
    }
    
    public:
    
    void attack (int numAttack, int direction, Character &chara, Board &board) {
        switch (chara.simbol) {
            case 'G':
                switch (numAttack) {
                case 1:
                    quebrajar (direction, chara, board);
                break;
                case 2:
                    estocada (direction, chara, board);
                break;
                case 3:
                    torbellino (direction, chara, board);
                break;
                } 
            break;
            case 'K':
                 switch (numAttack) {
                case 1:
                    novaDeLuz (direction, chara, board);
                break;
                case 2:
                    impactoSagrado (direction, chara, board);
                break;
                case 3:
                    luzSagrada (direction, chara, board);
                break;
                }
            break;
            case 'M':
                switch (numAttack) {
                case 1:
                    teletransportacion (direction, chara, board);
                break;
                case 2:
                    esferaDeHielo (direction, chara, board);
                break;
                case 3:
                    tempestad (direction, chara, board);
                break;
                }
            break;
            case 'A':
               switch (numAttack) {
                case 1:
                    multiDisparo (direction, chara, board);
                break;
                case 2:
                    disparoLongevo (direction, chara, board);
                break;
                case 3:
                    voltereta (direction, chara, board);
                break;
                }
            break;
            case 'E':
                switch (numAttack) {
                case 1:
                    disparoLongevo (direction, chara, board);
                break;
                case 2:
                    teletransportacion (direction, chara, board);
                break;
                case 3:
                    torbellino (direction, chara, board);
                break;
                }
            break;
        }
    }
};






class Input {
    fstream input;
    string aux;
    int action;
    int turn;
    Attack fire;

    void readAttributes (Character array[], int num) {
        for (int i = 0; i < num; i++) {
            input >> array[i].healthP;
            array[i].maxHealth = array[i].healthP;
            input >> array[i].attackP;
            input >> array[i].armorP;
            input >> array[i].magicRP;
            array[i].alive = true;

            // se mueve el cursor a la siguiente linea
            getline(input, aux);
        }
    }

    void readTurns(Character array[], int num, Board &board) {
        int direction, distance, attack;

        // se leen las acciones de los personajes
        for (int i = 0; i < num; i++) {
            // se le el primer entero para las acciones
            input >> action;

            // se define la direccion y distancia del movimiento
            direction = action / 100;
            distance = action / 10 % 10;
            // se define el atacke a realizar
            attack = action % 10;

            if (array[i].alive) {
                // se mueve el persona
                board.moveCharacter (direction, distance, array[i]);


                if ((array[i].simbol == 'K' || array[i].simbol == 'M') && (attack == 2 || attack == 3)) {
                    // ataques apuntados
                    input >> array[i].attackCoor[0].X;
                    input >> array[i].attackCoor[0].Y;
                } 

                // se define el dagno causado por el ataque
                fire.attack(attack, direction, array[i], board);
            }

            // se mueve el cursor a la linea de abajo
            getline (input, aux);
        }
    }

    public:
    void readInput (char* argv[], Board &board) {
        // se abre el archivo de entrada 
        input.open(argv[1], ios::in);


        // se verifica que se haya abierto el archivo correctamente
        if (input.is_open()) {

            // se lee el tamagno del tablero
            input >> board.row;
            input >> board.col;

            // se mueve el cursor a la linea de abajo
            getline(input, aux);
    
            // se crea y llena el tablero 
            board.fillBoard(input);

            // se leen en orden los atributos de los personajes
            readAttributes(G, board.numWarriors);
            readAttributes(K, board.numClerics);
            readAttributes(M, board.numMages);
            readAttributes(A, board.numArchers);
            readAttributes(E, board.numEnemies);

            // se lee la cantidad de turnos 
            input >> turn;
            // se mueve el cursor a la linea de abajo
            getline(input, aux);

            // se leen los turnos
            for (int i = 0; i < turn; i++) {
                // se leen las acciones de los personajes

                readTurns(G, board.numWarriors, board);
                readTurns(K, board.numClerics, board);
                readTurns(M, board.numMages, board);
                readTurns(A, board.numArchers, board);
                readTurns(E, board.numEnemies, board);
            }
        }

        // se cierra el archivo
        input.close();
    }
};

class Output {
    // se declara una variable de archivo para la salida
    fstream output;

    
    void printCharacter (Character array[], int num) {
        
        // se evalua un tipo de personaje a la vez y se imprimen sus atributos en orden
        for (int i = 0; i < num; i++) {
            output << array[i].healthP << " "; 
            output << array[i].attackP << " ";
            output << array[i].armorP << " ";
            output << array[i].magicRP << endl;
        }
       
    }

    public:
    void printOutput (char* argv[], Board board) {
        // se abre el archivo de salida
        output.open(argv[2], ios::out);
        
        // se imprime el estado final del tablero
        for (int i = 0; i < board.row; i++) {
            for (int j = 0; j < board.col; j++) {
                output << board.board[i][j];
            }
            output << endl;
        }
        
        // se imprimen los atributos finales de los personajes
        printCharacter(G, board.numWarriors);
        printCharacter(K, board.numClerics);
        printCharacter(M, board.numMages);
        printCharacter(A, board.numArchers);
        printCharacter(E, board.numEnemies);

        // se cierra el archivo de salida
        output.close();
    }
};

int main (int argc, char* argv[]) {
    Input input;
    Board board;
    Output output;

    // se leen las entradas
    input.readInput(argv, board);

    // se imprimen las salidas
    output.printOutput(argv, board);

    return 0;
}