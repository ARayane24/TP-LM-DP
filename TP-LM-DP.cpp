#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

// les structures de données
typedef struct Litteral {
    char leteral[2];
    Litteral* leteralSuiv;
};
typedef struct Clause {
    Litteral* clause;
    Clause* clauseSuiv;
};




// les fonction d'affichage
void afficheClause(Litteral* l);
void afficheFormule(Clause* c);


// les fonction de verification
bool verifierEntree(int nvar, char* varTab, char* input);
int StrToInt(string messageStr);

// les fonction de création
void insertDebutClause(Litteral*& clause, char* leteral);
void insertDebutFormule(Clause*& formule);
void creationFormule(Clause*& F1, int nvar, char* varTab);
void creationClause(Litteral*& clause, int nvar, char* varTab);
void creationCharTabTousVar(char*& varTab, int nvar);
void deleteCharTabTousVar(char*& varTab);

// les fonction de suppression
void suppressionLitteral(Litteral*& l);
void suppressionClause(Clause*& c);
void suppressionMilieuLitteral(Litteral*& l);
void suppressionMilieuClause(Clause*& c);

// les fonction de minimisation
void minimisationClause(Litteral* l);
void minimisationFormule(Clause*& c);
void minimisationFormuleTotal(Clause*& c);
bool clauseIdentiques(Litteral* a, Litteral* b);

// les fonction du base
int sizeFormule(Clause* l);
int sizeClause(Litteral* l);


int main() {
    Clause* F1 = 0;
    int nvar;
    char* varTab;

    nvar = StrToInt("Donne le nombre de var : ");
    creationCharTabTousVar(varTab, nvar);

    creationFormule(F1, nvar, varTab);
    cout << "La formule clausale est :\n";
    afficheFormule(F1);

    minimisationFormule(F1);
    cout << "La formule clausale est (minimisation):\n";
    afficheFormule(F1);

    minimisationFormuleTotal(F1);
    cout << "La formule clausale est (minimisation total):\n";
    afficheFormule(F1);


    deleteCharTabTousVar(varTab);
    return 0;
}



// les fonction d'affichage
void afficheClause(Litteral* l)
{
    while (l != NULL)
    {
        cout << l->leteral[0] << l->leteral[1];
        l = l->leteralSuiv;
        if (l != 0)
        {
            cout << ", ";
        }
    }
}
void afficheFormule(Clause* c)
{
    if (c == 0)
    {
        cout << "formule est vide" << endl;
    }

    while (c != NULL)
    {
        cout << "( ";
        afficheClause(c->clause);
        cout << " )";

        c = c->clauseSuiv;

        if (c != 0)
        {
            cout << ',';
        }
        else
        {
            cout << endl;
        }
    }
}


// les fonction de verification
bool verifierEntree(int nvar, char* varTab, char* input)
{
    bool existe = 0;



    if (input[0] == '-' || input[0] == '\'' || input[1] == ',')
    {
        for (int i = 0; i < nvar; i++)
        {
            if (varTab[i] == input[1])
            {
                existe = 1;
            }
            if (input[1] == ',')
            {
                if (input[i] == input[0])
                {
                    existe = 1;
                }

            }

        }
    }




    if (existe)
    {
        return true;
    }
    else
    {
        cout << "SVP,entrer une entree valide.." << endl;
        *input = '\0';
        return false;
    }



}
int StrToInt(string messageStr)
{
    //pour verifier que toutes les entrees ne sont que des nombres

    int X;

    bool intOrStr = 0; // 0 -> nomber || 1 -> n'est pas un nomber
    string inputStr;


    do
    {
        cout << messageStr;
        cin >> inputStr;
        X = 0;
        intOrStr = !inputStr.empty() && all_of(inputStr.begin(), inputStr.end(), isdigit);
        if (!intOrStr)
        {
            // affiche error massage
            cout << endl << "SVP,entrer une entree valide" << endl << endl;
        }
    } while (!intOrStr);
    X = stoi(inputStr); // str to int
    return X;
}

// les fonction de création
void insertDebutClause(Litteral*& clause, char* leteral)
{
    Litteral* AIDE = new Litteral;
    AIDE->leteral[0] = leteral[0];
    AIDE->leteral[1] = leteral[1];
    AIDE->leteralSuiv = clause;
    clause = AIDE;
}
void insertDebutFormule(Clause*& formule)
{
    Clause* AIDE = new Clause;
    AIDE->clause = 0;
    AIDE->clauseSuiv = formule;
    formule = AIDE;
}
void creationFormule(Clause*& F1, int nvar, char* varTab)
{
    int N;
    Litteral* AIDE = 0;
    N = StrToInt("donne le nombre de clause : ");

    cout << "\x1b[1m Remarque : \x1b[0m " << endl;
    cout << "\t > Si vous voulez d'entree un variable utilise \" , \" apres leur nom pour l'avoir " << endl;
    cout << "\t > Si vous voulez d'entree la negation d'un variable utilise \" ' \" ou \" - \" avant leur nom pour l'avoir " << endl;


    for (int i = 0; i < N; i++)
    {
        insertDebutFormule(F1);
        creationClause(AIDE, nvar, varTab);
        F1->clause = AIDE;
        AIDE = 0;
    }
}
void creationClause(Litteral*& clause, int nvar, char* varTab)
{
    string aide;
    char lit[4] = { ' ',' ',' ',' ' };
    int N;

    N = StrToInt("\nDonne le nombre de letteral dans cette clause : ");


    for (int i = 0; i < N; i++)
    {
        do
        {
            lit[0] = '\0';
            cout << "\nDonne letteral N° " << i + 1 << " : ";
            cin >> aide;

            lit[0] = aide[0];
            lit[1] = aide[1];
            lit[2] = '\0';


        } while (!verifierEntree(nvar, varTab, lit));

        if (lit[1] == ',')
        {
            lit[1] = lit[0];
            lit[0] = ' ';
        }
        else
        {
            lit[0] = '-';
        }
        lit[2] = '\0';
        insertDebutClause(clause, lit);

    }

    if (N == 0)
    {
        clause = 0;
    }
}
void creationCharTabTousVar(char*& varTab, int nvar)
{
    varTab = new char[nvar];
    string input;
    bool test = 0;// valide -> 1 | n'est pas valide -> 0

    for (int i = 0; i < nvar; i++)
    {
        do
        {
            cout << "Donne la lettres de var N° " << i + 1 << ": ";
            cin >> input;

            if (input[1] != '\0')
            {
                test = 0;
                cout << "SVP,entrer une entree valide\n";
                cout << "une variable a la fois\n" << endl;
            }
            else
            {
                test = 1;
            }

            if (input[0] < 65 || (input[0] > 90 && input[0] < 97) || input[0] > 122)
            {
                test = 0;
                cout << "SVP,entrer une entree valide\n";
                cout << "L'utilisation des chiffres ou des symboles est interdite\n" << endl;
            }

        } while (!test);

        varTab[i] = input[0];
    }
}
void deleteCharTabTousVar(char*& varTab)
{
    delete[] varTab;
    varTab = 0;
}

// les fonction de suppression
void suppressionLitteral(Litteral*& l)
{
    delete l;
    l = 0;
}
void suppressionClause(Clause*& c)
{
    delete c;
    c = 0;
}

void suppressionMilieuLitteral(Litteral*& l)
{
    Litteral* AIDE = l->leteralSuiv;
    l->leteralSuiv = 0;
    delete l;
    l = AIDE;
}
void suppressionMilieuClause(Clause*& c)
{
    Clause* AIDE = c->clauseSuiv;
    c->clauseSuiv = 0;
    delete c;
    c = AIDE;
}

// les fonction de minimisation
void minimisationClause(Litteral* l)
{
    char existence[2] = { ' ',' ' };
    Litteral* AIDE = 0;
    Litteral* AIDE1 = l;
    Litteral* AIDE2 = 0;
    bool autreOccurrence = 1; // 1 -> Il existe un littéral écrit au moins 2 fois.

    if (l != NULL)
    {
        while (AIDE1->leteralSuiv != NULL)
        {


            existence[0] = AIDE1->leteral[0];
            existence[1] = AIDE1->leteral[1];
            AIDE2 = AIDE1;

        agin:                //Pour confirmer que il y a aucun autre occurrence.

            AIDE = AIDE1->leteralSuiv;

            while (AIDE->leteralSuiv != NULL && (existence[0] != AIDE->leteral[0] || existence[1] != AIDE->leteral[1]))
            {
                AIDE = AIDE->leteralSuiv;
            }

            if (existence[0] == AIDE->leteral[0] && existence[1] == AIDE->leteral[1])
            {
                while (AIDE2->leteralSuiv != AIDE)
                {
                    AIDE2 = AIDE2->leteralSuiv;
                }
                if (AIDE->leteralSuiv == NULL)
                {

                    suppressionLitteral(AIDE2->leteralSuiv);
                }
                else {
                    suppressionMilieuLitteral(AIDE2->leteralSuiv);
                    goto agin;
                }
                AIDE = AIDE1;
            }
            if (AIDE1->leteralSuiv != 0)
            {
                AIDE1 = AIDE1->leteralSuiv;
            }
        }

    }
    else
    {
        cout << "clause vide" << endl;
    }
}
void minimisationFormule(Clause*& c)
{
    Clause* AIDE = c;
    Clause* AIDE2 = c;
    bool vide = 0; // 1 -> vide | 0 -> n'est pas vide

    if (c->clauseSuiv != NULL)
    {
        while (AIDE->clause != 0 && AIDE->clauseSuiv != 0)
        {
            minimisationClause(AIDE->clause);
            AIDE = AIDE->clauseSuiv;
        }

        if (AIDE->clause == 0)
        {
            vide = 1;
            cout << "éliminer un vide" << endl;
        }
    }
    else
    {
        if (c->clause == 0)
        {
            suppressionClause(c);
            vide = 0;
        }
        else
        {
            minimisationClause(AIDE->clause);
        }

    }

    if (vide)
    {
        if (AIDE != c)
        {
            while (AIDE2->clauseSuiv != AIDE)
            {
                AIDE2 = AIDE2->clauseSuiv;
            }
            if (AIDE->clauseSuiv == NULL)
            {
                suppressionClause(AIDE2->clauseSuiv);
            }
            else {
                suppressionMilieuClause(AIDE2->clauseSuiv);
            }
        }
        else
        {
            suppressionMilieuClause(c);
        }

        vide = 0;
    }
    else
    {
        return;
    }

    afficheFormule(c);
    minimisationFormule(c);
}

void minimisationFormuleTotal(Clause*& c)
{
    Clause* AIDE = c;
    Clause* AIDE1 = 0;
    Clause* AIDE2 = 0;
    int i = 0, j = 0;

    if (c != NULL)
    {
        minimisationFormule(c);

        if (c->clauseSuiv != NULL)
        {

            while (AIDE->clauseSuiv != 0)
            {
            agin:

                AIDE1 = AIDE->clauseSuiv;
                i = 0;
                cout << "hekk..." << endl;
                cout << AIDE1 << endl;

                while (AIDE1 != NULL)
                {
                    AIDE2 = c;

                    while (AIDE2->clauseSuiv != AIDE1)
                    {
                        AIDE2 = AIDE2->clauseSuiv;
                    }



                    if (clauseIdentiques(AIDE->clause, AIDE1->clause))
                    {
                        cout << "true" << endl;

                        if (AIDE1->clauseSuiv == 0)
                        {
                            cout << "last" << endl;
                            suppressionClause(AIDE2->clauseSuiv);
                            AIDE1 = 0;
                        }
                        else
                        {
                            cout << "not last" << endl;
                            suppressionMilieuClause(AIDE2->clauseSuiv);


                            cout << "donne !!" << endl;
                            afficheFormule(c);
                            goto agin;
                        }

                    }

                    cout << "hekk" << endl;

                    i++;
                    if (AIDE1 != 0)
                    {
                        AIDE1 = AIDE1->clauseSuiv;
                    }

                }
                cout << "i : " << i << endl;
                j++;

                if (AIDE->clauseSuiv != 0)
                {
                    AIDE = AIDE->clauseSuiv;
                }
            }
            cout << "j : " << j << endl;
        }
        else
        {
            return;
        }
    }
    else
    {
        cout << "la fonction est vide !! " << endl;
    }

}

bool clauseIdentiques(Litteral* a, Litteral* b)
{
    bool test = 0; // 0 ->  n'est pas identique
    Litteral* aide = b;

    cout << "start test" << endl;

    if (sizeClause(a) == sizeClause(b))
    {
        while (a != NULL) {
            aide = b;

            while (aide != NULL)
            {

                if (a->leteral[0] == aide->leteral[0] && a->leteral[1] == aide->leteral[1]) {
                    cout << aide->leteral[0] << aide->leteral[1] << endl;
                    test = 1;
                    break;
                }
                else
                {
                    test = 0;
                    aide = aide->leteralSuiv;
                }

            }

            if (!test)
            {
                cout << "false" << endl;
                return false;
            }
            else
            {
                // Si les deux littral de deux clause sont identique passez au suivant.

                a = a->leteralSuiv;
            }
        }
        // les clause 'a' et 'b' sont identiques

        if (a == NULL && test) {
            return true;
        }
    }
    else
    {
        return false;
    }

}

//size clause
int sizeFormule(Clause* c) {
    int cntr = 0;
    Clause* courant = c;
    while (courant != 0) {
        cntr++;
        courant = courant->clauseSuiv;

    }
    return cntr;
}
//size Litteral
int sizeClause(Litteral* l) {
    int cntr = 0;
    Litteral* courant = l;
    while (courant != 0) {
        cntr++;
        courant = courant->leteralSuiv;

    }
    return cntr;
}