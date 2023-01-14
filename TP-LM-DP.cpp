#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

// les structures de données
typedef struct Var {
    char letteral[3];
};
typedef struct Clause {
    Var vr;
    Clause* letteralSuiv;
};
typedef struct Formule {
    Clause* clause;
    Formule* clauseSuiv;
};




// les fonction d'affichage
void afficheClause(Clause* L);
void afficheFormule(Formule* C);


// les fonction de verification
bool verifierEntree(int nvar, Var* varTab, char* input);
int StrToInt(string messageStr);


// les fonction de création
void insertDebutClause(Clause*& clause, Var letteral);
void insertDebutFormule(Formule*& formule);
void insertTabVar(Var*& varTab, int nvar);
void creationFormule(Formule*& F1, int nvar, Var* varTab);
void creationClause(Clause*& clause, int nvar, Var* varTab);
void creationCharTabTousVar(Var*& varTab, int nvar);



// les fonction de suppression
void suppressionLitteral(Clause*& L);
void suppressionClause(Formule*& C);
void suppressionMilieuLitteral(Clause*& L);
void suppressionMilieuClause(Formule*& C);
void deleteCharTabTousVar(Var*& varTab);

// les fonction de minimisation
void minimisationClause(Clause* L);
void minimisationFormule(Formule*& C);
void minimisationFormuleTotal(Formule*& C);
bool clauseIdentiques(Clause* a, Clause* b);


// Les fonctions calcul de taille
int sizeFormule(Formule* L);
int sizeClause(Clause* L);

void creationTabClauseUnVar(Formule* c, Var* tab, int& nvar);

// RG1b
bool RG1b(Formule*& C, Var var);
void RG1bEtRG2Clause(Formule*& C2, Formule* C);
void RG1bLitiral(Clause*& l2, Clause* l);


// RG2
bool RG2(Formule*& C, Formule* clauseVar, Var var);

// the main
void DP(Formule*& F);


int main() {
    Formule* F1 = NULL;
    int nvar, nvar1 = 2 * 2;
    Var* varTab, * varClause;

    do
    {
        nvar = StrToInt("Donne le nombre de Var : ");
        if (nvar == 0)
        {
            cout << "\nErreur : " << endl;
            cout << "Vous ne pouvez pas avoir '0' variable , veuillez donc entrer un nombre autre que '0'\n" << endl;
        }
    } while (nvar == 0);

    creationCharTabTousVar(varTab, nvar);

    creationFormule(F1, nvar, varTab);
    cout << "La formule clausale est :\n";
    afficheFormule(F1);



    /*





    //from here to the rest are for test only and it will be replaced with DP call


    cout << "RG b2  : " << endl;
    // remember to remove insertTabVar and creationTabClauseUnVar it just to test RG1b
    insertTabVar(varClause, nvar1);
    cout << "RG b2  : " << endl;
    creationTabClauseUnVar(F1, varClause, nvar1);
    cout << "RG b2  : " << endl;
    RG1b(F1, varClause[0]);


    // (A,B),(A z)

    cout << "RG b2  : " << endl;
    afficheFormule(F1);

    cout << "RG 222  : " << endl;
    RG2(F1, F1, F1->clause->vr);

    cout << "RG 222  : " << endl;
    afficheFormule(F1);
    */

    cout << "dddddddddddpppppppp" << endl;
    DP(F1);




    deleteCharTabTousVar(varTab);
    return 0;
}



// les fonction d'affichage
void afficheClause(Clause* L)
{
    while (L != NULL)
    {
        cout << L->vr.letteral;
        L = L->letteralSuiv;
        if (L != NULL)
        {
            cout << ", ";
        }
    }
}

void afficheFormule(Formule* C)
{
    if (C == NULL)
    {
        cout << "formule est vide" << endl;
    }

    while (C != NULL)
    {
        cout << "( ";
        afficheClause(C->clause);
        cout << " )";

        C = C->clauseSuiv;

        if (C != NULL)
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
bool verifierEntree(int nvar, Var* varTab, char* input)
{
    //pour verifier que toutes les variable entrees sont déjà déclaré. 

    bool existe = 0; // 1 -> est un variable déjà déclaré. || 0 -> variable n'était pas déclaré. 

    if (input[0] == '-' || input[0] == '\'' || input[1] == ',')
    {
        for (int i = 0; i < nvar; i++)
        {
            if (varTab[i].letteral[1] == input[1] || varTab[i].letteral[0] == input[1])
            {
                existe = 1;
            }
            if (input[1] == ',')
            {
                if (varTab[i].letteral[0] == input[0])
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
        *input = '\0'; // intialise tab de char

        return false;
    }



}

int StrToInt(string messageStr)
{
    //pour verifier que toutes les entrees ne sont que des nombres.

    int X = 0;
    bool intOrStr = 0; //0 -> nomber || 1 -> n'est pas un nomber
    string inputStr;


    do
    {
        cout << messageStr;
        cin >> inputStr;

        if ((inputStr[0] >= 48 && inputStr[0] <= 57) ||
            (inputStr[0] >= 65 && inputStr[0] <= 90) ||
            (inputStr[0] >= 97 && inputStr[0] <= 122))
        {
            intOrStr = !inputStr.empty() && all_of(inputStr.begin(), inputStr.end(), isdigit);
        }

        if (!intOrStr)
        {
            // affiche error massage si l'entree n'est pas 'int'

            cout << endl << "SVP,entrer une entree valide" << endl << endl;
        }
    } while (!intOrStr);

    X = stoi(inputStr); // str to int
    return X;
}



// les fonction de création
void insertDebutClause(Clause*& clause, Var letteral)
{
    Clause* AIDE = new Clause;
    AIDE->vr.letteral[0] = letteral.letteral[0];
    AIDE->vr.letteral[1] = letteral.letteral[1];
    AIDE->vr.letteral[2] = '\0';
    AIDE->letteralSuiv = clause;
    clause = AIDE;
}

void insertDebutFormule(Formule*& formule)
{
    Formule* AIDE = new Formule;
    AIDE->clause = NULL;
    AIDE->clauseSuiv = formule;
    formule = AIDE;
}

void insertTabVar(Var*& varTab, int nvar)
{
    // pour insert un tableau dynamique type char.
    varTab = new Var[nvar];
}

void creationFormule(Formule*& F1, int nvar, Var* varTab)
{
    int N;
    Clause* AIDE = NULL;
    N = StrToInt("donne le nombre de clause : ");
    if (N != 0)
    {
        cout << "\x1b[1m Remarque : \x1b[0m " << endl;
        cout << "\t > Si vous voulez d'entree un variable utilise \" , \" apres leur nom pour L'avoir ." << endl;
        cout << "\t > Si vous voulez d'entree la negation d'un variable utilise \" ' \" ou \" - \" de plus avant leur nom pour L'avoir ." << endl;


        for (int i = 0; i < N; i++)
        {
            insertDebutFormule(F1);
            creationClause(AIDE, nvar, varTab);
            F1->clause = AIDE;
            AIDE = NULL;
        }
    }
    else
    {
        F1 = NULL;
        cout << "Vous avez générer un formule vide " << endl;
    }

}

void creationClause(Clause*& L, int nvar, Var* varTab)
{
    string aide;
    char aide1[4]{};
    Var lit{};
    int N;

    N = StrToInt("\nDonne le nombre de letteral dans cette clause : ");


    for (int i = 0; i < N; i++)
    {
        do
        {
            lit.letteral[0] = '\0';
            cout << "\nDonne letteral N° " << i + 1 << " : ";
            cin >> aide;

            /*   Un variable :
                     - est un lettres de l'alphabet ( a -> z || A -> Z ).
                     - plus " , " si il est un variable normal
                     - plus " ' " ou " - " avant vriable
            */



            // Pour confirmer que la chaîne de caractères à deux caractères c'est tout .


            aide1[0] = aide[0];
            aide1[1] = aide[1];
            aide1[2] = '\0';

        } while (!verifierEntree(nvar, varTab, aide1));

        lit.letteral[0] = aide[0];
        lit.letteral[1] = aide[1];
        lit.letteral[2] = '\0';

        if (lit.letteral[1] == ',')
        {
            // Écrire la variable sur la forme ' l'.
            lit.letteral[1] = lit.letteral[0];
            lit.letteral[0] = ' ';
        }
        else
        {
            // Écrire la négation variable sur la forme '-l'.
            lit.letteral[0] = '-';
        }
        insertDebutClause(L, lit);

    }

    if (N == 0)
    {
        L = NULL;
    }
}

void creationCharTabTousVar(Var*& varTab, int nvar)
{
    // pour remplir un tableau dynamique type char.

    insertTabVar(varTab, nvar);
    string input;
    bool test = 0;  // valide -> 1 | n'est pas valide -> NULL

    for (int i = 0; i < nvar; i++)
    {
        do
        {
            cout << "Donne la lettres de Var N " << i + 1 << ": ";
            cin >> input;

            /*   Un nom de variable :
                         - est un et un seul lettres de l'alphabet ( a -> z || A -> Z ).
            */

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

        varTab[i].letteral[0] = input[0];
    }
}




// les fonction de suppression
void suppressionLitteral(Clause*& L)
{
    // pour supprimer un litteral dans un clause (au debut / fin).
    delete L;
    L = NULL;
}

void suppressionClause(Formule*& C)
{
    // pour supprimer un clause dans la formule (au debut / fin).
    delete C;
    C = NULL;
}

void suppressionMilieuLitteral(Clause*& L)
{
    // pour supprimer un litteral dans un clause (au milieu).
    Clause* AIDE = L->letteralSuiv;
    L->letteralSuiv = NULL;
    delete L;
    L = AIDE;
}

void suppressionMilieuClause(Formule*& C)
{
    // pour supprimer un clause dans la formule (au milieu).
    Formule* AIDE = C->clauseSuiv;
    C->clauseSuiv = NULL;
    delete C;
    C = AIDE;
}

void deleteCharTabTousVar(Var*& varTab)
{
    // pour supprimer un tableau dynamique.
    delete[] varTab;
    varTab = NULL;
}



// les fonction de minimisation
void minimisationClause(Clause* L)
{
    //Pour supprimer les littéraux répétées.

    char existence[2]{};
    Clause* AIDE = NULL;
    Clause* AIDE1 = L;
    Clause* AIDE2 = NULL;


    if (L != NULL)
    {
        while (AIDE1->letteralSuiv != NULL)
        {


            existence[0] = AIDE1->vr.letteral[0];
            existence[1] = AIDE1->vr.letteral[1];
            AIDE2 = AIDE1;

        encore:

            AIDE = AIDE1->letteralSuiv;

            while (AIDE->letteralSuiv != NULL && (existence[0] != AIDE->vr.letteral[0] || existence[1] != AIDE->vr.letteral[1]))
            {
                AIDE = AIDE->letteralSuiv;
            }

            if (existence[0] == AIDE->vr.letteral[0] && existence[1] == AIDE->vr.letteral[1])
            {
                while (AIDE2->letteralSuiv != AIDE)
                {
                    AIDE2 = AIDE2->letteralSuiv;  // Pour l'utiliser dans les fonctions de suppression .
                }

                if (AIDE->letteralSuiv == NULL)
                {
                    suppressionLitteral(AIDE2->letteralSuiv);
                }
                else {
                    suppressionMilieuLitteral(AIDE2->letteralSuiv);
                    goto encore;       //Pour confirmer que il y a aucun autre occurrence.
                }
                AIDE = AIDE1;
            }

            if (AIDE1->letteralSuiv != NULL)
            {
                AIDE1 = AIDE1->letteralSuiv;
            }

        }

    }
    else
    {
        cout << "clause vide" << endl;
    }
}

void minimisationFormule(Formule*& C)
{
    /*
           - Pour supprimer les littéraux répétées et les clause vides.
           - Pour appeler cette fonction la formule doit être non vide ( C != 0 ).
    */

    bool vide = 0;   // 1 -> vide | 0 -> n'est pas vide
    Formule* AIDE = C;
    Formule* AIDE2 = C;


    if (C->clauseSuiv != NULL)
    {
        // il existe au mois deux clauses.

        while (AIDE->clause != NULL && AIDE->clauseSuiv != NULL)
        {
            minimisationClause(AIDE->clause); // Pour supprimer les littéraux répétées.
            AIDE = AIDE->clauseSuiv;
        }

        if (AIDE->clause == NULL)
        {
            vide = 1;
            cout << "eliminer un clause vide : " << endl;
        }
    }
    else
    {
        if (C->clause == NULL)
        {
            // la clause est vide

            suppressionClause(C);
            vide = 0;
        }
        else
        {
            minimisationClause(C->clause); // Pour supprimer les littéraux répétées.
        }

    }

    if (vide)
    {
        //Il existe un vide.

        if (AIDE != C)
        {
            // la clause vide n'est pas la 1er clause.

            while (AIDE2->clauseSuiv != AIDE)
            {
                AIDE2 = AIDE2->clauseSuiv;  // Pour l'utiliser dans les fonctions de suppression .
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
            // la clause vide est la 1er clause.

            suppressionMilieuClause(C);
        }

        vide = 0;
    }
    else
    {
        //Il n'existe aucun vide pour l'enlever.
        return;
    }

    afficheFormule(C);

    // Appelle récursif pour éliminer tout les littéraux répétées et les clause vides.
    minimisationFormule(C);

}

void minimisationFormuleTotal(Formule*& C)
{
    //Pour supprimer les littéraux et les clause répétées et les clause vides.

    Formule* AIDE = C;
    Formule* AIDE1 = NULL;
    Formule* AIDE2 = NULL;


    if (C != NULL)
    {
        //La formule n'est pas vide.

        minimisationFormule(C); // Pour supprimer les littéraux répétées et les clause vides.

        // la formule doit avoir au moins deux clauses pour entree ' while '
        AIDE = C;

        while (AIDE->clauseSuiv != NULL)
        {

        encore:

            AIDE1 = AIDE->clauseSuiv;

            while (AIDE1 != NULL)
            {
                AIDE2 = C;

                while (AIDE2->clauseSuiv != AIDE1)
                {
                    AIDE2 = AIDE2->clauseSuiv; // Pour l'utiliser dans les fonctions de suppression .
                }

                if (clauseIdentiques(AIDE->clause, AIDE1->clause))
                {
                    // les deux clauses sont identiques
                    if (AIDE1->clauseSuiv == NULL)
                    {
                        suppressionClause(AIDE2->clauseSuiv);
                        AIDE1 = NULL;
                    }
                    else
                    {
                        suppressionMilieuClause(AIDE2->clauseSuiv);
                        // Pour tester si il y a un autre occurrence de la même clause dans la formule.
                        goto encore;
                    }
                }

                if (AIDE1 != NULL)
                {
                    AIDE1 = AIDE1->clauseSuiv;
                }

            }
            if (AIDE->clauseSuiv != 0)
            {
                AIDE = AIDE->clauseSuiv;
            }

        }

    }
    else
    {
        cout << "la formule est vide !! " << endl;
    }

}

bool clauseIdentiques(Clause* a, Clause* b)
{
    // Retourner 1 si la cluse 'a' est 'b' sont identiques.

    bool test = 0; // 0 ->  n'est pas identique || 1 -> est identique .
    Clause* aide = NULL;


    if (sizeClause(a) == sizeClause(b))
    {
        // la taile de Formule 'a' est egal a la taile de Formule 'b' .


        while (a != NULL) {


            aide = b;
            test = 0;

            while (aide != NULL)
            {

                if (a->vr.letteral[0] == aide->vr.letteral[0] && a->vr.letteral[1] == aide->vr.letteral[1]) {

                    test = 1; // il existe un litteral dans Formule 'a' et 'b' en même temps.
                    break;
                }
                aide = aide->letteralSuiv;
            }

            if (!test)
            {
                return false;    // Il n'y a pas deux littéraux pareils entre les deux Clauses .
            }
            else
            {
                // Il y a un litteral commun .
                a = a->letteralSuiv;
            }
        }

        if (a == NULL && test) {

            /*      les clauses 'a' et 'b' sont identiques

                - Ils ont la même taile .
                - Ils ont les même littéraux .

            */

            return true;
        }
        else
        {
            /*      les clauses 'a' et 'b' ne sont pas identiques

                - Ils ont la même taile .
                - Ils n'ont pas les même littéraux .
            */

            return false;
        }
    }
    else
    {
        /*      les clauses 'a' et 'b' ne sont pas identiques

                - Ils ont la même taile .
        */

        return false;
    }

}



// Les fonctions calcul de taille
int sizeFormule(Formule* C) {

    // Retourner le nombre de closes dans chaque formule.

    int cntr = NULL;
    Formule* courant = C;

    while (courant != NULL) {

        cntr++;
        courant = courant->clauseSuiv;
    }

    return cntr;
}

int sizeClause(Clause* L) {

    // Retourner le nombre de littéraux dans chaque close.

    int cntr = NULL;
    Clause* courant = L;

    while (courant != NULL) {

        cntr++;
        courant = courant->letteralSuiv;
    }

    return cntr;
}


////                les fonction imprtant :

void creationTabClauseUnVar(Formule* c, Var* tab, int& nvar) {
    // pour remplir un tab par le nom de chaque variable qui forme un clause de un seul var . 

    int i = 0;
    if (c != NULL)
    {
        Formule* aide = c;
        while (aide != NULL)
        {
            if (sizeClause(aide->clause) == 1)
            {
                tab[i].letteral[0] = aide->clause->vr.letteral[0];
                tab[i].letteral[1] = aide->clause->vr.letteral[1];
                i++;
            }

            aide = aide->clauseSuiv;
        }
        nvar = i;
    }
    else {
        cout << "formule vide !!" << endl;
    }
}

// RG1b
bool RG1b(Formule*& C, Var var)
{
    // cette fonction est l'application de 'RG 1b' dans l'algo de DP .
    /*
        suppression de toutes les clauses qui contiennent var,
        et suppression de ¬var dans toutes les clauses où il apparaît .

    */



    Formule* AIDE = C;
    Clause* aide;
    int test = 0;

    if (AIDE != NULL)
    {
    agin:
        if (AIDE->clauseSuiv != NULL)
        {
            // la formule a au moins deux clauses

            AIDE = C;
            while (AIDE != NULL)
            {
                // parcourir la liste de clause (la formule) .

                aide = AIDE->clause;
                while (aide != NULL)
                {
                    // parcourir la liste de var dans chaque clause .

                    if (var.letteral[1] == aide->vr.letteral[1]) {
                        if (var.letteral[0] == aide->vr.letteral[0])
                        {
                            // sup clause dans le cas d'existence le meme var .
                            RG1bEtRG2Clause(C, AIDE);
                            afficheFormule(C);
                            
                        }
                        else {
                            

                            
                            
                            if (sizeClause(AIDE->clause) == 1)
                            {
                                test++;
                            }
                            if (test == 1)
                            {
                                return 1;
                            }
                            
                            
                            

                            //sup letteral dans le cas d'existence un négation de var .
                            RG1bLitiral(AIDE->clause, aide);
                            afficheFormule(C);

                            if (sizeClause(AIDE->clause) == 0)
                            {
                                cout << " in-SAT" << endl;
                                return 0;
                            }
                        }
                        goto agin; // Pour confirmer que il n'existe aucun autre occurrence de la même variable .
                    }

                    if (aide != NULL)
                    {
                        aide = aide->letteralSuiv;
                    }



                }
                if (AIDE != NULL)
                {
                    AIDE = AIDE->clauseSuiv;
                }

            }
        }
    }
    else {
        cout << "Formule vide !!" << endl;
    }
    return 0;
}

void RG1bLitiral(Clause*& l2, Clause* l) {

    //Pour supprimer un littiral de l'adresse qui pointer par le pointeur ' Clause* l ' .

    Clause* AIDE = l;
    Clause* AIDE2 = l2;

    if (l2 != l)
    {
        // le littiral est dans le milu ou a la fin de la liste Clause .

        while (AIDE2->letteralSuiv != AIDE)
        {
            AIDE2 = AIDE2->letteralSuiv;  // Pour l'utiliser dans les fonctions de suppression .
        }

        if (AIDE->letteralSuiv == NULL)
        {
            suppressionLitteral(AIDE2->letteralSuiv);
        }
        else {
            suppressionMilieuLitteral(AIDE2->letteralSuiv);
        }
        l2 = AIDE2;
    }
    else
    {
        // le littiral est dans le debut de la liste Clause .

        if (l2->letteralSuiv == NULL)
        {
            suppressionLitteral(l2);
        }
        else {
            suppressionMilieuLitteral(l2);
        }
    }



}

void RG1bEtRG2Clause(Formule*& C2, Formule* C) {

    //Pour supprimer la clausse de l'adresse qui pointer par le pointeur ' Formule* C ' .
    // cette fonction est utilse dans 'RG 1b' et 'RG 2'

    Formule* AIDE = C;
    Formule* AIDE2 = C2;

    if (AIDE2 != C)
    {
        // la clause est dans le milu ou a la fin de la liste Formule .
        while (AIDE2->clauseSuiv != AIDE)
        {
            AIDE2 = AIDE2->clauseSuiv;  // Pour l'utiliser dans les fonctions de suppression .
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
        // la clause est dans le debut de la liste Formule .

        if (C2->clauseSuiv == NULL)
        {
            suppressionClause(C2);
        }
        else {
            suppressionMilieuClause(C2);
        }
    }



}

// RG2
bool RG2(Formule*& C, Formule* clauseVar, Var var)
{
    // cette fonction est l'application de 'RG 2' dans l'algo de DP .
    /*
        suppression de toutes
        les clauses qui contiennent var .

    */

    Formule* AIDE = C;
    Formule* AIDE2 = C;
    Clause* aide;
    bool test = 0; // 0 -> il n'existe aucun negation de var

    if (C != NULL)
    {
        while (AIDE != NULL)
        {
            // parcourir la liste de clause (la formule) .

            aide = AIDE->clause;

            while (aide != NULL) {
                // parcourir la liste de var dans chaque clause .

                if (aide->vr.letteral[0] != clauseVar->clause->vr.letteral[0] && aide->vr.letteral[1] == clauseVar->clause->vr.letteral[1])
                {
                    // il existe un negation de var dans une des clause .
                    test = 1;
                    return 0;
                }
                aide = aide->letteralSuiv;
            }

            AIDE = AIDE->clauseSuiv;
        }


        if (!test)
        {
            // il n'existe aucun negation de var dans tous la formule .

        next:
            AIDE2 = C;
            while (AIDE2 != NULL)
            {
                // parcourir la liste de clause (la formule) .

                aide = AIDE2->clause;

                while (aide != NULL) {
                    // parcourir la liste de var dans chaque clause .

                    if (aide->vr.letteral[1] == var.letteral[1])
                    {
                        RG1bEtRG2Clause(C, AIDE2); // suppression de clause qui contiennent Var .
                        cout << "la formule apres la suppression d'une clause : " << endl;
                        afficheFormule(C);

                        goto next; // Pour confirmer que il n'existe aucun autre occurrence de Var dans les autres clauses .
                    }

                    aide = aide->letteralSuiv;

                }

                if (AIDE2 != NULL)
                {
                    AIDE2 = AIDE2->clauseSuiv;
                }
            }

        }
        return 1;
    }
    else {
        cout << "Formule vide !!" << endl;
        return 0;
    }

}


// DP
void DP(Formule*& F)
{
    Formule* AIDE = F;
    Formule* AIDE2 = F;
    Clause* aide = F->clause;
    bool succès = 0;
    bool échec = 1;
    bool donne = 0;

    //( -z, -e ),(  e, -e, -z ),(  e,  a )

    do
    {
    restart:
        cout << "La formule clausale est (minimisation total):\n";
        minimisationFormuleTotal(F);
        afficheFormule(F);
        AIDE2 = F;


        if (sizeFormule(F) == 0)
        {
            échec = 0;
            succès = 1;
            cout << "la formule est satisfiable." << endl;
        }
        else {
            cout << "str" << endl;
            cout << AIDE2 << endl;

            if (sizeClause(AIDE2->clause) == 1)
            {
                cout << "RG1b : " << endl;
                AIDE = F;

                if (RG1b(F, AIDE2->clause->vr))
                {
                    goto insat;
                }

                if (sizeClause(AIDE2->clause) == 1)
                {
                    goto insat;

                }


                AIDE2 = F;


                if (sizeFormule(F) >= 2)
                {
                    goto restart;
                }


                cout << "END RG1b : " << endl;
            }
            else
            {
                cout << "RG 2 : " << endl;
                donne = RG2(F, AIDE2, AIDE2->clause->vr);
                AIDE2 = F;

                cout << "END RG 2 : " << endl;

                if (sizeFormule(F) == 0)
                {
                    goto restart;
                }

                if (!donne)
                {
                    AIDE2->clause->vr = AIDE2->clause->letteralSuiv->vr;
                    cout << "RG1b 22: " << endl;
                    if (RG1b(F, AIDE2->clause->vr))
                    {
                        goto insat;
                    }


                    goto restart;

                    cout << "END RG1b 22: " << endl;

                }


                if (sizeClause(AIDE2->clause) == 1)
                {
                insat:
                    succès = 1;
                    échec = 0;
                    cout << "insatisfiable" << endl;

                }


            }

        }

    end:
        if (AIDE2 != 0)
        {
            AIDE2 = AIDE2->clauseSuiv;
        }
    } while (!succès || échec);

    if (échec)
    {
        cout << "";
    }
    else {
        cout << "";
    }

}
