#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

// les structures de données
typedef struct Var {
    char leteral[3];
};
typedef struct Clause {
    Var vr;
    Clause* leteralSuiv;
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
void insertDebutClause(Clause*& clause, Var leteral);
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
void RG1b(Formule*& C, Var* tab, int n);
void RG1bClause(Formule*& C, Clause* l);
void RG1bLitiral(Clause*& l2, Clause* l);

int main() {
    Formule* F1 = NULL;
    int nvar, nvar1 = 2*2;
    Var* varTab , *varClause;

    
    nvar = StrToInt("Donne le nombre de Var : ");
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

    cout << "RG b2  : " << endl;
    insertTabVar(varClause , nvar1);
    cout << "RG b2  : " << endl;
    creationTabClauseUnVar(F1 , varClause, nvar1);
    cout << "RG b2  : " << endl;
    RG1b(F1, varClause, nvar1);
    cout << "RG b2  : " << endl;
    afficheFormule(F1);


    deleteCharTabTousVar(varTab);
    return 0;
}



// les fonction d'affichage
void afficheClause(Clause* L)
{
    while (L != NULL)
    {
        cout << L->vr.leteral;
        L = L->leteralSuiv;
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
            if (varTab[i].leteral[1] == input[1] || varTab[i].leteral[0] == input[1])
            {
                existe = 1;
            }
            if (input[1] == ',')
            {
                if (varTab[i].leteral[0] == input[0])
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

        intOrStr = !inputStr.empty() && all_of(inputStr.begin(), inputStr.end(), isdigit);
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
void insertDebutClause(Clause*& clause, Var leteral)
{
    Clause* AIDE = new Clause;
    AIDE->vr.leteral[0] = leteral.leteral[0];
    AIDE->vr.leteral[1] = leteral.leteral[1];
    AIDE->vr.leteral[2] = '\0';
    AIDE->leteralSuiv = clause;
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

void creationClause(Clause*& L, int nvar, Var* varTab)
{
    string aide;
    char aide1[4]{};
    Var lit {};
    int N;

    N = StrToInt("\nDonne le nombre de letteral dans cette clause : ");


    for (int i = 0; i < N; i++)
    {
        do
        {
            lit.leteral[0] = '\0';
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

        lit.leteral[0] = aide[0];
        lit.leteral[1] = aide[1];
        lit.leteral[2] = '\0';

        if (lit.leteral[1] == ',')
        {
            // Écrire la variable sur la forme ' l'.
            lit.leteral[1] = lit.leteral[0];
            lit.leteral[0] = ' ';
        }
        else
        {
            // Écrire la négation variable sur la forme '-l'.
            lit.leteral[0] = '-';
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

        varTab[i].leteral[0] = input[0];
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
    Clause* AIDE = L->leteralSuiv;
    L->leteralSuiv = NULL;
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
    Clause* AIDE  = NULL;
    Clause* AIDE1 = L;
    Clause* AIDE2 = NULL;
 

    if (L != NULL)
    {
        while (AIDE1->leteralSuiv != NULL)
        {


            existence[0] = AIDE1->vr.leteral[0];
            existence[1] = AIDE1->vr.leteral[1];
            AIDE2 = AIDE1;

        encore:             

            AIDE = AIDE1->leteralSuiv;

            while (AIDE->leteralSuiv != NULL && (existence[0] != AIDE->vr.leteral[0] || existence[1] != AIDE->vr.leteral[1]))
            {
                AIDE = AIDE->leteralSuiv;
            }

            if (existence[0] == AIDE->vr.leteral[0] && existence[1] == AIDE->vr.leteral[1])
            {
                while (AIDE2->leteralSuiv != AIDE)
                {
                    AIDE2 = AIDE2->leteralSuiv;  // Pour l'utiliser dans les fonctions de suppression .
                }

                if (AIDE->leteralSuiv == NULL)
                {
                    suppressionLitteral(AIDE2->leteralSuiv);
                }
                else {
                    suppressionMilieuLitteral(AIDE2->leteralSuiv);
                    goto encore;       //Pour confirmer que il y a aucun autre occurrence.
                }
                AIDE = AIDE1;
            }

            if (AIDE1->leteralSuiv != NULL)
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

void minimisationFormule(Formule*& C)
{
    /*
           - Pour supprimer les littéraux répétées et les clause vides.
           - Pour appeler cette fonction la formule doit être non vide ( C != 0 ).
    */

    bool vide = 0;   // 1 -> vide | 0 -> n'est pas vide
    Formule* AIDE  = C;
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

    Formule* AIDE  = C;
    Formule* AIDE1 = NULL;
    Formule* AIDE2 = NULL;


    if (C != NULL)
    {
        //La formule n'est pas vide.

        minimisationFormule(C); // Pour supprimer les littéraux répétées et les clause vides.

        // la formule doit avoir au moins deux clauses pour entree ' while '

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

                if (a->vr.leteral[0] == aide->vr.leteral[0] && a->vr.leteral[1] == aide->vr.leteral[1]) {

                    test = 1; // il existe un litteral dans Formule 'a' et 'b' en même temps.
                    break;
                }
                aide = aide->leteralSuiv;
            }

            if (!test)
            {
                return false;    // Il n'y a pas deux littéraux pareils entre les deux Clauses .
            }
            else
            {
                // Il y a un litteral commun .
                a = a->leteralSuiv;
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
        courant = courant->leteralSuiv;
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
            if (aide->clause != NULL)
            {
                if (aide->clause->leteralSuiv == NULL)
                {
                    tab[i].leteral[0] = aide->clause->vr.leteral[0];
                    tab[i].leteral[1] = aide->clause->vr.leteral[1];
                    i++;
                }
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
void RG1b(Formule*& C, Var* tab, int n)
{
    Formule* AIDE = C;
    Clause* aide;

    if (AIDE != NULL)
    {
        agin:
        if (AIDE->clauseSuiv != NULL)
        {
       
            AIDE = C;
            while (AIDE != NULL)
            {
                aide = AIDE->clause;
                while (aide != NULL)
                {
                    
                    for (int i = 0; i < n; i++)
                    {
                        if (tab[i].leteral[1] == aide->vr.leteral[1]) {
                            if (tab[i].leteral[0] == aide->vr.leteral[0])
                            {
                                // sup clause 
                                RG1bClause(C, aide);    
                                afficheFormule(C);

                            }
                            else {
                                //sup letteral
                                RG1bLitiral(AIDE->clause, aide);
                                afficheFormule(C);
                            }
                            goto agin;
                        }
                    }

                    aide = aide->leteralSuiv;
                }

                donne :
                AIDE = AIDE->clauseSuiv;
            }
        }
    }  
    else {
        cout << "Formule vide !!" << endl;
    }

}

void RG1bClause(Formule*& C, Clause * l) {
    //Pour supprimer la clausse qu'a la negation de var.
    Formule* AIDE = C;
    Formule* AIDE2 = C;
    Clause* aide = C->clause;
    bool trouve = 0;

    while (AIDE != NULL)
    {
        aide = AIDE->clause;
        while (aide != NULL)
        {
            if (l == aide)
            {
                trouve = 1;
                goto end;
            }

            aide = aide->leteralSuiv;
        }
        

        AIDE = AIDE->clauseSuiv;
    }
    
end :


    if (trouve)
    {
        //Il existe un clause a la negation de var .

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
            // la clause qui a la negation de var est la 1er clause.

            suppressionMilieuClause(C);
        }

        trouve = 0;

    }
    else
    {
        return;
    }


}

void RG1bLitiral(Clause *& l2, Clause* l) {

    //Pour supprimer la litteral répétée dans chaque clause.

    Clause* AIDE = l;
    Clause* AIDE2 = l2;

    if (l2 != l)
    {
        while (AIDE2->leteralSuiv != AIDE)
        {
            AIDE2 = AIDE2->leteralSuiv;  // Pour l'utiliser dans les fonctions de suppression .
        }

        if (AIDE->leteralSuiv == NULL)
        {
            suppressionLitteral(AIDE2->leteralSuiv);
        }
        else {
            suppressionMilieuLitteral(AIDE2->leteralSuiv);
        }
        l2 = AIDE2;
    }
    else
    {
        if (l2->leteralSuiv == NULL)
        {
            suppressionLitteral(l2);
        }
        else {
            suppressionMilieuLitteral(l2);
        }
    }
    


}