#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

enum StatutReservation {
    EN_ATTENTE,
    VALIDEE,
    PAYEE,
    ANNULEE
};

enum StatutPaiement {
    NON_ENREGISTRE,
    ENREGISTRE,
    REMBOURSE
};

string nomStatutReservation(StatutReservation statut) {
    switch (statut) {
        case EN_ATTENTE: return "EN_ATTENTE";
        case VALIDEE:    return "VALIDEE";
        case PAYEE:      return "PAYEE";
        case ANNULEE:    return "ANNULEE";
    }
    return "INCONNU";
}

string nomStatutPaiement(StatutPaiement statut) {
    switch (statut) {
        case NON_ENREGISTRE: return "NON_ENREGISTRE";
        case ENREGISTRE:     return "ENREGISTRE";
        case REMBOURSE:      return "REMBOURSE";
    }
    return "INCONNU";
}

string lireTexte(const string& message) {
    string valeur;

    do {
        cout << message;
        getline(cin, valeur);

        if (valeur.empty()) {
            cout << "La saisie ne peut pas etre vide.\n";
        }
    } while (valeur.empty());

    return valeur;
}

int lireEntier(const string& message, int minimum, int maximum) {
    string saisie;
    int valeur;
    char reste;

    while (true) {
        cout << message;
        getline(cin, saisie);

        stringstream flux(saisie);

        if ((flux >> valeur) && !(flux >> reste) &&
            valeur >= minimum && valeur <= maximum) {
            return valeur;
        }

        cout << "Saisie invalide. Entrez un nombre entre "
             << minimum << " et " << maximum << ".\n";
    }
}

double lireMontant(const string& message) {
    string saisie;
    double montant;
    char reste;

    while (true) {
        cout << message;
        getline(cin, saisie);

        replace(saisie.begin(), saisie.end(), ',', '.');

        stringstream flux(saisie);

        if ((flux >> montant) && !(flux >> reste) && montant > 0) {
            return montant;
        }

        cout << "Montant invalide. Entrez un montant positif.\n";
    }
}

class Passager {
private:
    string id;
    string nom;
    string prenom;
    string telephone;
    string email;

public:
    Passager(const string& nouvelId, const string& nouveauNom,
             const string& nouveauPrenom, const string& nouveauTelephone,
             const string& nouvelEmail)
        : id(nouvelId), nom(nouveauNom), prenom(nouveauPrenom),
          telephone(nouveauTelephone), email(nouvelEmail) {
    }

    string getId() const {
        return id;
    }

    void afficher() const {
        cout << id << " - " << prenom << " " << nom
             << " | telephone : " << telephone
             << " | email : " << email << "\n";
    }
}

;

class Reservation;

class Vol {
private:
    string id;
    string origine;
    string destination;
    string dateHeure;
    int capacite;

public:
    Vol(const string& nouvelId, const string& nouvelleOrigine,
        const string& nouvelleDestination, const string& nouvelleDateHeure,
        int nouvelleCapacite)
        : id(nouvelId), origine(nouvelleOrigine),
          destination(nouvelleDestination), dateHeure(nouvelleDateHeure),
          capacite(nouvelleCapacite) {
    }

    string getId() const {
        return id;
    }

    int placesOccupees(const vector<Reservation>& reservations) const;
    int placesDisponibles(const vector<Reservation>& reservations) const;

    void afficher(const vector<Reservation>& reservations) const {
        cout << id << " | " << origine << " -> " << destination
             << " | " << dateHeure
             << " | places disponibles : "
             << placesDisponibles(reservations)
             << "/" << capacite << "\n";
    }
};

class Reservation {
private:
    string id;
    string passagerId;
    string volId;
    StatutReservation statut;

public:
    Reservation(const string& nouvelId, const string& nouvelIdPassager,
                const string& nouvelIdVol)
        : id(nouvelId), passagerId(nouvelIdPassager),
          volId(nouvelIdVol), statut(EN_ATTENTE) {
    }

    string getId() const {
        return id;
    }

    string getPassagerId() const {
        return passagerId;
    }

    string getVolId() const {
        return volId;
    }

    StatutReservation getStatut() const {
        return statut;
    }

    bool occupeUnePlace() const {
        return statut == VALIDEE || statut == PAYEE;
    }

    bool valider() {
        if (statut != EN_ATTENTE) {
            return false;
        }

        statut = VALIDEE;
        return true;
    }

    bool marquerPayee() {
        if (statut != VALIDEE) {
            return false;
        }

        statut = PAYEE;
        return true;
    }

    bool annuler() {
        if (statut == ANNULEE) {
            return false;
        }

        statut = ANNULEE;
        return true;
    }

    void afficher() const {
        cout << id
             << " | passager : " << passagerId
             << " | vol : " << volId
             << " | statut : " << nomStatutReservation(statut)
             << "\n";
    }
};

int Vol::placesOccupees(
    const vector<Reservation>& reservations) const {
    int nombre = 0;
    size_t i;

    for (i = 0; i < reservations.size(); ++i) {
        if (reservations[i].getVolId() == id &&
            reservations[i].occupeUnePlace()) {
            ++nombre;
        }
    }

    return nombre;
}

int Vol::placesDisponibles(
    const vector<Reservation>& reservations) const {
    int disponibles = capacite - placesOccupees(reservations);

    if (disponibles < 0) {
        return 0;
    }

    return disponibles;
}

class Paiement {
private:
    string id;
    string reservationId;
    double montant;
    string date;
    StatutPaiement statut;

public:
    Paiement(const string& nouvelId, const string& nouvelIdReservation,
             double nouveauMontant, const string& nouvelleDate)
        : id(nouvelId), reservationId(nouvelIdReservation),
          montant(nouveauMontant), date(nouvelleDate),
          statut(NON_ENREGISTRE) {
    }

    string getReservationId() const {
        return reservationId;
    }

    bool enregistrer() {
        if (statut != NON_ENREGISTRE) {
            return false;
        }

        statut = ENREGISTRE;
        return true;
    }

    bool rembourser() {
        if (statut != ENREGISTRE) {
            return false;
        }

        statut = REMBOURSE;
        return true;
    }

    void afficher() const {
        cout << "Paiement " << id
             << " | reservation : " << reservationId
             << " | montant : " << fixed << setprecision(2) << montant
             << " | date : " << date
             << " | statut : " << nomStatutPaiement(statut)
             << "\n";
    }
};

class Agence {
private:
    vector<Passager> passagers;
    vector<Vol> vols;
    vector<Reservation> reservations;
    vector<Paiement> paiements;
    int prochainNumeroPaiement;

    Passager* trouverPassager(const string& id) {
        size_t i;

        for (i = 0; i < passagers.size(); ++i) {
            if (passagers[i].getId() == id) {
                return &passagers[i];
            }
        }

        return NULL;
    }

    Vol* trouverVol(const string& id) {
        size_t i;

        for (i = 0; i < vols.size(); ++i) {
            if (vols[i].getId() == id) {
                return &vols[i];
            }
        }

        return NULL;
    }

    Reservation* trouverReservation(const string& id) {
        size_t i;

        for (i = 0; i < reservations.size(); ++i) {
            if (reservations[i].getId() == id) {
                return &reservations[i];
            }
        }

        return NULL;
    }

    Paiement* trouverPaiement(const string& idReservation) {
        size_t i;

        for (i = 0; i < paiements.size(); ++i) {
            if (paiements[i].getReservationId() == idReservation) {
                return &paiements[i];
            }
        }

        return NULL;
    }

public:
    Agence() : prochainNumeroPaiement(1) {
    }

    bool ajouterPassager(const Passager& passager) {
        if (trouverPassager(passager.getId()) != NULL) {
            return false;
        }

        passagers.push_back(passager);
        return true;
    }

    bool ajouterVol(const Vol& vol) {
        if (trouverVol(vol.getId()) != NULL) {
            return false;
        }

        vols.push_back(vol);
        return true;
    }

    void afficherVolsDisponibles() const {
        size_t i;
        bool trouve = false;

        for (i = 0; i < vols.size(); ++i) {
            if (vols[i].placesDisponibles(reservations) > 0) {
                vols[i].afficher(reservations);
                trouve = true;
            }
        }

        if (!trouve) {
            cout << "Aucun vol avec des places disponibles.\n";
        }
    }

    bool rechercherVol(const string& id) const {
        size_t i;

        for (i = 0; i < vols.size(); ++i) {
            if (vols[i].getId() == id) {
                vols[i].afficher(reservations);
                return true;
            }
        }

        cout << "Vol introuvable.\n";
        return false;
    }

    bool creerReservation(const string& idReservation,
                          const string& idPassager,
                          const string& idVol) {
        size_t i;
        Vol* vol;

        if (trouverReservation(idReservation) != NULL) {
            cout << "Cet identifiant de reservation existe deja.\n";
            return false;
        }

        if (trouverPassager(idPassager) == NULL) {
            cout << "Passager inconnu. Enregistrez-le d'abord.\n";
            return false;
        }

        vol = trouverVol(idVol);

        if (vol == NULL) {
            cout << "Vol introuvable.\n";
            return false;
        }

        for (i = 0; i < reservations.size(); ++i) {
            if (reservations[i].getPassagerId() == idPassager &&
                reservations[i].getVolId() == idVol &&
                reservations[i].getStatut() != ANNULEE) {
                cout << "Ce passager a deja une reservation active "
                     << "pour ce vol.\n";
                return false;
            }
        }

        if (vol->placesDisponibles(reservations) == 0) {
            cout << "Le vol est complet.\n";
            return false;
        }

        reservations.push_back(
            Reservation(idReservation, idPassager, idVol));

        cout << "Reservation creee en attente. "
             << "Elle ne bloque pas encore de place.\n";
        return true;
    }

    bool validerReservation(const string& id) {
        Reservation* reservation;
        Vol* vol;

        reservation = trouverReservation(id);

        if (reservation == NULL) {
            cout << "Reservation introuvable.\n";
            return false;
        }

        vol = trouverVol(reservation->getVolId());

        if (vol == NULL || vol->placesDisponibles(reservations) == 0) {
            cout << "Aucune place disponible. Reservation non validee.\n";
            return false;
        }

        if (!reservation->valider()) {
            cout << "Seule une reservation en attente peut etre validee.\n";
            return false;
        }

        cout << "Reservation validee. Une place est reservee.\n";
        return true;
    }

    bool enregistrerPaiement(const string& idReservation,
                             double montant, const string& date) {
        Reservation* reservation;
        stringstream fluxId;
        string idPaiement;

        reservation = trouverReservation(idReservation);

        if (reservation == NULL) {
            cout << "Reservation introuvable.\n";
            return false;
        }

        if (reservation->getStatut() != VALIDEE) {
            cout << "Paiement refuse : la reservation doit etre validee "
                 << "et ne doit pas deja etre payee.\n";
            return false;
        }

        fluxId << "P" << prochainNumeroPaiement;
        fluxId >> idPaiement;

        paiements.push_back(
            Paiement(idPaiement, idReservation, montant, date));

        if (!paiements.back().enregistrer()) {
            paiements.pop_back();
            cout << "Le paiement n'a pas pu etre enregistre.\n";
            return false;
        }

        if (!reservation->marquerPayee()) {
            paiements.pop_back();
            cout << "La reservation n'a pas pu passer au statut PAYEE.\n";
            return false;
        }

        ++prochainNumeroPaiement;
        cout << "Paiement enregistre.\n";
        return true;
    }

    bool annulerReservation(const string& idReservation) {
        Reservation* reservation;
        Paiement* paiement;
        bool etaitPayee;

        reservation = trouverReservation(idReservation);

        if (reservation == NULL) {
            cout << "Reservation introuvable.\n";
            return false;
        }

        if (reservation->getStatut() == ANNULEE) {
            cout << "Cette reservation est deja annulee.\n";
            return false;
        }

        etaitPayee = reservation->getStatut() == PAYEE;

        if (etaitPayee) {
            paiement = trouverPaiement(idReservation);

            if (paiement == NULL || !paiement->rembourser()) {
                cout << "Le remboursement n'a pas pu etre effectue.\n";
                return false;
            }
        }

        reservation->annuler();

        if (etaitPayee) {
            cout << "Reservation annulee et paiement rembourse. "
                 << "La place est liberee.\n";
        } else {
            cout << "Reservation annulee. La place est liberee "
                 << "si elle etait validee.\n";
        }

        return true;
    }

    void consulterReservations() const {
        size_t i;

        if (reservations.empty()) {
            cout << "Aucune reservation.\n";
            return;
        }

        for (i = 0; i < reservations.size(); ++i) {
            reservations[i].afficher();
        }
    }

    void afficherReservation(const string& id) const {
        size_t indexReservation = reservations.size();
        size_t i;

        for (i = 0; i < reservations.size(); ++i) {
            if (reservations[i].getId() == id) {
                indexReservation = i;
                break;
            }
        }

        if (indexReservation == reservations.size()) {
            cout << "Reservation introuvable.\n";
            return;
        }

        const Reservation& reservation = reservations[indexReservation];
        reservation.afficher();

        for (i = 0; i < passagers.size(); ++i) {
            if (passagers[i].getId() == reservation.getPassagerId()) {
                cout << "Passager : ";
                passagers[i].afficher();
                break;
            }
        }

        for (i = 0; i < vols.size(); ++i) {
            if (vols[i].getId() == reservation.getVolId()) {
                cout << "Vol : ";
                vols[i].afficher(reservations);
                break;
            }
        }

        bool paiementTrouve = false;

        for (i = 0; i < paiements.size(); ++i) {
            if (paiements[i].getReservationId() == id) {
                paiements[i].afficher();
                paiementTrouve = true;
                break;
            }
        }

        if (!paiementTrouve) {
            cout << "Aucun paiement enregistre.\n";
        }
    }
};

int main() {
    Agence agence;
    int choix;

    do {
        cout << "\n=== Agence de voyages de Cotonou ===\n"
             << "1. Enregistrer un passager\n"
             << "2. Ajouter un vol\n"
             << "3. Afficher les vols disponibles\n"
             << "4. Rechercher un vol\n"
             << "5. Creer une reservation\n"
             << "6. Valider une reservation\n"
             << "7. Enregistrer un paiement\n"
             << "8. Annuler une reservation\n"
             << "9. Consulter les reservations\n"
             << "10. Afficher les informations d'une reservation\n"
             << "11. Quitter\n";

        choix = lireEntier("Votre choix : ", 1, 11);

        if (choix == 1) {
            string id = lireTexte("Identifiant du passager : ");
            string nom = lireTexte("Nom : ");
            string prenom = lireTexte("Prenom : ");
            string telephone = lireTexte("Telephone : ");
            string email = lireTexte("Email : ");

            Passager passager(id, nom, prenom, telephone, email);

            if (agence.ajouterPassager(passager)) {
                cout << "Passager enregistre.\n";
            } else {
                cout << "Identifiant de passager deja utilise.\n";
            }
        } else if (choix == 2) {
            string id = lireTexte("Identifiant du vol : ");
            string origine = lireTexte("Origine : ");
            string destination = lireTexte("Destination : ");
            string dateHeure = lireTexte("Date et heure du vol : ");
            int capacite = lireEntier("Capacite du vol : ", 1, 100000);

            Vol vol(id, origine, destination, dateHeure, capacite);

            if (agence.ajouterVol(vol)) {
                cout << "Vol ajoute.\n";
            } else {
                cout << "Identifiant de vol deja utilise.\n";
            }
        } else if (choix == 3) {
            agence.afficherVolsDisponibles();
        } else if (choix == 4) {
            agence.rechercherVol(
                lireTexte("Identifiant du vol : "));
        } else if (choix == 5) {
            string idReservation =
                lireTexte("Identifiant de la reservation : ");
            string idPassager =
                lireTexte("Identifiant du passager : ");
            string idVol =
                lireTexte("Identifiant du vol : ");

            agence.creerReservation(idReservation, idPassager, idVol);
        } else if (choix == 6) {
            agence.validerReservation(
                lireTexte("Identifiant de la reservation : "));
        } else if (choix == 7) {
            string idReservation =
                lireTexte("Identifiant de la reservation : ");
            double montant =
                lireMontant("Montant du paiement : ");
            string date =
                lireTexte("Date du paiement : ");

            agence.enregistrerPaiement(idReservation, montant, date);
        } else if (choix == 8) {
            agence.annulerReservation(
                lireTexte("Identifiant de la reservation : "));
        } else if (choix == 9) {
            agence.consulterReservations();
        } else if (choix == 10) {
            agence.afficherReservation(
                lireTexte("Identifiant de la reservation : "));
        }

    } while (choix != 11);

    cout << "Au revoir.\n";
    return 0;
}
