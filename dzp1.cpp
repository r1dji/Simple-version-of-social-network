#include <iostream>
using namespace std;

#include <string>
#include <deque>

struct Odgovor;

struct Komentar {
	string komentar;
	int ocena = 0;
	int ocene_potomaka[10] = {0};
	int brkomentara = -1;
	Komentar* komotac = nullptr;
	Odgovor* odgotac = nullptr;

	Komentar** koment = {nullptr};

	Komentar(string k, int oc = 0) : komentar(k), ocena(oc) {
		koment = new Komentar * [10];
	};
};

struct Pitanje;
struct Odgovor {
	string odgovor;
	int ocena = 0;
	int ocene_potomaka_kom[10] = {0};
	Komentar** kom = {nullptr};
	int brkom = -1;
	Pitanje* otac = nullptr;

	Odgovor() = default;
	Odgovor(string s, int o = 0) :odgovor(s), ocena(o) {
		kom = new Komentar * [10];
	};
};

struct Pitanje {
	string pit;
	Pitanje* sled = nullptr;
	Odgovor** og;
	int brodg = -1;
	int ocene_potomaka_odgovora[10];

	Pitanje(string s, Pitanje* sl = nullptr) : pit(s), sled(sl) {
		og = new Odgovor * [10];
	};
};

int kreirajmeni() {
	int k = 0;
	cout << endl;
	cout << "0. Kraj programa" << endl;
	cout << "1. Dodavanje pitanja" << endl;
	cout << "2. Dodavanje odgovora ne neko pitanje" << endl;
	cout << "3. Pretrazi pitanje" << endl;
	cout << "4. Pretrazi odgovor" << endl;
	cout << "5. Povecavanje ocene odgovoru" << endl;
	cout << "6. Uredi po ocenama" << endl;
	cout << "7. Ispis pitanja" << endl;
	cout << "8. Ipis odgovarajuceg pitanja i njemu pridruzenih odgovora" << endl;
	cout << "9. Brisanje odgovora ili komentara i svih njegovih potomaka" << endl;
	cout << "10. Brisanje pojedinacnog pitanja i svih povezanih odgovora" << endl;
	cout << "11. Pronalazak odgovora ili komentara sa najvecom ocenom na dato pitanje" << endl;
	cout << "?";
	cin >> k;

	return k;
}

void dodajpitanje(string s, Pitanje*& p) {
	Pitanje* novi = nullptr;
	p = new Pitanje(s, p);
}

void dodajodgovor(string p, string o, Pitanje* prvi) {
	Pitanje* tek = prvi, *koren = nullptr;
	while (tek) {
		if (tek->pit == p) {
			koren = tek;
			break;
		}
		if (!koren) {
			tek = tek->sled;
		}
	}
	if (!koren) {
		cout << "Neuspesna pretraga" << endl;
		exit(0);
	}
	if ((koren->brodg) < 9) {
		Odgovor* otmp = new Odgovor(o);
		koren->og[(koren->brodg) + 1] = otmp;
		koren->brodg = koren->brodg + 1;
		koren->ocene_potomaka_odgovora[koren->brodg] = otmp->ocena;
	}
	else {
		cout << "Max broj odgovora";
	}
	
}

void dodajkomentar(string p, string o, string k ,Pitanje* prvi) {
	Pitanje* tek = prvi, * koren = nullptr;
	Odgovor* odg = nullptr;
	int pr = 0;
	int pro = 0;
	while (tek) {
		if (tek->pit == p) {
			koren = tek;
			pr = 1;
		}
		if (!koren) {
			tek = tek->sled;
		}
		//tek = tek->sled;
		if (pr == 1) {
			break;
		}
	}
	deque <Odgovor*> oq;
	if (koren) {
		if (koren->brodg > -1) {
			for (int i = 0; i <= koren->brodg;i++) {
				oq.push_back(koren->og[i]);
			}

			deque<Odgovor*>::iterator it;
			deque<Komentar*>kq;
			deque<Komentar*>::iterator ikq;
			for (it = oq.begin(); it != oq.end(); ++it) {
				odg = *it;
				if (odg->odgovor == o) {
					Komentar* kom = new Komentar(k);
					if (!(odg->kom)) {
						odg->kom = new Komentar * [10];
						odg->brkom = -1;
					}
					if (odg->brkom < 9) {
						odg->kom[((odg->brkom) + 1)] = kom;
						odg->kom[((odg->brkom) + 1)]->odgotac = (*it);
						odg->brkom++;
						odg->ocene_potomaka_kom[odg->brkom] = kom->ocena;
						pro = 1;
						break;
					}
					else {
						cout << "Max broj komentara" << endl;
					}
				}
			}
			if (!pro) {
				for (it = oq.begin(); it != oq.end();++it) {
					if ((*it)->kom) {
						for (int i = 0; i <= (*it)->brkom; i++) {
							kq.push_back((*it)->kom[i]);
						}
					}
				}
				for (int k = 0; k < kq.size(); k++) {
					if (kq[k]->koment) {
						if (kq[k]->brkomentara > -1) {
							for (int t = 0; t <= kq[k]->brkomentara; t++) {
								kq.push_back(kq[k]->koment[t]);
							}
						}
					}
				}
				for (ikq = kq.begin(); ikq != kq.end(); ++ikq) {
					if ((*ikq)->komentar == o) {
						Komentar* kom = new Komentar(k);
						if (!((*ikq)->koment)) {
							(*ikq)->koment = new Komentar * [10];
							(*ikq)->brkomentara = -1;
						}
						if ((*ikq)->brkomentara < 9) {
							(*ikq)->koment[(*ikq)->brkomentara + 1] = kom;
							(*ikq)->koment[(*ikq)->brkomentara + 1]->komotac = (*ikq);
							(*ikq)->brkomentara++;
							(*ikq)->ocene_potomaka[(*ikq)->brkomentara] = kom->ocena;
							break;
						}
						else {
							cout << "Max broj komentara" << endl;
						}
					}
				}
			}
		}
	}
}

void brisipitipovodg(string p, Pitanje *&prvi) {
	Pitanje* tek = prvi, *stari = nullptr;
	int pr = 0;
	while (tek) {
		if ((tek->sled) && (tek->sled->pit == p)) {
			stari = tek->sled;
			if (tek->sled->brodg > -1) {
				deque<Odgovor*> oq;
				deque<Odgovor*>::iterator ioq;
				deque<Komentar*> kq;
				for (int i = 0; i <= tek->sled->brodg; i++) {
					oq.push_back(tek->sled->og[i]);
				}
				for (int i = 0; i < oq.size(); i++) {
					if (oq[i]->kom) {
						if ((oq[i]->brkom) > (-1)) {
							for (int j = 0; j <= oq[i]->brkom; j++) {
								kq.push_back(oq[i]->kom[j]);
							}
						}
					}
					delete oq[i];
				}
				for (int k = 0; k < kq.size(); k++) {
					if (kq[k]->koment) {
						if (kq[k]->brkomentara > -1) {
							for (int j = 0; j <= kq[k]->brkomentara; j++) {
								kq.push_back(kq[k]->koment[j]);
							}
						}
					}
					delete kq[k];
				}
			}

			tek->sled = tek->sled->sled;
			delete stari;
			break;
		}
		if (tek->pit == p) {
			stari = tek;
			if (tek->brodg > -1) {
				deque<Odgovor*> oq;
				deque<Odgovor*>::iterator ioq;
				deque<Komentar*> kq;
				for (int i = 0; i <= tek->brodg; i++) {
					oq.push_back(tek->og[i]);
				}
				for (int i = 0; i < oq.size(); i++) {
					if (oq[i]->kom) {
						if ((oq[i]->brkom) > (-1)) {
							for (int j = 0; j <= oq[i]->brkom; j++) {
								kq.push_back(oq[i]->kom[j]);
							}
						}
					}
					delete oq[i];
				}
				for (int k = 0; k < kq.size(); k++) {
					if (kq[k]->koment) {
						if (kq[k]->brkomentara > -1) {
							for (int j = 0; j <= kq[k]->brkomentara; j++) {
								kq.push_back(kq[k]->koment[j]);
							}
						}
					}
					delete kq[k];
				}
			}

			prvi = tek->sled;
			delete stari;
			break;
		}
		
		tek = tek->sled;
	}
}

Odgovor* pretraziodg(string o, string p, Pitanje* prvi) {
	Pitanje* tek = prvi;
	Odgovor* pomodg = nullptr;
	int pr = 0;
	while (tek) {
		if (tek->pit == p) {
			deque <Odgovor*> oq;
			for (int i = 0; i <= tek->brodg; i++) {
				oq.push_back(tek->og[i]);
			}
			
			deque<Odgovor*>::iterator it;
			for (it = oq.begin(); it != oq.end(); ++it) {
				if (((*it)->odgovor) == o) {
					return (*it);
					pr = 1;
				}
			}
			if (!pr){
				return nullptr;
			}
		}
		if ((tek->sled == nullptr) && (!pr)) {
			return nullptr;
		}
		tek = tek->sled;
	}
}

Komentar* pretrazikom(string k, string p, Pitanje* prvi) {
	Pitanje* tek = prvi;
	Komentar* pomkom = nullptr;
	Odgovor* pomodg = nullptr;
	int pr = 0;
	while (tek) {
		if (tek->pit == p) {
			deque<Odgovor*>oq;
			deque<Komentar*>kq;
			for (int i = 0; i <= tek->brodg;i++) {
				oq.push_back(tek->og[i]);
			}
			for (int j = 0; j < oq.size(); j++) {
				if (oq[j]->kom) {
					if (oq[j]->brkom > -1) {
						for (int k = 0; k <= oq[j]->brkom; k++) {
							kq.push_back(oq[j]->kom[k]);
						}
					}
				}
			}
			for (int t = 0; t < kq.size(); t++) {
				if (kq[t]->koment) {
					if (kq[t]->brkomentara > -1) {
						for (int z = 0; z <= kq[t]->brkomentara; z++) {
							kq.push_back(kq[t]->koment[z]);
						}
					}
				}
			}
			deque<Komentar*>::iterator ikq;
			for (ikq = kq.begin(); ikq != kq.end(); ++ikq) {
				if ((*ikq)->komentar == k) {
					pr = 1;
					return (*ikq);
				}
			}
			if (!pr) {
				return nullptr;
			}
		}

		tek = tek->sled;
	}

	
}

void brisiodg(Odgovor* odg , string p,Pitanje* prvi) {
	Pitanje* tek = prvi;
	deque <Komentar*> kq;
	if (odg->kom) {
		for (int i = 0; i <= odg->brkom; i++) {
			kq.push_back(odg->kom[i]);
		}
	}
	for (int k = 0; k < kq.size();k++) {
		if (kq[k]->koment) {
			if (kq[k]->brkomentara > -1) {
				for (int j = 0; j <= kq[k]->brkomentara; j++) {
					kq.push_back(kq[k]->koment[j]);
				}
			}
		}
	}
	deque<Komentar*>::iterator ikq;
	for (ikq = kq.begin(); ikq != kq.end(); ++ikq) {
		delete (*ikq);
	}
	while (tek) {
		if ((tek->pit) == p) {
			for (int i = 0;i <= tek->brodg; i++) {
				if (tek->og[i]->odgovor == odg->odgovor) {
					for (int t = i; t <= tek->brodg;t++) {
						tek->og[t] = tek->og[t + 1];
					}
					break;
				}
			}
			tek->brodg = tek->brodg - 1;
		}
		tek = tek->sled;
	}
	delete odg;
}

void brisikom(Komentar* kom, string p, Pitanje *prvi) {
	deque <Komentar*> kq;
	if (kom->koment) {
		for (int j = 0; j <= kom->brkomentara;j++) {
			kq.push_back(kom->koment[j]);
		}
	}
	deque<Komentar*>::iterator ikq;
	for (int k = 0; k < kq.size(); k++) {
		if (kq[k]->koment) {
			if (kq[k]->brkomentara > -1) {
				for (int i = 0; i <= kq[k]->brkomentara; i++) {
					kq.push_back(kq[k]->koment[i]);
				}
			}
		}
		delete kq[k];
	}
	
	if (kom->odgotac) {
		if (kom->odgotac->kom) {
			if (kom->odgotac->brkom > -1) {
				for (int t = 0; t <= kom->odgotac->brkom; t++) {
					if (kom->odgotac->kom[t]->komentar == kom->komentar) {
						for (int p = t + 1; p <= kom->odgotac->brkom; p++) {
							kom->odgotac->kom[p - 1] = kom->odgotac->kom[p];
						}
						break;
					}
				}
			}
		}
		kom->odgotac->brkom = kom->odgotac->brkom - 1;
	}
	else {
		if (kom->komotac->koment) {
			if (kom->komotac->brkomentara > -1) {
				for (int t = 0; t <= kom->komotac->brkomentara; t++) {
					if (kom->komotac->koment[t]->komentar == kom->komentar) {
						for (int p = t + 1; p <= kom->komotac->brkomentara;p++) {
							kom->komotac->koment[p - 1] = kom->komotac->koment[p];
						}
					}
				}
				kom->komotac->brkomentara = kom->komotac->brkomentara - 1;
			}
		}
	}
	delete kom;

}

void uredi(Pitanje*&prvi) {
	Pitanje* tek = prvi;
	deque<Odgovor*> oq;
	deque<Komentar*>kq;
	deque<Odgovor*>::iterator ioq;
	while (tek) {
		if (tek->brodg > -1) {
			for (int i = 0; i <= (tek->brodg - 1); i++) {
				for (int j = i + 1; j <= tek->brodg; j++) {
					if (tek->og[j]->ocena > tek->og[i]->ocena) {
						Odgovor* tmp = tek->og[i];
						tek->og[i] = tek->og[j];
						tek->og[j] = tmp;
						int temp = tek->og[i]->ocena;
						tek->ocene_potomaka_odgovora[i] = tek->ocene_potomaka_odgovora[j];
						tek->ocene_potomaka_odgovora[j] = temp;
					}
				}
				oq.push_back(tek->og[i]);
				if (i == (tek->brodg - 1)) {
					oq.push_back(tek->og[i + 1]);
				}
			}
			for (ioq = oq.begin(); ioq != oq.end(); ++ioq) {
				if ((*ioq)->kom) {
					if ((*ioq)->brkom > -1) {
						for (int t = 0; t <= ((*ioq)->brkom - 1); t++) {
							for (int p = t + 1; p <= (*ioq)->brkom; p++) {
								if ((*ioq)->kom[p]->ocena > (*ioq)->kom[t]->ocena) {
									Komentar* kom = (*ioq)->kom[t];
									(*ioq)->kom[t] = (*ioq)->kom[p];
									(*ioq)->kom[p] = kom;
									int temp = (*ioq)->kom[t]->ocena;
									(*ioq)->ocene_potomaka_kom[t] = (*ioq)->ocene_potomaka_kom[p];
									(*ioq)->ocene_potomaka_kom[p] = temp;
								}
							}
							kq.push_back((*ioq)->kom[t]);
							if (t == ((*ioq)->brkom - 1)) {
								kq.push_back((*ioq)->kom[t + 1]);
							}
						}
					}
				}
			}
			const int z = 0;
			while (!(kq.empty())) {
				if (kq[z]->koment) {
					if ((kq[z]->brkomentara) > -1) {
						for (int i = 0; i <= (kq[z]->brkomentara - 1); i++) {
							for (int j = i + 1; j <= kq[z]->brkomentara; j++) {
								if ((kq[z]->koment[j]->ocena) > (kq[z]->koment[i]->ocena)) {
									Komentar* ktmp = kq[z]->koment[i];
									kq[z]->koment[i] = kq[z]->koment[j];
									kq[z]->koment[j] = ktmp;
									int temp = kq[z]->koment[i]->ocena;
									kq[z]->ocene_potomaka[i] = kq[z]->ocene_potomaka[j];
									kq[z]->ocene_potomaka[j] = temp;
								}
								kq.push_back(kq[z]->koment[i]);
								if (i == (kq[z]->brkomentara - 1)) {
									kq.push_back(kq[z]->koment[i + 1]);
								}
							}
						}
					}
				}
				kq.pop_front();
			}
		}
		tek = tek->sled;
	}
}

Odgovor* pretrazinajvocenuO(string p, Pitanje* prvi) {
	Pitanje* tek = prvi;
	Odgovor* otmp = nullptr;
	while (tek) {
		if (tek->pit == p) {
			deque<Odgovor*>oq;
			if (tek->brodg > -1) {
				otmp = tek->og[0];
				for (int i = 0;i <= tek->brodg;i++) {
					oq.push_back(tek->og[i]);
				}
				deque<Odgovor*>::iterator ioq;
				for (ioq = oq.begin(); ioq != oq.end(); ++ioq) {
					if ((*ioq)->ocena > otmp->ocena) {
						otmp = (*ioq);
					}
				}

				return otmp;
			}
			else {
				return nullptr;
			}
		}
		tek = tek->sled;
	}
}

Komentar* pretrazinajvocenuK(string p, Pitanje* prvi) {
	Pitanje* tek = prvi;
	Komentar* kom = nullptr;
	while (tek) {
		if (tek->pit == p) {
			deque<Odgovor*>oq;
			if (tek->brodg > -1) {
				for (int i = 0;i <= tek->brodg;i++) {
					oq.push_back(tek->og[i]);
				}
				deque<Odgovor*>::iterator ioq;
				deque<Komentar*> kq;
				for (ioq = oq.begin(); ioq != oq.end(); ++ioq) {
					if ((*ioq)->kom) {
						if ((*ioq)->brkom > -1) {
							kom = (*ioq)->kom[0];
							for (int i = 0; i <= (*ioq)->brkom;i++) {
								kq.push_back((*ioq)->kom[i]);
							}
						}
					}
				}
				for (int k = 0; k < kq.size(); k++) {
					if (kq[k]->koment) {
						if (kq[k]->brkomentara > -1) {
							for (int j = 0; j <= kq[k]->brkomentara; j++) {
								kq.push_back(kq[k]->koment[j]);
							}
						}
						if (kq[k]->ocena > kom->ocena) {
							kom = kq[k];
						}
					}
				}
				return kom;
			}
			else {
				return nullptr;
			}	
		}
		tek = tek->sled;
	}
}

int main() {
	Pitanje* prvi = nullptr;
	while (true) {

		//MENI
		int k = kreirajmeni();
		if (!k) {
			break;
		}

		switch (k)
		{
		case 1: {//dodavanje pitanja
			string pp;
			cout << "Pitanje?" << endl;
			cin.ignore();
			getline(std::cin, pp);
			dodajpitanje(pp, prvi);

			break;
		}
		case 2: { //dodavanje odgovora na pitanje ili odgovor
			string temp, odg, pitanje;
			int t;
			cout << "1. Dodaj odgovor" << endl;
			cout << "2. Dodaj komentar" << endl;
			cin >> t;
			switch (t)
			{
			case 1: {
				cout << "Pitanje na koje treba odgovoriti?" << endl;
				cin.ignore();
				getline(cin, pitanje);
				cout << "Odgovor?" << endl;
				//cin.ignore();
				getline(cin, odg);
				dodajodgovor(pitanje, odg, prvi);

				break;
			}
			case 2: {
				cout << "Pitanje koje je povezano sa komentarom?" << endl;
				cin.ignore();
				getline(cin, pitanje);
				cout << "Odgovor koji treba komentarisati?" << endl;
				//cin.ignore();
				getline(cin, odg);
				cout << "Komentar?" << endl;
				//cin.ignore();
				getline(cin, temp);
				dodajkomentar(pitanje, odg, temp, prvi);

				break;

			}

			default:
				break;
			}

			break;
		}
		case 3: { //pretraga pitanja
			Pitanje* tek = prvi, * koren = nullptr;
			string ptmp;
			cout << "Pitanje za pronalazenje" << endl;
			cin.ignore();
			getline(cin, ptmp);
			while (tek) {
				if (tek->pit == ptmp) {
					koren = tek;
					break;
				}
				else {
					tek = tek->sled;
				}
			}
			if (!koren) {
				cout << "Neuspesna pretraga" << endl;
				break;
			}
			else {
				cout << "Uspesna pretraga" << endl;
				cout << koren->pit << endl;
			}
			break;
		}
		case 4: { //pretraga odgovora
			string ptmp, odgtmp;
			Odgovor* trodg = nullptr;
			Komentar* trkom = nullptr;
			cout << "Pitanje za koje je vezan odgovor?" << endl;
			cin.ignore();
			getline(cin, ptmp);
			cout << "Odgovor?" << endl;
			//cin.ignore();
			getline(cin, odgtmp);
			trodg = pretraziodg(odgtmp, ptmp, prvi);
			if (!trodg) {
				trkom = pretrazikom(odgtmp, ptmp, prvi);
			}
			if (trodg) {
				cout << "Uspesna pretraga" << endl;
				cout << trodg->odgovor << " ocena: " << trodg->ocena << endl;
			}
			if (trkom) {
				cout << "Uspesna pretraga" << endl;
				cout << trkom->komentar << " ocena: " << trkom->ocena << endl;
			}
			break;
		}
		case 5: { //povecanje ocene nekog odgovora
			string otmp, ptmp;
			Odgovor* odg;
			Komentar* kom;
			int oc;
			cout << "Pitanje za koji je vezan odgovor?" << endl;
			cin.ignore();
			getline(cin, ptmp);
			cout << "Odgovor za koji treba uvecati ocenu?" << endl;
			//cin.ignore();
			getline(cin, otmp);
			cout << "Ocena?" << endl;
			cin >> oc;
			odg = pretraziodg(otmp, ptmp, prvi);
			if (odg) {
				odg->ocena = oc;
				if (odg->otac) {
					for (int i = 0; i <= odg->otac->brodg; i++) {
						if ((odg->otac->og[i]->odgovor) == (odg->odgovor)) {
							odg->otac->ocene_potomaka_odgovora[i] = oc;
						}
					}
				}
			}
			if (!odg) {
				kom = pretrazikom(otmp, ptmp, prvi);
				if (kom) {
					kom->ocena = oc;
					if (kom->odgotac) {
						for (int i = 0; i <= kom->odgotac->brkom; i++) {
							if ((kom->odgotac->kom[i]->komentar) == (kom->komentar)) {
								kom->odgotac->ocene_potomaka_kom[i] = oc;
							}
						}
					}
					else {
						for (int i = 0; i <= kom->komotac->brkomentara;i++) {
							if ((kom->komotac->koment[i]->komentar) == (kom->komentar)) {
								kom->komotac->ocene_potomaka[i] == oc;
							}
						}
					}
				}
			}
			break;
		}
		case 6: { //uredi odgovore prema oceni
			uredi(prvi);
			break;
		}
		case 7: { //ispis svih pitanja
			Pitanje* tek = prvi;
			if (tek) {
				while (tek) {
					cout << tek->pit << endl;
					tek = tek->sled;
				}
			}
			break;
		}
		case 8: { //ispis pojed. pitanja i njegovih odg
			Pitanje* tek = prvi;
			string ptmp;
			int p = 0;
			cout << "Pitanje za ispis sa odgovorima?" << endl;
			cin.ignore();
			getline(cin, ptmp);
			deque<Komentar*>kq;
			deque<Komentar*>::iterator ikq;
			while (tek) {
				if (tek->pit == ptmp) {
					if (tek->brodg > -1) {
						for (int i = 0; i <= tek->brodg; i++) {
							cout << "Odgovor " << i << ": " << tek->og[i]->odgovor
								<< ", ocena " << tek->og[i]->ocena << endl;
							if (tek->og[i]->kom) {
								if (tek->og[i]->brkom > -1) {
									for (int j = 0; j <= tek->og[i]->brkom; j++) {
										kq.push_back(tek->og[i]->kom[j]);
									}
								}
							}
						}
						if (!(kq.empty())) {
							for (int k = 0; k < kq.size(); k++) {
								cout << "Komentar " << k << " : " << kq[k]->komentar
									<< ", ocena " << kq[k]->ocena
									<< " (" <<
									((kq[k]->odgotac) ? kq[k]->odgotac->odgovor : kq[k]->komotac->komentar)
									<< ")" << endl;
								if (kq[k]->koment) {
									if (kq[k]->brkomentara > -1) {
										for (int t = 0; t <= kq[k]->brkomentara; t++) {
											kq.push_back(kq[k]->koment[t]);
										}
									}
								}
							}
						}
					}
				}
				tek = tek->sled;
			}
			break;
		}
		case 9: { //brisanje odg ili kom i svih potomaka
			string ptmp, otmp;
			cout << "Pitanje za koje je vezan odgovor?" << endl;
			cin.ignore();
			getline(cin, ptmp);
			cout << "Odgovor za brisanje?" << endl;
			//cin.ignore();
			getline(cin, otmp);
			Odgovor* podg = pretraziodg(otmp, ptmp, prvi);
			if (podg) {
				brisiodg(podg, ptmp, prvi);
			}
			else {
				Komentar* pkom = pretrazikom(otmp, ptmp, prvi);
				brisikom(pkom, ptmp, prvi);
			}
			break;
		}
		case 10: {//brisanje 1 pitanja i svih povezanih odg
			string ptmp;
			cout << "Pitanje za brisanje?" << endl;
			cin.ignore();
			getline(cin, ptmp);
			brisipitipovodg(ptmp, prvi);
			break;
		}
		case 11: {//pronalazak odg ili kom sa najvecom ocenom na zadato pitanje
			string ptmp;
			cout << "Pitanje za pronalazak odgovora?" << endl;
			cin.ignore();
			getline(cin, ptmp);
			Odgovor* odgovor = pretrazinajvocenuO(ptmp, prvi);
			Komentar* komentar = pretrazinajvocenuK(ptmp, prvi);
			if (komentar && odgovor) {
				if (komentar->ocena > odgovor->ocena) {
					odgovor = nullptr;
					cout << komentar->komentar << " ocena: " << komentar->ocena << endl;
				}
				else {
					komentar = nullptr;
					cout << odgovor->odgovor << " ocena: " << odgovor->ocena << endl;
				}
			}
			else if (odgovor && (!komentar)) {
				cout << odgovor->odgovor << " ocena: " << odgovor->ocena << endl;
			}
			else if (komentar && (!odgovor)) {
				cout << komentar->komentar << " ocena: " << komentar->ocena << endl;
			}
			break;
		}
		default:
			break;
		}
	}

	return 0;
}
