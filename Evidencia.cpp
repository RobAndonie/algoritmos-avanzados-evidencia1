// 1- Códigos están o no en transmisiones
// 2- Cantidad de incidencias de cada código en transmisiones
// 3- Posiciones donde se encontraron
// 4- Subsecuencia del código con un caracter menos del original que fue mas 
//		encontrado dentro de los tres archivos de transmisión

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

string readFile(const string& fileName) {
	ifstream file(fileName);
	string transmission, line;
	while (getline(file, line)) {
		transmission += line;
	}

	file.close();
	return transmission;
}

void readCodes(const string& fileName, vector<string>& mcode) {
	ifstream file(fileName);
	string codes, line;
	while (getline(file, line)) {
		mcode.push_back(line);
	}

	file.close();
}

vector<int> lps(string str) {
	int m = str.size();
	vector<int> lpsv(m, 0);

	for (int i = 1, j = 0; i < m;) {
		if (str[i] == str[j]) {
			lpsv[i++] = ++j;
		} else {
			if (!j) {
				lpsv[i++] = 0;
			} else {
				j = lpsv[j - 1];
			}
		}
	}

	return lpsv;
}

vector<int> kmp(string str, string pattern) {
	vector<int> lpsv = lps(pattern);
	vector<int> posMatch;
	int n = str.size(), m = pattern.size();
	int i = 0, j = 0;

	while (i < n) {
		if (str[i] == pattern[j]) {
			j++, i++;
			if (j == m) {
				posMatch.push_back(i - m);
				j = lpsv[j - 1];
			}
		} else {
			if (!j) {
				i++;
			} else {
				j = lpsv[j - 1];
			}
		}
	}

	return posMatch;
}

void searchCode(const string& transmision, const string& mcode, ofstream& outputFile, int numTrans) {
    vector<int> posMatch = kmp(transmision, mcode);
    
    outputFile << "transmission" << numTrans << ".txt ==> " << posMatch.size() << " veces\n";

    int i = 0;
    for(int pos : posMatch) {
    	outputFile << pos;
    	if (i++ < posMatch.size() - 1)
			outputFile << ", ";
    }

    outputFile << "\n";
}



int main() {
	string trans1 = readFile("transmission1.txt");
	string trans2 = readFile("transmission2.txt");
	string trans3 = readFile("transmission3.txt");

	vector<string> mcode;
	readCodes("mcode.txt", mcode);

	ofstream outputFile("cheking.txt");

	for(const string& code : mcode) {
        outputFile << "Código: " << code << endl;
        searchCode(trans1, code, outputFile, 1);
        searchCode(trans2, code, outputFile, 2);
        searchCode(trans3, code, outputFile, 3);    
        outputFile << "--------------" << endl;
    }

	outputFile.close();

	return 0;
}






